#ifndef KIM_NETWORK_H
#define KIM_NETWORK_H

#include <string>
#include <dstring.h>
#include <xml.h>
#include <buffer.hpp>

#include "crypto/misc.h"

/* Low-Level Network Error Codes */
#define KIM_NETWORK_NOERROR   101 // No error happened.  Connect() calls should use this.
#define KIM_NETWORK_INVOP     102 // Invalid operation.
#define KIM_NETWORK_IOERR     103 // Input/Output error.
#define KIM_NETWORK_INVADDR   104 // Invalid address passed to socket.
#define KIM_NETWORK_INVSOCK	  105 // Invalid socket (uninitialized).
#define KIM_NETWORK_NOHOST    106 // No corresponding host.
#define KIM_NETWORK_INVPORT	  107 // Invalid port.
#define KIM_NETWORK_WOULDBLOCK 108 // The socket is non-blocking and the operation would block.
#define KIM_NETWORK_TIMEDOUT  109 // The timeout for this operation has expired.
#define KIM_NETWORK_MEMERR    110 // Memory exhausted.
#define KIM_NETWORK_UNKNOWNERR 111 // Unknown error.
#define KIM_NETWORK_ALLOCERR  112 // Allocation error, specifically a "new" call

using namespace std;
using namespace CryptoPP;

template <class T>
T NBO(const T& in) { if (CheckEndianess(false)) return byteReverse(in); else return in; }


/**
   Network interface

   This is what the Protocol implementations use to communicate with the outside world.
   the functions here should be implemented in a non blocking fashion whenever possible,
   this is to not interfere with the rest of the client

*/
class Protocol;
class PeerProtocol;

class Network
{
public:
  enum{Connected,
          Closed,
          HostLookupFailure,
          ConnectionRefused,
          FailedRead,
          FailedWrite,
          NotConnected
          };
  Network(Protocol *o): owner(o) {};
  /**
     connect to a specified host returning a socket identifier

  */
  virtual int  connectTo(string host,int port)=0;

  /**
     true iff connected to a host
   */
  virtual bool isConnected() =0;

  /**
     Return how much available data there is waiting to be read.
   */
  virtual int  availableData() =0;

  /**
     Return status
   */
  virtual int  status() { return m_status;}

  /**
     Send data
  */
  virtual void sendData(const char *data,unsigned long len)=0;
  virtual void sendData(const string &data) { sendData(data.c_str(),(unsigned long)data.length());}
  virtual void sendData(const XMLNode &data){string tmp=data; sendData(tmp.c_str(),tmp.length());}
  virtual void sendData(const dstring &data) { sendData((const char*)data.data(), data.length());}

  virtual void sendData(const vbuf& data) = 0;
  /**
      Disconnect
  */
  virtual void disconnect()=0;

  /**
     Call to retrieve all data available
  */
  virtual void socketData(char* &data,int &len)=0;
  virtual void socketData(string& data) = 0;
  virtual void socketData(vbuf& data) = 0;

  /**
     update socket and check for data
  */
  virtual void checkForData()=0;

  /**
      return socket handle
   */
  virtual int getSock()=0;

  /**
      Set our owner. The protocol instance we should pass all incoming data to
  */
  virtual void setOwner(Protocol *p) { owner=p;};
protected:
  Protocol *owner;
  int m_status;
};

/**
 * Network Server Interface
 *
 * This server listens for incomming connections on a specified port
 * and accepts incomming connections, assigning them a Network * object
 *
 */

class NetworkServer
{
public:
  // States
  enum{ Listening, NotListening, Error };

  // Default to port 3000 for listen server
  NetworkServer(PeerProtocol *o, unsigned int p=3000): owner(o), m_listenPort(p) {}

  /** Tells the server to start/stop listening */
  virtual void StartListen()=0;
  virtual void StopListen()=0;

  /** Checks to see if there is an incomming connection */
  virtual bool isIncomming()=0;


  /** Returns a new Network instance if there is a connection */
  virtual Network * getIncomming()=0;


  /** Gets listen server status */
  virtual int status() { return m_status;}

  /** Gets the port that server is listening on or zero for no port */

  virtual unsigned short port() {return m_listenPort;}

  /** Sets owner of server */
  virtual void setOwner(PeerProtocol *p) { owner=p;};

protected:

  PeerProtocol *owner;
  int m_status;
  unsigned short m_listenPort;

};

#endif
