/*
    $Id: protocol.cpp,v 1.2 2002/06/25 16:48:14 thementat Exp $

    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2001  Emil Styrke <emil@lysator.liu.se>

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
    $Log: protocol.cpp,v $
    Revision 1.2  2002/06/25 16:48:14  thementat
    Got TOC done! (mostly)

    Revision 1.1.1.1  2002/06/06 17:21:48  thementat
    Checkin of new sources BETA 2

    Revision 1.5  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.4  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)
#include <log.h>
#include "manager.h"
#include "protocol.h"
#include "basenetwork.h"

using namespace std;

Protocol::Protocol(const XMLNode &conf,ProtocolManager *manager,
                   const string &name="unnamedProto"):
  m_conf(conf), m_manager(manager)
{
    m_name = name;
    if (manager)
        manager->addProtocol(this);
}

Protocol::~Protocol()
{

}

void Protocol::eventLoggedIn()
{
  if (m_manager)
    m_manager->c_loggedIn(protocol());
}
void Protocol::eventLoggedOut()
{
  if (m_manager)
    m_manager->c_loggedOut(protocol());

}

void Protocol::eventRecvdMessageNotBuddy(const Contact &c, const string &message)
{
    debug() << "m_manage:" << (int)m_manager << endl;
    debug() << message << endl;
    if (m_manager)
        m_manager->c_recvdMessageAnony(protocol(),c,message);

}
void Protocol::eventRecvdMessage(const Contact &c, const string &message)
{
  debug() << "m_manage:" << (int)m_manager << endl;
  debug() << message << endl;
  if (m_manager)
    m_manager->c_recvdMessage(protocol(),c,message);

}
void Protocol::eventStatusChange(const Contact &c)
{
  if (m_manager)
    m_manager->c_statusChange(protocol(),c);
}
void Protocol::eventError(int err_no,const string &error)
{
  if (m_manager)
    m_manager->c_error(protocol(),err_no,error);
}
void Protocol::eventStateChange(int state)
{
  if (m_manager)
    m_manager->c_stateChange(protocol(),state);
}
void Protocol::eventGotPubkey(const string &key)
{
  if (m_manager)
    m_manager->c_gotPubkey(protocol(),key);
}

Network *Protocol::addNet()
    {
        Network *n = m_manager->createNet(this);
        n->setOwner(this);
        m_nets.push_back(n);
        for (list<Network *>::iterator it=m_nets.begin();it!=m_nets.end();it++)
            debug() << "Net ";
        debug() << endl;
        return n;
    }

void Protocol::removeNet(Network *n)
    {
        m_nets.remove(n);
        m_manager->removeNet(n);
    }

void Protocol::eventGotBuddy(const Contact &c)
{
  if (m_manager)
    m_manager->c_gotBuddy(protocol(),c);
}
void Protocol::eventGotCustom(const XMLNode &n)
{
  if (m_manager)
    m_manager->c_custom(protocol(),n);
}
