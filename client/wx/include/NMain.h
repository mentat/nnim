// --*-c++-*--
/*
    $Id: NMain.h,v 1.1 2002/06/06 17:21:42 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2001  Jesse Lovelace
 
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

#include "NTaskBar.h"

#include "authload.h"
#include "contact.h"

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

	void SendEvent(gmEvent& event);

	void OnSocketEvent(wxSocketEvent& event);
	void OnServerEvent(wxSocketEvent& event);
	void OnRefreshNetwork(gmEvent& event);
	void OnRefreshContacts(gmEvent& event);

#ifdef _WIN32
protected:
  NNIMTaskBarIcon m_TaskBarIcon;
#endif

private:

	AuthLoad *m_pAuthLoader;
	ProtocolManager *m_pProtoManager;
	wxWindow *m_pContactView;
	wxWindow *m_pLoginView;
	wxWindow *m_pLogView;

	DECLARE_EVENT_TABLE()

	friend class guiContact;
	friend class guiLogin;
	friend class guiLog;

};

#endif
/*
    -----
    $Log: NMain.h,v $
    Revision 1.1  2002/06/06 17:21:42  thementat
    Initial revision

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
