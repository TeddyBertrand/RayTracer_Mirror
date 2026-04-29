include(FetchContent)

set(GOOGLETEST_URL "https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip")
set(GOOGLETEST_PROBE_FILE "${CMAKE_BINARY_DIR}/_deps/googletest-probe.zip")

file(DOWNLOAD
    "${GOOGLETEST_URL}"
    "${GOOGLETEST_PROBE_FILE}"
    STATUS GOOGLETEST_DOWNLOAD_STATUS
    LOG GOOGLETEST_DOWNLOAD_LOG
    TIMEOUT 10
    TLS_VERIFY ON
)

list(GET GOOGLETEST_DOWNLOAD_STATUS 0 GOOGLETEST_DOWNLOAD_CODE)

if(NOT GOOGLETEST_DOWNLOAD_CODE EQUAL 0)
    message(STATUS "Skipping unit tests: unable to fetch GoogleTest.")
    message(STATUS "Download log: ${GOOGLETEST_DOWNLOAD_LOG}")
    return()
endif()

file(REMOVE "${GOOGLETEST_PROBE_FILE}")

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL "${GOOGLETEST_URL}"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(raytracer_unit_tests)

add_subdirectory(math)
add_subdirectory(scene)
add_subdirectory(primitives)
add_subdirectory(lights)
add_subdirectory(materials)
add_subdirectory(core)
add_subdirectory(skies)

target_include_directories(raytracer_unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(raytracer_unit_tests PRIVATE
    raytracer_core
    raytracer_math
    GTest::gtest_main
)

gtest_discover_tests(raytracer_unit_tests)

find_program(GCOVR_EXECUTABLE gcovr)
if(GCOVR_EXECUTABLE)
    if(NOT ENABLE_COVERAGE)
        message(STATUS "Target 'coverage' is available, but ENABLE_COVERAGE=ON is recommended.")
    endif()

    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/coverage
        COMMAND $<TARGET_FILE:raytracer_unit_tests>
        COMMAND ${CMAKE_COMMAND} -E rm -f ${PROJECT_BINARY_DIR}/src/CMakeFiles/raytracer.dir/main.cpp.gcda ${PROJECT_BINARY_DIR}/src/CMakeFiles/raytracer.dir/main.cpp.gcno
        COMMAND ${GCOVR_EXECUTABLE} -r ${PROJECT_SOURCE_DIR} --txt --exclude "build/_deps" --exclude "tests" --exclude "(^|.*/)main\\.cpp$" --gcov-ignore-errors output_error
        COMMAND ${GCOVR_EXECUTABLE} -r ${PROJECT_SOURCE_DIR} --html-details ${PROJECT_SOURCE_DIR}/coverage/index.html --exclude "build/_deps" --exclude "tests" --exclude "(^|.*/)main\\.cpp$" --gcov-ignore-errors output_error
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        DEPENDS raytracer_unit_tests
        COMMENT "Running unit tests and generating coverage report"
    )
else()
    message(STATUS "gcovr not found: coverage target not created")
endif()