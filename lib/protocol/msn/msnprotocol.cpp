/*
    $Id: msnprotocol.cpp,v 1.3 2002/06/19 19:14:45 thementat Exp $

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
    $Log: msnprotocol.cpp,v $
    Revision 1.3  2002/06/19 19:14:45  thementat
    Working towards GCC 3.0.4 compile, many modifications and new automake-1.5 files.

    Revision 1.2  2002/06/06 18:43:02  thementat
    Added copyrights, fixed cryptography compile errors, lib builds in vc7

    Revision 1.1.1.1  2002/06/06 17:21:50  thementat
    Checkin of new sources BETA 2

    Revision 1.11  2001/12/12 01:47:26  mentat
    Adding gm_config.h.in and more #ifdef WIN32s to get rid of MSW template warning.

    Revision 1.10  2001/12/12 01:40:02  mentat
    Fixed compilation error with gcc3.0.2 and CryptoPP also added new #defines

    Revision 1.9  2001/12/08 21:45:27  mentat
    Added setAway and setAllAway to Protocol Manager along with modifying some protocol code.

    Revision 1.8  2001/10/28 20:41:08  abelsson
    Got rid of msn memleak

    Revision 1.7  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.6  2001/10/04 16:26:37  abelsson
    Added missing protocol status callbacks.

    Revision 1.5  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <iostream>
#include <sstream> // gcc sucks
#include <string>
#include <list>


#include <crypto/pch.h>
#include <crypto/md5.h>
#include <crypto/hex.h>
#include <crypto/filters.h>
#include <stdio.h>

using namespace CryptoPP;



#include "manager.h"
#include "msnprotocol.h"
#include "log.h"
#include "xml.h"

  string states[] ={
  "NLN", /* online */
  "FLN", /* offline */
  "HDN", /* hidden */
  "BSY", /* busy */
  "IDL", /* idle */
  "BRB", /* be right back */
  "AWY", /* away */
  "PHN", /* on the phone */
  "LUN" /* out to lunch */
  };

string m_mimeheader=
"MIME-Version: 1.0\r\n"
"Content-Type: text/plain\r\n"
"X-MMS-IM-Format: FN=MS%20Sans%20Serif; EF=; CO=0; CS=0; PF=0\r\n\r\n";

vector<string> MsnProtocol::splitStr(string str,char delim)
{
  unsigned int i=0;
  vector<string> words;
  string tmp;
  while (i<str.length())
  {
    while(str[i]!=delim && i<str.length())
      tmp+=str[i++];
    i++;
//    debug() << "word:" << tmp << endl;
    words.push_back(tmp);
    tmp="";
  }
  return words;

}

string MsnProtocol::deSpaceify(string s)
{
  string buf;
  unsigned int buflen=0;
  for (unsigned int i=0;i<s.length();i++)
  {
    char c=s[i];
    if (c != ' ' && c!='\t' && c!='\n' && c!='\r')
    {
      buflen++;
      buf+=c;
    }
  }
  buf=buf.substr(0,buflen);
  return buf;
}

void MsnProtocol::sendCmd(Network *net,string cmd, string args)
{
  string s;
  s=cmd + " " + trId() + " " + args + "\r\n";
  debug() << "sending: " <<s;
  net->sendData(s);
}

void MsnProtocol::sendCmd(Network *net,string cmd, vector<string> args)
{
}

MsnProtocol::MsnProtocol(const XMLNode &c,ProtocolManager *manager):
  Protocol(c,manager, "msn")
{
  debug() << "new msnProtocol instance" << endl;
  m_net = addNet();
}

MsnProtocol::~MsnProtocol()
{
  //removeNet(m_net);
  debug() << "Deleting msnProtocol instance" << endl;
}

/*vector<Network *> MsnProtocol::getNetworks()
{
  vector<Network*> tmp;
  for (list<Network*>::iterator i=nets.begin();i!=nets.end();i++)
  {
    tmp.push_back(*i);
    debug() << "A net!\n";
  }
  return tmp;
}*/

