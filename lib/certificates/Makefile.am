INCLUDES = -g -I$(CRYPTODIR) -I../include

noinst_LTLIBRARIES = libcerts.la

libcerts_la_SOURCES = sign.cpp gmstorage.cpp securecomm.cpp
noinst_HEADERS = dstring.h
check_PROGRAMS = test
test_SOURCES = test.cpp
test_INCLUDES = -I.
test_LDFLAGS = -L. -L$(CRYPTODIR)

