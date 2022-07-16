//
// Created by Timi Adeniran on 29/03/2022.
//

#include "../include/engine.h"

#include "../include/anchorutil.h"

#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

using namespace anchors;

namespace anchorstest {

class EngineFixture : public ::testing::Test {
   protected:
    EngineFixture() : d_engine() {}

    Engine d_engine;
};

TEST_F(EngineFixture, IntegerArithmetic_observedValuesShouldBeUpToDate) {
    AnchorPtr<int> anchorA(Anchors::create(2));
    AnchorPtr<int> anchorB(Anchors::create(3));

    auto sum = [](int a, int b) { return a + b; };

    AnchorPtr<int> anchorC(Anchors::map2<int>(anchorA, anchorB, sum));

    d_engine.observe(anchorC);

    EXPECT_EQ(d_engine.get(anchorC), 5);

    d_engine.set(anchorA, 10);

    auto triple = [](int a) { return a * 3; };
    auto anchorD(Anchors::map<int>(anchorC, triple));

    d_engine.observe(anchorD);

    EXPECT_EQ(d_engine.get(anchorC), 13);
    EXPECT_EQ(d_engine.get(anchorD), 39);
}

TEST_F(EngineFixture, StringConcatenation_observedValueShouldBeUpToDate) {
    auto username(Anchors::create(std::string("John")));

    auto concatenate = [](const std::string& text) { return "Hello, " + text; };

    auto greeting(Anchors::map<std::string>(username, concatenate));

    d_engine.observe(greeting);

    EXPECT_EQ("Hello, John", d_engine.get(greeting));

    d_engine.set(username, std::string("Samuel"));
    EXPECT_EQ("Hello, Samuel", d_engine.get(greeting));
}

TEST_F(EngineFixture, OnlyModifiedAnchorIsRecomputed) {
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
    auto result(Anchors::map2<int>(
        anchorY, anchorZ, [&subtractionCounter](int a, int b) {
            subtractionCounter++;
            return a - b;
        }));

    d_engine.observe(result);

    EXPECT_EQ(d_engine.get(result), 9);
    EXPECT_EQ(additionCounter, 1);
    EXPECT_EQ(subtractionCounter, 1);

    d_engine.set(anchorZ, 7);
    EXPECT_EQ(d_engine.get(result), 7);
    EXPECT_EQ(
        additionCounter,
        1);  // It shouldn't recompute anchorY because its value did not change
    EXPECT_EQ(subtractionCounter, 2);
}

TEST_F(EngineFixture, VectorManipulation_observedValuesShouldBeUpToDate) {
    auto myOrders = Anchors::create(std::vector<int>{150, 200, 300});

    AnchorPtr<int> maxOrder = Anchors::map<int, std::vector<int>>(
        myOrders, [](const std::vector<int>& v) {
            return *std::max_element(v.begin(), v.end());
        });

    AnchorPtr<int> minOrder = Anchors::map<int, std::vector<int>>(
        myOrders, [](const std::vector<int>& v) {
            return *std::min_element(v.begin(), v.end());
        });

    AnchorPtr<int> orderRange = Anchors::map2<int>(
        maxOrder, minOrder, [](int max, int min) { return max - min; });

    std::vector<AnchorPtr<int>> anchorsToObserve{
        maxOrder, minOrder, orderRange};

    d_engine.observe(anchorsToObserve);

    EXPECT_EQ(d_engine.get(maxOrder), 300);
    EXPECT_EQ(d_engine.get(minOrder), 150);
    EXPECT_EQ(d_engine.get(orderRange), 150);

    d_engine.set(myOrders, {300, 400, 800});

    EXPECT_EQ(d_engine.get(maxOrder), 800);
    EXPECT_EQ(d_engine.get(minOrder), 300);
    EXPECT_EQ(d_engine.get(orderRange), 500);
}

TEST_F(EngineFixture, Map3_Arithmetic_observedValuesShouldBeUpToDate) {
    AnchorPtr<int>    anchorA(Anchors::create(2));
    AnchorPtr<int>    anchorB(Anchors::create(3));
    AnchorPtr<double> anchorC(Anchors::create(0.5));

    auto multiply = [](int a, int b, double c) { return a * b * c; };

    AnchorPtr<double> anchorD(Anchors::map3<double, int, int, double>(
        anchorA, anchorB, anchorC, multiply));

    d_engine.observe(anchorD);

    EXPECT_EQ(d_engine.get(anchorD), 3.0);

    d_engine.set(anchorA, 10);
    d_engine.set(anchorC, 1.0);

    auto addFive = [](double a) { return a + 5; };
    auto anchorE(Anchors::map<double>(anchorD, addFive));

    d_engine.observe(anchorE);

    EXPECT_EQ(d_engine.get(anchorD), 30.0);
    EXPECT_EQ(d_engine.get(anchorE), 35.0);
}

TEST_F(EngineFixture, Map4_StringConcatenation) {
    auto anchorOne   = Anchors::create(std::string("Liberte"));
    auto anchorTwo   = Anchors::create(std::string("Egalite"));
    auto anchorThree = Anchors::create(std::string("Fraternite"));
    auto anchorFour  = Anchors::create(std::string("Beyonce"));

    using FunctionType = std::function<std::string(
        std::string&, std::string&, std::string&, std::string&)>;

    FunctionType updater = [](const std::string& string1,
                              const std::string& string2,
                              const std::string& string3,
                              const std::string& string4) {
        return string1 + ", " + string2 + ", " + string3 + ", " + string4;
    };

    auto result(Anchors::map4<std::string>(
        anchorOne, anchorTwo, anchorThree, anchorFour, updater));

    d_engine.observe(result);

    EXPECT_EQ("Liberte, Egalite, Fraternite, Beyonce", d_engine.get(result));

    d_engine.set(anchorTwo, std::string("Beyonce"));
    d_engine.set(anchorFour, std::string("Fiance"));

    EXPECT_EQ("Liberte, Beyonce, Fraternite, Fiance", d_engine.get(result));
}

TEST_F(EngineFixture, QuadraticFormula) {
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

        // Only the value of C changed, so only the anchors
        // that depend on C should be recalculated
        EXPECT_EQ(1, bsquareCounter);
        EXPECT_EQ(2, fourACCounter);
        EXPECT_EQ(1, denominatorCounter);
    }
}

}  // namespace anchorstest
