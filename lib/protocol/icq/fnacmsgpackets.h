/*
    $Id: fnacmsgpackets.h,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnacmsgpackets.h,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __FNACMSGPACKETS_H
#define __FNACMSGPACKETS_H

#include "packet.h"
#include "fnacpacket.h"

#include <list>
///////////////////////////////
// Server originated packets //
///////////////////////////////

class PktSMsgRecv : public FSPacket
{
public:
  PktSMsgRecv(const FSPacket &p);
  virtual ~PktSMsgRecv();
  const string &sender() const {return _ui.name();};
  const string &message() const {return _message;};
private:
  string _message;
  UserInfo _ui;
};

///////////////////////////////
// Client originated packets //
///////////////////////////////

class PktCMsgSend : public FCPacket
{
public:
  PktCMsgSend(uint16 seq, const string &uin, const string &message);
  virtual ~PktCMsgSend();

  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< MSG < Send";
  };
};

#endif
