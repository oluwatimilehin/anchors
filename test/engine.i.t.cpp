//
// Created by Timi Adeniran on 29/03/2022.
//

#include "../src/engine.h"

#include "../src/anchorutil.h"

#include <gtest/gtest.h>

using namespace anchors;

namespace anchorstest {

class EngineFixture : public ::testing::Test {
   protected:
    EngineFixture() : d_engine() {}

    Engine d_engine;
};

TEST_F(EngineFixture, TestIntegerArithmetic_observedValuesShouldBeUpToDate) {
    AnchorPtr<int> anchorA(AnchorUtil::create(2));
    AnchorPtr<int> anchorB(AnchorUtil::create(3));

    auto sum = [](int a, int b) { return a + b; };

    AnchorPtr<int> anchorC(AnchorUtil::map2(anchorA, anchorB, sum));

    d_engine.observe(anchorC);

    EXPECT_EQ(d_engine.get(anchorC), 5);

    d_engine.set(anchorA, 10);

    auto triple = [](int a) { return a * 3; };
    auto anchorD(AnchorUtil::map(anchorC, triple));

    d_engine.observe(anchorD);

    EXPECT_EQ(d_engine.get(anchorC), 13);
    EXPECT_EQ(d_engine.get(anchorD), 39);
}

TEST_F(EngineFixture, TestStringConcatenation_observedValueShouldBeUpToDate) {
    auto username(AnchorUtil::create(std::string("John")));

    auto concatenate = [](std::string& text) { return "Hello, " + text; };

    auto greeting(AnchorUtil::map(username, concatenate));

    d_engine.observe(greeting);

    EXPECT_EQ("Hello, John", d_engine.get(greeting));

    d_engine.set(username, std::string("Samuel"));
    EXPECT_EQ("Hello, Samuel", d_engine.get(greeting));
}

TEST_F(EngineFixture, TestOnlyModifiedAnchorIsRecomputed) {
    auto anchorW(AnchorUtil::create(10));
    auto anchorX(AnchorUtil::create(4));

    int  addCounter = 0;
    auto add        = [&addCounter](int a, int b) {
        addCounter++;
        return a + b;
    };

    auto anchorY(AnchorUtil::map2(anchorW, anchorX, add));
    auto anchorZ(AnchorUtil::create(5));

    int  subtractCounter = 0;
    auto subtract        = [&subtractCounter](int a, int b) {
        subtractCounter++;
        return a - b;
    };

    auto result(AnchorUtil::map2(anchorY, anchorZ, subtract));
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

}  // namespace anchorstest
