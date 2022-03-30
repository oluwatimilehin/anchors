find_package(GTest CONFIG REQUIRED)

add_executable(anchorstest engine.i.t.cpp)

target_link_libraries(anchorstest PRIVATE
        anchors
        GTest::gmock
        GTest::gtest
        GTest::gmock_main
        GTest::gtest_main)


