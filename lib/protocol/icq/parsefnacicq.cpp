/*
    $Id: parsefnacicq.cpp,v 1.1 2002/06/06 17:21:52 thementat Exp $

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
    $Log: parsefnacicq.cpp,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.5  2001/12/12 01:47:26  mentat
    Adding gm_config.h.in and more #ifdef WIN32s to get rid of MSW template warning.

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

#include "fnacicqpackets.h"
#include "v7protocol.h"
#include "basenetwork.h"

#include <strstream>

using namespace std;
int IcqProtocol::parseFnacIcq(const FSPacket &fs)
{
  debug() << "> ICQ ";
  switch (fs.subtype())
    {
    case SUB_ICQ_RESPONSE:
      {
	TLV t;
	bool ok = false;
	uint16 _msgtype;
	uint32 _subtype;
	uint16 i=0;
	FSPacket tmp = fs;
	debug() << "> Response" << endl;
	//PktSIcqResponse pak(fs);
	tmp.reset();

	if (tmp.getTLV(t))
	  {
	    if (t.type() != TLV::TLV_ICQRES)
	      debug() << "SUB_ICQ_RESPONSE: wrong tlv catched" << endl;
	    else
	      {
		ok = true;
		dstring data = t.value();
		
		//Parse out common fields
		uint16 size = littleToU16(data, i);
		i+= 2;
		uint32 my_uin = littleToU32(data, i);
		i+= 4;
		_msgtype = littleToU16(data, i);
		i+= 2;
		uint16 req_id = littleToU16(data, i);
		i+= 2;
		
		switch (_msgtype)
		  {
		  case 0x0041:
		    //Offline message
		    break;
		  case 0x0042:
		    // End of offline messages
		    break;
		  case 0x07D0:
		    // XML-something...
		    break;
		  case 0x07DA:
		    // Hmm.. _3_ bytes subtype? why? let's do an ugly hack to read it
		    _subtype = littleToU32(data, i) & 0x00FFFFFF;
		    i+=3;
		    switch(_subtype)
		      {
		      case 0x0A08A2:
			// Where to get ads stuff
			break;
		      case 0x3201a4:
		      case 0x3201ae:
			// Empty whitepages result
			break;
		      case 0x0a01a4:
			// wp-full-request result (more coming)
			break;
		      case 0x0a01ae:
			// wp-full-request result (last)
			break;
		      case 0x0a0190:
			// wp-short-request result (more coming)
			break;
		      case 0x0a019a:
			{
			  // wp-short-request result (last)
			  i+=2; // skip unknown bytes
			  strstream foo;
			  uint16 len;
			  
			  foo << littleToU32(data, i) << '\0';
			  i+=4;
			  string uin = foo.str();
			  len = littleToU16(data, i);
			  i+= 2;
			  string nick = data.substr(i, len);
			  i+=len;
			  /*len = littleToU16(data, i);
			    i+=2;
			    first = getString(len);
			    len = littleToU16(data, i);
			    i+=2;
			    last = getString(len);
			    len = littleToU16(data, i);
			    i+=2;
			    email = getString(len);*/
			  //*this >> _auth >> _status >> _unknown;
			  
			  // Update the contact list
			  m_buddies[uin].setNick(nick);
			  eventStatusChange(m_buddies[uin]);
			  debug() << uin << " has nickname " << nick << endl;
			}
			break;
		      case 0x0a00c8:
			// main-home-info
			break;
		      case 0x0a00d2:
			// work-info
			break;
		      case 0x0a00e6:
			// about
			break;
		      case 0x0a00f0:
			// personal-interests-info
			break;
		      case 0x0a00fa:
			// past-background-info
			break;
		      case 0x1400fa:
			// query result: non-existing user
			break;
		      case 0x0a00eb:
			// more-email-info
			break;
		      case 0x0a00dc:
			// homepage-more-info
			break;
		      case 0x0a010e:
			// additional info
			break;
		      case 0x0a0064:
			// ack to modify main/home
			break;
		      case 0x0a0078:
			// ack to modify homepage/more
			break;
		      case 0x0a0082:
			// ack to modify about
			break;
		      case 0x0a006e:
			// ack to modify work
			break;
		      case 0x0a00b4:
			// ack to remove user
			break;
		      case 0x0a00aa:
			// ack to change password
			break;
		      case 0x0a00a0:
			// unknown, reply to 2404
			break;
		      default:
			ok=false;
			debug() << "PktSIcqResponse: unknown message subtype: 0x" << hex << _subtype << dec << endl;
			break;
		      }
		    break;
		  default:
		    ok=false;
		    debug() << "PktSIcqResponse: unknown message type: 0x" << hex << _msgtype << dec << endl;
		    break;
		  }
	      }
	  }
	
	if (!ok)
	  {
	    packetError(&tmp, "Malformed message packet\n");
	    return -1;
	  }

	debug() << "Got response: 0x" << hex << _msgtype <<
	  " subtype 0x" << _subtype << dec << endl;
      }
      break;
    default:
      debug() << "> Error" << endl;
      packetError(&fs, "Unknown ICQ subtype %d\n", fs.subtype());
      return -1;
      break;
    }
  return 0;
}
