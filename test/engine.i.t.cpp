//
// Created by Timi Adeniran on 29/03/2022.
//

#include "../src/engine.h"

#include "../src/anchorutil.h"

#include <algorithm>
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

    auto concatenate = [](std::string& text) { return "Hello, " + text; };

    auto greeting(Anchors::map<std::string>(username, concatenate));

    d_engine.observe(greeting);

    EXPECT_EQ("Hello, John", d_engine.get(greeting));

    d_engine.set(username, std::string("Samuel"));
    EXPECT_EQ("Hello, Samuel", d_engine.get(greeting));
}

TEST_F(EngineFixture, OnlyModifiedAnchorIsRecomputed) {
    auto anchorW(Anchors::create(10));
    auto anchorX(Anchors::create(4));

    int  addCounter = 0;
    auto add        = [&addCounter](int a, int b) {
        addCounter++;
        return a + b;
    };

    auto anchorY(Anchors::map2<int>(anchorW, anchorX, add));
    auto anchorZ(Anchors::create(5));

    int  subtractCounter = 0;
    auto subtract        = [&subtractCounter](int a, int b) {
        subtractCounter++;
        return a - b;
    };

    auto result(Anchors::map2<int>(anchorY, anchorZ, subtract));
    d_engine.observe(result);

    EXPECT_EQ(d_engine.get(result), 9);
    EXPECT_EQ(addCounter, 1);
    EXPECT_EQ(subtractCounter, 1);

    d_engine.set(anchorZ, 7);
    EXPECT_EQ(d_engine.get(result), 7);
    EXPECT_EQ(
        addCounter,
        1);  // It shouldn't recompute anchorY because its value did not change
    EXPECT_EQ(subtractCounter, 2);
}

TEST_F(EngineFixture, VectorManipulation_observedValuesShouldBeUpToDate) {
    auto myOrders = Anchors::create(std::vector<int>{150, 200, 300});

    auto maxOrder =
        Anchors::map<int, std::vector<int>>(myOrders, [](std::vector<int>& v) {
            return *std::max_element(v.begin(), v.end());
        });

    auto minOrder =
        Anchors::map<int, std::vector<int>>(myOrders, [](std::vector<int>& v) {
            return *std::min_element(v.begin(), v.end());
        });

    auto orderRange = Anchors::map2<int>(
        maxOrder, minOrder, [](int max, int min) { return max - min; });

    d_engine.observe(maxOrder);
    d_engine.observe(minOrder);
    d_engine.observe(orderRange);


    EXPECT_EQ(d_engine.get(maxOrder), 300);
    EXPECT_EQ(d_engine.get(minOrder), 150);
    EXPECT_EQ(d_engine.get(orderRange), 150);

    d_engine.set(myOrders, {300, 400, 800});

    EXPECT_EQ(d_engine.get(maxOrder), 800);
    EXPECT_EQ(d_engine.get(minOrder), 300);
    EXPECT_EQ(d_engine.get(orderRange), 500);
}

}  // namespace anchorstest
