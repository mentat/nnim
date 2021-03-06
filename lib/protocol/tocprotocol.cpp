/*
    $Id: tocprotocol.cpp,v 1.11 2002/06/27 22:33:27 thementat Exp $

    GNU Messenger - The secure instant messenger

    Original TOC implementation: 
	Copyright (C) 1999-2001  Henrik Abelsson <henrik@abelsson.com>

	Re-written implementation in C++:
	Copyright (C) 2002 Jesse Lovelace <jllovela@eos.ncsu.edu>

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
*/

#pragma warning(disable:4786)

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <basenetwork.h>

#include "log.h"
#include "dstring.h"
#include "protocol.h"
#include "tocprotocol.h"
#include "crypto/misc.h"
#include "buffer.hpp"

using namespace std;
using namespace CryptoPP;

string GetError(int i, const string& name)
{
     switch (i)
     {
         case(901): return name + string(" not currently available"); break;
         case(902): return string("Warning of ") + name + (" not currently available"); break;
         case(903): return "A message has been dropped, you are exceeding the server speed limit"; break;

         case(911): return "Error validating input"; break;
         case(912): return "Invalid account"; break;
         case(913): return "Error encountered while processing request"; break;
         case(914): return "Service unavailable"; break;
         case(950): return string("Chat in ") + name + string("is unavailable"); break;
         case(960): return string("You are sending message too fast to ") + name; break;
         case(961): return string("You missed an im from ") + name + (" because it was too big."); break;
         case(962): return string("You missed an im from ") + name + (" because it sent to fast."); break;
         case(970): return "Failure"; break;
         case(971): return "Too many matches"; break;
         case(972): return "Need more qualifiers"; break;
         case(973): return "Dir service temporarily unavailable"; break;
         case(974): return "Email lookup restricted"; break;
         case(975): return "Keyword Ignored"; break;
         case(976): return "No Keywords"; break;
         case(977): return "Language not supported"; break;
         case(978): return "Country not supported"; break;
         case(979): return string("Failure unknown ") + name; break;
         case(980): return "Incorrect nickname or password."; break;
         case(981): return "The service is temporarily unavailable."; break;
         case(982): return "Your warning level is currently too high to sign on."; break;
         case(983): return string("You have been connecting and disconnecting too frequently.  "
         "Wait 10 minutes and try again.  "
	     "If you continue to try, you will need to wait even longer."); break;
         case(989): return string("An unknown signon error has occurred ") + name; break;

         //default: return "An unknown error has occured." break;
    }
}


  /*
 * General Errors *
   901   - $1 not currently available
   902   - Warning of $1 not currently available
   903   - A message has been dropped, you are exceeding
	   the server speed limit

   * Admin Errors  *
   911   - Error validating input
   912   - Invalid account
   913   - Error encountered while processing request
   914   - Service unavailable

   * Chat Errors  *
   950   - Chat in $1 is unavailable.

   * IM & Info Errors *
   960   - You are sending message too fast to $1
   961   - You missed an im from $1 because it was too big.
   962   - You missed an im from $1 because it was sent too fast.

   * Dir Errors *
   970   - Failure
   971   - Too many matches
   972   - Need more qualifiers
   973   - Dir service temporarily unavailable
   974   - Email lookup restricted
   975   - Keyword Ignored
   976   - No Keywords
   977   - Language not supported
   978   - Country not supported
   979   - Failure unknown $1

   * Auth errors *
   980   - Incorrect nickname or password.
   981   - The service is temporarily unavailable.
   982   - Your warning level is currently too high to sign on.
   983   - You have been connecting and
	   disconnecting too frequently.  Wait 10 minutes and try again.
	   If you continue to try, you will need to wait even longer.
   989   - An unknown signon error has occurred $1
*/



class flapHdr {
public:
  
	flapHdr() 
	{ 
		m_ast=(byte)'*';
		m_type=(byte)2;
		m_seq = NBO(real_seq++);
		m_len=(byte)0;
	}


 private:
	byte m_ast;
	word16 m_seq;
 public:
 	byte m_type;
	word16 m_len;

	vbuf getHdr()
	{
		vbuf header;

		header += m_ast;
		header += m_type;
		header += (byte) (m_seq & 0xff);
		header += (byte) ((m_seq >> 8)& 0xff);
		header += (byte) (m_len & 0xff);
		header += (byte) ((m_len >> 8)& 0xff);

		return header;
	}

private:
	static word16 real_seq;
};

word16 flapHdr::real_seq=1;

