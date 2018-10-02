ACLOCAL_AMFLAGS = -I m4 --install     

lib_LTLIBRARIES = libobj/libthp.la
libobj_libthp_la_SOURCES =  include/thp_init.c 
							
libobj_libthp_la_LIBADD = $(libevent_LIBS) \
			  -lssl -lcrypto \
			  $(jansson_LIBS) 
							
libobj_libthp_la_LDFLAGS = -version-info 0:0
include_HEADERS =   include/thp.h 

bin_PROGRAMS = punchctl
punchctl_SOURCES = src/punchctl.c
punchctl_CPPFLAGS = -DDEBUG
punchctl_LDADD = $(lib_LTLIBRARIES) \
	     $(libevent_LIBS) 

noinst_PROGRAMS = prototest
prototest_SOURCES = tests/proto_test.c
prototest_CFLAGS = -DDEBUG -g3 -O0
prototest_LDADD = $(lib_LTLIBRARIES) \
	     $(libevent_LIBS) 
EXTRA_DIST = m4/NOTES                                                           
