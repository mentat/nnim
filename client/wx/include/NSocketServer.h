#ifndef WX_KIM_NETWORK_SERVER_H
#define WX_KIM_NETWORK_SERVER_H

#include "basenetwork.h"
#include "wx/socket.h"

/**
 * wxWindows Listen Server Implimentation
 * 
 * Uses the wxWindows socket system to listen at a certain
 * port for incomming connections, if one if found, it is accepted
 * and given to the owning protocol (usually a peer-2-peer)
 *
 * @author Jesse Lovelace
 * @version 0.1
 */
class wxNetworkServer: public NetworkServer, wxSocketServer
{
public:
  // States
	enum{ Listening, NotListening, Error };

  // Default to port 3000 for listen server
	wxNetworkServer(PeerProtocol *o, unsigned int p=3000);

  /** Tells the server to start/stop listening */
	virtual void StartListen();
	virtual void StopListen();

  /** Checks to see if there is an incomming connection */
	virtual bool isIncomming();


  /** Returns a new Network instance if there is a connection */
	virtual Network * getIncomming();

	void checkForIncomming();

protected:

	PeerProtocol *owner;
	int m_status;
	unsigned short m_listenPort;

};

#endif