string 
TocProtocol::aim_encode(const string& s)
{
  string buf;
  for (unsigned int i = 0; i < s.length(); i++)
  {
    switch (s[i]) {
    case '$':
    case '{':
    case '}':
    case '[':
    case ']':
    case '(':
    case ')':
    case '\"':
    case '\\':
      buf += '\\';
      buf += s[i];
      break;

    default:
      buf += s[i];
      break;
    }
  }
  return buf;
}

string TocProtocol::aim_normalize(const string& s)
{
	string buf;

	for (unsigned int i=0; i<s.length(); i++)
	{
		char c=s[i];
		if (c != ' ' && c!='\t' && c!='\n')
		buf+=tolower(c);
	}
	return buf;
}

string TocProtocol::roastPassword(const string& pass)
{
    /* Trivial "encryption" */
    stringstream pword;
    pword << "0x";
    string roast("Tic/Toc");

    for (unsigned int i = 0; i < pass.length(); i++)
    {
        pword << hex << setfill('0') << setw(2) << (word16)(pass[i] ^ roast[i % roast.length()]);
    }

	return pword.str();
}

void TocProtocol::send_flap(int type, const vbuf& data)
{
	m_net->sendData(return_flap(type, data));
}

vbuf TocProtocol::return_flap(int type, const vbuf& data)
{
	// output buffer object
	vbuf buffer;

	/*
	* FLAP Header (6 bytes)
	* -----------


	* Offset   Size  Type
	* 0        1     ASTERISK (literal ASCII '*')
	* 1        1     Frame Type
	* 2        2     Sequence Number
	* 4        2     Data Length
	*/

	// flap header object
	flapHdr fh;

	// set length to payload length
	word16 len = data.size();

	// set flap type
	fh.m_type = (byte)type;
	// put length in network byte order
	fh.m_len = NBO(len);

	// get the header and return it to the output buffer
	buffer = fh.getHdr();

	if (len > 0)
		buffer += data;


	return buffer;
}

void TocProtocol::toc_send_keep_alive()
{
  send_flap(TYPE_KEEPALIVE, vbuf());
}


TocProtocol::TocProtocol(const XMLNode &n, ProtocolManager *manager):
  Protocol(n,manager, "toc")
{
  m_away = false;
  m_realLength = 0;
  m_state=S_offline;
  m_net = addNet();
  debug() <<  "New TocProtocol instance" << endl ;
}

TocProtocol::~TocProtocol()
{
  //removeNet(m_net);
  debug() << "Deleting TocProtocol instance" << endl ;
}


void TocProtocol::login() //string server, int port, string name, string pass)
{
	// initiate socket connection to host
	m_net->connectTo(m_conf.child("server").property("host"),
		m_conf.child("server").intProperty("port"));

	// fire connecting event
	eventStateChange(S_connecting);

}

void TocProtocol::logout()
{
	m_net->disconnect();

	for(buddy_t::iterator i=m_buddies.begin();i!=m_buddies.end();i++)
	{
		i->second.setStatus(Contact::Offline);
		eventStatusChange(i->second);
	}
	eventStateChange(S_offline);
	m_state = S_offline;


}


void TocProtocol::sendMessage(const Contact &c, const string &message)
{
    debug() << "sendMessage";
    vbuf msg;
    msg += string("toc_send_im ");
    msg += aim_normalize(c.serverId());
    msg += string(" \"");
    msg += aim_encode(message);
    msg += string("\"");
    msg += (byte)0;

    send_flap(TYPE_DATA,msg);
}


void TocProtocol::sendMessageAuto(const Contact &c, const string &message)
{
    debug() << "sendMessageAuto\n";
    vbuf msg;
    msg += string("toc_send_im ");
    msg += aim_normalize(c.serverId());
    msg += (byte)' ';
    msg += (byte)'\"';
    msg += aim_encode(message);
    msg += string("\" \"auto\"");
    msg += (byte)0;

    send_flap(TYPE_DATA,msg);
}

void TocProtocol::addBuddy(const Contact &c)

{
	if (m_state==S_online)
		send_flap(TYPE_DATA,"toc_add_buddy "+aim_normalize(c.serverId()));

	m_buddies[aim_normalize(c.serverId())] = c;
	m_buddies[aim_normalize(c.serverId())].setStatus(Contact::Offline);
	eventStatusChange(c);

//   if (nick=="")
//     m_buddies[name]=name;
//   else
//     m_buddies[name]=nick;

}

