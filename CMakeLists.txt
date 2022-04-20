cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
project(anchors VERSION 0.1.0)

add_subdirectory(src)
add_subdirectory(docs)

enable_testing()
include(CTest)
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)