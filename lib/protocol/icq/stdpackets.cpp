/*
    $Id: stdpackets.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $
 
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
    $Log: stdpackets.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.3  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#include "stdpackets.h"
#include "v7protocol.h"

// Server CONN_ACK
PktSConnAck::PktSConnAck(const Packet &p) :
  SPacket(p)
{
  _ok = false;
  if (hdr().channel() != 1)
    return;
  if (hdr().len() != 4)
    return;
  *this >> _ver;
  if (_ver != (uint32)0x00000001)
    return;
  _ok = true;
}

PktSConnAck::~PktSConnAck()
{
}

// Server COOKIE_REPLY
PktSCookieReply::PktSCookieReply(const Packet &p) : SPacket(p)
{
  _ok = false;

  TLV temp;

  reset();

  *this >> temp;
  if (temp.type() != TLV::TLV_UIN)
    return;
  string uin = temp.value();
  
  *this >> temp;
  if (temp.type() != TLV::TLV_ADDR)
    return;
  string ta = temp.value();
  int split=ta.find(':');
  _addr = ta.substr(0, split);
  _port = atoi(ta.substr(split+1).c_str());

  *this >> temp;
  if (temp.type() != TLV::TLV_COOKIE)
    return;
  _cookie = temp.value();
  _ok = true;

//  DEBUG_PRINT(8, "uin: %s", uin.c_str());
//  DEBUG_PRINT(8, "addr: '%s'", _addr.c_str());
//  DEBUG_PRINT(8, "port: %d", _port);

}

PktSCookieReply::~PktSCookieReply()
{
}

/* Client Cookie request:
 *  DWORD: 0x00000001
 *    TLV: 0x0001(UIN         ),    var, UIN
 *    TLV: 0x0002(ENCPASSWD   ),    var, Encryped (hah!) password
 *    TLV: 0x0003(CLIENTSTRING), 0x0033, "ICQ Inc. - Product of ICQ (TM).2000a.4.30.1.3141.85"
 *    TLV: 0x0016(MAJOR2      ), 0x0002, 0x010a     (266)
 *    TLV: 0x0017(MAJOR       ), 0x0002, 0x0004     (4)
 *    TLV: 0x0018(MINOR       ), 0x0002, 0x001e     (30)
 *    TLV: 0x0019(MINOR2      ), 0x0002, 0x0001     (1)
 *    TLV: 0x001a(BUILD       ), 0x0002, 0x0c45     (3141)
 *    TLV: 0x0014(BUILD2      ), 0x0004, 0x00000055 (85)
 *    TLV: 0x000f(LANGUAGE    ), 0x0002, LANGUAGE
 *    TLV: 0x000e(COUNTRY     ), 0x0002, COUNTRY
 */
PktCCookieRequest::PktCCookieRequest(int seq, const string &uin, const string &pass)
  : CPacket(seq, ICQ_CHANNEL_OPEN)
{
  unsigned int i;
  dstring encoded_password;

  // ROFL!!! "encrypt" the password.. blah..
  for (i=0;i<pass.length() && i < 16;i++)
    {
      uint8 x = pass[i] ^ PASSWORD_MASK[i];
      encoded_password.append(x);
    }
  
  *this << (uint32)0x00000001;
  *this << TLV(TLV::TLV_UIN, uin);
  *this << TLV(TLV::TLV_ENCPASS, encoded_password);
  *this << TLV(TLV::TLV_CLIENTSTRING, string(CLIENTSTRING));
  *this << TLV(TLV::TLV_MAJOR2, MAJOR2);
  *this << TLV(TLV::TLV_MAJOR,  MAJOR );
  *this << TLV(TLV::TLV_MINOR,  MINOR );
  *this << TLV(TLV::TLV_MINOR2, MINOR2);
  *this << TLV(TLV::TLV_BUILD,  BUILD );
  *this << TLV(TLV::TLV_BUILD2, BUILD2);
  *this << TLV(TLV::TLV_LANGUAGE, LANGUAGE);
  *this << TLV(TLV::TLV_COUNTRY, COUNTRY);
}

PktCCookieRequest::~PktCCookieRequest()
{
}

PktCSignon::PktCSignon(uint16 seq, const dstring &cookie)
  : CPacket(seq, ICQ_CHANNEL_OPEN)
{
  *this << (uint32)0x00000001;
  *this << TLV(TLV::TLV_COOKIE, cookie);
}

PktCSignoff::PktCSignoff(uint16 seq) :
  CPacket(seq, ICQ_CHANNEL_CLOSE)
{
}
