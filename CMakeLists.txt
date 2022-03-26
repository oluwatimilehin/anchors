cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
project(anchors VERSION 0.1.0)

find_package(Boost REQUIRED)
find_package(GTest CONFIG REQUIRED)

include(CTest)
enable_testing()

file(GLOB SOURCES *.h *.cpp)

# add_library(anchors anchors.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

link_libraries(${PROJECT_NAME} PRIVATE Boost::uuid) # This doesn't work if I prepend `target_`
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
