/*
    $Id: icqprotocol.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $

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
*/


#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <log.h>

#include "icqprotocol.h"

#include "icq/packets.h"

IcqProtocol::IcqProtocol(const XMLNode &c,ProtocolManager *manager):
  Protocol(c, manager, "icq")
{
  m_state = S_offline;
  m_status = S_available;
  m_net = manager->createNet(this);
  debug() << "New IcqProtocol instance" << endl;
}

IcqProtocol::~IcqProtocol()
{
  logout();
  //removeNet(m_net);
}

void IcqProtocol::login()
{
  m_state = S_connecting;
  _istate = AUTHORIZER;


  if (!m_conf.hasChild("server"))
    return;

  if (m_conf.child("user")["username"]=="")
    return;

  if (m_conf.child("server")["host"] == "")
    m_conf.child("server")["host"] = "login.icq.com";
  if (m_conf.child("server")["port"] == "")
    m_conf.child("server")["host"] = "5190";

  m_net->connectTo(m_conf.child("server")["host"],
		   m_conf.child("server").intProperty("port"));
  eventStateChange(S_connecting);
}

void IcqProtocol::logout()
{
  m_net->sendData(PktCSignoff(_seq++));
  m_net->disconnect();
  m_state = S_offline;
  eventStateChange(S_offline);
  for (buddy_t::iterator i=m_buddies.begin();i!=m_buddies.end();i++)
  {
	  i->second.setStatus(Contact::Offline);
	  eventStatusChange(i->second);
  }
}

/*void IcqProtocol::setStatus(uint32 status)
{
  _status = status;
  net_send(PktCGenSetStatus(_seq++, _status));
  return 0;
  }*/

void IcqProtocol::sendMessage(const Contact &c, const string &message)
{
  m_net->sendData(PktCMsgSend(_seq++, c.serverId(), message));
}

void IcqProtocol::addBuddy(const Contact &c)
{
  m_buddies[c.serverId()] = c;
  m_buddies[c.serverId()].setStatus(Contact::Offline);
  eventStatusChange(m_buddies[c.serverId()]);
  if (m_state == S_online)
    m_net->sendData(PktCBudAdd(_seq++, c));

}

void IcqProtocol::delBuddy(const Contact &c)
{
  m_buddies.erase(c.serverId());
  if (m_state == S_online)
    m_net->sendData(PktCBudDel(_seq++, c));
}

void IcqProtocol::connectedToServer(Network *net)
{
  cerr << "net_connected()" << endl;
}

void IcqProtocol::connectionError(Network *net,int error)
{
  (void)error;

//  eventLoggedOut();
  eventStateChange(S_offline);
  m_state=S_offline;
}

void IcqProtocol::handleData(Network *net,const string& buf)
{
  dstring data;

  //int status = net_recv(data);
  data = dstring(buf);

  /*  if (status <= 0)
    {
      cerr << "net_readyRead(): ERror!!" << endl;
      return;
      }*/

  _recvbuf += data;

  while(1)
    {
      if (_recvbuf.length() < 6)
	return;

      dstring::iterator it;
      unsigned int i;

      Header hdr(_recvbuf);

      if (_recvbuf.length() < (6 + hdr.len()))
	return;

      it=_recvbuf.begin();
      for (i=0;i<6;i++)
	it++;

      _recvbuf.erase(_recvbuf.begin(), it);

      it=_recvbuf.begin();
      for (i=0;i<hdr.len();i++)
	it++;

      data.clear();
      data.insert(data.begin(), _recvbuf.begin(), it);
      _recvbuf.erase(_recvbuf.begin(), it);

      parsePacket(Packet(hdr, data));
    }
}

/*int IcqProtocol::run()
{
  if (n.run()== -1)
    {
      disconnected("No reason"); //Call virtual function
      _state=OFFLINE;
      return -1;
    }

  if (waiting == false)
    {
      if (n.dataAvail() < 6)
	{
	  return 0;
	}
      else
	{
	  DEBUG_PRINT(9, "Received header");
	  waiting = true;
	  dstring data;
	  n.recv(data, 6);
	  curHdr = Header(data);
	  return 1;
	}
    }
  else
    {
      DEBUG_PRINT(10, "Waiting for data...");
      if (n.dataAvail() < curHdr.len())
	{
	  return 0;
	}
      else
	{
	  DEBUG_PRINT(8, "Full packet received");
	  waiting = false;
	  dstring data;
	  n.recv(data, curHdr.len());
	  parsePacket(Packet(curHdr, data));
	  return 1;
	}
    }
  return 0;
  }*/

void IcqProtocol::packetError(const Packet *p, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  fprintf(stderr, "Packet level error: ");
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  p->dump();
}
/*
    -----
    $Log: icqprotocol.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.9  2001/12/08 21:45:27  mentat
    Added setAway and setAllAway to Protocol Manager along with modifying some protocol code.

    Revision 1.8  2001/11/12 07:11:18  henrik
    Logging on win32, icq buddies dont go offline when you disconnect bug fixed, minor expat cleanup.

    Revision 1.7  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.6  2001/10/05 21:49:14  abelsson
    Improved contact handling.

    Revision 1.5  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.4  2001/10/04 16:26:37  abelsson
    Added missing protocol status callbacks.

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