void TocProtocol::delBuddy(const Contact &c)
{
	(void)c;
	debug() << "delBuddy(): Not implemented yet" << endl;
}


// some kind of error occured.
void TocProtocol::connectionError(Network *net,int e)
{
  eventError(e, "A Network error occured.");

  m_state = S_offline;

  debug() << "Connection closed!" << endl;

  m_net->disconnect();

  for(buddy_t::iterator i=m_buddies.begin(); i!=m_buddies.end(); i++)
  {
    i->second.setStatus(Contact::Offline);
    eventStatusChange(i->second);
  }

  eventStateChange(S_offline);
}

void TocProtocol::connectedToServer(Network *net)
{
	// when socket tells that we are connected, start TOC process
	vbuf flap("FLAPON\r\n\r\n", 10);
	m_net->sendData(flap);

	debug() << ("FLAPON");

	// update state to connecting
	m_state=S_connecting;
}

void TocProtocol::signup()
{
	debug() << "doing FLAP SIGNON" ;

	string normalizedUsername(aim_normalize(m_conf.child("user").property("username")));
	word16 usernameLength = (word16)normalizedUsername.length();

    // create pre-flapon packet (same packet really)
    flapHdr pre;
    pre.m_type = (byte)TYPE_SIGNON;
    pre.m_len = NBO(word16(usernameLength + 8));

    usernameLength = NBO(usernameLength);

    // create flapon packet
    vbuf flapOn;
	flapOn += (byte)0;
	flapOn += (byte)0;
	flapOn += (byte)0;
	flapOn += (byte)1;
	flapOn += (byte)0;
	flapOn += (byte)1;

	flapOn += (byte) (usernameLength & 0xff);
	flapOn += (byte) ((usernameLength >> 8) & 0xff);
	
	flapOn += normalizedUsername;

    // send pre flapon
    m_net->sendData(pre.getHdr());
    // send flapon
    m_net->sendData(flapOn);

	//send_flap(TYPE_SIGNON, flapOn);

  /*
   * toc_signon <authorizer host> <authorizer port> <User Name> <Password>
   *            <language> <version>
   */

	vbuf tocSignon;
    tocSignon += string("toc_signon ");
    tocSignon += m_conf.child("loginserver").property("host");
    tocSignon += (byte)' ';
    tocSignon += m_conf.child("loginserver").property("port");
    tocSignon += (byte)' ';
    tocSignon += normalizedUsername;
    tocSignon += (byte)' ';
    tocSignon += roastPassword(m_conf.child("user").property("password"));
    tocSignon += string(" english \"NNIM\"");
    tocSignon += (byte)0;

	send_flap(TYPE_DATA, tocSignon);

	m_state=S_online;
}

vector<string> splitStr(const string& str)
{
	unsigned int i=0;
	vector<string> words;

	string tmp;
	while (i<str.length())
	{
		while(str[i]!=':' && i<str.length())
		tmp+=str[i++];
		i++;
		debug() << ( "word:" + tmp);
		words.push_back(tmp);

		tmp="";
	}

	return words;

}

vector<string> splitStrLF(const string& str)
{
	unsigned int i=0;
	vector<string> words;
	string tmp;


	while (i<str.length())
	{
		while(str[i]!=0xA && i<str.length())
			tmp+=str[i++];
		i++;
		debug() << ( "word:" + tmp) << endl;
		words.push_back(tmp);
		tmp="";
	}
	return words;

}

void TocProtocol::handleData(Network *net)
{
	vbuf data;
	net->socketData(data);

	if (data.size() == 0)
		return;

	handleData(net,data);

}

void TocProtocol::handleData(Network *net, const vbuf &data)
{

	if (m_buffer.size() != 0)
	{ 
		vbuf newData;
		// do concat buffer with present data

		newData += m_buffer;
		m_buffer.clear();

		newData += data;
		handleData(net, newData);
		return;
	}

	// extract payload length from data
	word16 payLoadLength;
	payLoadLength = (word16)data[4];
	payLoadLength |= (word16)(data[5] << 8);

	// convert from network byte order
	payLoadLength = NBO(payLoadLength);

	// see if payload length is greater then actual payload, 
	// if so, more on the way

    debug() << "Payload length: " << payLoadLength << "\nData length: " << data.size() << endl;

	if (payLoadLength > (data.size() - 6))
		m_buffer +=  data;
	else
	{
		handleRealData(net, string((const char *)(data.data()+6), payLoadLength));
		
		// recurse if there is more than one command in data stream
		if (data.size() > (payLoadLength+6))
		{
            debug() << "More than one command in data stream.\n";
			handleData(net, data.sub(payLoadLength + 6));
		}
	}
}

