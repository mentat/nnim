/*
    $Id: emptyprotocol.h,v 1.1 2002/06/06 17:21:48 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2001  Henrik Abelsson <henrik@abelsson.com>
 
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

    -----
    $Log: emptyprotocol.h,v $
    Revision 1.1  2002/06/06 17:21:48  thementat
    Initial revision

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

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
   A protocol template

   The configuration XML looks like this:
   \begin{verbatim}
    ...
   \end{verbatim}

 */
class EmptyProtocol: public Protocol

{

 public:

  EmptyProtocol(XMLNode &, ProtocolManager *manager);
  virtual ~EmptyProtocol();

  const string protocol(){return "empty";};
  const string screenName() { return ""; };

  virtual void update();

  virtual void login();
  virtual void logout();
  virtual void sendMessage(const Contact &recipient, const string &message);
  virtual void addBuddy(const Contact &c);
  virtual void delBuddy(const Contact &c);
  virtual void newUser();
  virtual void getPubkey();

  virtual void handleData();
  virtual void handleData(const char *buf,int avail);
  virtual void connectionError(int error);
  virtual void connectedToServer();
private:
  enum KitState { S_none=2*65536, S_registering, S_getPubKey};
    Network *m_net; //main network object
};

#endif
