// --*-c++-*--
/*
    $Id: NProtocolManager.cpp,v 1.8 2002/06/27 02:54:08 thementat Exp $
 
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
#include <string>
#include "NInclude.h"

#include "NProtocolManager.h"
#include "NSocket.h"
#include "NMain.h"
#include "NEvent.h"

#include "basenetwork.h"
#include "manager.h"
#include "icqprotocol.h"
#include "kitprotocol.h"
#include "tocprotocol.h"
#include "msnprotocol.h"
#include "yahooprotocol.h"

using namespace std;

DECLARE_APP(wxNNIM)

void
InitProtoManager(AuthLoad & myLoader, ProtocolManager& myMan)
{
    wxLogDebug(wxT("InitProtoManager"));
	XMLNode config;
	
	myLoader.U().GetNet("toc", config);
	if (config.child("user").property("username") != "")
	{
		myMan.addProtocol(new TocProtocol(config, &myMan));
	}
	myLoader.U().GetNet("icq", config);
	if (config.child("user").property("username") != "")
	{
		myMan.addProtocol(new IcqProtocol(config, &myMan));
	}
	myLoader.U().GetNet("msn", config);
	if (config.child("user").property("username") != "")
	{
		myMan.addProtocol(new MsnProtocol(config, &myMan));
	}

	myLoader.U().GetNet("yahoo", config);
	if (config.child("user").property("username") != "")
	{
		myMan.addProtocol(new YahooProtocol(config, &myMan));
	}

	myLoader.U().GetNet("kit", config);
	if (config.child("user").property("username") != "")
	{
		myMan.addProtocol(new KitProtocol(config, &myMan));
	}

}

wxProtocolManager::~wxProtocolManager() 
{

  list<Network *> lMyNetworkSockets = getNetsAll();

  for(list<Network *>::iterator it=lMyNetworkSockets.begin(); it != lMyNetworkSockets.end(); it++)
    ((wxNetwork *)(*it))->Destroy();

}

  // override the c_* functions you are interested in. hopefully the names should be
  // descriptive enough.

Network * wxProtocolManager::createNet(Protocol *proto)
{
  wxNetwork * pMyNet = new wxNetwork(proto);
  pMyNet->SetEventHandler(wxGetApp(), wxNNIM::SOCKET_ID);
  pMyNet->SetNotify(wxSOCKET_CONNECTION_FLAG |
                    wxSOCKET_INPUT_FLAG |
                    wxSOCKET_LOST_FLAG);
  pMyNet->Notify(TRUE);
  return pMyNet;
}

NetworkServer * wxProtocolManager::createServer(Protocol *proto)
{
	return NULL;
}

void wxProtocolManager::removeNet(const Network * n) 
{
}

void wxProtocolManager::c_loggedIn(const string &proto)
{
  wxLogMessage(wxString(wxT("Connected: ")) + wxString(proto.c_str(), wxConvUTF8));
  updateAll();
}

void wxProtocolManager::c_statusChange(const string &proto,const Contact &c)
{
  wxLogMessage(wxString("Status change event for ") + wxString(proto.c_str(), wxConvUTF8) +
    wxString(" ") + wxString(c.nick().c_str(), wxConvUTF8) );

  gmEvent myEvent(gmEVT_STATUS_CHANGE, wxNNIM::ID_CONTACTS_STATUS_CHANGE);
  myEvent.contact = c;

  SendEvent(myEvent);
}

void wxProtocolManager::c_recvdMessage(const string &proto,const Contact &c, const string &message)
{
	wxString wProto(proto.c_str(), wxConvUTF8);
	wxString wMessage(message.c_str(), wxConvUTF8);
	
	gmEvent myEvent(gmEVT_MESSAGE,wxNNIM::ID_CONTACTS_INCOMMING_MESSAGE);
	myEvent.setProtocol(wProto);
	myEvent.setServerId(wxString(c.nick().c_str(), wxConvUTF8));
	myEvent.setMessage(wMessage);

    //wxLogDebug("Hello");
	SendEvent(myEvent);

	//wxLogDebug(wxString(wxT("Message from ")) + wxString(c.nick().c_str(), wxConvUTF8) +
		//wxString(wxT(":")) + wMessage );
}

void wxProtocolManager::c_recvdMessageAnony(const string& proto, const Contact &c, const string& message)
{
    //wxString wProto(proto.c_str(), wxConvUTF8);
	//wxString wMessage(message.c_str(), wxConvUTF8);
	
	gmEvent myEvent(gmEVT_MESSAGE_ANONY,wxNNIM::ID_CONTACTS_INCOMMING_MESSAGE_ANONY);
	myEvent.setProtocol(proto.c_str());
	myEvent.setServerId(c.nick().c_str());
	myEvent.setMessage(message.c_str());

    //wxLogDebug("here123");
	SendEvent(myEvent);

	//wxLogDebug(wxString(wxT("Anonymous Message from ")) +
       // wxString(c.nick().c_str()) + wxString(wxT(":")) +
       // wxString(message.c_str())  );
}

void wxProtocolManager::c_error(const string &proto,int err_no,const string &error)
{
  wxLogMessage(wxString("An error occured: (") + wxString(") ") + wxString(error.c_str(), wxConvUTF8));
}
  
void wxProtocolManager::c_stateChange(const string &proto,int state)
{
  if (state==Protocol::S_online)
    wxLogDebug(wxT("Current state is online"));
  if (state==Protocol::S_connecting)
    wxLogDebug(wxT("Current state is connecting"));
  if (state==Protocol::S_offline)
   wxLogDebug(wxT("Current state is offline"));
}

void wxProtocolManager::SendEvent(gmEvent & event)
{
    wxLogDebug(wxT("SendGMEvent"));

    if (m_handler) {
        m_handler->AddPendingEvent(event);
    }

}

void wxProtocolManager::SetEventHandler(wxEvtHandler& handler, int id)
{
    m_handler = &handler;
    m_id = id;
}

/*
    -----
    $Log: NProtocolManager.cpp,v $
    Revision 1.8  2002/06/27 02:54:08  thementat
    Changes to the Event handling.

    Revision 1.7  2002/06/26 04:27:08  thementat
    Event fixes.

    Revision 1.6  2002/06/25 19:09:11  thementat
    Added anonymous incoming message handling.

    Revision 1.5  2002/06/23 14:50:01  thementat
    Work on TOC protocol and new buffer class.

    Revision 1.4  2002/06/20 01:25:00  thementat
    Removed unicode for the time being to fix linux build.

    Revision 1.3  2002/06/16 04:08:28  thementat
    Hopefully fixed Authload and related classes.


    Revision 1.2  2002/06/14 22:02:24  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.1.1.1  2002/06/06 17:21:29  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2002/01/17 20:00:51  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/
