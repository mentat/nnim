/*
    $Id: fnacicqpackets.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $

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
    $Log: fnacicqpackets.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.4  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#include <log.h>

#include "fnacicqpackets.h"
#include "v7protocol.h"

#include <string.h>

PktSIcqResponse::PktSIcqResponse(const FSPacket &p) :
  FSPacket(p)
{
  //uint16 u16;
  //uint32 u32;
  /*  TLV t;
  int i=0;
  
  reset();
  _ok=false;

  if (!getTLV(t))
    return;

  if (t.type() != TLV::TLV_ICQRES)
    {
      debug() << "PktSIcqResponse: wrong tlv catched" << endl;
      return;
    }

  dstring data = t.value();

  //Parse out common fields
  uint16 size = littleToU16(data, i);
  i+= 2;
  uint32 my_uin = littleToU32(data, i);
  i+= 4;
  _msgtype = littleToU16(data, i);
  i+= 2;
  uint16 req_id = littleToU16(data, i);
  i+= 2;

  switch (_msgtype)
    {
    case 0x0041:
      //Offline message
      break;
    case 0x0042:
      // End of offline messages
      break;
    case 0x07D0:
      // XML-something...
      break;
    case 0x07DA:
      // Hmm.. _3_ bytes subtype? why? let's do an ugly hack to read it
      _subtype = littleToU32(data, i) & 0x00FFFFFF;
      i+=3;
      switch(_subtype)
	{
	case 0x0A08A2:
	  // Where to get ads stuff
	  break;
	case 0x3201a4:
	case 0x3201ae:
	  // Empty whitepages result
	  break;
	case 0x0a01a4:
	  // wp-full-request result (more coming)
	  break;
	case 0x0a01ae:
	  // wp-full-request result (last)
	  break;
	case 0x0a0190:
	  // wp-short-request result (more coming)
	  break;
	case 0x0a019a:
	  {
	    // wp-short-request result (last)
	    i+=2; // skip unknown bytes
	    strstream foo;
	    uint16 len;
	    foo << littleToU32(data, i);
	    i+=4;
	    _uin = foo.string();
	    len = littleToU16(data, i);
	    i+= 2;
	    _nick = getString(len);
	    len = littleToU16(data, i);
	    i+=2;
	    _first = getString(len);
	    len = littleToU16(data, i);
	    i+=2;
	    _last = getString(len);
	    len = littleToU16(data, i);
	    i+=2;
	    _email = getString(len);
	    *this >> _auth >> _status >> _unknown;
	  }
	  break;
	case 0x0a00c8:
	  // main-home-info
	  break;
	case 0x0a00d2:
	  // work-info
	  break;
	case 0x0a00e6:
	  // about
	  break;
	case 0x0a00f0:
	  // personal-interests-info
	  break;
	case 0x0a00fa:
	  // past-background-info
	  break;
	case 0x1400fa:
	  // query result: non-existing user
	  break;
	case 0x0a00eb:
	  // more-email-info
	  break;
	case 0x0a00dc:
	  // homepage-more-info
	  break;
	case 0x0a010e:
	  // additional info
	  break;
	case 0x0a0064:
	  // ack to modify main/home
	  break;
	case 0x0a0078:
	  // ack to modify homepage/more
	  break;
	case 0x0a0082:
	  // ack to modify about
	  break;
	case 0x0a006e:
	  // ack to modify work
	  break;
	case 0x0a00b4:
	  // ack to remove user
	  break;
	case 0x0a00aa:
	  // ack to change password
	  break;
	case 0x0a00a0:
	  // unknown, reply to 2404
	  break;
	default:
	  debug() << "PktSIcqResponse: unknown message subtype: 0x" << hex << _subtype << dec << endl;
	  return;
	  break;
	}
      break;
    default:
      debug() << "PktSIcqResponse: unknown message type: 0x" << hex << _msgtype << dec << endl;
      return;
      break;
    }
  _ok = true;*/



  
	    
  /* *this >> u32;
  *this >> u32; //Discard cookie
  *this >> u16; //Channel id
  debug() << *this;
  if (u16 != 0x0001)
    {
      cerr << "Channel ID: " << hex << u16 << dec << endl;
      return;
    }

  *this >> _ui;

  debug() << "name(" << _ui.name().length() << "): " << _ui.name() << endl;
  debug() << "TLV count: " << _ui.TLVs().size() << endl;
  map<uint16, TLV>::iterator it;
  for (it=_ui.TLVs().begin();it!=_ui.TLVs().end();it++)
    {
      debug() << "Userinfo TLV of type: " << hex << it->first << dec;
      switch (it->first)
	{
	default:
	  debug() << " (unknown)";
	}
      debug() << endl;
    }
  debug() << "Done userinfo tlvs" << endl;

  TLV foo;
  while (!EOP())
    {
      *this >> foo;
      debug() << "TLV of type: " << hex << foo.type() << dec;
      switch (foo.type())
	{
	case 2:
	  _message = foo.value().substr(0x0D);
	  break;
	default:
	  debug() << " (unknown)";
	  break;
	}
      debug() << endl;
    }
    _ok = true;*/
}

PktSIcqResponse::~PktSIcqResponse()
{
}

// Client SUB_ICQ_REQUEST
//PktCIcqRequest::PktCIcqRequest(uint16 seq, uint32 myUin, uint16 type,
//			       uint16 cookie, const dstring &data)
//  : FCPacket(seq, FAMILY_ICQ, SUB_ICQ_REQUEST)

dstring mkUin(const string &uin)
{
  return u32ToLittle(atoi(uin.c_str()));
}

TLV icqRequest(const string &myUin, uint16 type, uint16 subtype=0,
	       const dstring &data=dstring(""))
{
  static uint16 reqid=0;
  dstring foo;
  dstring extra;

  reqid++;
  switch (type)
    {
    case 0x3c00:  // ask for offline msgs
      break;
    case 0x3e00:  // ack to offline msgs
      break;
    case 0xd007:
      extra += u16ToChar(subtype);
      extra += data;
    }
  foo += u16ToLittle(extra.length() + 8);
  foo += mkUin(myUin.c_str());
  foo += u16ToChar(type);
  foo += u16ToLittle(reqid);
  foo += extra;
  
  return TLV(TLV::TLV_ICQREQ,foo);
}

/*PktCIcqRequest::~PktCIcqRequest()
{
}*/

PktCIcqGetSInfo::PktCIcqGetSInfo(uint16 seq, const string &myUin,
				 const string &uin)
  : FCPacket(seq, FAMILY_ICQ, SUB_ICQ_REQUEST)
{
  *this << icqRequest(myUin, 0xD007, 0x1F05, mkUin(uin));
  //debug() << "GetSInfo" << endl;
  //debug() << *this;
  //debug() << "--------" << endl;
}


