/*
    $Id: fnacicqpackets.h,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnacicqpackets.h,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.3  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __FNACICQPACKETS_H
#define __FNACICQPACKETS_H

#include "packet.h"
#include "fnacpacket.h"

#include <list>
///////////////////////////////
// Server originated packets //
///////////////////////////////

class PktSIcqResponse : public FSPacket
{
public:
  PktSIcqResponse(const FSPacket &p);
  virtual ~PktSIcqResponse();
  uint16 msgtype() const {return _msgtype;};
  uint32 msgsubtype() const {return _subtype;};
  const string &uin() const {return _uin;};
  const string &nick() const {return _nick;};
  const string &first() const {return _first;};
  const string &last() const {return _last;};
  const string &email() const {return _email;};
private:
  uint16 _msgtype;
  uint32 _subtype;
  string _uin;
  string _nick;
  string _first;
  string _last;
  string _email;
  uint8 _auth, _status, _unknown;
};

///////////////////////////////
// Client originated packets //
///////////////////////////////

class PktCIcqGetSInfo : public FCPacket
{
public:
  PktCIcqGetSInfo(uint16 seq, const string &myUin, const string &uin);
  virtual ~PktCIcqGetSInfo() {};

  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< ICQ < Request < Simple info";
  };
};

#endif
