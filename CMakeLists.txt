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
setup_day(05)
setup_day(06)
setup_day(07)
setup_day(08)
setup_day(09)
setup_day(10)
setup_day(11)
setup_day(12)
setup_day(13)
setup_day(14)
setup_day(15)
setup_day(16)
setup_day(17)
setup_day(18)
setup_day(19)

# Enable tests
enable_testing()
