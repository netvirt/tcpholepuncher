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
target_link_libraries(thp_shared event_core)

add_library(thp_static STATIC ${THP_SRCS})
set_target_properties(thp_static PROPERTIES OUTPUT_NAME thp)
target_link_libraries(thp_static event_core)

install(TARGETS thp_shared DESTINATION lib)
install(FILES include/thp.h DESTINATION include)

set(THP_CTL_SRC
	punchctl.c)

add_executable(punchctl ${THP_CTL_SRC})

target_link_libraries(punchctl
	event_core
	thp_static)

add_subdirectory(tests)
