/*
    $Id: old_tocprotocol.cpp,v 1.1 2002/06/06 17:21:49 thementat Exp $

    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2001  Henrik Abelsson <henrik@abelsson.com>

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
    $Log: old_tocprotocol.cpp,v $
    Revision 1.1  2002/06/06 17:21:49  thementat
    Initial revision

    Revision 1.5  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.4  2001/10/04 16:26:37  abelsson
    Added missing protocol status callbacks.

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Note: windows has no sleep() because has no unistd, does have a Sleep()
// declared in windows.h -jll

#ifndef _WIN32
#include <netinet/in.h>
#include <unistd.h>
#else

#include <winsock.h>
#endif

#include <basenetwork.h>

#include "log.h"
#include "protocol.h"
#include "tocprotocol.h"


class flapHdr {
public:
  flapHdr() { ast='*';type=2;seq=htons(real_seq++);len=0;};

  unsigned char ast;
  unsigned char type;
  unsigned short seq;
  unsigned short len;
private:
  static unsigned short real_seq;
};

unsigned short flapHdr::real_seq=0;

string  TocProtocol::aim_encode(string s)
{
  int len = s.length();
  int i = 0;
  string buf;
  for (i = 0; i < len; i++)
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

string TocProtocol::aim_normalize(string s)
{
  string buf;

  for (int i=0;i<s.length();i++)
  {
    char c=s[i];
    if (c != ' ' && c!='\t' && c!='\n')
      buf+=tolower(c);
  }
  return buf;
}

unsigned char *TocProtocol::roast_password(const char *pass)
{
  /* Trivial "encryption" */
  static char rp[256];
  static char *roast =  "Tic/Toc";
  int pos = 2;
  int x;

  strcpy(rp, "0x");
  for (x = 0; (x < 150) && pass[x]; x++)
    pos += sprintf(&rp[pos], "%02x", pass[x] ^ roast[x % strlen(roast)]);
  rp[pos] = '\0';
  return (unsigned char *) rp;
}

void TocProtocol::send_flap(int type, string data)
{
  send_flap(type,data.c_str(),data.length()+1);
}
void TocProtocol::send_flap(int type, const char * data,int _len)
{
  char buff[2048];
  int len;

  /*
   * FLAP Header (6 bytes)
   * -----------
   * Offset   Size  Type
   * 0        1     ASTERISK (literal ASCII '*')
   * 1        1     Frame Type
   * 2        2     Sequence Number
   * 4        2     Data Length
   */

  flapHdr fh;

  if (!data)
    len=0;
  else if (_len==-1)
    len=strlen(data)+1;
  else
    len=_len;


  fh.type = type;
  fh.len = htons((short)len);

  memcpy(buff,&fh.ast,1);
  memcpy(buff+1,&fh.type,1);
  memcpy(buff+2,&fh.seq,2);
  memcpy(buff+4,&fh.len,2);

  if (len>0)
    memcpy(buff + 6, data,len);
  m_net->sendData(buff,6+len);
}

void TocProtocol::toc_send_keep_alive()
{
  send_flap(TYPE_KEEPALIVE,NULL,0);
}


TocProtocol::TocProtocol(const XMLNode &n, ProtocolManager *manager):
  Protocol(n,manager, "toc")
{
  m_state=S_offline;
  m_net = addNet();
  debug() <<  "New TocProtocol instance" ;
}

TocProtocol::~TocProtocol()
{
  debug() << "Deleting TocProtocol instance" ;
}


void TocProtocol::login() //string server, int port, string name, string pass)
{
  m_net->connectTo(m_conf.child("server").property("host"),
                   m_conf.child("server").intProperty("port"));
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
}

void TocProtocol::sendMessage(const Contact &c, const string &message)
{
  string msg="toc_send_im ";
  msg+=aim_normalize(c.serverId())+" \"";
  msg+=aim_encode(message)+"\"";

  send_flap(TYPE_DATA,msg);
}

void TocProtocol::addBuddy(const Contact &c)
{
  if (m_state==S_online)
    send_flap(TYPE_DATA,"toc_add_buddy "+aim_normalize(c.serverId()));

  m_buddies[c.serverId()] = c;
  m_buddies[c.serverId()].setStatus(Contact::Offline);
  eventStatusChange(c);

//   if (nick=="")
//     m_buddies[name]=name;
//   else
//     m_buddies[name]=nick;

}

void TocProtocol::delBuddy(const Contact &c)
{
  (void)c;
  debug() << "delBuddy(): Not implemented yet";
}


// some kind of error occured.
void TocProtocol::connectionError(Network *net,int e)
{
  (void)e;
  m_state = S_offline;
  debug() << "Connection closed!";
  m_net->disconnect();
  for(buddy_t::iterator i=m_buddies.begin();i!=m_buddies.end();i++)
  {
    i->second.setStatus(Contact::Offline);
    eventStatusChange(i->second);
  }
  eventStateChange(S_offline);
}

void TocProtocol::connectedToServer(Network *net)
{
  char flapon[]="FLAPON\r\n\r\n";
  m_net->sendData(flapon,strlen(flapon));
  debug() << ("FLAPON");
  m_state=S_connecting;
}

void TocProtocol::signup()
{
  string normalized_username = aim_normalize(m_conf.child("user").property("username"));
  unsigned short username_length = htons(normalized_username.length());

  debug() << "doing FLAP SIGNON" ;
  char buff[2048];
  char sflap_header[] = { 0, 0, 0, 1, 0, 1 };


  memcpy(buff , sflap_header, 6);
  memcpy(buff+6, &username_length, 2);
  memcpy(buff+8, normalized_username.c_str(), normalized_username.length());

  send_flap(TYPE_SIGNON,buff,normalized_username.length()+8);


  /*
   * toc_signon <authorizer host> <authorizer port> <User Name> <Password>
   *            <language> <version>
   */
  sprintf(buff, "toc_signon %s %d %s %s %s \"%s\"",
          m_conf.child("loginserver").property("host").c_str(),
          m_conf.child("loginserver").intProperty("port"),
          normalized_username.c_str(),
          roast_password(m_conf.child("user").property("password").c_str()),
          "english",
          "Everybuddy");

  send_flap(TYPE_DATA, buff);
  m_state=S_online;
}

