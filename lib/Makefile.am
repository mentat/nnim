EXTRA_DIST = XMLSpec.xml libGM.sln libGM.vcproj
SUBDIRS = common protocol

lib_LIBRARIES = libGM.a
libGM_a_SOURCES = null.c
libGM_a_LIBADD = common/libcommon.a \
		common/expat/libexpat.a \
		protocol/libprotocol.a \
		protocol/icq/libicq.a \
		protocol/msn/libmsn.a
		
