// --*-c++-*--
/*
    $Id: NMain.cpp,v 1.8 2002/06/26 04:27:08 thementat Exp $
 
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

#include "NInclude.h"
#include "NMain.h"
#include "NXpms.h"
#include "NLogin.h"
#include "NLog.h"
#include "NContact.h"
#include "NSocket.h"
#include "NSocketServer.h"
#include "NTaskbar.h"
#include "NProtocolManager.h"
#include "NEvent.h"

#include "authload.h"
#include "manager.h"

IMPLEMENT_APP(wxNNIM)

class myAuthLoad: public AuthLoad
{
public:
  virtual ~myAuthLoad() {}

protected:

  virtual void LogText(const string& text) 
  { wxLogDebug(wxString(text.c_str(), wxConvUTF8)); }

};

BEGIN_EVENT_TABLE(wxNNIM, wxApp)
    EVT_SOCKET(wxNNIM::SOCKET_ID, wxNNIM::OnSocketEvent)
    EVT_SOCKET(wxNNIM::SERVER_ID,  wxNNIM::OnServerEvent)
    EVT_CUSTOM(gmEVT_REFRESH_CONTACTS, ID_NETWORK_REFRESH_CONTACTS, wxNNIM::OnRefreshContacts)
    EVT_CUSTOM(gmEVT_REFRESH_NETWORK, ID_NETWORK_REFRESH_NETWORK, wxNNIM::OnRefreshNetwork)
END_EVENT_TABLE()

void wxNNIM::OnServerEvent(wxSocketEvent &event)
{
	wxLogDebug(wxT("Server event."));

	if (!m_ContactView.get() || !m_ProtoManager.get())
		return;

	wxSocketBase * sock = event.GetSocket();
	
}

void wxNNIM::OnSocketEvent(wxSocketEvent &event)
{

    if (!m_ContactView.get() || !m_ProtoManager.get())
	{
		wxLogError(wxT("Error in OnSocketEvent"));
        return;
	}

    wxNetwork * inboundSocket(NULL);

    switch(event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        wxLogDebug(wxT("Socket Event: wxSOCKET_INPUT"));
        inboundSocket = (wxNetwork *)event.GetSocket();
        inboundSocket->checkForData();
        break;
    case wxSOCKET_LOST: 
        wxLogDebug(wxT("Socket Event: wxSOCKET_LOST"));
        inboundSocket = (wxNetwork *)event.GetSocket();
        inboundSocket->errClosed();
        break;
    case wxSOCKET_CONNECTION: 
        wxLogDebug(wxT("Socket Event: wxSOCKET_CONNECTION"));
        inboundSocket = (wxNetwork *)event.GetSocket();
        inboundSocket->connected();
        break;
    default: 
        wxLogDebug(wxT("Unexpected event !")); 
        break;
  }

}

void wxNNIM::OnRefreshNetwork(gmEvent& event)
{
  wxLogDebug(wxT("Got refresh network event."));
  wxLogDebug(event.getProtocol());

  bool reLogin = false;


  if (AccessManager().getState(event.getProtocol().c_str()) != Protocol::S_offline)
    reLogin = true;
}

void wxNNIM::OnRefreshContacts(gmEvent& event)
{
  wxLogDebug(wxT("Got refresh contacts event."));
}

bool wxNNIM::OnInit()
{ 
    m_LogView.reset( InitLogView(NULL) );

	m_LoginView.reset( InitLoginView(NULL) );
	
	SetTopWindow(m_LoginView.get());

	// Special directory access needs to be gathered from registry/home dir here and passed
	// to the AuthLoad constructor.
    m_AuthLoader.reset( new myAuthLoad() );

#ifdef _WIN32
    // If compiled for win32, add taskbar icon
	wxIcon appIcon;
	appIcon.CopyFromBitmap(wxBitmap(nnim_i_xpm));
	if (!m_TaskBarIcon.SetIcon(appIcon, wxT("GM: NNIM")))   
		wxLogError(wxT("Could not set taskbar icon."));   
#endif

  return true;
}

bool wxNNIM::Logout()
{
    // do a final save
	m_AuthLoader->CommitToFile();

    // destroy configuration manager
	m_AuthLoader.reset( NULL );

    // if the contact view exits, destroy it
	m_ContactView.reset( NULL );

    // recreate the login winodw
	m_LoginView.reset( InitLoginView(NULL) );
	
    // set it as top
	SetTopWindow(m_LoginView.get());

    // recreate the auth loader
    m_AuthLoader.reset( new myAuthLoad() );
  
	return true;

}

int wxNNIM::OnExit() // Called as App is dying
{
	Shutdown();
  
	return 0;
}

void wxNNIM::SendNEvent(gmEvent& event)
{
    wxLogDebug(wxT("wxNNIM::SendEvent"));
    // if Contact view is ok send event
    if (m_ContactView.get() != NULL)
    {
        wxLogDebug(wxT("Sending event..."));
        m_ContactView->ProcessEvent(event);
        return;
    }
    wxLogDebug(wxT("Event not sent..."));

}

bool wxNNIM::Login(bool newUser)
{
    // create contact window
    m_ContactView.reset(InitContactView(NULL, false) );
    SetTopWindow(m_ContactView.get());

    // create protocol manager
    m_ProtoManager.reset ( new wxProtocolManager(this) );

    // Load all protocol network information into the protocol manager 
    // from the authload class
    InitProtoManager(AccessLoader(), AccessManager());

    // destroy the login window

    m_LoginView.reset( NULL );

    return true;

}

AuthLoad& 
wxNNIM::AccessLoader()
{
    if (!m_AuthLoader.get())
        throw gmException("Pointer Error", gmException::gFATAL);
    return *m_AuthLoader;

}

ProtocolManager&
wxNNIM::AccessManager()
{
    if (!m_ProtoManager.get())
        throw gmException("Pointer Error", gmException::gFATAL);

    return *m_ProtoManager;
}

bool wxNNIM::Shutdown() // a callable shutdown command
{

    m_LogView.reset( NULL );
    m_AuthLoader.reset ( NULL );
    m_ProtoManager.reset( NULL );
    m_ContactView.reset( NULL );
    m_LoginView.reset( NULL );

	return true;
}
/*
    -----
    $Log: NMain.cpp,v $
    Revision 1.8  2002/06/26 04:27:08  thementat
    Event fixes.

    Revision 1.7  2002/06/23 14:50:01  thementat
    Work on TOC protocol and new buffer class.

    Revision 1.6  2002/06/20 16:21:56  thementat
    Many GCC fixes and updates to configure engine.

    Revision 1.5  2002/06/20 01:25:00  thementat
    Removed unicode for the time being to fix linux build.

    Revision 1.4  2002/06/14 22:02:24  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.3  2002/06/10 12:43:25  thementat
    Fixed bug that prevented shutdown (related to auto pointers)

    Revision 1.2  2002/06/09 19:45:03  thementat
    Liberal use of auto_ptr.

    Revision 1.1.1.1  2002/06/06 17:21:29  thementat
    Checkin of new sources BETA 2

    Revision 1.10  2002/01/17 20:00:51  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.

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

