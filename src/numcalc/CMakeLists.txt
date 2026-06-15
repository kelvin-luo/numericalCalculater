# numcalc : pure C++ algorithm library (no MFC dependency, fully testable).

add_library(numcalc STATIC
    nc_plot.cpp
    nc_matrix.cpp
    nc_probability.cpp
    nc_statistics.cpp
    nc_logistic.cpp
    nc_knn.cpp
    nc_ann.cpp
    nc_cnn.cpp
    nc_yuv.cpp
)

target_include_directories(numcalc PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}/generated"   # nc_version.h
)

target_link_libraries(numcalc PUBLIC
    ${OpenCV_LIBS}
    numcalc_eigen
)

if(TARGET glog::glog)
    target_link_libraries(numcalc PUBLIC glog::glog)
    target_compile_definitions(numcalc PUBLIC NC_HAS_GLOG=1)
endif()
if(TARGET gflags::gflags)
    target_link_libraries(numcalc PUBLIC gflags::gflags)
    target_compile_definitions(numcalc PUBLIC NC_HAS_GFLAGS=1)
endif()
