/*
    $Id: yahooprotocol.cpp,v 1.1 2002/06/06 17:21:49 thementat Exp $

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


	Modified by Jesse Lovelace <jllovela@eos.ncsu.edu>
    -----
    $Log: yahooprotocol.cpp,v $
    Revision 1.1  2002/06/06 17:21:49  thementat
    Initial revision

    Revision 1.4  2001/11/25 12:17:31  henrik
    gcc 3.0 fixes.

    Revision 1.3  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)
#include <iostream>
#include <string>

#include <stdio.h>
#include <strstream>

#include "yahooprotocol.h"
#include "log.h"
#include "xml.h"

HttpClient::HttpClient() {}

HttpClient::~HttpClient()
{
    m_parent->removeNet(m_net);
}

void HttpClient::setOwner(YahooProtocol *parent)
{
    m_parent = parent;
    m_net = parent->addNet();
}

void HttpClient::connect(const string &host, int port) {
    m_host = host;
    m_cid = m_net->connectTo(host, port);
}

void HttpClient::get(const string &url, const httpArgMap &args) {
    string st;
    httpArgMap::const_iterator it;
    st = "GET " + url;
    if (!args.empty())
    {
        st += "?";
        for(it = args.begin();it!=args.end();it++)
        {
            st += it->first + "=" + urlEncode(it->second);
            st += "&";
        }
        st = st.substr(0, st.length()-1);
    }
    st += " HTTP/1.0\r\n";
    st += "User-Agent: Mozilla/4.5\r\n";
    st += "Host: " + m_host + "\r\n";
    if (m_cookie != "")
        st += "Cookie: " + m_cookie + "\r\n";
    st += "\r\n";
    //debug() << "----" << endl << st << "----" << endl;
    m_net->sendData(st);
};

void HttpClient::setCookie(const string &c)
{
    m_cookie = c;
}

void HttpClient::clearCookie()
{
    m_cookie = "";
}

string HttpClient::urlEncode(const string &s) {
    return s;
}

vector<string> HttpClient::lineSplit(const string &s)
{
    int i=0, oi=0;
    vector<string> result;

    while ((i = s.find("\r\n", oi)) != -1)
    {
        result.push_back(s.substr(oi, i-oi));
        oi = i+2;
    }
    return result;
}

YahooProtocol::YahooProtocol(XMLNode &c,ProtocolManager *manager):
    Protocol(c,manager, "yahoo")
{
  debug() << "New YahooProtocol instance" << endl;
  m_net = addNet();
//  m_name = "yahoo";
  m_magicId = 0;
  httpClient.setOwner(this);
}

YahooProtocol::~YahooProtocol()
{
  debug() << "Deleting YahooProtocol instance" << endl;
}

void YahooProtocol::login()
{
  debug() << ("Logging in?");
  // Get auth cookie
  if (!m_conf.hasChild("authserver"))
      m_conf.addChild("authserver");
  if (m_conf.child("authserver")["host"] == "")
      m_conf.child("authserver")["host"] = "edit.messenger.yahoo.com";
  if (m_conf.child("authserver").intProperty("port") == 0)
      m_conf.child("authserver")["port"] = "80";

  m_istate=S_getCookie;
  m_state=S_connecting;
  debug() << "Connecting to cookie host" << endl;
  httpClient.connect(m_conf.child("authserver")["host"],
                     m_conf.child("authserver").intProperty("port"));

  eventStateChange(m_state);
}

void YahooProtocol::connectedToServer(Network *n)
{
    HttpClient::httpArgMap m;

    switch(m_istate)
    {
    case S_getCookie:
        m["login"] = m_conf.child("user")["username"];
        m["passwd"] = m_conf.child("user")["password"];
        m["n"] = "1";
        debug() << "Connected to cookie host" << endl;
        httpClient.get("/config/ncclogin", m);
        break;
    case S_connect:
        //Get config
        debug() << "Connected to Pager host" << endl;
        if (!m_conf.hasChild("data"))
            m_conf.addChild("data");
        if (m_conf.child("data")["host"] == "")
            m_conf.child("data")["host"] = "edit.messenger.yahoo.com";
        if (m_conf.child("data").intProperty("port") == 0)
            m_conf.child("data")["port"] = "80";
        m_istate = S_getConfig;
        httpClient.connect(m_conf.child("data")["host"],
                           m_conf.child("data").intProperty("port"));
        break;
    case S_getConfig:
        debug() << "Connected to conf host" << endl;
        m[".src"] = "bl";
        httpClient.get("/config/get_buddylist", m);
        break;
    default:
        debug() << "argh!" << endl;
    }
}

void YahooProtocol::logout()
{
    m_net->disconnect();
  eventStateChange(S_offline);
}

void YahooProtocol::update()
{

}


void YahooProtocol::sendMessage(const Contact &recipient, const string &message)
{
}

void YahooProtocol::addBuddy(const Contact &c)
{
  m_buddies[c.serverId()]=c;
  m_buddies[c.serverId()].setStatus(Contact::Offline);
  eventStatusChange(c);
}

void YahooProtocol::delBuddy(const Contact &c)
{
}

void YahooProtocol::newUser()
{
}

void YahooProtocol::getPubkey()
{

}

void YahooProtocol::handleData(Network *n)
{
  string data;
  debug() << "handle" << endl;
  m_net->socketData(data);

  if (data.empty())
    return;
  handleData(n, data);
}

/////////////////// network stuff ////////////////////////////
void YahooProtocol::handleData(Network *n, const string& buf)
{
  if (buf.empty())
    return;

  switch (m_istate)
  {
  case S_getCookie:
      {
          debug() << "Cookie reply" << endl;
          vector<string> lines = HttpClient::lineSplit(buf);
          vector<string>::iterator it;
          for (it=lines.begin();it!=lines.end();it++)
          {
              if (it->substr(0, 14) == "Set-Cookie: Y=")
              {
                  httpClient.setCookie(it->substr(12));
                  int start = it->find("n=") + 2;
                  int end = it->find("&", start);
                  m_login_cookie = it->substr(start, end-start);
                  debug() << "Cookie: '" << *it << "'" << endl;
                  debug() << "Login-Cookie: '" << m_login_cookie << "'" << endl;
              }
          }
      }
      break;
  case S_getConfig:
      debug() << "Conf reply" << endl;
      break;
  case S_logon:
      debug() << "logon reply" << endl;
      debug() << "data received from " << hex << n << dec << endl;
      debug() << "----" << endl << buf << "----" << endl;
      m_state = S_online;
      break;
  default:
      debug() << "FOO" << endl;
      break;
  }
}

void YahooProtocol::connectionError(Network *n, int error)
{
  switch (error)
    {
    case 0:
        if (n == httpClient.net())
        {
            switch (m_istate)
            {
            case S_getCookie:
                debug() << "Cookie host disconnected" << endl;
                // Connect to pager
                if (!m_conf.hasChild("pager"))
                    m_conf.addChild("pager");
                if (m_conf.child("pager")["host"] == "")
                    m_conf.child("pager")["host"] = "cs.yahoo.com";
                if (m_conf.child("pager").intProperty("port") == 0)
                    m_conf.child("pager")["port"] = "5050";
                debug() << "Connecting to pager host" << endl;
                m_istate = S_connect;
                m_net->connectTo(m_conf.child("pager")["host"],
                                 m_conf.child("pager").intProperty("port"));
                break;
            case S_getConfig:
                {
                    string msg;
                    msg = m_login_cookie + (char)1 +
                          m_conf.child("user")["username"];
                    debug() << "Config host disconnected" << endl;
                    debug() << "Logging on" << endl;
                    m_istate = S_logon;
                    sendCmd(YAHOO_SERVICE_LOGON,msg, 0);
                }
                break;
            }
            return;
        }
        else
        {
            debug() << ("Connection to server was refused.\n");
        }
        break;
      break;
    case 1:
      debug() << ("Server not found\n");
      break;
    case 2:
      debug() << ("Error reading from socket");
      break;
    }
    m_state = S_offline;
  eventStateChange(S_offline);

}

void YahooProtocol::storeInt(unsigned char *dest, unsigned int src)
{
    unsigned int tmp = src;
    int i;

    if (!dest)
        return;

    for (i = 0; i < 4; i++)
    {
        dest[i] = tmp % 256;
        tmp >>= 8;
    }
}

void hexdump(const unsigned char *data, int len);

/* Packet format
   00   4bytes          PROTOID         "YPNS"
   04   2bytes          VERSION         07 00
   06   4bytes          DATALEN
   0A   2bytes          ???
   0C   4bytes          ???
   10   4bytes          ???
   14   variable        DATA
*/
void YahooProtocol::sendCmd(int cmd, const string &data, int msgtype)
{
    int size = 4*256 + YAHOO_PACKET_HEADER_SIZE;
    int maxcontentsize = 4 * 256;
    unsigned char *packet = new unsigned char[size];
    unsigned char *point = packet;

    if (packet == 0)
        return;

    memset(packet, 0, size);

    //8 bytes version
    strcpy((char *)point, YAHOO_PROTOCOL_HEADER);
    point += 8;

    //4 bytes size
    storeInt(point, size);
    point += 4;

    //4 bytes command
    storeInt(point, cmd);
    point += 4;

    //4 bytes connection id
    storeInt(point, 0);
    point += 4;

    //4 bytes magic id
    storeInt(point, m_magicId);
    point += 4;

    //4 bytes Unknown1
    storeInt(point, 0);
    point += 4;

    //4 bytes msgtype
    storeInt(point, msgtype);
    point += 4;

    // 36 bytes nick1
    strncpy((char *)point, m_conf.child("user")["username"].c_str(), 35);
    point += 36;

    // 36 bytes active nick
    strncpy((char *)point, m_conf.child("user")["username"].c_str(), 35);
    point += 36;

    // data
    strncpy((char *)point, data.c_str(), maxcontentsize - 1);

    hexdump(packet, size);
    m_net->sendData((char *)packet, size);
}

#define DUMP_WIDTH 16

void hexdump(const unsigned char *data, int len)
{
#ifndef _WIN32
  unsigned int i, j, l=len;

  i=0;
  while(1)
    {
      for(j=0;j<DUMP_WIDTH;j++)
	{
	  if(j == DUMP_WIDTH/2)
	    debug() << " ";
	  if(i > len - 1)
	    debug() << "   ";
	  //	  else
	  //	    debug().form("%02x ", data[i]);
	  i++;
	}
      i -= DUMP_WIDTH;
      debug() << " ";
      for(j=0;j<DUMP_WIDTH;j++)
	{
	  if(j == DUMP_WIDTH/2)
	    debug() << " ";
	  if(i > len - 1)
	    debug() << " ";
	  else if (data[i] <= 32 || data[i] >= 128)
              debug() << ".";
	  else
	    debug() << (char)data[i];

	  i++;
	}
      debug() << endl;

      if (i>=len)
	break;
    }

/*  return;

  for (i=1;i<=l;++i)
    {
      debug().form("%02x ", (uint8)data[i-1]);
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
    } */
#endif
}

#ifdef TEST

int main()
{
  XMLNode n;
  YahooProtocol proto(n);
  debug() << "hej" << endl;
}
#endif
