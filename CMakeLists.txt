cmake_minimum_required(VERSION 3.22...3.25)

project(analyzer_link
  VERSION 1.9.0
  DESCRIPTION "Decodes analyzer frames and exports instrument errors to the LIS"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(LINK_BUILD_TESTS "Build the GoogleTest suites" ON)
option(LINK_ENABLE_SANITIZERS "Build with AddressSanitizer and UBSan" OFF)
option(LINK_ENABLE_CLANG_TIDY "Run clang-tidy on every translation unit" OFF)
option(LINK_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(LINK_ENABLE_COVERAGE "Instrument for line coverage (GCC --coverage)" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(Sanitizers)
include(ClangTidy)

if(LINK_BUILD_TESTS)
  find_package(GTest 1.11 QUIET)
endif()
add_subdirectory(third_party)

add_library(alink
  src/instrument_error.cpp
  src/frame.cpp
  src/lis_export.cpp)
add_library(alink::alink ALIAS alink)
target_include_directories(alink PUBLIC include)
link_set_warnings(alink)
link_enable_tidy(alink)

add_executable(link-cli apps/link-cli/main.cpp)
target_link_libraries(link-cli PRIVATE alink::alink)
link_set_warnings(link-cli)
link_enable_tidy(link-cli)

if(LINK_BUILD_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()
