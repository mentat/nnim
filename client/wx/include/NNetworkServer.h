#ifndef NNETWORKSERVER_H
#define NNETWORKSERVER_H

#include "basenetwork.h"
#include "wx/socket.h"

class wxNetworkServer: public NetworkServer, wxSocketServer
{
public:
  // States
  enum{ Listening, NotListening, Error };

  // Default to port 3000 for listen server
  wxNetworkServer(PeerProtocol *o, unsigned int p=3000): owner(o), m_listenPort(p)

  /** Tells the server to start/stop listening */
  virtual void StartListen() =0;
  virtual void StopListen()=0;

  /** Checks to see if there is an incomming connection */
  virtual bool isIncomming()=0;


  /** Returns a new Network instance if there is a connection */
  virtual Network * getIncomming()=0;


  /** Gets listen server status */
  virtual int status() { return m_status;}

  /** Gets the port that server is listening on or zero for no port */

  virtual unsigned short port() {return m_port;}

  /** Sets owner of server */
  virtual void setOwner(PeerProtocol *p) { owner=p;};

protected:

  PeerProtocol *owner;
  int m_status;
  unsigned short m_listenPort;

};

#endif