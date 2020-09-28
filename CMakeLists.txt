cmake_minimum_required(VERSION 3.16)

set(WREGOOLAR_VERSION "0.0.1")

set(CMAKE_BUILD_TYPE Debug)

project(wregoolar VERSION ${SQUEAL_VERSION} LANGUAGES CXX)

add_library(wregoolar INTERFACE)

target_compile_features(wregoolar INTERFACE cxx_std_20)

include_directories(include)

# testing
enable_testing()
option(SQUEAL_BUILD_TESTS "Build test programs" ON)
if(SQUEAL_BUILD_TESTS)
  add_subdirectory(src/tests/wregoolar)
endif()

