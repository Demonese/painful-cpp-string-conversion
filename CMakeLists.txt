cmake_minimum_required(VERSION 3.10)
project(painful-cpp-string-conversion VERSION 5.0.0 DESCRIPTION "Why the conversion between C++ string/wstring/u8string/u16string/u32string so painful?" LANGUAGES CXX)

option(PAINFUL_CPP_STRING_CONVERSION_ENABLE_TEST "Enable test" OFF)

set(LIB_NAME painful-cpp-string-conversion)
set(TEST_NAME painful-cpp-string-conversion-test)

add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE include)

if (PAINFUL_CPP_STRING_CONVERSION_ENABLE_TEST)
    include(cmake/simdutf.cmake)

    add_executable(${TEST_NAME})
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TEST_NAME} PRIVATE "/utf-8" "/sdl" "/W4")
    endif ()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${TEST_NAME} PRIVATE "-Wall" "-Wextra")
        set_target_properties(${TEST_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    endif ()
    target_compile_features(${TEST_NAME} PRIVATE cxx_std_17)
    target_sources(${TEST_NAME} PRIVATE test/test.cpp)
    target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME} simdutf)

    include(CTest)

    add_test(NAME test-all COMMAND $<TARGET_FILE:${TEST_NAME}>)
endif ()