void TocProtocol::handleRealData(Network *net, const string& data)
{

    debug() << "Got Data: " << data << endl;

	string command;

	// if we are connecting, we should just go to signup
	// but the actual data should be the flap signon msg
	if (m_state == S_connecting)
	{
		signup();
		return;
	}

	unsigned int position(data.find(':', 0));



	command = data.substr(0, position);

    debug() << command << endl;

	if (command == "IM_IN")
	{
        debug() << "In IM IN (say that 5 times fast)" << endl;
		vector <string> words = splitStr(data);

		string msg = words[3];

		if (m_buddies.find(words[1])==m_buddies.end())
		{	
            debug() << "Anonymous user msg." << endl;
			// if we can't find the user, we prompt for acceptance
			Contact c;
			c.setProtocol(protocol());
			c.setServerId(words[1]);
			c.setNick(words[1]);

			eventRecvdMessageNotBuddy(c, msg);
			return;
		}

		// if we are away, respond with away message
		if (m_away)
		{
			if (m_awayMessage == "")
				sendMessageAuto(m_buddies[words[1]], "I am away");
			else
				sendMessageAuto(m_buddies[words[1]], m_awayMessage);
		}
		// if is auto responce maybe say so
		if (words[2] == "T")
			msg = "Auto responce: " + msg;

		eventRecvdMessage(m_buddies[words[1]],msg);
		return;
	}


	if (command == "UPDATE_BUDDY")
	{
		vector <string> words=splitStr(data);

		string user=words[1];
		/* are we online? */
		bool online;
		if (words[2] == "T")
			online = true;
		else 
			online = false;

		if (m_buddies.count(aim_normalize(user))==1)
		{
			if (online)
				m_buddies[aim_normalize(user)].setStatus(Contact::Online);
			else
				m_buddies[aim_normalize(user)].setStatus(Contact::Offline);

			if (words[6][2] == 'U')
				m_buddies[aim_normalize(user)].setStatus(Contact::Away);

			switch (words[6][1]) {
				case ('A'): break; // Oscar Admin
				case ('U'): break; // Oscar Unconfirmed
				case ('O'): break; // Oscar Normal
			}

			if (words[6][0] == 'A')
			{
				// on AOL
			}

			eventStatusChange(m_buddies[aim_normalize(user)]);
		}
		return;
	}

	if (command == "FLAP SIGNON")
	{
		signup();
		return;
	}
	if (command == "SIGN_ON")
	{
        debug() << "CONNECTED TO TOC!" << endl;
		return;
	}
	if (command == "CONFIG")
	{
		vbuf setInfo("toc_set_info ");
        setInfo += aim_encode("\"NNIM\""/*"\"Visit NNIM at <A HREF=\"http://nnim.sourceforge.net\">http://nnim.sourceforge.net</A>.\""*/);
		setInfo += (byte)0;
        send_flap(TYPE_DATA, setInfo);
		
        vbuf addBuddy("toc_add_buddy");

		if (m_buddies.empty())
        {
            addBuddy += (byte)' ';
		    addBuddy += aim_normalize(screenName());
		
				
		    for (buddy_t::iterator i=m_buddies.begin(); i!=m_buddies.end(); i++)
		    {
                addBuddy += (byte)' ';
			    addBuddy +=aim_normalize(i->second.serverId());
		    }

        }

        addBuddy += (byte)0;
        send_flap(TYPE_DATA, addBuddy);

        vbuf initDone("toc_init_done");
        initDone += (byte)0;

        send_flap(TYPE_DATA, initDone);

		eventStateChange(S_online);

		//m_net->sendData(tempConfigWhole);

		tocParseConfig(data.substr(7,data.length()));
		return;
	}
	if (command == "NICK")
	{
		debug() << "got my nick\n";
		
		m_screenName = data.substr(5, data.length());
		return;
	}

	if (command == "ERROR")
	{
        eventError(Protocol::E_network, data);
        debug() << data << endl;
		return;
	}
	if (command == "EVILED")
	{
		return;
	}

	if (command == "CHAT_JOIN")
	{
		return;

	}

	if (command == "CHAT_IN")
	{
		return;
	}

	if (command == "CHAT_UPDATE_BUDDY")
	{
		return;

	}

	if (command == "CHAT_INVITE")
	{
		return;

	}

	if (command == "CHAT_LEFT")
	{
		return;

	}

	if (command == "GOTO_URL")
	{
		return;
	}

	if (command == "DIR_STATUS")
	{
		return;

	}

	if (command == "ADMIN_NICK_STATUS")
	{
		return;

	}

	if (command == "ADMIN_PASSWD_STATUS")
	{
		return;
	}

	if (command == "PAUSE")

	{
		return;

	}


	if (command == "RVOUS_PROPOSE")
	{
		return;
	}

	debug() << "TOC: unknown command OR got other half (or more) of user config\n";

}

