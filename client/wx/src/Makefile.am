INCLUDES = `wx-config --cflags` \
    -I../../../crypto -I../../../lib/include \
    -I../include -I../../../lib/protocol/msn \
	-I../../../lib/protocol/icq \
    -I../../../ -I.

bin_PROGRAMS = nnim

nnim_SOURCES =    \
NAbout.cpp        \
NAutoTxt.cpp      \
NChat.cpp         \
NContact.cpp      \
NContactTreeXML.cpp \
NContactWiz.cpp   \
NEvent.cpp        \
NLog.cpp          \
NLogin.cpp        \
NMain.cpp         \
NPrefGen.cpp      \
NPrefNet.cpp      \
NPrefs.cpp        \
NPrefSec.cpp      \
NProtocolManager.cpp \
NProtocol_wdr.cpp \
NSocket.cpp       \
NSocketServer.cpp \
NTaskbar.cpp      \
NTreeCtrlXML.cpp  \
NUserWiz.cpp      \
NWizards_wdr.cpp

nnim_LDADD = ../../../lib/libGM.a ../../../crypto/libcryptopp.a



LDFLAGS = `wx-config --libs` \
	-L../../../crypto -L$(GMDIR)
