find_package(Boost REQUIRED)

set(HEADER_FILES anchor.h anchorbase.h anchorutil.h engine.h)

add_library(anchors anchor.cpp anchorutil.cpp engine.cpp ${HEADER_FILES})
add_executable(demo main.cpp)

target_link_libraries(anchors PUBLIC Boost::headers)
target_link_libraries(demo anchors)