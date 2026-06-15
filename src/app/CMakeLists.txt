# numericalCalculater : MFC GUI executable.

# Link MFC as a shared DLL (also defines _AFXDLL and sets the include order).
set(CMAKE_MFC_FLAG 2)

# Generate the version resource so the .exe carries the CMake project version.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
    @ONLY)

set(APP_SOURCES
    app.cpp
    MainDlg.cpp
    AboutDlg.cpp
    MatDisplay.cpp
    app.rc
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
)

add_executable(numericalCalculater WIN32 ${APP_SOURCES})

# Use the shared MFC DLL. With the Ninja generator CMAKE_MFC_FLAG does not
# auto-define _AFXDLL, so set it explicitly. MFC headers then auto-link the
# correct mfc*.lib via #pragma comment.
target_compile_definitions(numericalCalculater PRIVATE _AFXDLL)

target_include_directories(numericalCalculater PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}")

# The version resource references winver.h relative to its own folder.
target_link_libraries(numericalCalculater PRIVATE numcalc ${OpenCV_LIBS})

# Ninja does not auto-link MFC even when CMAKE_MFC_FLAG is set.
if(MSVC)
    get_filename_component(_MSVC_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_MSVC_HOST "${_MSVC_BIN}" DIRECTORY)
    get_filename_component(_MSVC_BIN_DIR "${_MSVC_HOST}" DIRECTORY)
    get_filename_component(_MSVC_VER_DIR "${_MSVC_BIN_DIR}" DIRECTORY)
    set(_ATLMFC_LIB_DIR "${_MSVC_VER_DIR}/ATLMFC/lib/x64")
    target_link_directories(numericalCalculater PRIVATE "${_ATLMFC_LIB_DIR}")
    target_link_libraries(numericalCalculater PRIVATE
        $<$<CONFIG:Debug>:mfc140ud>
        $<$<CONFIG:Debug>:mfcs140ud>
        $<$<NOT:$<CONFIG:Debug>>:mfc140u>
        $<$<NOT:$<CONFIG:Debug>>:mfcs140u>
    )
    # Unicode MFC GUI: CRT must use the wide-char entry (wWinMain from mfcs*.lib).
    target_link_options(numericalCalculater PRIVATE "/ENTRY:wWinMainCRTStartup")
endif()

# Deploy dependency DLLs next to the executable after each build.
set(_dll_globs)
foreach(d IN LISTS NC_RUNTIME_DLL_DIRS)
    if(EXISTS "${d}")
        file(GLOB _dlls "${d}/*.dll")
        list(APPEND _dll_globs ${_dlls})
    endif()
endforeach()

foreach(dll IN LISTS _dll_globs)
    add_custom_command(TARGET numericalCalculater POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}" "$<TARGET_FILE_DIR:numericalCalculater>"
        VERBATIM)
endforeach()

# ---- Install rules -------------------------------------------------------
install(TARGETS numericalCalculater RUNTIME DESTINATION bin)
foreach(dll IN LISTS _dll_globs)
    install(FILES "${dll}" DESTINATION bin)
endforeach()
install(DIRECTORY "${NC_RUNTIME_DIR}/input/" DESTINATION bin/input OPTIONAL)
install(DIRECTORY DESTINATION output)  # create empty output dir on install
install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/" DESTINATION docs OPTIONAL)
