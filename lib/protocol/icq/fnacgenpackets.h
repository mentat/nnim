/*
    $Id: fnacgenpackets.h,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnacgenpackets.h,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __FNACGENPACKETS_H
#define __FNACGENPACKETS_H

#include "fnacpacket.h"
#include "protocol.h"

///////////////////////////////
// Server originated packets //
///////////////////////////////

//SUB_GEN_SERVERREADY
class PktSGenServerReady : public FSPacket
{
 public:
  PktSGenServerReady(const FSPacket &fs);
  virtual ~PktSGenServerReady();
};

///////////////////////////////
// Client originated packets //
///////////////////////////////

//SUB_GEN_CLIENTVERSIONS
class PktCGenClientVersions : public FCPacket
{
 public:
  PktCGenClientVersions(uint16 seq);
  virtual ~PktCGenClientVersions();

  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< GEN < ClientVersions";
  };
};

class PktCGenSetStatus : public FCPacket
{
public:
  PktCGenSetStatus(uint16 seq, Protocol::Status status);
  virtual ~PktCGenSetStatus();

  virtual const string breadCrumb() const {
    return FCPacket::breadCrumb() + "< GEN < SetStatus";
  };
};
  
#endif
