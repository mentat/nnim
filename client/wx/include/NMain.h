// --*-c++-*--
/*
    $Id: NMain.h,v 1.7 2002/06/27 02:54:07 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2001-2002  Jesse Lovelace
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef WX_MAIN_H
#define WX_MAIN_H

#include "NTaskbar.h"

#include "authload.h"
#include "contact.h"
#include "boost/smart_ptr.hpp"

class ProtocolManager;

class wxSocketEvent;
class gmEvent;

/**
 * NNIM Base Class
 *
 * All wxWindows Applications are derived from a wxApp class.
 *
 * This class holds pointers to the GUI windows and is the
 * bridge between wxWindows and non-wx code.
 *
 * @author Jesse Lovelace
 * @version 0.1
 */

using namespace boost;

class wxNNIM: public wxApp
{
public:
	bool OnInit();
	int OnExit();

	bool Shutdown();

	AuthLoad& AccessLoader();


	ProtocolManager& AccessManager();
	
	bool Login(bool newUser = false);
	bool Logout();

	void SendNEvent(gmEvent& event);

	void OnSocketEvent(wxSocketEvent& event);
	void OnServerEvent(wxSocketEvent& event);
	void OnRefreshNetwork(gmEvent& event);
	void OnRefreshContacts(gmEvent& event);

#ifdef _WIN32
protected:
  NNIMTaskBarIcon m_TaskBarIcon;
#endif

private:
    scoped_ptr<wxFrame> m_LogView;
    scoped_ptr<wxFrame> m_LoginView;
    scoped_ptr<wxFrame> m_ContactView;
    scoped_ptr<ProtocolManager> m_ProtoManager;
    scoped_ptr<AuthLoad> m_AuthLoader;

	DECLARE_EVENT_TABLE()

    // ugly, needs to be removed
	friend class guiContact;
	friend class guiLogin;
	friend class guiLog;
    friend class wxProtocolManager;

public:

    enum {
        ID_CONTACTS_BUTT = 18000, 
        ID_CONTACTS_MENU_AWAY,
        ID_CONTACTS_MENU_SET_INFO,
        ID_CONTACTS_MENU_LOGOUT,
        ID_CONTACTS_LOGOUT,
        ID_CONTACTS_DIRECT,
        ID_CONTACTS_COMMIT,
        ID_CONTACTS_RESET_TREE,
        ID_CONTACTS_QUIT,
        ID_CONTACTS_ABOUT,
        ID_CONTACTS_MENU_POPUP_CONNECT,
        ID_CONTACTS_MENU_POPUP_DISCONNECT,
        ID_CONTACTS_MENU_POPUP_SERVICES,
        ID_CONTACTS_MENU_POPUP_PREFS,
        ID_CONTACTS_MENU_POPUP_WIZARD,
        ID_CONTACTS_MENU_POPUP_ADD_CONTACT,
        ID_CONTACTS_MENU_POPUP_QUIT,
        ID_CONTACTS_MENU_POPUP_TESTCRASH,
        ID_CONTACTS_MENU_POPUP_ADD_FOLDER,
        ID_CONTACTS_MENU_POPUP_UPDATE_DEBUG,
        ID_CONTACTS_TASKBAR_RESTORE,
        ID_CONTACTS_TASKBAR_EXIT,
        ID_CONTACTS_TASKBAR_HIDE,
        ID_CONTACTS_INCOMMING_MESSAGE,
        ID_CONTACTS_INCOMMING_MESSAGE_ANONY,
        ID_CONTACTS_STATUS_CHANGE,

        ID_LOGIN_LOGO,
        ID_LOGIN_USERNAME,
        ID_LOGIN_PASSWORD,
        ID_LOGIN_LOGIN,
        ID_LOGIN_MENU_NEWUSER,
        ID_LOGIN_MENU_QUIT,
        ID_LOGIN_MENU_ABOUT,

