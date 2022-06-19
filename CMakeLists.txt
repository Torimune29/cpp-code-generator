# This file specifies how the project should be built, using CMake.
# If you are unfamiliar with CMake, don't worry about all the details.
# The sections you might want to edit are marked as such, and
# the comments should hopefully make most of it clear.
#
# For many purposes, you may not need to change anything about this file.

cmake_minimum_required(VERSION 3.14) # for fetchcontent

# --------------------------------------------------------------------------------
# Project Environment (change as needed).
# --------------------------------------------------------------------------------
# Set project name, version and languages here. (change as needed)
# e.g.) If set "test" as project name, "test" as app name and "testlib" as library name set automatically
set(PROJECT_NAME cppcodegen) # Default name for the library built from src/*.cpp (change if you wish)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

# Options: Things you can set via commandline options to cmake (e.g. -DENABLE_LTO=[ON|OFF])
option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                                 Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)

# Include stuff for global scope. No change needed.
include(cmake/common/common.cmake)

# Check for LTO support.
find_lto(CXX)

# --------------------------------------------------------------------------------
# Locate files (change as needed).
# --------------------------------------------------------------------------------
set(TESTFILES # All .cpp files in tests/
        tests/main.cpp
)

# --------------------------------------------------------------------------------
# Build! (Change as needed)
# --------------------------------------------------------------------------------

# Add external libs. No change needed.
add_subdirectory(cmake/external)

# Set up tests (see tests/CMakeLists.txt).
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage")
        add_subdirectory(tests)
endif()