void TocProtocol::update()
{

//  toc_send_keep_alive();
}
void TocProtocol::newUser()
{

}
void TocProtocol::getPubkey()
{

}

void TocProtocol::setAway(const string& msg)
{

  if (msg == "")
  {
    m_away = false;
    m_awayMessage = "";
  }
  else
  {
    m_away = true;
    m_awayMessage = aim_encode(msg);
  }


  string temp = "toc_set_away \"" + aim_encode(msg) + '\"';
  send_flap(TYPE_DATA, temp);

}

void TocProtocol::setInfo(const string& info)
{

  string temp = "toc_set_info \"" + aim_encode(info) + '\"';
  send_flap(TYPE_DATA, temp);

}

void TocProtocol::tocParseConfig(const string& config)
{
  debug() << config << endl;
  vector<string> myContacts = splitStrLF(config);

  string currentGroup = "Buddies";
  for (unsigned int it = 0; it < myContacts.size(); it++)
  {
    if (myContacts[it][0] == 'g')
    {
      currentGroup = myContacts[it].substr(2);
    }
    else
      if (myContacts[it][0] == 'b')
      {
        m_buddiesFromServer[myContacts[it].substr(2)] = currentGroup;
        debug() << myContacts[it].substr(2) << " " << currentGroup << endl;
      }
  }

}

/*
    -----
    $Log: tocprotocol.cpp,v $
    Revision 1.11  2002/06/27 22:33:27  thementat
    Fixed TOC error and applied patch for gcc 2.95 in AuthLoad

    Revision 1.10  2002/06/27 11:52:51  thementat
    More event handling fixes.

    Revision 1.9  2002/06/26 17:40:12  thementat
    Added the Open-Source ssh2 lib from Bitvise.

    Revision 1.8  2002/06/26 04:27:08  thementat
    Event fixes.

    Revision 1.7  2002/06/25 16:48:14  thementat
    Got TOC done! (mostly)

    Revision 1.6  2002/06/25 04:46:21  thementat
    More toc work.

    Revision 1.5  2002/06/24 18:00:50  thementat
    Fixed TOC sequence bug.

    Revision 1.4  2002/06/24 12:07:40  thementat
    Toc fixes.

    Revision 1.3  2002/06/23 18:35:51  thementat
    Added vbuf class and changing all protocol to use.

    Revision 1.2  2002/06/23 14:50:01  thementat
    Work on TOC protocol and new buffer class.

    Revision 1.1.1.1  2002/06/06 17:21:50  thementat
    Checkin of new sources BETA 2

    Revision 1.13  2001/12/17 18:33:42  mentat
    Importing changes for NNIM Beta 1 client.

    Revision 1.11  2001/12/16 19:46:50  mentat
    Updates to TOC protocol and authload class.

    Revision 1.10  2001/12/13 19:01:54  mentat
    Took out some logging i added from toc, fixed a bug in toc that was
    confusing my client (and me), it was:  m_state was defined as private
    in tocprotocol.h (int) but it was also defined in the parent protocol.h, so
    when I called getState, the protocol would return the wrong one. Fixed now.

    Revision 1.9  2001/12/12 02:10:46  mentat
    Added #include <sys/time.h> to tocprotocol.cpp to resolve FreeBSD compile error, fixed small error in authload.cpp

    Revision 1.8  2001/12/08 21:45:27  mentat
    Added setAway and setAllAway to Protocol Manager along with modifying some protocol code.



    Revision 1.7  2001/12/06 04:46:40  mentat
    Added setAway() and setAllAway(...) to manager class and to toc protocol, also added changes to toc so that will log in with wx client.

    Revision 1.6  2001/11/24 00:32:44  henrik
    Addcontact dialog restructuring, chat improvements, login/logout improvements.

    Revision 1.5  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.4  2001/10/04 16:26:37  abelsson
    Added missing protocol status callbacks.



    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