        ID_PREF_GENERAL,
        ID_PREF_SECURITY,
        ID_PREF_AIM,
        ID_PREF_YAHOO,
        ID_PREF_MSN,
        ID_PREF_ICQ,
        ID_PREF_KIT,
        ID_PREF_PEER,
        ID_PREF_NETWORKS,
        ID_PREF_CANCEL,
        ID_PREF_SAVE,
        ID_PREF_CONTACTS,
        ID_PREF_UPDATED,
        ID_PREF_CONTACT_LIST,
        ID_PREF_CONTACT_COMBO,
        ID_PREF_ADDUSER,
        ID_TREE_CONTROL,
        ID_TREE_ABOUT,
        ID_TREE_CHANGE_STATUS,
        ID_TREE_DELETE,
        ID_TREE_EDIT,
        ID_TREE_ADD_CONTACT,
        ID_TREE_ADD_FOLDER,
        ID_TREE_RENAME,
        ID_MENU_QUIT,

        ID_MENU_ABOUT,
        ID_MENU_CHECKVERSION,
        ID_MENU_OPTIONS,
        ID_MENU_CHATWINDOW,
        ID_MENU_COMMIT,
        ID_MENU_LOAD,
        ID_MENU_CLEAR_CONTACTS,
        ID_MENU_LOGOUT,
        ID_MENU_CONTACT_EDIT,
        ID_MENU_START,
        ID_MENU_POPUP_KIT,
        ID_MENU_POPUP_AIM,
        ID_MENU_POPUP_ICQ,
        ID_MENU_POPUP_PEER,
        ID_MENU_POPUP_CONNECT,
        ID_MENU_POPUP_DISCONNECT,
        ID_MENU_POPUP_SERVICES,
        ID_MENU_POPUP_PREFS,
        ID_MENU_POPUP_WIZARD,
        ID_MENU_POPUP_ADD_CONTACT,

        ID_MENU_POPUP_QUIT,
        ID_PREF_TREE_CTRL,
        ID_PREF_SPLIT,
        ID_PREF_BUTT_OK,
        ID_PREF_BUTT_CANCEL,
        ID_PREF_BUTT_APPLY,
        ID_AWAY_WINDOW,
        ID_NEW_USER_WIZARD,

        PREF_TREEITEM_CONTACT,
        PREF_TREEITEM_FOLDER,
        PREF_TREEITEM_CONTACTROOT,
        PREF_TREEITEM_SECURITY,
        PREF_TREEITEM_NETWORKROOT,
        PREF_TREEITEM_PLUGINS,
        PREF_TREEITEM_GENERAL,

        ID_LOG_QUIT,
        ID_LOG_ENTER,

        SOCKET_ID,
        SERVER_ID,

        ID_NETWORK_REFRESH_CONTACTS,
        ID_NETWORK_REFRESH_NETWORK,

        ID_CHAT_QUIT,
        ID_CHAT_ABOUT,
        ID_CHAT_SEND,
        ID_CHAT_SEND_BUTT,
        ID_CHAT_NOTEBOOK,
        ID_CHAT_P2_BROWSE_BUTT,
        ID_CHAT_P2_FILESEND_BUTT
    };

};

#endif
/*
    -----
    $Log: NMain.h,v $
    Revision 1.7  2002/06/27 02:54:07  thementat
    Changes to the Event handling.

    Revision 1.6  2002/06/26 04:27:07  thementat
    Event fixes.

    Revision 1.5  2002/06/25 19:09:10  thementat
    Added anonymous incoming message handling.

    Revision 1.4  2002/06/19 19:14:43  thementat
    Working towards GCC 3.0.4 compile, many modifications and new automake-1.5 files.

    Revision 1.3  2002/06/14 22:02:23  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.2  2002/06/09 19:45:00  thementat
    Liberal use of auto_ptr.

    Revision 1.1.1.1  2002/06/06 17:21:42  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.9  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.8  2001/12/12 19:25:25  mentat
    Fixes for gcc 2.95.3 and new makefile to build (for now until I sort out automake).
    Builds in FreeBSD4.4 with gcc 2.95.3 and wxWindows 2.3.2

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:59  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i

*/

