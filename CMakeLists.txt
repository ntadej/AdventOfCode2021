cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# Add additional CMake macros and definitions
set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Setup project
project(AdventOfCode LANGUAGES CXX)

# Includes
include(Config)
include(CTest)

# Add days
setup_day(01)
setup_day(02)
setup_day(03)
setup_day(04)

# Enable tests
enable_testing()
