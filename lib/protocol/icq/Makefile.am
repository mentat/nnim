INCLUDES = $(all_includes) -I../../include -I.. -Wall


noinst_LTLIBRARIES = libicq.la

libicq_la_SOURCES = util.cpp stdpackets.cpp parsepacket.cpp \
		   parsefnacicq.cpp parsefnacmsg.cpp parsefnacloc.cpp parsefnacgen.cpp \
		   parsefnacbud.cpp parsefnacbos.cpp parsefnac.cpp packets.cpp packet.cpp icqprotocol.cpp fnacpacket.cpp \
		   fnacicqpackets.cpp fnacmsgpackets.cpp fnaclocpackets.cpp fnacgenpackets.cpp \
		   fnacbudpackets.cpp fnacbospackets.cpp

noinst_HEADERS =   fnaclocpackets.h iceq.h packet.h stdpackets.h \
		   fnacbudpackets.h fnacmsgpackets.h packets.h \
		   util.h fnacgenpackets.h fnacpacket.h v7protocol.h \
		   fnacicqpackets.h
