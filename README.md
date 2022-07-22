# Anchors

![ubuntu](https://github.com/oluwatimilehin/anchors/actions/workflows/ubuntu.yml/badge.svg)

Anchors is a C++ library for [incremental computing](https://en.wikipedia.org/wiki/Incremental_computing) based
on [lord/anchors](https://github.com/lord/anchors) for rust
and [janestreet/incremental](https://github.com/janestreet/incremental) for OCaml.

Quoting [janestreet/incremental](https://github.com/janestreet/incremental), it allows you build large calculations 
(of the kind you might build into a spreadsheet) that can react efficiently to changing data.

[API Documentation.](https://oluwatimilehin.github.io/anchors/)

## Usage

An `Anchor` represents a node in the graph (think a spreadsheet cell) and you can either create an `Anchor` with a value
or define an `updater` function to create an `Anchor` from one or more Anchors.

As a basic example, let's define an `Anchor` whose value is the sum of two other anchors.

````cpp
#include <anchors/anchorutil.h>
#include <anchors/engine.h>

using namespace anchors;

Engine d_engine; // First set up the anchors engine

AnchorPtr<int> anchorA(Anchors::create(2));
AnchorPtr<int> anchorB(Anchors::create(3));

auto sum = [](int a, int b) { return a + b; }; // Updater function

auto anchorC(Anchors::map2<int>(anchorA, anchorB, sum)); // Note that the function will not be called until you `get` the value of `anchorC`.

````

Anchors follows a demand-driven model and will only (re)compute the value of an `Anchor` when you observe the `Anchor` 
and call `get()`.

````cpp
d_engine.observe(anchorC);
EXPECT_EQ(d_engine.get(anchorC), 5);
````

If you update the value of an `Anchor` that an observed `Anchor` depends on, subsequent calls for the observed `Anchor`
will return the updated value.

````cpp
d_engine.set(anchorA, 10);
EXPECT_EQ(d_engine.get(anchorC), 13);
````

### More Examples

#### String Concatenation

````cpp
auto username(Anchors::create(std::string("John")));

auto concatenate = [](const std::string& text) { return "Hello, " + text; };

auto greeting(Anchors::map<std::string>(username, concatenate));

d_engine.observe(greeting);

EXPECT_EQ("Hello, John", d_engine.get(greeting));

d_engine.set(username, std::string("Samuel"));
EXPECT_EQ("Hello, Samuel", d_engine.get(greeting));
````

#### Using an Input of a Different Type

````cpp
// Create the different anchors.

auto myOrders = Anchors::create(std::vector<int>{150, 200, 300});

// `maxOrder` and `minOrder` accept a vector and return an integer
AnchorPtr<int> maxOrder = Anchors::map<int, std::vector<int>>(
        myOrders, [](const std::vector<int>& v) {
            return *std::max_element(v.begin(), v.end());
        });

AnchorPtr<int> minOrder = Anchors::map<int, std::vector<int>>(
        myOrders, [](const std::vector<int>& v) {
            return *std::min_element(v.begin(), v.end());
        });

// `orderRange` depends on two derived anchors. None of the values will be computed until you call `get`.
AnchorPtr<int> orderRange = Anchors::map2<int>(
        maxOrder, minOrder, [](int max, int min) { return max - min; });


// Observe multiple anchors at a time. 
// Note that `orderRange` will still return the correct values below if you do not observe `maxOrder` and `minOrder`.
// However, their values will be undefined unless you call `get` for the unobserved values after `orderRange` has been computed.
std::vector<AnchorPtr<int>> anchorsToObserve{
        maxOrder, minOrder, orderRange};

d_engine.observe(anchorsToObserve);

// Verify the expected values
EXPECT_EQ(d_engine.get(maxOrder), 300);
EXPECT_EQ(d_engine.get(minOrder), 150);
EXPECT_EQ(d_engine.get(orderRange), 150);

// Update the input list and re-verify
d_engine.set(myOrders, {300, 400, 800});

EXPECT_EQ(d_engine.get(maxOrder), 800);
EXPECT_EQ(d_engine.get(minOrder), 300);
EXPECT_EQ(d_engine.get(orderRange), 500);
````

#### Verifying That It Avoids Needless Computations

````cpp
// Given an Anchor `result`, whose value is derived from (W + X) - Z, 
// Anchors will not recompute the sum if only the value of Z changes after we have first computed `result`.

auto anchorW(Anchors::create(10));
auto anchorX(Anchors::create(4));

int  additionCounter = 0;
auto anchorY(
    Anchors::map2<int>(anchorW, anchorX, [&additionCounter](int a, int b) {
        additionCounter++;
        return a + b;
    }));

auto anchorZ(Anchors::create(5));

int  subtractionCounter = 0;
auto result(
    Anchors::map2<int>(anchorY, anchorZ, [&subtractionCounter](int a, int b) {
        subtractionCounter++;
        return a - b;
    }));

d_engine.observe(result);

EXPECT_EQ(d_engine.get(result), 9);
EXPECT_EQ(additionCounter, 1);
EXPECT_EQ(subtractionCounter, 1);

d_engine.set(anchorZ, 7);

EXPECT_EQ(d_engine.get(result), 7);
EXPECT_EQ(additionCounter,1);  // It shouldn't recompute anchorY because its value did not change
EXPECT_EQ(subtractionCounter, 2);
````

#### A Quadratic Formula Calculator

````cpp
    auto a(Anchors::create(2));
    auto b(Anchors::create(-5));
    auto c(Anchors::create(-3));

    int bsquareCounter = 0;
    int fourACCounter  = 0;

    auto negativeB = Anchors::map<double>(b, [](double b) { return -b; });
    auto bSquare   = Anchors::map<double>(b, [&bsquareCounter](double b) {
        bsquareCounter++;
        return b * b;
    });

    auto fourAC =
        Anchors::map2<double>(a, c, [&fourACCounter](double x, double y) {
            fourACCounter++;
            return 4 * x * y;
        });

    auto squareRoot = Anchors::map2<double>(
        bSquare, fourAC, [](double x, double y) { return std::sqrt(x - y); });

    int  denominatorCounter = 0;
    auto denominator = Anchors::map<double>(a, [&denominatorCounter](double a) {
        denominatorCounter++;
        return 2 * a;
    });

    using FunctionType  = std::function<double(double&, double&, double&)>;
    FunctionType x1Func = [](double x, double y, double z) {
        return (x + y) / z;
    };

    FunctionType x2Func = [](double x, double y, double z) {
        return (x - y) / z;
    };

    auto x1 = Anchors::map3<double>(negativeB, squareRoot, denominator, x1Func);
    auto x2 = Anchors::map3<double>(negativeB, squareRoot, denominator, x2Func);

    d_engine.observe(x1);
    d_engine.observe(x2);

    {
        EXPECT_EQ(3, d_engine.get(x1));
        EXPECT_EQ(-0.5, d_engine.get(x2));

        EXPECT_EQ(1, bsquareCounter);
        EXPECT_EQ(1, fourACCounter);
        EXPECT_EQ(1, denominatorCounter);
    }

    d_engine.set(c, -7);

    {
        EXPECT_EQ(3.5, d_engine.get(x1));
        EXPECT_EQ(-1, d_engine.get(x2));

        // Only the value of C changed, so only anchors
        // that depend on C should be recalculated
        EXPECT_EQ(1, bsquareCounter);
        EXPECT_EQ(2, fourACCounter);
        EXPECT_EQ(1, denominatorCounter);
    }
````

### Note

- When you `get` an observed node, it will bring any other "stale" observed nodes up to date. An observed node is stale
  if any of its input has changed since it was last brought up to date.

## Installation
You can use Anchors from a CMake project by extracting the [file](https://github.com/oluwatimilehin/anchors/releases/download/v0.1.0/anchors_ubuntu.7z.zip) and adding the following:

````
# CMakeLists.txt
list(APPEND CMAKE_PREFIX_PATH <path_to_library_folder)
find_package(Boost REQUIRED) # Anchors requires a Boost installation
find_package(Anchors REQUIRED)
....

target_link_libraries(${YOUR_TARGET} PRIVATE Anchors::anchors)
````

## Roadmap

This is still a work in progress, and some tasks I intend to work on in the near future are:

- Implement [lord/anchors](https://lord.io/spreadsheets/) optimization - scroll to "anchors, a hybrid solution".
- Implement a `setUpdater()` function that allows you change the updater function for an `Anchor`.
- Cycle Detection.
- Add support for
  an [Incremental.bind](https://ocaml.janestreet.com/ocaml-core/latest/doc/incremental/Incremental__/Incremental_intf/#bind)
  equivalent.
- Support caching input parameters.
- ~~Support for `map3`, `map4`, etc.~~
- More tests.
