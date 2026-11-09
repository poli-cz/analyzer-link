# third_party

Unmodified copy of GoogleTest 1.18.0 (googletest only, no googlemock), trimmed to what the
build needs. Source: https://github.com/google/googletest/releases/tag/v1.18.0, license
BSD-3-Clause in `googletest/LICENSE`. CMake uses it only when `find_package(GTest)` does not
find an installed GoogleTest, so the project builds without network access.
