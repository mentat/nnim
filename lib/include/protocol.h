// -*- C++ -*-
#ifndef KIM_PROTOCOL_H
#define KIM_PROTOCOL_H

#include <string>
#include <list>
#include "xmlnode.h"
#include "contact.h"

using namespace std;

class ProtocolManager;
class Network;
/**
 Abstract baseclass for all protocols.
 Defines the interface that all protocols should comply with.

 The various c_* functions are callbacks, override these in a child class if
 you're interested in the event that happens. Typically, a client will override all
 of them. For example c_loggedIn() is called as soon as the login process is finished.


 The states that the protocol can be in are defined here. The current state the protocol is in
 is returned by getState().

 \begin{itemize}
    \item Protocol::S_online       - Online and connected. Ready to send and recive messages and presence notification
    \item Protocol::S_connecting   - Connection in progress
    \item Protocol::S_offline      - Disconnected.
    \item Protocol::S_registring   - While registering a new account
    \item Protocol::S_away         - When the user is online, but away.
    \item Protocol::S_getPubKey    - When a public rsa key is needed (internal to kit protocol only)
 \end{itemize}

 Sometimes everything doesnt go as planned, and a way of reporting error are needed. These are
 the currently defined protocol errors.When an error occurs, c_error() will be called with the error
 code (this) and plaintext description of the error.

 \begin{itemize}
    \item Protocol::E_invalidLogin - Either the password or the username was wrong while attempting to log in
    \item Protocol::E_userTaken    - If the user name was already taken while attempting to register a new user
    \item Protocol::E_network      - A Network error
    \item Protocol::E_success      - An error that's no error :)
 \end{itemize}

 @author Henrik Abelsson <henrik@abelsson.com>
 @version 0.1
 */
class Protocol
{

public:
  enum State { S_online =0, S_connecting, S_offline, S_disconnecting };
  enum Errors { E_invalidLogin, E_userTaken, E_network, E_success };
  enum Status { S_available, S_away, S_na, S_occupied, S_dnd, S_ffc };

  Protocol(const XMLNode &,ProtocolManager *man, const string &name);
  virtual ~Protocol();

  /**
   The current state the protocol is in (as defined in the state enum above)
   */
  virtual State getState() const {return m_state;};

  /**
   The current user status (Away, dnd, etc..)
   */
  virtual Status getStatus() const {return m_status;};

  /**
      The name of the protocol as a string identifer
  */
  virtual const string protocol() {return m_name;};

  /**
     The users username, as displayed to others
  */
  virtual const string screenName()=0;

  /**
      Allow the protocol to update things. Should be called periodically to allow the
      protocols to send keepalive messages and other housekeeping tasks.
  */
  virtual void update() {};

  /** The Action functions
   You call these functions to initiate an event. For example, to send an instant message
   or to login to the server. Hopefully the names should be descriptive enough for you
   to figure out what they do.
   Usually all actions are supported by all protocols with the exception of getPubkey()
  */

  /**
     Login to the server (start the login process)
  */
  virtual void login()=0;

  /**
     Logout and disconnect
  */
  virtual void logout()=0;

  /**
     Send an IM message to a user
  */
  virtual void sendMessage(const Contact &recipient, const string &message)=0;

  /**
     Add a buddy to the presence notification list. You will now be notified of this
     buddies connection status. (with c_stateChange() )
  */
  virtual void addBuddy(const Contact &c)=0;

  /**
     Delete a buddy when you're no longer interested in him :)
  */
  virtual void delBuddy(const Contact &c)=0;

  /**
     Request the servers public key (only KitProtocol supports this, it's a noop
     for the other protocols)
  */
  virtual void getPubkey() {}

  /**
      Register a new user
   */
  virtual void newUser() =0;

  virtual void setAway(const string& msg = "") {} 

  virtual void setInfo(const string& info = "") {}

  virtual void customRequest(XMLNode n) {}

  /** Network functions
  The protocol classes are completely network agnostic - they contain no code to interface
  with the outside world. This is both to increase portability and flexibility. User interfaces
  and network code needs to share a common event system, and the protocol can't know in advance
  how a client may choose to implement the network.

  @see Network - the network interface class
   */

  /**
    Data is available, go fetch yourself.
   */
  virtual void handleData(Network *net)=0;

  /**
    Parse this data.
   */
  virtual void handleData(Network *net,const string& data)=0;

  /**
    A socket or network error has occured, tell the protocol about it.
   */
  virtual void connectionError(Network *net,int error)=0;

  /**
    Connection to the server has been established.
  */
  virtual void connectedToServer(Network *net) {};

  /**
    Clear all buddies for this protocol
  */
  virtual void clearBuddies() { m_buddies.clear(); }

  void setConfig(XMLNode conf) { m_conf=conf;};
  virtual list<Network *> getNetworks() { return m_nets;};

  typedef map<string,Contact > buddy_t;

protected:
	
	void eventLoggedIn();
	void eventLoggedOut();
	void eventRecvdMessage(const Contact &c, const string &message);
	void eventStatusChange(const Contact &c);
	void eventError(int err_no,const string &error);
	void eventStateChange(int state);
	void eventGotPubkey(const string &key);
	void eventGotCustom(const XMLNode &n);
	
protected:

	virtual Network *addNet();
	virtual void removeNet(Network *);
	void eventGotBuddy(const Contact &c);
	
	State m_state;
	Status m_status;
	
	XMLNode m_conf;
	buddy_t m_buddies;

//	Network *m_net;
	list<Network *> m_nets;

private:
	
	string m_name;
  
	ProtocolManager *m_manager;
};

#include "manager.h"

#endif
