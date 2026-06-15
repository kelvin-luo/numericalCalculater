# --------------------------------------------------------------------------
# Dependency discovery for numericalCalculater.
#
# Default locations match the development machine, but every path can be
# overridden from the command line, e.g.:
#   cmake -S code -B build_msvc -DOpenCV_DIR=/path/to/opencv ...
# --------------------------------------------------------------------------

# ---- OpenCV 5.0 ----------------------------------------------------------
if(NOT OpenCV_DIR)
    set(OpenCV_DIR "D:/win10/opencv500/build" CACHE PATH "OpenCV build dir")
endif()
find_package(OpenCV REQUIRED)

# ---- Eigen 3 -------------------------------------------------------------
if(NOT NC_EIGEN_INCLUDE_DIR)
    set(NC_EIGEN_INCLUDE_DIR
        "D:/win10/deps_MD/eigen/eigen3.3.7_msvc2015_x64/include/eigen3"
        CACHE PATH "Eigen3 include dir")
endif()
if(NOT EXISTS "${NC_EIGEN_INCLUDE_DIR}/Eigen/Dense")
    # Fall back to package config if the hard-coded path is missing.
    find_package(Eigen3 QUIET)
    if(TARGET Eigen3::Eigen)
        get_target_property(NC_EIGEN_INCLUDE_DIR Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
    endif()
endif()

if(NOT TARGET numcalc_eigen)
    add_library(numcalc_eigen INTERFACE)
    target_include_directories(numcalc_eigen INTERFACE "${NC_EIGEN_INCLUDE_DIR}")
endif()

# ---- glog ----------------------------------------------------------------
set(NC_GLOG_DIR
    "D:/win10/deps_MD/glog-0.4.0_msvcx64NoGflags/glog_0.4.0_MSVC_C_19.44.35211.0_CXX_19.44.35211.0__x64_install"
    CACHE PATH "glog install dir")
# ---- gflags --------------------------------------------------------------
set(NC_GFLAGS_DIR
    "D:/win10/deps_MD/gflags-2.2.2msvcX64/gflags_2.2.2_MSVC_CXX_19.44.35211.0__x64_install"
    CACHE PATH "gflags install dir")

find_package(gflags QUIET CONFIG PATHS "${NC_GFLAGS_DIR}")
find_package(glog   QUIET CONFIG PATHS "${NC_GLOG_DIR}")

# ---- GoogleTest ----------------------------------------------------------
set(NC_GTEST_DIR
    "D:/win10/deps_MD/googletestV1.12.1Shared/googletest_MSVC_C_19.44.35211.0_CXX_19.44.35211.0x64"
    CACHE PATH "GoogleTest install dir")
find_package(GTest QUIET CONFIG PATHS "${NC_GTEST_DIR}")

# ---- Collect runtime DLLs so they can be deployed next to the .exe -------
set(NC_RUNTIME_DLL_DIRS
    "${OpenCV_DIR}/x64/vc16/bin"
    "${NC_GLOG_DIR}/bin"
    "${NC_GFLAGS_DIR}/bin"
    "${NC_GTEST_DIR}/bin"
    CACHE INTERNAL "Directories that contain dependency DLLs")
