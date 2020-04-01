cmake_minimum_required(VERSION 3.15)
project(SpinLock)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(google_tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_library(spinlocklib STATIC spinlock.cpp spinlock.hpp)

target_link_libraries(spinlocklib gtest gtest_main)


add_executable(SpinLock main.cpp google_tests/tests.cpp)

target_link_libraries(SpinLock spinlocklib)
