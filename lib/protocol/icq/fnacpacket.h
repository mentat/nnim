/*
    $Id: fnacpacket.h,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnacpacket.h,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __FNACPACKET_H
#define __FNACPACKET_H

#include "platform.h"
#include "dstring.h"
#include "packet.h"


class FNAC
{
public:
  enum {DATA_LEN = 10};
  FNAC(uint16 family=0, uint16 subtype=0, uint32 reqID=0, uint16 flags=0);
  FNAC(const dstring &data);
  virtual ~FNAC();

  virtual operator dstring() const;

  virtual uint16 family() const {return _family;};
  virtual uint16 subtype() const {return _subtype;};
  virtual const uint8 *flags() const {return _flags;};
  virtual uint32 reqID() const {return _reqID;};

  virtual void setFamily(uint16 f) {_family=f;};
  virtual void setSubtype(uint16 s) {_subtype=s;};
  virtual void setFlags(uint8 f1, uint8 f2) {_flags[0]=f1;_flags[1]=f2;};
  virtual void setReqID(uint32 r) {_reqID=r;};

  friend class Packet &operator>> (Packet &p, FNAC &f);

//protected:
  uint16 _family;
  uint16 _subtype;
  uint8 _flags[2];
  uint32 _reqID;
};

class FCPacket : public CPacket, public FNAC
{
public:
  FCPacket(uint16 seq, uint16 family, uint16 subtype, const dstring &data=dstring());
  virtual ~FCPacket();

  virtual const string breadCrumb() const {
    return CPacket::breadCrumb() + "< FNAC ";
  };

  virtual operator dstring() {return CPacket::operator dstring();};
};

class FSPacket : public SPacket, public FNAC
{
public:
  FSPacket(const Packet &p);
  FSPacket(const FSPacket &p);
  virtual ~FSPacket() {};

  virtual void reset();
  virtual void dump() const;
  //  virtual UserInfo readUserInfo();
};

#endif
