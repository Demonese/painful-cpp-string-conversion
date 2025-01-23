cmake_minimum_required(VERSION 3.10)
project(painful-cpp-string-conversion VERSION 1.0.0 DESCRIPTION "Why the conversion between C++ string/wstring/u8string/u16string/u32string so painful?" LANGUAGES CXX)

add_library(painful-cpp-string-conversion INTERFACE)
target_include_directories(painful-cpp-string-conversion INTERFACE .)