void MsnProtocol::login()
{
  debug() << "Logging in to msn";

  if (!m_conf.hasChild("server"))
	  return;
  if (m_conf.child("server").property("host")=="" ||
	  m_conf.child("server").property("port")=="")
	  return;

  m_state=Protocol::S_connecting;
  m_msnstate=S_connecting;
  m_net->connectTo(m_conf.child("server").property("host"),
                   m_conf.child("server").intProperty("port"));
  eventStateChange(m_state);
}

void MsnProtocol::setProtocol()
{
  sendCmd(m_net,"VER","MSNP2");
  m_msnstate=S_settingProtocol;
}

void MsnProtocol::getPolicy()
{
  sendCmd(m_net,"INF");
  m_msnstate=S_gettingPolicy;
}

void MsnProtocol::authenticateUser()
{
  string arg;

  arg = "MD5 I "+m_conf.child("user").property("username");
  sendCmd(m_net,"USR",arg);
  m_msnstate=S_authenticatingUser1;
}

void MsnProtocol::sendMD5Auth(string challenge)
{
  string in,out,cmd;
  in=deSpaceify(challenge)+m_conf.child("user").property("password");
  debug() << "'" << in << "'\n";


  MD5 md5;

  StringSource(in,true,new HashFilter(md5,new HexEncoder(new StringSink(out))));


  for (unsigned int i=0;i<out.length();i++)
    out[i]=tolower(out[i]);
  cmd="MD5 S "+out;
  sendCmd(m_net,"USR",cmd);
}

void MsnProtocol::requestSwitchboard()
{
}

void MsnProtocol::parseMessage(const string &msg)
{

}

void MsnProtocol::connectedToServer(Network *net)
{
  debug() << m_msnstate <<"  "<< chatSessions.size() << endl;

  for (unsigned int i=0;i<chatSessions.size();i++)
  {
    debug() << "is " << (int)net <<"="<<(int)chatSessions[i].net << " - " << chatSessions[i].status << endl;
    if (net==chatSessions[i].net)
    {
      switch (chatSessions[i].status)
      {
        case Conversation::negotiating2:
        {
          string tmp;
          tmp=m_conf.child("user").property("username")+" "+deSpaceify(chatSessions[i].cookie);
          sendCmd(chatSessions[i].net,"USR",tmp);
        }
      break;
        case Conversation::answering:
        {
          string tmp;
          tmp=m_conf.child("user").property("username")+" "+deSpaceify(chatSessions[i].cookie)+" "+deSpaceify(chatSessions[i].sessionid);
          sendCmd(chatSessions[i].net,"ANS",tmp);
          chatSessions[i].status=Conversation::online;
        }
      break;
      }
    }
  }
  switch (m_msnstate)
  {
  case S_connecting:
    setProtocol();
    break;
  case S_xfer1:
    authenticateUser();
    break;
  }
}

void MsnProtocol::logout()
{

  eventStateChange(S_offline);
}

void MsnProtocol::update()
{

}


void MsnProtocol::sendMessage(const Contact &recipient, const string &message)
{
  // look for existing chat session
  for (unsigned int i=0;i<chatSessions.size();i++)
  {
    if (chatSessions[i].status==Conversation::negotiating1)
      chatSessions[i].pendingMsgs.push_back(message);
    if (chatSessions[i].user==recipient && chatSessions[i].status==Conversation::online)
    {
      // send msg here
      chatSessions[i].pendingMsgs.push_back(message);
      sendPendingMsgs(chatSessions[i].net);
      return;
    }
  }
  // request switchboard session.
  sendCmd(m_net,"XFR","SB");
  Conversation tmp;
  tmp.user=recipient;
  tmp.pendingMsgs.push_back(message);
  tmp.status=Conversation::negotiating1;
  chatSessions.push_back(tmp);
}

void MsnProtocol::addBuddy(const Contact &c)
{
  m_buddies[c.serverId()]=c;
  m_buddies[c.serverId()].setStatus(Contact::Offline);
  eventStatusChange(c);
}

void MsnProtocol::delBuddy(const Contact &c)
{
}

void MsnProtocol::newUser()
{
}

void MsnProtocol::getPubkey()
{

}

