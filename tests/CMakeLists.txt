# Unit tests for the numcalc algorithm library (GoogleTest).

if(NOT TARGET GTest::gtest)
    message(WARNING "GoogleTest not found - skipping unit tests.")
    return()
endif()

add_executable(numcalc_tests test_numcalc.cpp)
target_link_libraries(numcalc_tests PRIVATE numcalc GTest::gtest)

add_test(NAME numcalc_tests COMMAND numcalc_tests)

# GoogleTest here is a shared build; make its DLL available next to the test.
if(DEFINED NC_GTEST_DIR AND EXISTS "${NC_GTEST_DIR}/bin")
    file(GLOB _gtest_dlls "${NC_GTEST_DIR}/bin/*.dll")
    foreach(dll IN LISTS _gtest_dlls)
        add_custom_command(TARGET numcalc_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${dll}" "$<TARGET_FILE_DIR:numcalc_tests>"
            VERBATIM)
    endforeach()
endif()
