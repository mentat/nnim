/*
    $Id: fnacpacket.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $

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
    $Log: fnacpacket.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.4  2001/10/27 18:29:13  estyrke
    Fixed icq parser bug.. #475544

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#include <log.h>

#include "fnacpacket.h"
#include "v7protocol.h"

FNAC::FNAC(uint16 family, uint16 subtype, uint32 reqID, uint16 flags)
{
  _family=family;
  _subtype=subtype;
  _reqID=reqID;
  _flags[0]=flags >> 8;
  _flags[1]=flags & 0xff;
}

FNAC::FNAC(const dstring &data)
{
  dstring::const_iterator it=data.begin();

  _family=charToU16(data);
  it += 2;
  _subtype=charToU16(it);
  it += 2;
  _flags[0]=charToU8(it);
  it++;
  _flags[1]=charToU8(it);
  it++;
  _reqID=charToU32(it);
}

FNAC::~FNAC()
{
}

FNAC::operator dstring() const
{
  return u16ToChar(_family) + u16ToChar(_subtype) +
    u8ToChar(_flags[0]) + u8ToChar(_flags[1]) + u32ToChar(_reqID);
}


Packet &operator>> (Packet &p, FNAC &f)
{
  p >> f._family;
  p >> f._subtype;
  p >> f._flags[0];
  p >> f._flags[1];
  p >> f._reqID;
  return p;
}


FCPacket::FCPacket(uint16 seq, uint16 family, uint16 subtype, const dstring &data) :
  CPacket(seq, ICQ_CHANNEL_FNAC),
  FNAC(family, subtype, subtype)
{
  *this << _family << _subtype << _flags[0] << _flags[1] << _reqID;
  *this << data;
}

FCPacket::~FCPacket()
{
}

FSPacket::FSPacket(const Packet &p) : SPacket(p), FNAC(p.data())
{
  //_data = _data.substr(FNAC::DATA_LEN);
  reset();
}

FSPacket::FSPacket(const FSPacket &p) : SPacket(p), FNAC(p.data())
{
  FSPacket::reset();
}


void FSPacket::reset()
{
  _cur=0;
  _cur += FNAC::DATA_LEN;
}

void FSPacket::dump() const
{
  debug() << "FNAC Header:" << endl;
  debug() << "Family: " << _family << endl;
  debug() << "Subtype: " << _subtype << endl;
  debug() << "Flags: 0x" << hex << _flags[0] << " 0x" << _flags[1] << dec << endl;
  debug() << "Request ID: 0x" << hex << _reqID << dec << endl;

  Packet::dump();
}
