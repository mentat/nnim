/*
    $Id: parsefnacmsg.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsefnacmsg.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.8  2001/12/13 19:01:54  mentat
    Took out some logging i added from toc, fixed a bug in toc that was
    confusing my client (and me), it was:  m_state was defined as private
    in tocprotocol.h (int) but it was also defined in the parent protocol.h, so
    when I called getState, the protocol would return the wrong one. Fixed now.

    Revision 1.7  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.6  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.5  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.4  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <log.h>
#include "icqprotocol.h"

#include "fnacmsgpackets.h"
#include "fnacicqpackets.h"
#include "v7protocol.h"
#include "basenetwork.h"

int IcqProtocol::parseFnacMsg(const FSPacket &fs)
{
  debug() << "> MSG ";
  switch (fs.subtype())
    {
    case SUB_MSG_PARMINFO:
      debug() << "> ParmInfo" << endl;
      break;
    case SUB_MSG_RECVMSG:
      debug() << "> RecvMsg" << endl;
      {
	PktSMsgRecv mp(fs);
	if (!mp.ok())
	  {
	    packetError(&mp, "Malformed message packet\n");
	    return -1;
	  }
	debug() << "Got a message from: " << mp.sender() << endl;
	debug() << "Message: '" << mp.message() << "'" << endl;
	if (!m_buddies.count(mp.sender()))
	  {
	    Contact c;
	    c.setProtocol(protocol());
	    c.setServerId(mp.sender());
	    c.setStatus(Contact::Online);
	    c.info().setProperty("temp", 1);
	    m_buddies[mp.sender()] = c;
	  }
	//m_net->sendData(PktCIcqGetSInfo(_seq++, m_conf.child("user")["username"], mp.sender()));
	eventRecvdMessage(m_buddies[mp.sender()], mp.message());
      }
      break;
    default:
      debug() << "> Error" << endl;
      packetError(&fs, "Unknown MSG subtype %d\n", fs.subtype());
      return -1;
      break;
    }
  return 0;
}
