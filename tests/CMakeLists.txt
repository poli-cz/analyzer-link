add_executable(alink_tests
  instrument_error_test.cpp
  frame_test.cpp
  lis_export_test.cpp
  recorded_frames_test.cpp)
target_link_libraries(alink_tests PRIVATE alink::alink GTest::gtest_main)
target_compile_definitions(alink_tests PRIVATE
  ALINK_RECORDED_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/recorded")
link_set_warnings(alink_tests)
gtest_discover_tests(alink_tests DISCOVERY_TIMEOUT 30)

# End-to-end check of the command line tool on one recorded capture.
add_test(NAME link_cli.recorded_capture
  COMMAND ${CMAKE_COMMAND}
    -DCLI=$<TARGET_FILE:link-cli>
    -DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/data/recorded/an07-2026-10-14.frames
    -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/data/recorded/an07-2026-10-14.expected
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_cli.cmake)
