/*
    $Id: fnacbudpackets.h,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnacbudpackets.h,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.3  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __FNACBUDPACKETS_H
#define __FNACBUDPACKETS_H

#include "fnacpacket.h"
#include "contact.h"
#include "protocol.h"
#include "platform.h"

#include <string>
#include <list>
///////////////////////////////
// Server originated packets //
///////////////////////////////

class PktSBudOncoming : public FSPacket
{
public:
  PktSBudOncoming(const FSPacket &fs);
  virtual ~PktSBudOncoming();

  const string &name() const {return _ui.name();};
  int status() const {return _status;};
private:
  UserInfo _ui;
  uint16 _status;
};

class PktSBudOffgoing : public FSPacket
{
public:
  PktSBudOffgoing(const FSPacket &fs);
  virtual ~PktSBudOffgoing();

  const string &name() const {return _name;};
private:
  string _name;
};

///////////////////////////////
// Client originated packets //
///////////////////////////////

class PktCBudReqRights : public FCPacket
{
public:
  PktCBudReqRights(uint16 seq);

  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< BUD < ReqRights";
  };
};

class PktCBudAdd : public FCPacket
{
public:
  PktCBudAdd(uint16 seq, const Contact &uin);
  PktCBudAdd(uint16 seq, const Protocol::buddy_t &buddies);
  
  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< BUD < Add";
  };
};

class PktCBudDel : public FCPacket
{
public:
  PktCBudDel(uint16 seq, const Contact &uin);
  PktCBudDel(uint16 seq, const Protocol::buddy_t &uins);
  
  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< BUD < Add";
  };
};

#endif
