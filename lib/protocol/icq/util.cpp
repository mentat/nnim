/*
    $Id: util.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: util.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.7  2001/11/25 12:17:31  henrik
    gcc 3.0 fixes.

    Revision 1.6  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.5  2001/10/27 18:29:13  estyrke
    Fixed icq parser bug.. #475544

    Revision 1.4  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#include "util.h"
#include <log.h>

uint8 charToU8(const dstring &s, int pos)
{
  if (pos < 0 || s.length() - pos < 1)
    {
      debug() << "charToU8: data too short" << endl;
      return 0;
    }
  return s[pos];
}

uint16 charToU16(const dstring &s, int pos)
{
  if (pos < 0 || s.length() - pos < 2)
    {
      debug() << "charToU16: data too short" << endl;
      return 0;
    }
  uint16 a = 0;
  a |= s[pos];
  a <<= 8;
  a |= s[pos+1];
  return a;
}

uint32 charToU32(const dstring &s, int pos)
{
  if (pos < 0 || s.length() - pos < 4)
    {
      debug() << "charToU32: data too short" << endl;
      return 0;
    }
  uint32 a = 0;
  a |= s[pos];
  a <<= 8;
  a |= s[pos+1];
  a <<= 8;
  a |= s[pos+2];
  a <<= 8;
  a |= s[pos+3];
  return a;
}

dstring u8ToChar(uint8 i)
{
  dstring tmp;
  tmp.append(i);
  return tmp;
}

dstring u16ToChar(uint16 i)
{
  dstring tmp;
  tmp.append(i >> 8);
  tmp.append(i & 0xff);
  return tmp;
}

dstring u32ToChar(uint32 i)
{
  dstring tmp;
  tmp.append((i >> 24) & 0xff);
  tmp.append((i >> 16) & 0xff);
  tmp.append((i >>  8) & 0xff);
  tmp.append((i      ) & 0xff);
  return tmp;
}

uint8 charToU8(dstring::const_iterator it)
{
  return *it;
}

uint16 charToU16(dstring::const_iterator it)
{
  uint16 a = 0;
  a |= *it++;
  a <<= 8;
  a |= *it;
  return a;
}

uint32 charToU32(dstring::const_iterator it)
{
  uint32 a = 0;
  a |= *it++;
  a <<= 8;
  a |= *it++;
  a <<= 8;
  a |= *it++;
  a <<= 8;
  a |= *it;
  return a;
}

// Little Endian <---> uint

uint8 littleToU8(const dstring &s, int pos)
{
  if (pos < 0 || s.length() - pos < 1)
    return 0;
  return s[pos];
}

uint16 littleToU16(const dstring &s, int pos)
{
  if (pos < 0 || s.length() - pos < 2)
    return 0;
  uint16 a = 0;
  a |= s[pos];
  a |= (s[pos+1] << 8);
  return a;
}

uint32 littleToU32(const dstring &s, int pos)
{
  if (pos < 0 || s.length() - pos < 4)
    return 0;
  uint32 a = 0;
  a |= s[pos];
  a |= (s[pos+1] <<  8);
  a |= (s[pos+2] << 16);
  a |= (s[pos+3] << 24);
  return a;
}

dstring u8ToLittle(uint8 i)
{
  dstring tmp;
  tmp.append(i);
  return tmp;
}

dstring u16ToLittle(uint16 i)
{
  dstring tmp;
  tmp.append(i & 0xff);
  tmp.append((i >> 8) & 0xff);
  return tmp;
}

dstring u32ToLittle(uint32 i)
{
  dstring tmp;
  tmp.append((i      ) & 0xff);
  tmp.append((i >>  8) & 0xff);
  tmp.append((i >> 16) & 0xff);
  tmp.append((i >> 24) & 0xff);
  return tmp;
}

uint8 littleToU8(dstring::const_iterator it)
{
  return *it;
}

uint16 littleToU16(dstring::const_iterator it)
{
  uint16 a = 0;
  a |= *it++;
  a |= (*it << 8);
  return a;
}

uint32 littleToU32(dstring::const_iterator it)
{
  uint32 a = 0;
  a |= *it++;
  a |= (*it++ <<  8);
  a |= (*it++ << 16);
  a |= (*it   << 24);
  return a;
}

#define DUMP_WIDTH 16

void hexdump(const dstring &data)
{
#ifndef _WIN32
  unsigned int i, j, l=data.length();
  /*  debug() << "'";
  for (i=0;i<l;i++)
    {
      if (data[i]<32)
	debug() << '.';
      else
	debug() << data[i];
    }
    debug() << "'" << endl;*/
  i=0;
  while(1)
    {
      for(j=0;j<DUMP_WIDTH;j++)
	{
	  if(j == DUMP_WIDTH/2)
	    debug() << " ";
	  if(i > data.length() - 1)
	    debug() << "   ";
	  //	  else
	    //	    debug().form("%02x ", (uint8)data[i]);
	  i++;
	}
      i -= DUMP_WIDTH;
      debug() << " ";
      for(j=0;j<DUMP_WIDTH;j++)
	{
	  if(j == DUMP_WIDTH/2)
	    debug() << " ";
	  if(i > data.length() - 1)
	    debug() << " ";
	  //	  else if (data[i] <= 32 || data[i] >= 128)
	  //	    debug().form("%01x", (uint8)data[i] >> 4);
	  else
	    debug() << (char)data[i];

	  i++;
	}
      debug() << endl;

      if (i>=data.length())
	break;
    }

  return;

  for (i=1;i<=l;++i)
    {
      //      debug().form("%02x ", (uint8)data[i-1]);
      //debug() << setfill('0') << hex << setw(4) << uint16(data[i-1]) << dec << " ";
      if (i % DUMP_WIDTH == DUMP_WIDTH/2)
	debug() << " ";

      if (!(i % DUMP_WIDTH))
	{
	  debug() << " ";
	  for (j=1;j<=DUMP_WIDTH;j++)
	    {
	      char c = data[i-DUMP_WIDTH-1+j];
	      debug() << (c<32?'.':c);
	      if (j % DUMP_WIDTH == DUMP_WIDTH/2)
		debug() << " ";
	    }
	  debug() << endl;
	}
    }

  if ((i % DUMP_WIDTH))
    {
      int w = i % DUMP_WIDTH;
      for (j=0;j<DUMP_WIDTH-w+1;j++)
	debug() << "   ";
      if (w <= DUMP_WIDTH / 2)
	debug() << " ";

      debug() << " ";
      for (j=1;j<w;j++)
	{
	  uint8 c = data[i-w-1+j];
	  debug() << (c<32?'.':(char)c);
	  if (!(j % (DUMP_WIDTH/2)))
	    debug() << " ";
	}
      debug() << endl;
    }
#endif
}
