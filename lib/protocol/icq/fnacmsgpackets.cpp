/*
    $Id: fnacmsgpackets.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $

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
    $Log: fnacmsgpackets.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.6  2001/11/11 16:01:27  henrik
    Fixed interoperability with mac icq clients.

    Revision 1.5  2001/11/09 10:13:43  estyrke
    Fixed some bugs in the icq code. Messaging should now work.

    Revision 1.4  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#include <log.h>

#include "fnacmsgpackets.h"
#include "v7protocol.h"

PktSMsgRecv::PktSMsgRecv(const FSPacket &p) :
  FSPacket(p)
{
  uint16 u16;
  uint32 u32;

  reset();
  _ok=false;
  *this >> u32;
  *this >> u32; //Discard cookie
  *this >> u16; //Channel id
  debug() << *this;
  if (u16 != 0x0001 && u16 != 0x0004)
    {
      debug() << "Channel ID: " << hex << u16 << dec << endl;
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
  while (getTLV(foo))
    {
      debug() << "TLV of type: " << hex << foo.type() << dec;
      switch (foo.type())
	{
	case 2:
	  _message = foo.value().substr(0x0D);
	  break;
	case 5:
	  u16 = littleToU16(foo.value(), 6);
	  _message = foo.value().substr(8, u16);
	  break;
	default:
	  debug() << " (unknown)";
	  break;
	}
      debug() << endl;
    }
  _ok = true;
}

PktSMsgRecv::~PktSMsgRecv()
{
}

// Client SUB_MSG_SENDIM
PktCMsgSend::PktCMsgSend(uint16 seq, const string &uin, const string &message)
  : FCPacket(seq, FAMILY_MSG, SUB_MSG_SENDMSG)
{
  int i;
  for(i=0;i<8;i++)
    *this << (uint8)0x00;  //message cookie
  *this << (uint16)0x0001; //Channel ID
  *this << PascalString(uin);
  *this << (uint16)0x0002;                    // \ actually a TLV
  *this << uint16(message.length() + 0xd);    // /

  *this << (uint8)0x05;
  *this << (uint8)0x01;
  *this << (uint8)0x00;
  *this << (uint8)0x01
	<< (uint8)0x01
	<< (uint8)0x01
	<< (uint8)0x01;
  *this << uint16(message.length() + 0x04);
  *this << (uint16)0x0000;
  *this << (uint16)0x0000;
  *this << dstring(message);
  *this << TLV(TLV::type_e(6), dstring());
  debug() << *this;
}

PktCMsgSend::~PktCMsgSend()
{
}
