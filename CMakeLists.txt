cmake_minimum_required(VERSION 3.0.0)
project(anchors VERSION 0.1.0)

include(CTest)
enable_testing()

# add_library(anchors anchors.cpp)
add_executable(main main.cpp)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include") 

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
