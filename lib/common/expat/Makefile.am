INCLUDES = -Iinclude

noinst_LIBRARIES = libexpat.a

libexpat_a_SOURCES = gennmtab.c \
	hashtable.c \
	xmlparse.c \
	xmlrole.c \
	xmltok.c 

CFLAGS = -g -Wall

EXTRA_DIST = xmltok_impl.c xmltok_ns.c \
include/asciitab.h   include/iasciitab.h  include/nametab.h  include/xmldef.h \
include/xmlrole.h    include/xmltok_impl.h include/hashtable.h  include/latin1tab.h \
include/utf8tab.h  include/xmlparse.h  include/xmltok.h
