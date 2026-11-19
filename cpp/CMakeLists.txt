
cmake_minimum_required(VERSION 3.16)

project(LR3_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LR3_ENABLE_COVERAGE "Enable code coverage" ON)

include(CTest)
include(FetchContent)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)

add_library(lr3_structures
    src/Array.cpp
    src/List.cpp
    src/DList.cpp
    src/Queue.cpp
    src/Stack.cpp
    src/HashTable.cpp
    src/AVLTree.cpp
    src/Serialization.cpp
)

target_include_directories(lr3_structures PUBLIC include)

target_link_libraries(lr3_structures PUBLIC
    nlohmann_json::nlohmann_json
)

add_executable(lr3_demo app/main.cpp)
target_link_libraries(lr3_demo PRIVATE lr3_structures)

# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(lr3_tests
    tests/tests.cpp
    tests/serialization_tests.cpp
)

target_link_libraries(lr3_tests PRIVATE
    lr3_structures
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(lr3_tests)

# Boost.Test
add_executable(boost_tests tests_boost/boost_tests.cpp)
target_link_libraries(boost_tests PRIVATE
    lr3_structures
    Boost::unit_test_framework
)

add_test(NAME boost_tests COMMAND boost_tests)

# GoogleTest
add_executable(gtest_tests tests_google/google_tests.cpp)
target_link_libraries(gtest_tests PRIVATE
    lr3_structures
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gtest_tests)

# Existing benchmark
add_executable(lr3_benchmark benchmarks/benchmark.cpp)
target_link_libraries(lr3_benchmark PRIVATE lr3_structures)

# Google Benchmark
add_executable(lr3_google_benchmark benchmarks/google_benchmark.cpp)
target_link_libraries(lr3_google_benchmark PRIVATE
    lr3_structures
    benchmark::benchmark
)

# Coverage
if(LR3_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lr3_structures PRIVATE --coverage -O0 -g)

    foreach(target
        lr3_tests
        boost_tests
        gtest_tests
        lr3_demo
        lr3_benchmark
        lr3_google_benchmark
    )
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    endforeach()
endif()