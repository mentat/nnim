#ifndef KIM_SOCKET_H
#define KIM_SOCKET_H

#include "basenetwork.h"
#include "wx/socket.h"
#include "buffer.hpp"

using namespace std;

class Protocol;

/**
 * wxWindows implementation of the Network class
 *
 * This class encapsulates a socket and event functions
 * for cross-platform communication
 *
 * @author Jesse Lovelace
 * @version 0.1
 */

class wxNetwork: public Network, public wxSocketClient
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

  wxNetwork(Protocol *proto);

  /**
     connect to a specified host returning a socket identifier

  */
  virtual int connectTo(string host,int port);

  /**
     true iff connected to a host
   */
  virtual bool isConnected();

  /**
     Return how much available data there is waiting to be read.
   */
  virtual int availableData() { return 0; }

  /**
     Return status
   */
  virtual int status() { return m_status;}

  /**
     Send data
  */
  virtual void sendData(const char *data,int len);
  virtual void sendData(const vbuf& data);
 
  /**
      Disconnect
  */
  virtual void disconnect();

  /**
     Call to retrieve all data available
  */
  virtual void socketData(char* &data,int &len);
  virtual void socketData(string & data);
  virtual void socketData(vbuf & data);

  /**
     update socket and check for data
  */
  virtual void checkForData();

  /**
      return socket handle
   */
  virtual int getSock() { return 0; }

  void connected();
  void connectionError(int error);
  void errClosed();

};

#endif
