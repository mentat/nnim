// -*- C++ -*-
/*
    $Id: kitprotocol.h,v 1.2 2002/06/23 18:35:51 thementat Exp $

    GNU Messenger - The secure instant messenger

	Copyright (C) 1999-2002  Henrik Abelsson <henrik@abelsson.com>

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
