/*
    $Id: parsepacket.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsepacket.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.5  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.4  2001/10/27 18:29:13  estyrke
    Fixed icq parser bug.. #475544

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#include "icqprotocol.h"
#include <basenetwork.h>
#include <log.h>
#include "stdpackets.h"

int IcqProtocol::parsePacket(const Packet &p)
{
  int ret = 0;
  // Login?
  debug() << "Packet ";

  switch (p.hdr().channel())
    {
    case 1:

      //if (SPacket::isType<PktSConnAck>(p))
	  //{
	  debug() << "> ConnAck" << endl;
	  if (_istate==AUTHORIZER)
	    {
	      // New connection Negotiation
	      // Request cookie
	      debug() << "Requesting cookie" << endl;
	      m_net->sendData(PktCCookieRequest(_seq++,
						m_conf.child("user")["username"],
						m_conf.child("user")["password"]));
	    }
	  else if (_istate == BOS)
	    {
	      m_net->sendData(PktCSignon(_seq++, _cookie));
	    }
	  //}
      //else
	  //{
	  //	    packetError(&p, "Unknown packet on channel 1\n");
      //}
      break;
    case 2:
      // FNAC data
      ret = parseFnac(p);
      break;
    case 3:
      // FLAP level errors
      break;
    case 4:
      // Close Connection Negotiation
      if (_istate == AUTHORIZER)
	{
	  PktSCookieReply r(p);

	  if (r.ok())
	    {
	      debug() << "> CookieReply" << endl;
	      _cookie = r.cookie();
//	      DEBUG_PRINT(8, "connecting to %s:%d", r.addr().c_str(), r.port());
	      m_net->disconnect();
	      m_net->connectTo(r.addr(), r.port());
	      m_state = S_connecting;
	      _istate=BOS;
	      break;
	    }
	  else
	    {
	      debug() << "> Error" << endl;
	      TLV t;
	      string uin, strerror;
	      uint16 interror=0;
	      Packet tmp = p;
	      
	      while(tmp.getTLV(t))
		{
		  if (t.type() == TLV::TLV_UIN)
		    uin = t.value();
		  else if (t.type() == TLV::TLV_ERRORCODE)
		    interror = charToU16(t.value());
		  else if (t.type() == TLV::TLV_ERRORMSG)
		    strerror = t.value();
		}
	      switch (interror)
		{
		case 0x0005:
		  // Wrong password
		  m_state = S_offline;
		  m_net->disconnect();
		  eventError(interror, "Bad password");
		  eventLoggedOut();
		  break;
		case 0x0018:
		  //Reconnecting too fast or something...
		  m_state = S_offline;
		  m_net->disconnect();
		  eventError(interror, "Reconnecting too fast: " + strerror);
		  eventLoggedOut();
		  //icq_loggedOut("0x0018: Reconnecting too fast: " + string(msg.value()));
		  break;
		default:
		  cerr << "Error in cookie reply" << endl;
		  cerr << "Uin:   " << uin << endl;
		  cerr << "Error: 0x" << interror << endl;
		  cerr << "Msg:   " << strerror << endl;
		  packetError(&p, "");
		  m_state = S_offline;
		  m_net->disconnect();
		  eventError(interror, "Authorizer error:" + strerror);
		  eventLoggedOut();
		  break;
		}
	      ret = -1;
	      break;
	    }
	}
      m_state = S_offline;
      m_net->disconnect();
      eventLoggedOut();
      break;
    default:
      debug() << "> Unknown" << endl;
      packetError(&p, "Unknown channel ID: %d\n", p.hdr().channel());
      break;
    }

  return ret;
}
