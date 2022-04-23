find_package(Boost REQUIRED)

set(HEADER_FILES anchor.h anchorbase.h anchorutil.h engine.h)

add_library(${PROJECT_NAME} anchor.cpp anchorutil.cpp engine.cpp ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers)