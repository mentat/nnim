// --*-c++-*--
/*
    $Id: NProtocolManager.cpp,v 1.1 2002/06/06 17:21:29 thementat Exp $
 
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

DECLARE_APP(wxNNIM)

void
InitProtoManager(AuthLoad & myLoader, ProtocolManager& myMan)
{
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
  pMyNet->SetEventHandler(wxGetApp(), SOCKET_ID);
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

  gmEvent myEvent(gmEVT_STATUS_CHANGE, ID_CONTACTS_STATUS_CHANGE);
  myEvent.contact = c;

  wxGetApp().SendEvent(myEvent);
}

void wxProtocolManager::c_recvdMessage(const string &proto,const Contact &c, const string &message)
{
	wxString wProto(proto.c_str(), wxConvUTF8);
	wxString wMessage(message.c_str(), wxConvUTF8);
	
	gmEvent myEvent(gmEVT_MESSAGE,ID_CONTACTS_INCOMMING_MESSAGE);
	myEvent.setProtocol(wProto);
	myEvent.setServerId(wxString(c.nick().c_str(), wxConvUTF8));
	myEvent.setMessage(wMessage);

	wxGetApp().SendEvent(myEvent);

	wxLogMessage(wxString(wxT("Message from ")) + wxString(c.nick().c_str(), wxConvUTF8) + 
		wxString(wxT(":")) + wMessage );
}

void wxProtocolManager::c_error(const string &proto,int err_no,const string &error)
{
  wxLogMessage(wxString("An error occured: (") + wxString(") ") + wxString(error.c_str(), wxConvUTF8));
}
  
void wxProtocolManager::c_stateChange(const string &proto,int state)
{
  if (state==Protocol::S_online)
    wxLogMessage(wxT("Current state is online"));
  if (state==Protocol::S_connecting)
    wxLogMessage(wxT("Current state is connecting"));
  if (state==Protocol::S_offline)
   wxLogMessage(wxT("Current state is offline"));
}
/*
    -----
    $Log: NProtocolManager.cpp,v $
    Revision 1.1  2002/06/06 17:21:29  thementat
    Initial revision

    Revision 1.1  2002/01/17 20:00:51  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/
