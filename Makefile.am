ACLOCAL_AMFLAGS = -I m4 --install     

lib_LTLIBRARIES = libobj/libthp.la
libobj_libthp_la_SOURCES =  src/thp_init.c 
							
libobj_libthp_la_LIBADD = $(libevent_LIBS) \
			  -lssl -lcrypto \
			  $(jansson_LIBS) 
							
libobj_libthp_la_LDFLAGS = -version-info 0:0
include_HEADERS =   include/thp.h 

bin_PROGRAMS = punchctl
punchctl_SOURCES = punchctl.c
punchctl_CPPFLAGS = -DDEBUG
punchctl_LDADD = $(lib_LTLIBRARIES) \
	     $(libevent_LIBS) 

noinst_PROGRAMS = tests/setuptest
tests_setuptest_SOURCES = tests/setup_test.c
tests_setuptest_CFLAGS = -DDEBUG 
tests_setuptest_LDADD = $(lib_LTLIBRARIES) \
	     $(libevent_LIBS) 
EXTRA_DIST = m4/NOTES                                                           
