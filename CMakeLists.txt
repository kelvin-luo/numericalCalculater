cmake_minimum_required(VERSION 3.20)

project(numericalCalculater
        VERSION 1.0.0
        DESCRIPTION "Numerical Calculator - MFC + OpenCV + Eigen"
        LANGUAGES CXX)

# --------------------------------------------------------------------------
# Global settings
# --------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Use the dynamically-linked MS runtime (/MD, /MDd) so that MFC-shared and the
# pre-built _MD dependencies (OpenCV/glog/gflags/...) link consistently.
if(MSVC)
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options(/utf-8 /EHsc)
    add_definitions(-DUNICODE -D_UNICODE -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()

# --------------------------------------------------------------------------
# Output directories (all paths relative to the repository root so the tree
# can be relocated without breaking builds / runs).
# PROJECT_DIR = parent of this "code" directory.
# --------------------------------------------------------------------------
get_filename_component(NC_PROJECT_DIR "${CMAKE_SOURCE_DIR}/.." ABSOLUTE)
set(NC_RUNTIME_DIR "${NC_PROJECT_DIR}/msvc_release")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NC_RUNTIME_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NC_RUNTIME_DIR}")
foreach(cfg DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} "${NC_RUNTIME_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg} "${NC_RUNTIME_DIR}")
endforeach()

if(NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX STREQUAL "")
    set(CMAKE_INSTALL_PREFIX "${NC_PROJECT_DIR}/install" CACHE PATH "Install dir" FORCE)
endif()
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${NC_PROJECT_DIR}/install" CACHE PATH "Install dir" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# --------------------------------------------------------------------------
# Third-party dependencies (paths can be overridden on the command line).
# --------------------------------------------------------------------------
include(Dependencies)

# --------------------------------------------------------------------------
# Configure version header.
# --------------------------------------------------------------------------
configure_file(
    "${CMAKE_SOURCE_DIR}/src/numcalc/nc_version.h.in"
    "${CMAKE_BINARY_DIR}/generated/nc_version.h"
    @ONLY)

# --------------------------------------------------------------------------
# Targets
# --------------------------------------------------------------------------
add_subdirectory(src/numcalc)   # numcalc : algorithm static library
add_subdirectory(src/app)       # numericalCalculater : MFC GUI executable

option(NC_BUILD_TESTS "Build unit tests with GoogleTest" ON)
if(NC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# --------------------------------------------------------------------------
# Copy the default sample media into the runtime input directory so the GUI
# can use them out of the box (only if the sources exist on this machine).
# --------------------------------------------------------------------------
set(NC_DEFAULT_IMAGE "D:/k8/media_images/xi_an_hot/people.jpeg")
set(NC_DEFAULT_VIDEO "D:/k8/media_260612/2026-06-15_154048_036.mp4")
file(MAKE_DIRECTORY "${NC_RUNTIME_DIR}/input")
if(EXISTS "${NC_DEFAULT_IMAGE}")
    file(COPY "${NC_DEFAULT_IMAGE}" DESTINATION "${NC_RUNTIME_DIR}/input")
endif()
if(EXISTS "${NC_DEFAULT_VIDEO}")
    file(COPY "${NC_DEFAULT_VIDEO}" DESTINATION "${NC_RUNTIME_DIR}/input")
endif()

message(STATUS "================ numericalCalculater ${PROJECT_VERSION} ================")
message(STATUS "  Runtime dir : ${NC_RUNTIME_DIR}")
message(STATUS "  Install dir : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  OpenCV      : ${OpenCV_VERSION} (${OpenCV_DIR})")
message(STATUS "  Eigen3      : ${NC_EIGEN_INCLUDE_DIR}")
message(STATUS "==========================================================")
