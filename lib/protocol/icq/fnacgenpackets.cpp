/*
    $Id: fnacgenpackets.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnacgenpackets.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.3  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#include "fnacgenpackets.h"
#include "v7protocol.h"


//SUB_GEN_SERVERREADY
PktSGenServerReady::PktSGenServerReady(const FSPacket &fs) :
  FSPacket(fs)
{
}

PktSGenServerReady::~PktSGenServerReady()
{
}

// SUB_GEN_CLIENTVERSIONS
PktCGenClientVersions::PktCGenClientVersions(uint16 seq) :
  FCPacket(seq, FAMILY_GEN, SUB_GEN_CLIENTVERSIONS)
{
  // FIXME: Find out what these mean..
  *this << (uint32)0x00010003 << (uint32)0x00130002 << (uint32)0x00020001
	<< (uint32)0x00030001 << (uint32)0x00150001 << (uint32)0x00040001
	<< (uint32)0x00060001 << (uint32)0x00090001 << (uint32)0x000a0001
	<< (uint32)0x000b0001;
}

PktCGenClientVersions::~PktCGenClientVersions()
{
}

// SUB_GEN_SETSTATUS
PktCGenSetStatus::PktCGenSetStatus(uint16 seq, Protocol::Status status) :
  FCPacket(seq, FAMILY_GEN, SUB_GEN_SETSTATUS)
{
  // Great! Having multiple meanings for a single TLV type!
  uint32 s = 0x00030000;
  switch (status)
    {
    case Protocol::S_available:
      s &= 0x0000;
      break;
    case Protocol::S_away:
      s &= 0x0001;
      break;
    case Protocol::S_na:
      s &= 0x0005;
      break;
    case Protocol::S_occupied:
      s &= 0x0011;
      break;
    case Protocol::S_dnd:
      s &= 0x0013;
      break;
    case Protocol::S_ffc:
      s &= 0x0020;
      break;
    }
  *this << TLV(TLV::TLV_COOKIE, s);
}

PktCGenSetStatus::~PktCGenSetStatus()
{
}
