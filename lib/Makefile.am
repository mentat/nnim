EXTRA_DIST = XMLSpec.xml libGM.sln libGM.vcproj include/peerprotocol.h \
    include/ssh.h include/nnimprotocol.h
SUBDIRS = certificates common protocol

include_HEADERS = include/authload.h include/basenetwork.h include/contact.h \
 include/cryptography.h include/dstring.h include/expat.h include/globals.h \
 include/gmException.h include/gmstorage.h include/icqprotocol.h \
 include/kitprotocol.h include/log.h include/manager.h include/msnprotocol.h \
 include/platform.h \
 include/protocol.h include/securecomm.h include/sign.h \
 include/tocprotocol.h include/WString.h include/xml.h include/xmlnode.h \
 include/yahooprotocol.h	

lib_LTLIBRARIES = libGM.la
libGM_la_SOURCES = null.c
libGM_la_LIBADD = common/libcommon.la \
		common/expat/libexpat.la \
		protocol/libprotocol.la \
		protocol/icq/libicq.la \
		protocol/msn/libmsn.la \
		../crypto/libcryptopp.a
		
