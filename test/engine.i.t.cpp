//
// Created by Timi Adeniran on 29/03/2022.
//

#include "../src/engine.h"

#include <gtest/gtest.h>

#include "../src/anchorutil.h"

using namespace anchors;

namespace anchorstest {

class EngineFixture : public ::testing::Test {
   protected:
    EngineFixture() : d_engine() {}

    Engine d_engine;
};

TEST_F(EngineFixture, TestIntegerAddition_anchorValueShould){

}

}  // namespace anchorstest
