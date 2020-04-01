cmake_minimum_required(VERSION 3.15)
project(SpinLock)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(google_tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(SpinLock spinlock.cpp spinlock.hpp google_tests/tests.cpp)
