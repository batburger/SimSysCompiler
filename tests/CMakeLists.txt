cmake_minimum_required(VERSION 2.6)
project(basic_test)

# Setup testing
enable_testing()
include(GoogleTest)
file(GLOB Test "./*.cpp")
file(GLOB_RECURSE Classes "../Source/*")
file(GLOB Standalones "../Data/*")


# Add test cpp file
add_executable(runUnitTests testmain.cpp ${Test} ${Classes} ${Standalones})
target_link_libraries(runUnitTests gtest gtest_main pthread)

gtest_discover_tests(runUnitTests)
