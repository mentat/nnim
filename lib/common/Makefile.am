SUBDIRS = expat
INCLUDES = -Iexpat/include -g -I$(CRYPTODIR) -I../include

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = xml.cpp xmlnode.cpp contact.cpp expat.cpp cryptography.cpp \
authload.cpp log.cpp

check_PROGRAMS = test
test_SOURCES = test.cpp
test_INCLUDES = -I.
test_LDFLAGS = -L. -L$(CRYPTODIR)

