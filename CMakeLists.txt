cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
project(anchors VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB SOURCES
        *.h
        *.cpp
        )

# add_library(anchors anchors.cpp)
add_executable(main ${SOURCE_FILES} ${SOURCES} anchorbase.h)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
