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

