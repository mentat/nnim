/*
    $Id: parsefnacgen.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsefnacgen.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.6  2001/12/12 01:47:26  mentat
    Adding gm_config.h.in and more #ifdef WIN32s to get rid of MSW template warning.

    Revision 1.5  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.4  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.3  2001/10/04 16:26:37  abelsson
    Added missing protocol status callbacks.

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


int IcqProtocol::parseFnacGen(const FSPacket &fs)
{
  debug() << "> GEN ";
  switch (fs.subtype())
    {
    case SUB_GEN_SERVERREADY: // Server is ready for normal operation
      debug() << "> ServerReady" << endl;
      m_net->sendData(PktCGenSetStatus(_seq++, m_status));
      m_net->sendData(PktCGenClientVersions(_seq++));
      break;
    case SUB_GEN_SERVERVERSIONS:
      debug() << "> ServerVersions" << endl;
      m_net->sendData(FCPacket(_seq++, FAMILY_GEN, SUB_GEN_RATEREQUEST));
      break;
    case SUB_GEN_RATERESPONSE:
      debug() << "> RateResponse" << endl;
      m_net->sendData(FCPacket(_seq++, FAMILY_GEN, SUB_GEN_RATERESPONSEACK,
			       dstring((uint8*)"\0\x1\0\x2\0\x3\0\x4\0\x5", 10)));
      m_net->sendData(FCPacket(_seq++, FAMILY_GEN, SUB_GEN_REQSELFINFO));
      m_net->sendData(FCPacket(_seq++, FAMILY_LOC, SUB_LOC_REQRIGHTS));
      m_net->sendData(FCPacket(_seq++, FAMILY_BUD, SUB_BUD_REQRIGHTS));
      m_net->sendData(FCPacket(_seq++, FAMILY_MSG, SUB_MSG_REQPARMINFO));
      m_net->sendData(FCPacket(_seq++, FAMILY_BOS, SUB_BOS_REQRIGHTS));
      //Continue in SUB_GEN_SELFINFO
      break;
    case SUB_GEN_SELFINFO:
      debug() << "> SelfInfo" << endl;

      m_net->sendData(FCPacket(_seq++, FAMILY_MSG, SUB_MSG_ADDPARAM,
			       dstring((uint8*)ICBM_PARAM_STRING, 16)));
      m_net->sendData(PktCLocCapinfo(_seq++));
      if (m_buddies.size() > 0)
	m_net->sendData(PktCBudAdd(_seq++,m_buddies));
      m_net->sendData(FCPacket(_seq++, FAMILY_GEN, SUB_GEN_CLIENTREADY,
		      dstring((uint8*)CLIENT_RDY_STRING, 64)));
      
      break;
    case SUB_GEN_MOTD:
      debug() << "> MOTD" << endl;
      debug() << "-------------------------" << endl;
      debug() << "And we're up and running!" << endl;
      debug() << "-------------------------" << endl;
      m_state = S_online;
      eventStateChange(S_online);

      eventLoggedIn(); //Call virtual function
      { // Added braces because VC++ complains, needed to change scope
        for (buddy_t::const_iterator it=m_buddies.begin();it!=m_buddies.end();it++)
        m_net->sendData(PktCBudAdd(_seq++, it->second));
      }
      break;
    default:
      debug() << "> Error" << endl;
      packetError(&fs, "Unknown GEN subtype %d\n", fs.subtype());
      return -1;
      break;
    }
  return -100;
}
