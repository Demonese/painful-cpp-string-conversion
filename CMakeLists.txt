cmake_minimum_required(VERSION 3.10)
project(painful-cpp-string-conversion VERSION 2.0.0 DESCRIPTION "Why the conversion between C++ string/wstring/u8string/u16string/u32string so painful?" LANGUAGES CXX)

option(PAINFUL_CPP_STRING_CONVERSION_ENABLE_TOOL "Enable tool (encoding-converter)" ON)
option(PAINFUL_CPP_STRING_CONVERSION_ENABLE_TEST "Enable test" OFF)

set(LIB_NAME painful-cpp-string-conversion)
set(TEST_NAME painful-cpp-string-conversion-test)

add_library(${LIB_NAME} INTERFACE)
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER include/encoding/string.hpp)
target_include_directories(${LIB_NAME} INTERFACE include)

if (PAINFUL_CPP_STRING_CONVERSION_ENABLE_TOOL)
    # encoding-converter
    add_executable(encoding-converter)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(encoding-converter PRIVATE "/utf-8")
    endif ()
    target_compile_features(encoding-converter PRIVATE cxx_std_20)
    target_sources(encoding-converter PRIVATE tool/encoding-converter.cpp)
    target_link_libraries(encoding-converter PRIVATE ${LIB_NAME})
endif ()

if (PAINFUL_CPP_STRING_CONVERSION_ENABLE_TEST)
    add_library(simdutf STATIC)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(simdutf PRIVATE "/utf-8")
    endif ()
    target_compile_features(simdutf PRIVATE cxx_std_20)
    target_include_directories(simdutf PUBLIC external/simdutf)
    target_sources(simdutf PRIVATE external/simdutf/simdutf.h external/simdutf/simdutf.cpp)

    add_executable(${TEST_NAME})
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TEST_NAME} PRIVATE "/utf-8")
    endif ()
    target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
    target_sources(${TEST_NAME} PRIVATE test/test_string.cpp)
    target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME} simdutf)
endif ()

include(GNUInstallDirs)
install(TARGETS ${LIB_NAME} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/encoding)
