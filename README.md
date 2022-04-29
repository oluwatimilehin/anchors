# Anchors

![ubuntu](https://github.com/oluwatimilehin/anchors/actions/workflows/main.yml/badge.svg)

Anchors is a C++ library for [self-adjusting computations](https://lord.io/spreadsheets/) based
on [lord/anchors](https://github.com/lord/anchors) for rust
and [janestreet/incremental](https://github.com/janestreet/incremental) for OCaml.

It allows you model complex computations as a DAG which will update efficiently in response to an input change.

[API Documentation](https://oluwatimilehin.github.io/anchors/)

## Usage

An `Anchor` represents a node in the graph (or think of it like a cell in a spreadsheet) and you can define an updater
function to create an `Anchor` from one or more Anchors.

As a basic example, let's define an `Anchor` whose value is the sum of two other anchors.

````cpp
#include <anchors/anchorutil.h>
#include <anchors/engine.h>

using namespace anchors;

Engine d_engine; // First set up the anchors engine

AnchorPtr<int> anchorA(Anchors::create(2));
AnchorPtr<int> anchorB(Anchors::create(3));

auto sum = [](int a, int b) { return a + b; };

auto anchorC(Anchors::map2<int>(anchorA, anchorB, sum)); // Note that the function will not be called until you `get` the value of `anchorC`.

````

Anchors follows a demand-driven model and will only (re)compute the value of an `Anchor` if you observe the `Anchor`.
That is, only observed `Anchor` nodes are guaranteed to return their latest value.

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
// `maxOrder` and `minOrder` accept a vector and return an integer
auto myOrders = Anchors::create(std::vector<int>{150, 200, 300});

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

### Note

- When you `get` an observed node, it will bring any other "stale" observed nodes up to date. An observed node is stale
  if any of its input has changed since it was last brought up to date.

## Installation
You can use Anchors from a CMake project by extracting the linked file[TODO] and adding the following:

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
- Add support for
  an [Incremental.bind](https://ocaml.janestreet.com/ocaml-core/latest/doc/incremental/Incremental__/Incremental_intf/#bind)
  equivalent.
- Support caching input parameters.
- Support for `map3`, `map4`, etc.
- More tests.