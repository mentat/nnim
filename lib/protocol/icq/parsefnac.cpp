/*
    $Id: parsefnac.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsefnac.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.4  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#include <log.h>
#include "icqprotocol.h"

#include "fnacpacket.h"
#include "v7protocol.h"

int IcqProtocol::parseFnac(const Packet &p)
{
  FSPacket fs(p);

  debug() << "> FNAC ";

  if (!fs.ok())
    return -1;

  switch (fs.family())
    {
    case FAMILY_GEN:
      return parseFnacGen(fs);
      break;
    case FAMILY_LOC:
      return parseFnacLoc(fs);
      break;
    case FAMILY_BUD:
      return parseFnacBud(fs);
      break;
    case FAMILY_MSG:
      return parseFnacMsg(fs);
      break;
    case FAMILY_BOS:
      return parseFnacBos(fs);
    case FAMILY_ICQ:
      return parseFnacIcq(fs);
    default:
      debug() << "> Error" << endl;
      packetError(&fs, "Unknown family %d in parseFnac()!\n", fs.family());
      return -1;
      break;
    }
}
