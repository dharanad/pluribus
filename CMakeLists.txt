cmake_minimum_required(VERSION 3.14)

project(velox-lab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

list(
    PREPEND
    CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/CMake"
    "${PROJECT_SOURCE_DIR}/velox/CMake"
)

include(ResolveDependency) # Related to velox submodule

# Velox configuration options
set(VELOX_ENABLE_PARQUET ON)
set(VELOX_BUILD_TEST_UTILS OFF)
set(VELOX_BUILD_TESTING OFF)
set(VELOX_ENABLE_SPARK_FUNCTIONS OFF)
set(VELOX_ENABLE_ICEBERG_FUNCTIONS OFF)

# TODO: Write notes
set(VELOX_DEPENDENCY_SOURCE AUTO CACHE STRING "Default dependency source: AUTO SYSTEM or BUNDLED.")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include_directories(.)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/velox)
include_directories(SYSTEM velox)

add_subdirectory(velox)

add_executable(velox-lab main.cpp)
target_link_libraries(velox-lab velox)