#ifndef KIM_PROTOCOL_MANAGER_H
#define KIM_PROTOCOL_MANAGER_H

#include <string>
#include <vector>
#include <protocol.h>

using namespace std;

/**
   ProtocolManager - Manages a number of different protocols

   In order to make it easier to write clients this class in the only one that is needed to
   interace with multiple protocols. You create a stand alone protocol as usual, then add it
   to the manager which will then take care of routing requests and callbacks to the right
   protocol.

   Just change the protocol string depending on which protocol you're interested in

 */
class Network;
class NetworkServer;

class ProtocolManager
{
public:
    ProtocolManager();
    virtual ~ProtocolManager();

  /**
     Add a new protocol
(don't call directly.. will be called internally by the protocols)
   */
  void addProtocol(Protocol *);


  void removeProtocol(const string& proto);

  int getState(const string &proto);
  const vector<string> protocols();
  Protocol *protocol(const string &proto);

  const string screenName(const string &proto);

  /* call periodically */
  void update(const string &proto);
  void updateAll();

  /* actions */

  /**
     This should create a new system dependant Network pointer and return it. It's called when a protocol
     needs a new socket.

     Also should add the Network to any event monitoring it may do.
   */
  virtual Network *createNet(Protocol *n)=0;
  virtual NetworkServer * createServer(Protocol *proto)=0;

    /** Should ONLY remove the socket from event handling! DON'T delete it!
*/
    virtual void removeNet(const Network *n) {};
  /**
     Returns all Network* associated with a given protocol.
  */
  list<Network*> getNets(const string &proto);

  /**
     Returns all Network*s currently in use.
  */
  list<Network*> getNetsAll();

  void login(const string &proto);

  void setAway(const string& proto, const string& msg);
  void setAllAway(const string& msg);
  void setInfo(const string& proto, const string& info);
  void setAllInfo(const string& info);

  void logout(const string &proto);
  void sendMessage(const string &proto,const Contact &recipient, const string &message);


  void addBuddy(const string &proto,const Contact &c);

  void delBuddy(const string &proto,const Contact &c);
  void getPubkey(const string &proto);
  void newUser(const string &proto);
  void customRequest(const string &proto,const XMLNode &n);
  void wipeBuddies(const string& proto);

  /**
     The login process has succeded and the client is connected
  */
  virtual void c_loggedIn(const string &proto);

  /**
     The client has been logged out
   */
  virtual void c_loggedOut(const string &proto);

  /**
     A message has come! Someone is interested in talking to our user
   */
  virtual void c_recvdMessage(const string &proto,const Contact &c, const string &message);

  /**
    The status for a contact has changed (May have gone offline, or gone away)
    @see Contact
   */
  virtual void c_statusChange(const string &proto,const Contact &c);

  /**
     An error has occured.
     @param errno The error code
     @param error Human readable error description
   */
  virtual void c_error(const string &proto,int err_no,const string &error);

  /**
     Our own state (as opposed to c_statusChange for buddy states) has changed.
   */
  virtual void c_stateChange(const string &proto,int state);

  /**
    We've gotten the servers public key (only kitprotocol)
   */
  virtual void c_gotPubkey(const string &proto,const string &key);

  /**
    We've gotten a buddy of the server
   */
  virtual void c_gotBuddy(const string &proto,const Contact &c);

  virtual void c_custom(const string &proto,const XMLNode &n);

  typedef map<string,Contact > buddy_t;
private:
  typedef map<string,Protocol *> provider_t;
  provider_t m_provider;
};
#endif
