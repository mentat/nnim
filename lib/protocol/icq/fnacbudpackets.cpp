/*
    $Id: fnacbudpackets.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $

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
    $Log: fnacbudpackets.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#include <log.h>

#include "fnacbudpackets.h"
#include "v7protocol.h"

PktSBudOncoming::PktSBudOncoming(const FSPacket &fs)
  : FSPacket(fs)
{
  _ok = true;
  map<uint16, TLV>::iterator it;

  *this >> _ui;
  uint16 status = 1;

  _status=1;
  for(it=_ui.TLVs().begin();it!=_ui.TLVs().end();it++)
    {
      switch (it->first)
	{
	case 6:
	  debug() << "Status: ";
	  status = (charToU32(it->second.value()) & 0xffff);
	  switch (status)
	    {
	    case 0x0000:
	      debug() << "Online" << endl;
	      _status = 1;
	      break;
	    case 0x0001:
	      debug() << "Away" << endl;
	      _status = 2;
	      break;
	    case 0x0002:
	      debug() << "DND" << endl;
	      _status = 3;
	      break;
	    case 0x0004:
            case 0x0005:
	      debug() << "N/A" << endl;
	      _status = 4;
	      break;
	    case 0x0010:
            case 0x0017:

	      debug() << "Occupied" << endl;
	      _status = 5;
	      break;
            case 0x0019:
	    case 0x0020:
	      debug() << "Free for chat" << endl;
	      _status = 6;
	      break;
	    default:
	      debug() << "Unknown (" << status << ")" << endl;
	      break;
	    }
	  break;
	default:
	  debug() << "Userinfo: " << it->first << " len = " << it->second.length() << endl;
	  /*debug() << "  Value:    " << string(it->second.value()) << endl;
	  string foo = ""; //it->second.value();
	  debug() << "  U32Value: " << hex << charToU32(it->second.value()) << endl;
	  debug() << "  U16Value: " << hex << charToU16(string(it->second.value())) << endl;
	  debug() << "  U8Value:  " << hex << charToU8(string(it->second.value())) << endl;
	  debug() << dec;*/
	  break;
	}
    }
}

PktSBudOncoming::~PktSBudOncoming()
{
}

PktSBudOffgoing::PktSBudOffgoing(const FSPacket &fs) :
  FSPacket(fs)
{
  _ok = true;
  PascalString buddy;

  *this >> buddy;
  _name = dstring(buddy);
}

PktSBudOffgoing::~PktSBudOffgoing()
{
}

PktCBudReqRights::PktCBudReqRights(uint16 seq) :
  FCPacket(seq, FAMILY_BUD, SUB_BUD_REQRIGHTS)
{
}

PktCBudAdd::PktCBudAdd(uint16 seq, const Contact &c) :
  FCPacket(seq, FAMILY_BUD, SUB_BUD_ADDBUDDY)
{
  *this << PascalString(c.serverId());
}

//extern vector<PascalString> foo;

PktCBudAdd::PktCBudAdd(uint16 seq, const Protocol::buddy_t &cs) :
  FCPacket(seq, FAMILY_BUD, SUB_BUD_ADDBUDDY)
{
  Protocol::buddy_t::const_iterator it;

  for(it=cs.begin();it!=cs.end();it++)
  {
      string tmp= it->first;
      *this << PascalString(tmp);
  }
}

PktCBudDel::PktCBudDel(uint16 seq, const Contact &c) :
  FCPacket(seq, FAMILY_BUD, SUB_BUD_DELBUDDY)
{
  *this << PascalString(c.serverId());
}

PktCBudDel::PktCBudDel(uint16 seq, const Protocol::buddy_t &cs) :
  FCPacket(seq, FAMILY_BUD, SUB_BUD_DELBUDDY)
{
  Protocol::buddy_t::const_iterator it;

  for(it=cs.begin();it!=cs.end();it++)
    *this << PascalString(it->first);
}

