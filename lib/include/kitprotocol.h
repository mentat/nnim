// -*- C++ -*-
#ifndef KIM_KIT_PROTOCOL_H
#define KIM_KIT_PROTOCOL_H

#ifdef _WIN32
#pragma warning(disable:4786)
#endif


#include <vector>
#include <string>

#include "protocol.h"
#include "basenetwork.h"
#include "expat.h"
class XMLNode;

/**
   The kit protocol implementation.

   The configuration XML looks like this:
   \begin{verbatim}
    <kit>
       <server host="a.kitserver.com" port="4444" />
       <user username="username" password="secret.password" />
    </kit>
   \end{verbatim}

 */
class KitProtocol: public Protocol, private Expat

{

 public:

  KitProtocol(const XMLNode &, ProtocolManager *manager);
  virtual ~KitProtocol();

  const string screenName() { return m_conf.child("user")["username"]; };
  virtual void update();

  virtual void login();
  virtual void logout();
  virtual void sendMessage(const Contact &recipient, const string &message);
  virtual void addBuddy(const Contact &c);
  virtual void delBuddy(const Contact &c);
  virtual void newUser();
  virtual void getPubkey();
  virtual void customRequest(XMLNode n);

  virtual void handleData(Network *net);
  virtual void handleData(Network *net,const string& buf);
  virtual void connectionError(Network *net,int error);
  virtual void connectedToServer(Network *net);
private:
  enum KitState { S_none=2*65536, S_registering, S_getPubKey};
  void rootTagRecieved(XMLNode &root);
  void recievedTag(XMLNode &n);
  void parserError(string msg,int line);
  void updateContacts();
  void sendData(const string &str);

  int m_connectionEncrypted;
  unsigned char *m_sessionKey;
  int m_sessionKeyLen;
  string toParse;
  KitState m_kitstate;
  Network *m_net;

};

#endif
