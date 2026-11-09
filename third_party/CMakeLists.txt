# Copy of GoogleTest for machines that do not have it installed. It is built only when
# find_package(GTest) in the top-level CMakeLists.txt did not find it. Its headers are
# system headers, so first-party warnings and clang-tidy skip them.

if(LINK_BUILD_TESTS AND NOT GTest_FOUND)
  message(STATUS "GoogleTest: copy in third_party/")
  set(BUILD_GMOCK OFF)
  set(INSTALL_GTEST OFF)
  # Same C runtime as the rest of the build (/MD) with MSVC.
  set(gtest_force_shared_crt ON)
  add_subdirectory(googletest EXCLUDE_FROM_ALL)
  set_target_properties(gtest PROPERTIES
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:gtest,INTERFACE_INCLUDE_DIRECTORIES>)
endif()
