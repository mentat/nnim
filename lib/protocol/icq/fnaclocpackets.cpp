/*
    $Id: fnaclocpackets.cpp,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: fnaclocpackets.cpp,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.3  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#include <log.h>

#include "fnaclocpackets.h"
#include "v7protocol.h"

PktCLocCapinfo::PktCLocCapinfo(uint16 seq) : FCPacket(seq, FAMILY_LOC, SUB_LOC_CAPINFO)
{
  //*this << (uint32)0x09461349 << (uint32)0x4c7f11d1
  //	<< (uint32)0x82224445 << (uint32)0x53540000; // Cap. 1
  *this << (uint32)0x09461344 << (uint32)0x4c7f11d1
	<< (uint32)0x82224445 << (uint32)0x53540000; // Cap. 2
}

PktCLocCapinfo::~PktCLocCapinfo() {}
