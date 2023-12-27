## Declare project
project(yage-switch)

cmake_minimum_required(VERSION 3.21)

# We set the C standard to 11.
set(CMAKE_C_STANDARD 11)

## CMake modules

# We have to add the cmake directory to have access to
# the Switch tools and also find_package for libnx.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/platform_switch/cmake)
message([TRACE] ${CMAKE_MODULE_PATH})
## Required libraries and utilities

# SwitchTools provide various helper functions for building Switch file formats.
include(SwitchTools)

# Find libnx, which is required by our application.
find_package(Libnx REQUIRED)


add_subdirectory(templates)