vector<string> splitStr(string str)
{
  int i=0;
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

void TocProtocol::handleData(Network *net)
{
  string data;
  char *_data;
  int avail;

  net->socketData(_data,avail);

  if (!_data || !avail)
    return;
  handleData(net,_data,avail);
}

void TocProtocol::handleData(Network *net,const char *__data,int avail)
{

  unsigned short len;
  int it = 0;
  char *_data= NULL;

  if (m_buffer.size() > 0)
  {
    _data = new char[avail+m_buffer.size()];
    for (it = 0; it < m_buffer.size(); it++)
    {
      _data[it] = m_buffer[it];

    }
    memcpy(_data+avail, __data, avail);
    m_buffer.clear();
  }
  else
  {
    _data = new char[avail];
    memcpy(_data, __data, avail);
  }

  string data;

again:

  memcpy(&len,_data+4,2);
  len=ntohs(len);
  debug() <<  "len=" << len << endl;

  //  len=avail-6;
  //  _data[len+6]=0;
  data=_data+6;

  if ((data.length() < len) && (len != 4))
  {  // for incomplete packets, save partial data to the vector<char> m_buffer
    string debug_view_buffer;
    for (it = 0; it < data.length() + 6; it++)
    {
      m_buffer.push_back(_data[it]);
      debug_view_buffer+=_data[it];
    }
    debug() << (string("Partial data is: ") + debug_view_buffer ) << endl;
    return;
  }
  else
    m_buffer.clear();

  string req=data.substr(0,len);
//  debug() <<(string("---data--:") + "!"+req+"!"+ endl+"--- end data---" );

  if (m_state==S_connecting)
  {
    signup();
    return;
  }

  if (req.substr(0,11) == "FLAP SIGNON")
  {
    signup();
  }
  else if (req.substr(0,7) =="SIGN_ON")
  {
    debug() << ( "doing signon");
    //      tocInitDone();
  }
  else if (req.substr(0,6) =="CONFIG")
  {
    debug() << ( "got config");
    send_flap(TYPE_DATA, "toc_set_info \"Visit the kim website at "
              "<A HREF=\\\"http://www.abelsson.com/kit\\\">http://www.abelsson.com/kit</A>.\"");

  /*  string str="toc_add_buddy ";
    for(buddy_t::iterator i=m_buddies.begin();i!=m_buddies.end();i++)
      str+=aim_normalize(i->second.serverId()) + " ";
    send_flap(TYPE_DATA,str);*/
//    send_flap(TYPE_DATA, "toc_add_deny johnjtran");
//    send_flap(TYPE_DATA, "toc_add_permit abelssontest");

    send_flap(TYPE_DATA, "toc_init_done");
    eventStateChange(S_online);

    //tocParseConfig(data, &(serverConfig.buddyList), &(serverConfig.permitList), &(serverConfig.denyList), &(serverConfig.permitStatus));
  }
  else if (req.substr(0,4) =="NICK")
  {
    debug() <<  "got my nick\n";
  }
  else if (req.substr(0,5) =="IM_IN")
  {

    vector <string> words=splitStr(req);

    int i=0,j=0;
    while(j<3 && i<req.length())
      if (req[i++]==':') j++;

    string msg = req.substr(i,req.length());
    if (m_buddies.find(words[1])==m_buddies.end())
    {
      Contact c;
      c.setProtocol(protocol());
      c.setServerId(words[1]);
      c.setNick(words[1]);
      m_buddies[words[1]]=c;
    }
    eventRecvdMessage(m_buddies[words[1]],msg);
    //doIM(data);
  }
  else if (req.substr(0,12) =="UPDATE_BUDDY")
  {
    vector <string> words=splitStr(req);

    string user=words[1];
    /* are we online? */
    int online = (words[2] == "T") ? 1 : 0;

    if (m_buddies.count(user)==1)
    {
      if (online)
        m_buddies[user].setStatus(Contact::Online);
      else
        m_buddies[user].setStatus(Contact::Offline);
      eventStatusChange(m_buddies[user]);
    }
    //doUpdate(data);
  }
  else if (req.substr(0,5) =="ERROR")
  {

    //doError(data);
  }
  else if (req.substr(0,6) =="EVILED")
  {

    //      doEviled(data);
  }
  else if (req.substr(0,9) =="CHAT_JOIN")
  {

    // ignore until chat is added
  }
  else if (req.substr(0,7) =="CHAT_IN")
  {

    // ignore until chat is added
  }
  else if (req.substr(0,17) =="CHAT_UPDATE_BUDDY")
  {
    // ignore until chat is added
  }
  else if (req.substr(0,11) =="CHAT_INVITE")
  {
    // ignore until chat is added
  }
  else if (req.substr(0,9) =="CHAT_LEFT")
  {
    // ignore until chat is added
  }
  else if (req.substr(0,8) =="GOTO_URL")
  {
    // strip the GOTO_URL

  }
  else if (req.substr(0,5) =="PAUSE")
  {
    //setPaused();
  }
  else
  {
    debug() << "TOC: unknown command\n";
  }
  if (avail-(len+6)>6)
  {
    memmove(_data,_data+len+6,avail-(len+6));
    avail-=len+6;
    goto again;
  }
  delete [] _data;
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
