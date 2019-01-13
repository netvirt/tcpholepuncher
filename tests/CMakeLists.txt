include_directories("${CMAKE_SOURCE_DIR}/src")



add_executable(test_init test_init.c)
add_test(test_init test_init)

add_executable(test_log test_log.c)
add_test(test_log test_log)

target_link_libraries(test_log thp_static)
