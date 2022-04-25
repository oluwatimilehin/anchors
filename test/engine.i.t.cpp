//
// Created by Timi Adeniran on 29/03/2022.
//

#include "../include/engine.h"

#include "../include/anchorutil.h"

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

}  // namespace anchorstest
