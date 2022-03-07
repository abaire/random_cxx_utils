cmake_minimum_required(VERSION 3.21)
project(scratchpad)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(
        Boost 1.70
        COMPONENTS
        unit_test_framework
        REQUIRED
)
include_directories("${Boost_INCLUDE_DIR}")

add_executable(
        scratchpad
        main.cpp
)


# module_registry_tests
add_executable(
        circular_buffer_tests
        test/circular_buffer/test_main.cpp
        circular_buffer/circular_buffer.c
        circular_buffer/circular_buffer.h
        circular_buffer/circular_buffer_impl.h
)
target_include_directories(
        circular_buffer_tests
        PRIVATE circular_buffer
)
target_link_libraries(
        circular_buffer_tests
        LINK_PRIVATE
        ${Boost_LIBRARIES}
)
add_test(NAME circular_buffer_tests COMMAND circular_buffer_tests)
