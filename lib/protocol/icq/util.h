/*
    $Id: util.h,v 1.1 2002/06/06 17:21:51 thementat Exp $
 
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
    $Log: util.h,v $
    Revision 1.1  2002/06/06 17:21:51  thementat
    Initial revision

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __UTIL_H
#define __UTIL_H

#include "dstring.h"
#include "platform.h"
// #include <iostream>
// #include <iomanip>

uint8 charToU8(const dstring &s, int pos=0);
uint16 charToU16(const dstring &s, int pos=0);
uint32 charToU32(const dstring &s, int pos=0);

uint8 charToU8(dstring::const_iterator it);
uint16 charToU16(dstring::const_iterator it);
uint32 charToU32(dstring::const_iterator it);

dstring u8ToChar(uint8 i);
dstring u16ToChar(uint16 i);
dstring u32ToChar(uint32 i);

uint8 littleToU8(const dstring &s, int pos=0);
uint16 littleToU16(const dstring &s, int pos=0);
uint32 littleToU32(const dstring &s, int pos=0);

uint8 littleToU8(dstring::const_iterator it);
uint16 littleToU16(dstring::const_iterator it);
uint32 littleToU32(dstring::const_iterator it);

dstring u8ToLittle(uint8 i);
dstring u16ToLittle(uint16 i);
dstring u32ToLittle(uint32 i);


void hexdump(const dstring &data);

#endif
