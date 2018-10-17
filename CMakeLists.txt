cmake_minimum_required(VERSION 2.6)
project(thp)

enable_testing()

SET(CPACK_PACKAGE_VERSION "0.0.1")

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0042 NEW)
endif(COMMAND cmake_policy)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(THP_SRCS
	src/punch.c
	src/log.c	
)

add_library(thp_shared SHARED ${THP_SRCS})
set_target_properties(thp_shared PROPERTIES OUTPUT_NAME thp)

add_library(thp_static STATIC ${THP_SRCS})
set_target_properties(thp_static PROPERTIES OUTPUT_NAME thp)

install(TARGETS thp_shared DESTINATION lib)
install(FILES include/thp.h DESTINATION include)

add_subdirectory(tests)
