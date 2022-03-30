find_package(Boost REQUIRED)

set(HEADER_FILES anchor.h anchorbase.h anchorutil.h engine.h)

include_directories(${Boost_INCLUDE_DIRS})

add_library(anchors anchor.cpp anchorutil.cpp engine.cpp ${HEADER_FILES})
add_executable(demo main.cpp)

link_libraries(anchors PRIVATE Boost::uuid) # This doesn't work if I prepend `target_`
target_link_libraries(demo anchors)