/*
    $Id: parsefnacbud.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsefnacbud.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.6  2001/12/12 01:47:26  mentat
    Adding gm_config.h.in and more #ifdef WIN32s to get rid of MSW template warning.

    Revision 1.5  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.4  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <log.h>

#include "icqprotocol.h"

#include "packets.h"
#include "v7protocol.h"
#include "basenetwork.h"
#include "contact.h"

int IcqProtocol::parseFnacBud(const FSPacket &fs)
{
  debug() << "> BUD ";

  switch (fs.subtype())
    {
    case SUB_BUD_RIGHTS:
      debug() << "> Rights" << endl;
      break;
    case SUB_BUD_ONCOMING:
      debug() << "> Oncoming" << endl;
      {
	PktSBudOncoming op(fs);
	if (!op.ok())
	  break;
	debug() << "Oncoming buddy: " << op.name() << endl;
	int s;
	switch(op.status())
	  {
	  case 1:
	    s = Contact::Online;
	    break;
	  case 2:
	    s = Contact::Away;
	    break;
	  case 3:
	    s = Contact::Dnd;
	    break;
	  case 4:
	    s = Contact::Na;
	    break;
	  case 5:
	    s = Contact::Occupied;
	    break;
	  case 6:
	    s = Contact::Ffc;
	    break;
	  default:
	    debug() << "Buddy has unknown status " << op.status() << endl;
	    s = Contact::Online;
	    break;
	  }
	m_buddies[op.name()].setStatus(s);
	eventStatusChange(m_buddies[op.name()]);
	m_net->sendData(PktCIcqGetSInfo(_seq++, m_conf.child("user")["username"], op.name()));
      }
      break;
    case SUB_BUD_OFFGOING:
      debug() << "> Offgoing" << endl;
      {
	PktSBudOffgoing op(fs);
	if (!op.ok())
	  break;
	debug() << "Offgoing buddy: " << op.name() << endl;
	m_buddies[op.name()].setStatus(Contact::Offline);
	eventStatusChange(m_buddies[op.name()]);
      }
      break;

    default:
      debug() << "> Error" << endl;
      packetError(&fs, "Unknown BUD subtype %d\n", fs.subtype());
      return -1;
      break;
    }
  return 0;
}
