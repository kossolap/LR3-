cmake_minimum_required(VERSION 3.16)

project(LR3_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LR3_ENABLE_COVERAGE "Enable code coverage" ON)

include(CTest)
include(FetchContent)

# Библиотека структур данных
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

target_include_directories(lr3_structures
    PUBLIC
    include
)

# Демонстрационная программа
add_executable(lr3_demo
    app/main.cpp
)

target_link_libraries(lr3_demo
    PRIVATE
    lr3_structures
)

# Catch2 для тестирования
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

# Тесты
add_executable(lr3_tests
    tests/tests.cpp
)

target_link_libraries(lr3_tests
    PRIVATE
    lr3_structures
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(lr3_tests)

# Benchmark
add_executable(lr3_benchmark
    benchmarks/benchmark.cpp
)

target_link_libraries(lr3_benchmark
    PRIVATE
    lr3_structures
)

# Покрытие кода
if(LR3_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lr3_structures PRIVATE
        --coverage
        -O0
        -g
    )

    target_compile_options(lr3_tests PRIVATE
        --coverage
        -O0
        -g
    )

    target_link_options(lr3_tests PRIVATE
        --coverage
    )

    target_link_options(lr3_demo PRIVATE
        --coverage
    )

    target_link_options(lr3_benchmark PRIVATE
        --coverage
    )
endif()