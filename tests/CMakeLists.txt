include_directories("${CMAKE_SOURCE_DIR}/src")



add_executable(unit_tests unit_tests.c)
add_test(unit_tests unit_tests)

add_executable(test_log test_log.c)
add_test(test_log test_log)

target_link_libraries(test_log thp_static)
