SUBDIRS= common protocol certificates
INCLUDES= -I$(CRYPTODIR) -I./include -I./common/expat/include -I. -I..

pkglib_LTLIBRARIES = libgm.la
libgm_la_SOURCES = null.c
libgm_la_LIBADD = \
common/authload.lo  common/cryptography.lo  common/xml.lo \
common/contact.lo   common/expat.lo         common/xmlnode.lo \
common/log.lo \
common/expat/gennmtab.lo common/expat/xmlparse.lo common/expat/xmltok.lo \
common/expat/hashtable.lo \
common/expat/xmlrole.lo \
protocol/peerprotocol.lo protocol/yahooprotocol.lo \
protocol/kitprotocol.lo protocol/protocol.lo \
protocol/icq/fnacbospackets.lo protocol/icq/icqprotocol.lo \
protocol/icq/parsefnacicq.lo protocol/icq/parsefnacgen.lo \
protocol/icq/fnacicqpackets.lo protocol/icq/fnacbudpackets.lo \
protocol/icq/packet.lo \
protocol/icq/parsefnacloc.lo protocol/icq/fnacgenpackets.lo  protocol/icq/packets.lo \
protocol/icq/parsefnacmsg.lo protocol/icq/fnaclocpackets.lo  protocol/icq/parsefnac.lo \
protocol/icq/parsepacket.lo protocol/icq/fnacmsgpackets.lo  protocol/icq/parsefnacbos.lo \
protocol/icq/stdpackets.lo protocol/icq/fnacpacket.lo      protocol/icq/parsefnacbud.lo \
protocol/msn/msnprotocol.lo \
protocol/icq/util.lo \
protocol/tocprotocol.lo \
protocol/manager.lo \
certificates/securecomm.lo  certificates/sign.lo  certificates/gmstorage.lo 

noinst_HEADERS = \
include/authload.h      include/expat.h        include/manager.h       include/tocprotocol.h \
include/basenetwork.h   include/globals.h      include/msnprotocol.h   include/xml.h \
include/contact.h       include/icqprotocol.h  include/peerprotocol.h  include/xmlnode.h \
include/cryptography.h  include/kitprotocol.h  include/platform.h      include/yahooprotocol.h \
include/dstring.h       include/log.h          include/protocol.h 

CPPFLAGS = -g -Wall

CC = gcc
LDFLAGS = -rdynamic -L/usr/local/lib -L/usr/local/lib/mysql -L$(CRYPTODIR) -version-info $(GM_MAJOR_VERSION):$(GM_MINOR_VERSION)$(GM_MICRO_VERSION)
LIBS = 

EXTRA_DIST = include/authload.h include/basenetwork.h include/contact.h include/cryptography.h \
include/dstring.h include/expat.h include/globals.h include/icqprotocol.h \
include/kitprotocol.h include/log.h include/manager.h include/msnprotocol.h \
include/peerprotocol.h include/platform.h include/protocol.h include/ssh.h \
include/tocprotocol.h include/xml.h include/xmlnode.h include/yahooprotocol.h

