/*
    $Id: packet.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $

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
    $Log: packet.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.7  2001/12/12 01:47:26  mentat
    Adding gm_config.h.in and more #ifdef WIN32s to get rid of MSW template warning.

    Revision 1.6  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.5  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.4  2001/10/27 18:29:13  estyrke
    Fixed icq parser bug.. #475544

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <log.h>
#include "packet.h"
#include "v7protocol.h"

Header::Header()
{
  _id = ICQ_PACKET_ID;
}

Header::Header(const dstring &data)
{
  _id = data[0];
  _channel = data[1];
  _seq = charToU16(data, 2);
  _len = charToU16(data, 4);
}

Header::operator dstring()
{
  return u8ToChar(_id) + u8ToChar(_channel) +
    u16ToChar(_seq) + u16ToChar(_len);
}

TLV::TLV(type_e t, dstring value)
{
  _type = t;
  _value = value;
}

TLV::TLV(type_e t, uint8 value)
{
  _type = t;
  _value = u8ToChar(value);
}

TLV::TLV(type_e t, uint16 value)
{
  _type = t;
  _value = u16ToChar(value);
}

TLV::TLV(type_e t, uint32 value)
{
  _type = t;
  _value = u32ToChar(value);
}

TLV::operator dstring()
{
  //debug() << *this ;
  return u16ToChar(_type) + u16ToChar(_value.length()) + _value;
}

ostream &operator<< (ostream &o, const TLV &t)
{
  o << "Type: " << hex << t._type << dec << endl;
  o << "Length: " << hex << t._value.length() << dec << endl;
  o << "Value: " << endl;
  hexdump(t._value);

  return o;
}

Packet::Packet(const dstring &data)
{
  _hdr = Header(data.substr(0, 6));
  _data = data.substr(6);
  reset();
}

Packet::Packet(Header hdr, const dstring &data)
{
  _hdr = hdr;
  _data = data;
  reset();
//  if (hdr.len() != data.length())
//    DEBUG_PRINT(3, "Packet::Packet(): Data length mismatch!");
}

Packet::Packet(uint16 seq, uint8 channel, const dstring &data)
{
  _hdr.setChannel(channel);
  _hdr.setSeq(seq);
  _hdr.setLen(data.length());
  _data = data;
  reset();
}

Packet::~Packet()
{
}

void Packet::dump() const
{
  cerr << *this;
}

Packet::operator dstring()
{
  debug() << breadCrumb() << endl;
  return dstring(_hdr) + _data;
}

Packet &Packet::operator<< (const dstring &data)
{
  _data += data;
  _hdr.setLen(_data.length());
  return *this;
}

Packet &Packet::operator<< (uint8 data)
{
  _data += u8ToChar(data);
  _hdr.setLen(_data.length());
  return *this;
}

Packet &Packet::operator<< (uint16 data)
{
  _data += u16ToChar(data);
  _hdr.setLen(_data.length());
  return *this;
}

Packet &Packet::operator<< (uint32 data)
{
  _data += u32ToChar(data);
  _hdr.setLen(_data.length());
  return *this;
}

Packet &Packet::operator<< (const PascalString &data)
{
  _data += u8ToChar(dstring(data).length());
  _data += (dstring)data;
  _hdr.setLen(_data.length());
  return *this;
};

dstring Packet::getString (int len)
{
  dstring tmp;
  /*dstring::iterator it=_cur;

  for (;len > 0;len--)
    {
      _cur++;
      if (EOP())
	{
	  debug() << "STOP" << endl;
	  break;
	}
    }

    tmp.insert(tmp.end(), it, _cur);*/
  if ((_cur+len) > _data.length())
    debug() << "Packet::getString tried to eat too much.." << endl;
  tmp=_data.substr(_cur, len);
  _cur += len;
  return tmp;
}

Packet &Packet::operator>> (uint8 &data)
{
  if ((_data.length()-_cur-1) < 0)
    return *this;
  data = _data[_cur++];
  return *this;
}

Packet &Packet::operator>> (uint16 &data)
{
  if ((_data.length()-_cur-2) < 0)
    return *this;
  data = charToU16(_data, _cur);
  _cur += 2;
  return *this;
}

Packet &Packet::operator>> (uint32 &data)
{
  if ((_data.length()-_cur-4) < 0)
    return *this;
  data = charToU32(_data, _cur);
  _cur += 4;
  return *this;
}

Packet &Packet::operator>> (PascalString &data)
{
  if ((_data.length()-_cur-1) < 0)
    return *this;
  uint8 len = charToU8(_data, _cur);
  _cur++;

  if ((_data.length()-_cur-len) < 0)
    return *this;
  
  data = getString(len);

  return *this;
}

Packet &Packet::operator>> (TLV &t)
{
  getTLV(t);
  return *this;
}

int Packet::getTLV(TLV &t)
{
  if ((_data.length()-_cur-4) < 0)
    return 0;

  t.setType(charToU16(_data, _cur));
  _cur += 2;

  uint16 len = charToU16(_data, _cur);
  _cur += 2;

  if ((_data.length()-_cur-len) < 0)
    {
      return 0;
    }
  dstring foo = getString(len);

  t.setValue(foo);
  
  return 1;
}  
  
Packet &Packet::operator>> (UserInfo &ui)
{
  uint16 tlvcount;
  TLV tmp;
  PascalString ps;
  uint16 warnlvl;
  int i;

  *this >> ps;
  ui.setName(dstring(ps));

  *this >> warnlvl;
  ui.setWarnLevel(warnlvl);

  *this >> tlvcount;

  ui.TLVs().clear();

  for (i=0;i<tlvcount;i++)
    {
      if (!getTLV(tmp))
	{
	  debug() << "Userinfo had too few tlv:s.. (" << i <<
	    ", expected " << tlvcount << ")" << endl;
	  break;
	}
      ui.TLVs()[tmp.type()] = tmp;
    }

  return *this;
}

ostream &operator<< (ostream &o, const Packet &p)
{
  int l=p._hdr.len();

  o << "Channel: " << (int)p._hdr.channel() << endl;
  o << "Sequence #: " << p._hdr.seq() << endl;
  o << "Data length: " << l << "(" << p._data.length() << ")" << endl;

  hexdump(p._data);
  return o;
}
