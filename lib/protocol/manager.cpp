/*
    $Id: manager.cpp,v 1.2 2002/06/26 17:40:12 thementat Exp $

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
    $Log: manager.cpp,v $
    Revision 1.2  2002/06/26 17:40:12  thementat
    Added the Open-Source ssh2 lib from Bitvise.

    Revision 1.1.1.1  2002/06/06 17:21:48  thementat
    Checkin of new sources BETA 2

    Revision 1.9  2001/12/16 19:46:50  mentat
    Updates to TOC protocol and authload class.

    Revision 1.8  2001/12/13 17:24:54  mentat
    Manager now returns Protocol::s_Offline if the protocol isnt loaded.

    Revision 1.7  2001/12/06 04:46:40  mentat
    Added setAway() and setAllAway(...) to manager class and to toc protocol, also added changes to toc so that will log in with wx client.

    Revision 1.6  2001/11/24 00:32:44  henrik
    Addcontact dialog restructuring, chat improvements, login/logout improvements.

    Revision 1.5  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.4  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)

#include <log.h>

#include "manager.h"
#include <iostream>

using namespace std;

ProtocolManager::ProtocolManager()
{

}

ProtocolManager::~ProtocolManager()
{

}

void ProtocolManager::removeProtocol(const string &proto)
{
  logout(proto);
  Protocol * myProto = protocol(proto);

  if (!myProto)
    return;

  m_provider.erase(proto);

  delete myProto;

}


void ProtocolManager::addProtocol(Protocol *proto)
{

  if (proto)
  {
    debug() << "adding " << proto->protocol() << " to manager" << endl;
    m_provider[proto->protocol()]=proto;
  }
}

int ProtocolManager::getState(const string &proto)
{
  if (m_provider.count(proto))
    return m_provider[proto]->getState(); // needs to return

  return Protocol::S_offline; 

}

const vector<string> ProtocolManager::protocols()
{

  vector<string> tmp;
  for  (provider_t::iterator i=m_provider.begin();i!=m_provider.end();i++)
    tmp.push_back(i->first);

  return tmp;

}

Protocol *ProtocolManager::protocol(const string &proto)
{

  if (m_provider.count(proto))
    return m_provider[proto];
  else return NULL;
}

const string ProtocolManager::screenName(const string &proto)
{

  if (m_provider.count(proto))
    return m_provider[proto]->screenName();

  return "";

}

void ProtocolManager::update(const string &proto)
{

  if (m_provider.count(proto))
    m_provider[proto]->update();
}

void ProtocolManager::updateAll()
{

  for  (provider_t::iterator i=m_provider.begin();i!=m_provider.end();i++)
    i->second->update();
}

void ProtocolManager::setAway(const string& proto, const string& msg)
{
  
  if (m_provider.count(proto))
    m_provider[proto]->setAway(msg);
}

void ProtocolManager::setAllAway(const string& msg)
{

  for (provider_t::iterator i=m_provider.begin();i!=m_provider.end();i++)
    i->second->setAway(msg);
}

void ProtocolManager::setInfo(const string& proto, const string& info)
{
  if (m_provider.count(proto))
    m_provider[proto]->setInfo(info);

}

void ProtocolManager::setAllInfo(const string& info)
{
  for (provider_t::iterator i=m_provider.begin();i!=m_provider.end();i++)
    i->second->setInfo(info);
}

void ProtocolManager::login(const string &proto)
{

  if (m_provider.count(proto))
    m_provider[proto]->login();

}

void ProtocolManager::logout(const string &proto)
{
  
  if (proto != "" && m_provider.count(proto))
    m_provider[proto]->logout();

}

void ProtocolManager::sendMessage(const string &proto,const Contact &recipient, const string &message)
{

  if (m_provider.count(proto))
    m_provider[proto]->sendMessage(recipient,message);

}

void ProtocolManager::addBuddy(const string &proto,const Contact &c)
{

  if (m_provider.count(proto))
    m_provider[proto]->addBuddy(c);

}

void ProtocolManager::delBuddy(const string &proto,const Contact &c)
{

  if (m_provider.count(proto))
    m_provider[proto]->delBuddy(c);

}

void ProtocolManager::getPubkey(const string &proto)
{

  if (m_provider.count(proto))
    m_provider[proto]->getPubkey();

}

void ProtocolManager::newUser(const string &proto)
{

  if (m_provider.count(proto))
    m_provider[proto]->newUser();


}

void ProtocolManager::wipeBuddies(const string& proto)
{
  if (m_provider.count(proto))
    m_provider[proto]->clearBuddies();

}

void ProtocolManager::customRequest(const string &proto,const XMLNode &n)
{

  if (m_provider.count(proto))
    m_provider[proto]->customRequest(n);

}

void ProtocolManager::c_loggedIn(const string &proto)
{

}

void ProtocolManager::c_loggedOut(const string &proto)
{

}

//void ProtocolManager::c_recvdMessage(const string &proto,const Contact &c, const string &message)
//{

//}

void ProtocolManager::c_statusChange(const string &proto,const Contact &c)
{

}

void ProtocolManager::c_error(const string &proto,int err_no,const string &error)
{

}

void ProtocolManager::c_stateChange(const string &proto,int state)
{

}

void ProtocolManager::c_gotPubkey(const string &proto,const string &key)
{

}

void ProtocolManager::c_custom(const string &proto,const XMLNode &n)
{

}

void ProtocolManager::c_gotBuddy(const string &proto,const Contact &c)
{

  if (m_provider.count(c.protocol()))

    m_provider[c.protocol()]->addBuddy(c);

  c_statusChange(c.protocol(),c);

}

list<Network*> ProtocolManager::getNets(const string &proto)
{

  if (m_provider.count(proto))
    return m_provider[proto]->getNetworks();

  // temp fix for "not all paths return a value
  list<Network *> dummy;

  return dummy;

}

list<Network*> ProtocolManager::getNetsAll()
{

  list<Network *> tmp;

  for  (provider_t::const_iterator i=m_provider.begin();i!=m_provider.end();i++)

  {

    list<Network *> nets=i->second->getNetworks();

    for (list<Network*>::const_iterator j=nets.begin();j!=nets.end();j++)

    {

      tmp.push_back(*j);

    }

  }

  return tmp;

}