void MsnProtocol::handleStatus(Contact c,string status)
{
  if (status == "NLN")
  {
    c.setStatus(Contact::Online);
  }
  else if (status == "FLN")
  {
    c.setStatus(Contact::Offline);
  }
  else if (status == "HDN")
  {
    c.setStatus(Contact::Offline);
  }
  else if (status == "BSY")
  {
    c.setStatus(Contact::Na);
  }
  else if (status == "IDL")
  {
    c.setStatus(Contact::Ffc);
  }
  else if (status == "BRB")
  {
    c.setStatus(Contact::Away);
  }
  else if (status == "AWY")
  {
    c.setStatus(Contact::Away);
  }
  else if (status == "PHN")
  {
    c.setStatus(Contact::Occupied);
  }
  else if (status == "LUN")
  {
    c.setStatus(Contact::Away);
  }

  eventStatusChange(c);
}
void MsnProtocol::handleData(Network *net)
{
  string data;
 
  m_net->socketData(data);

  if (data.empty())
    return;
  handleData(net,data);
}

void MsnProtocol::handleNS(Network *net,vector<string> &words)
{
  if (words[0]=="VER")
  {
    getPolicy();
  }
  else if (words[0]=="INF")
  {
    authenticateUser();
  }
  else if (words[0]=="XFR")
  {
    debug() << "Got XFR request: type: " << words[2] << "host: " << words[3] << endl;
    if (words[2]=="NS")
    {
      vector<string> host = splitStr(words[3],':');
      notifyServer=addNet();
      m_msnstate=S_xfer1;

      m_net->disconnect();
      removeNet(m_net);

      notifyServer->connectTo(host[0],atoi(host[1].c_str()));
      m_net=notifyServer;
    }
    if (words[2]=="SB")
    {
      vector<string> host = splitStr(words[3],':');
      Network *chat = addNet();
      for (unsigned int i=0;i<chatSessions.size();i++)
      {
        if (chatSessions[i].status==Conversation::negotiating1)
        {
          chatSessions[i].net=chat;
          chatSessions[i].status=Conversation::negotiating2;
          chatSessions[i].cookie=words[5];
          break;
        }
      }
      chat->connectTo(host[0],atoi(host[1].c_str()));
    }
  }
  else if (words[0]=="USR")
  {
    if (words[2]=="MD5")
      sendMD5Auth(words[4]);
    else if (words[2]=="OK")
    {
      debug() << "woho! we're online!\n";
      eventLoggedIn();
      eventStateChange(S_online);
      sendCmd(m_net,"SYN","1");
      sendCmd(m_net,"CHG","NLN");
    }

  }
  else if (words[0]=="CHL")
  {
    // do some kind of magic. (some kind of msn anti-official-clients i suppose). The magic parts stolen from gaim.
    string out;
    string cmd;
    string in;

    in = words[2]+"Q1P7W2E4J9R8U3S5";

    MD5 md5;
    StringSource(in,true,new HashFilter(md5,new HexEncoder(new StringSink(out))));

    for (unsigned int i=0;i<out.length();i++)
      out[i]=tolower(out[i]);


    cmd= string("QRY ") + trId()+ " msmsgs@msnmsgr.com 32\r\n"+out;
    debug() << cmd;
    m_net->sendData(cmd);

  }
  else if (words[0]=="ILN") // Initial login notification
  {
    if (m_buddies.find(words[3]) == m_buddies.end())
    {
      Contact c;
      c.setServerId(words[3]);
      c.setNick(words[4]);
      c.setStatus(Contact::Online);
      m_buddies[words[3]]=c;
    }
    m_buddies[words[3]].setStatus(Contact::Online);

    eventStatusChange(m_buddies[words[3]]);
  }
  else if (words[0]=="NLN") // Status change
  {
    if (m_buddies.find(words[2]) == m_buddies.end())
    {
      Contact c;
      c.setServerId(words[2]);
      c.setNick(words[3]);
      c.setStatus(Contact::Online);
      m_buddies[words[2]]=c;
    }
    handleStatus(m_buddies[words[2]],words[1]);
  }
  else if (words[0]=="RNG") // Someone wanting to chat with us.
  {
    vector<string> host = splitStr(words[2],':');
    m_msnstate=S_establishingChat;
    Network *chat = addNet();


    Conversation tmp;
    tmp.status=Conversation::answering;
    tmp.cookie=words[4];
    tmp.net=chat;
    tmp.user.setProtocol(protocol());
    tmp.user.setServerId(words[5]);
    tmp.user.setNick(words[6]);
    tmp.sessionid=words[1];
    chatSessions.push_back(tmp);
    chat->connectTo(host[0],atoi(host[1].c_str()));

  }
  else if (words[0]=="")
  {
  }
  else if (words[0]=="")
  {
  }
}

