// --*-c++-*--
/*
    $Id: NSocketServer.cpp,v 1.2 2002/06/14 22:02:24 thementat Exp $
 
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

#include "NInclude.h"
#include "NSocket.h"
#include "NSocketServer.h"
#include <wx/socket.h>
#include <wx/protocol/protocol.h>

wxIPV4address ToAddr(int num)
{
	wxIPV4address me;
	me.Service(num);
	return me;
}

wxNetworkServer::wxNetworkServer(PeerProtocol *o, unsigned int p)
: NetworkServer(o, p), wxSocketServer(ToAddr(p))
{
}


  /** Tells the server to start/stop listening */
void wxNetworkServer::StartListen()
{
	Notify(TRUE);
}

void wxNetworkServer::StopListen()
{
	Notify(FALSE);
}

  /** Checks to see if there is an incomming connection */
bool wxNetworkServer::isIncomming()
{
	return false;
}

void wxNetworkServer::checkForIncomming()
{
	//owner->

}


  /** Returns a new Network instance if there is a connection */
Network * wxNetworkServer::getIncomming()
{
	wxNetwork * newSock = new wxNetwork((Protocol *)owner);

	AcceptWith(*newSock);
	return newSock;
}

