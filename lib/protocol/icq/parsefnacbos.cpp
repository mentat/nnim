/*
    $Id: parsefnacbos.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsefnacbos.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.4  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#include <log.h>

#include "icqprotocol.h"

#include "packets.h"
#include "v7protocol.h"
#include "basenetwork.h"

int IcqProtocol::parseFnacBos(const FSPacket &fs)
{
  debug() << "> BOS ";
  switch (fs.subtype())
    {
    case SUB_BOS_RIGHTS:
      debug() << "> Rights" << endl;
      break;
    default:
      debug() << "> Error" << endl;
      packetError(&fs, "Unknown BOS subtype %d\n", fs.subtype());
      return -1;
      break;
    }
  return 0;
}