void MsnProtocol::handleSW(Network *net,vector<string> &words)
{
  if (words[0]=="USR")
  {
    if (words[2]=="OK")
    {
      debug() << "Chat connection established!";
      for (unsigned int i=0;i<chatSessions.size();i++)
        if (net==chatSessions[i].net && chatSessions[i].status==Conversation::negotiating2)
        {
          chatSessions[i].status=Conversation::online;
          sendCmd(net,"CAL",chatSessions[i].user.serverId());
          break;
        }
    }
  }
  else if (words[0]=="JOI")
  {
    debug() << words[1] << "has joined our chat!!\n";
    sendPendingMsgs(net);
  }
  else if (words[0]=="IRO")
  {

  }
  else if (words[0]=="ANS")
  {
    if (words[2]=="OK")
    {
    }
  }
  else if (words[0]=="")
  {
  }
}

void MsnProtocol::handleMsg(string sender,string nick,string data,int pos)
{
  string msg=data.substr(pos);

  debug() << "------ msg -----\n"<< msg <<"------ end msg -----\n";
  int idx=msg.find("\r\n\r\n");
  if (idx==string::npos)
    return;

  if (m_buddies.find(sender) == m_buddies.end())
  {
    Contact c;
    c.setServerId(sender);
    c.setNick(nick);
    c.setStatus(Contact::Online);
    m_buddies[sender]=c;
  }
  msg=msg.substr(idx+4);
  eventRecvdMessage(m_buddies[sender],msg);
}

/////////////////// network stuff ////////////////////////////
void MsnProtocol::handleData(Network *net,const string& data)
{
  vector<string> cmds;
  if (data.empty())
    return;

  int last=0;
  for (unsigned int i=0;i<data.length()-1;i++)
    if (data[i]=='\r' && data[i+1]=='\n')
    {
      string cmd=data.substr(last,i-last);
      //    debug() << "data:" << cmd << endl;
      vector<string> words;
      words=splitStr(cmd);
//      debug() << "cmd:" << words[0]<<":"<< endl;

      if (words[0]=="MSG")
        handleMsg(words[1],words[2],data,last);
      else if (net==m_net)
        handleNS(net,words);
      else
        handleSW(net,words);

      last=i;
    }


}

void MsnProtocol::connectionError(Network *net,int error)
{
  m_state = S_offline;

  switch (error)
  {
  case 0:
    debug() << "Connection to server was refused.\n";
    break;
  case 1:
    debug() << "Server not found\n";
    break;
  case 2:
    debug() << "Error reading from socket";
    break;
  }
  eventStateChange(S_offline);

}



void MsnProtocol::sendPendingMsgs(Network *net)
{

  Conversation &send=findConv(net);

  for (unsigned int i=0;i<send.pendingMsgs.size();i++)
  {
    string msg= m_mimeheader + send.pendingMsgs[i];
    stringstream hej;
    hej << "MSG 123 N " << msg.length() << "\r\n" << msg << '\0';
    //hej.freeze();
    debug() << "sending: " << hej.str() << endl;
    net->sendData(hej.str());
  }
  send.pendingMsgs.erase(send.pendingMsgs.begin(),send.pendingMsgs.end());
}

MsnProtocol::Conversation &MsnProtocol::findConv(Network *net)
{
  static Conversation empty;
  for (unsigned int i=0;i<chatSessions.size();i++)
    if (chatSessions[i].net==net)
      return chatSessions[i];

  return empty;
}

#ifdef TEST

int main()
{
  XMLNode n;
  MsnProtocol proto(n);
  debug() << "hej" << endl;
}
#endif
