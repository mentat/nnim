/*
    $Id: emptyprotocol.cpp,v 1.1 2002/06/06 17:21:47 thementat Exp $
 
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
    $Log: emptyprotocol.cpp,v $
    Revision 1.1  2002/06/06 17:21:47  thementat
    Initial revision

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)
#include <iostream>
#include <string>

#include <stdio.h>

#include "emptyprotocol.h"
#include "log.h"
#include "xml.h"


EmptyProtocol::EmptyProtocol(XMLNode &c, ProtocolManager *manager):
  Protocol(c,manager)
{
  if (manager)
    manager->addProtocol(this);

  debug() << "New emptyProtocol instance" << endl;
  // Here you will probably want to create your main network socket
  m_net = manager->createNet(this);
}

EmptyProtocol::~EmptyProtocol()
{
  debug() << "Deleting emptyProtocol instance" << endl;
}

void EmptyProtocol::login()
{
  kLog("Logging in?");
  m_state=S_connecting;
  eventStateChange(m_state);
}

void EmptyProtocol::connectedToServer()
{

}

void EmptyProtocol::logout()
{

  eventStateChange(S_offline);
}

void EmptyProtocol::update()
{

}


void EmptyProtocol::sendMessage(const Contact &recipient, const string &message)
{
}

void EmptyProtocol::addBuddy(const Contact &c)
{
  m_buddies[c.serverId()]=c;
  m_buddies[c.serverId()].setStatus(Contact::Offline);
  eventStatusChange(c);
}

void EmptyProtocol::delBuddy(const Contact &c)
{
}

void EmptyProtocol::newUser()
{
}

void EmptyProtocol::getPubkey()
{

}

void EmptyProtocol::handleData()
{
  string data;
  char *_data;
  int avail;

  m_net->socketData(_data,avail);

  if (!_data || !avail)
    return;
  handleData(_data,avail);
}

/////////////////// network stuff ////////////////////////////
void EmptyProtocol::handleData(const char *_data,int avail)
{
  if (!_data || !avail)
    return;

}

void EmptyProtocol::connectionError(int error)
{
  m_state = S_offline;

  switch (error)
    {
    case 0:
      kLog("Connection to server was refused.\n");
      break;
    case 1:
      kLog("Server not found\n");
      break;
    case 2:
      kLog("Error reading from socket");
      break;
    }

  m_connectionEncrypted = 0;
  eventStateChange(S_offline);

}


#ifdef TEST

int main()
{
  XMLNode n;
  EmptyProtocol proto(n);
  debug() << "hej" << endl;
}
#endif
