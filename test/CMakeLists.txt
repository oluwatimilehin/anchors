## Setup GTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
### For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(anchorstest engine.i.t.cpp)

target_link_libraries(anchorstest PRIVATE
        anchors
        gtest
        gtest_main)

include(GoogleTest)
gtest_discover_tests(anchorstest)


