bin_PROGRAMS = nnim

nnim_SOURCES = gui_chat.cpp gui_contact.cpp gui_contact_tree.cpp \
	gui_log.cpp gui_login.cpp gui_prefs.cpp gui_xpms.cpp \
	wx_main.cpp wx_protocol.cpp wxgui_wdr.cpp gui_taskbar.cpp \
	wxnetwork.cpp

INCLUDES= $(all_includes) `$(WXCONFIG) --cxxflags` \
    -I$(CRYPTODIR) -I$(GMDIR) \
	`$(GTKCONFIG) --cflags` -I../include -I$(GMDIR)/protocol/msn \
	-I$(GMDIR)/protocol/icq

LDFLAGS = `$(WXCONFIG) --libs` `$(WXCONFIG) --cxxflags` `$(GTKCONFIG) --cflags` \
	-L$(CRYPTODIR) -L$(GMDIR)  `$(GTKCONFIG) --libs` -lgm -lcryptopp -lstdc++
