/*
    $Id: kitprotocol.cpp,v 1.2 2002/06/06 18:43:02 thementat Exp $

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
    $Log: kitprotocol.cpp,v $
    Revision 1.2  2002/06/06 18:43:02  thementat
    Added copyrights, fixed cryptography compile errors, lib builds in vc7

    Revision 1.1.1.1  2002/06/06 17:21:48  thementat
    Checkin of new sources BETA 2

    Revision 1.17  2001/12/12 01:40:02  mentat
    Fixed compilation error with gcc3.0.2 and CryptoPP also added new #defines

    Revision 1.16  2001/12/08 21:45:27  mentat
    Added setAway and setAllAway to Protocol Manager along with modifying some protocol code.

    Revision 1.15  2001/11/19 00:24:10  henrik
    Simplified kit protocol: Encrypted messages are now sent as xml, same as
    everything else.

    Revision 1.14  2001/11/18 22:39:04  henrik
    Misc updates.

    Revision 1.13  2001/10/28 20:41:08  abelsson
    Got rid of msn memleak

    Revision 1.12  2001/10/21 10:24:59  abelsson
    Fixed some xml bugs, contact handling bugs.

    Revision 1.11  2001/10/05 21:49:14  abelsson
    Improved contact handling.

    Revision 1.10  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.9  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)
#include <iostream>
#include <string>

#include <gm_config.h>
#include <stdio.h>

#ifdef HAVE_CRYPTO

#include <crypto/pch.h>
#include <crypto/default.h>
#include <crypto/base64.h>
#include <crypto/hex.h>
#include <crypto/rsa.h>
#include <crypto/osrng.h>
#include <crypto/randpool.h>
#include <crypto/gzip.h>
#include <crypto/files.h>

using namespace CryptoPP;

#endif

#include "kitprotocol.h"
#include "log.h"
#include "xml.h"

#ifndef WIN32
#include <unistd.h>  // remove
#endif


KitProtocol::KitProtocol(const XMLNode &c, ProtocolManager *manager):
  Protocol(c,manager, "kit"),Expat()
{
  debug() << "New kitProtocol instance";
  m_connectionEncrypted=0;
  m_state=S_offline;
  m_sessionKeyLen=0;
  m_net = addNet();
}

KitProtocol::~KitProtocol()
{
  m_connectionEncrypted=0;
  m_state=S_offline;
  m_kitstate=S_none;
  m_sessionKeyLen=0;

  m_net->disconnect();

  if (m_sessionKeyLen>0)
    {
      memset(m_sessionKey,0,m_sessionKeyLen);
      delete[] m_sessionKey;
      m_sessionKeyLen=0;
    }

  //removeNet(m_net);
  debug() << "Deleting kitProtocol instance" << endl;
}

void KitProtocol::sendData(const string &data)
{
  cout << "Sending **********\n"<< data << "**************\n";
#ifdef HAVE_CRYPTO
  if (m_connectionEncrypted)
    {
      string out;
      StringSource(data,true,new Gzip(new DefaultEncryptorWithMAC(m_sessionKey,m_sessionKeyLen,new Base64Encoder(new StringSink(out),false))));
      /*      
	      int len = encryptor.MaxRetrievable();
	      byte *b = new byte[len];
	      encryptor.Get(b,len);
      */
      XMLNode n;
      n.setName("encr");
      n.setData(out);
      //m_net->sendData((char*)b,len);
      m_net->sendData(n);
    }
  else
#endif
    m_net->sendData(data.c_str(),data.length());

}

void KitProtocol::login()
{
  reset();
  debug() << "Logging in: ";
  debug() << m_conf.child("server").property("host") << ":" << m_conf.child("server").intProperty("port") << endl;
//  if (!m_conf.hasChild("server"))
//    return;
  debug() << "1 \n";
  if (m_state==S_offline)
  {
    debug() << "1.5 ";
    // connect
    m_kitstate=S_none;
    m_connectionEncrypted=0;
    m_net->connectTo(m_conf.child("server").property("host"),
                     m_conf.child("server").intProperty("port"));
  }
  else
  {
    m_connectionEncrypted=0;
    m_state=S_offline;
    m_kitstate=S_none;
    m_sessionKeyLen=0;

    m_net->disconnect();

    if (m_sessionKeyLen>0)
    {

      memset(m_sessionKey,0,m_sessionKeyLen);
      delete[] m_sessionKey;
      m_sessionKeyLen=0;

    }
    login();
  }
  m_state=S_connecting;
  eventStateChange(m_state);
}

void KitProtocol::connectedToServer(Network *net)
{
  // set encryption options and generate session key.
  debug() << "connection established - sending root\n";
  string s;

  m_state=S_connecting;
}



void KitProtocol::logout()
{
  reset();
  m_net->disconnect();
  m_state=S_offline;
  m_kitstate=S_none;
  m_connectionEncrypted=0;
  if (m_sessionKeyLen>0)
    {
      memset(m_sessionKey,0,m_sessionKeyLen);
      delete[] m_sessionKey;
      m_sessionKeyLen=0;
    }
  for (buddy_t::iterator it=m_buddies.begin();it!=m_buddies.end();it++)
    {
      it->second.setStatus(Contact::Offline);
      eventStatusChange(it->second);      
    }

  eventStateChange(S_offline);
}

void KitProtocol::update()
{
}

// send a update contact request
void KitProtocol::updateContacts()
{
#if 0
  string s;
  XMLNode r;

  if (m_state != S_online)
    return;

  r.setName("isonline");
  // encode all contacts in config.

  for (buddy_t::iterator i=m_buddies.begin();i!=m_buddies.end();i++)
  {
      XMLNode &user= r.addChild("contact");
      user.setProperty("user",i->second.serverId());

  }
  sendData(r);
#endif
}

void KitProtocol::sendMessage(const Contact &recipient, const string &message)
{
  XMLNode msg;

  msg.setName("postmsg");
  msg.setData(message);

  msg.setProperty("recipient",recipient.serverId());

  sendData(msg);
}

void KitProtocol::addBuddy(const Contact &c)
{
  m_buddies[c.serverId()]=c;

  m_buddies[c.serverId()].setStatus(Contact::Offline);
  eventStatusChange(m_buddies[c.serverId()]);

  if (m_state == S_online)
  {
    XMLNode n;
    n.setName("subscribe");
    n.setProperty("user",c.serverId());
    sendData(n);
    eventStatusChange(c);
  }
}
void KitProtocol::delBuddy(const Contact &c)
{
  // TODO

}
void KitProtocol::newUser()
{
  if (m_state==S_offline)
  {
    reset();
    m_kitstate=S_registering;
    m_connectionEncrypted=0;
    m_net->connectTo(m_conf.child("server").property("host"),
                     m_conf.child("server").intProperty("port"));

  }
  else
  {
    XMLNode m;
    m.setName("register");
    m.setProperty("user",m_conf.child("user").property("username"));
    m.setProperty("password",m_conf.child("user").property("password"));
    m.setProperty("pubkey",m_conf.child("user").property("mypubkey"));
    sendData(m);
  }
}
void KitProtocol::getPubkey()
{
  if (m_state==S_offline)
    {
      reset();
      // connect
      m_kitstate=S_getPubKey;
      m_connectionEncrypted=0;
      m_net->connectTo(m_conf.child("server").property("host"),
                       m_conf.child("server").intProperty("port"));
    }
  else
    {
      string data = "<pubkey />";
      sendData(data);
    }

}

void KitProtocol::customRequest(XMLNode n)
{
  if (n.name() == "store" && n.property("type")=="contact")
  {
    XMLNode r;
    r.setName("addcontact");
    r["nick"]=n["nick"];
    r["protocol"]=n["protocol"];
    r["user"]=n["user"];
    sendData(r);
  }
  if (n.name() == "config")
  {
    string out;
    XMLNode r;
    r.setName("setvar");
    r["name"]="config";
#ifdef HAVE_CRYPTO
    string str;
    str=n;
    StringSource(str,true,new Gzip(new Base64Encoder(new StringSink(out),false)));
#else
#error Broken
#endif
    r["value"]=out;
    sendData(r);
  }

}

void KitProtocol::handleData(Network *net)
{
  string data;


  m_net->socketData(data);

  if (data.empty())
    return;
  handleData(net,data);
}

/////////////////// network stuff ////////////////////////////
void KitProtocol::handleData(Network *net,const string& buf)
{
  if (buf.empty())
    return;

  debug() << buf << endl;
  parse(buf);

  if (toParse != "")
    parse(toParse);

  toParse="";

}

void KitProtocol::rootTagRecieved(XMLNode &root)
{
  string r ="<kit version='1'>";
  if (root.name() != "kit")
    debug() << "invalid server!" << endl;

//  sendData("<kit version=\"1\">");

  if (m_kitstate==S_registering)
    {

      debug() << "sending registration command";
      XMLNode m;
      m.setName("register");
      m.setProperty("user",m_conf.child("user").property("username"));
      m.setProperty("password",m_conf.child("user").property("username"));
      m.setProperty("pubkey",m_conf.child("user").property("mypubkey"));
      sendData(r+string(m));
      m_state=S_connecting;
      m_kitstate = S_none;
      return;
    }
  if (m_kitstate==S_getPubKey)
    {
      string data = r+"<pubkey />";
      sendData(data);
      m_kitstate = S_none;
      m_state=S_connecting;
      return;
    }

  XMLNode n;

#ifdef HAVE_CRYPTO

  // if we dont have a public key
  if (m_conf.child("server").property("pubkey").length() == 0)
    {
      n.setName("signon");
      n.setProperty("user",m_conf.child("user").property("username"));
      n.setProperty("password",m_conf.child("user").property("password"));
      sendData(r+string(n));
      return;
    }
  StringSource pubFile(m_conf.child("server").property("pubkey"), true, new HexDecoder);
  debug() << m_conf.child("server").property("pubkey") << endl;
  RSAES_OAEP_SHA_Encryptor pub(pubFile);

  if (16 > pub.MaxPlainTextLength())
    {
      debug() <<"!message too long for this key!";
      abort();
    }

  AutoSeededRandomPool randPool(false,128);
  m_sessionKeyLen=16;
  m_sessionKey= new unsigned char[m_sessionKeyLen];

  randPool.GenerateBlock(m_sessionKey,m_sessionKeyLen);

  debug() <<"Sessionkey: ";
  string tmps;
  for (int i=0;i<m_sessionKeyLen;i++)
    tmps += m_sessionKey[i];

  debug() <<(tmps.c_str());

  char *outstr = new char[2*pub.CipherTextLength()+1];
  debug() << m_sessionKeyLen << endl;
  debug() << pub.CipherTextLength() << endl;
  pub.Encrypt(randPool, m_sessionKey, 16, (byte *)outstr);

  HexEncoder hexEncoder;
  hexEncoder.Put((byte *)outstr, pub.CipherTextLength());
  hexEncoder.MessageEnd();
  hexEncoder.Get((byte *)outstr, 2*pub.CipherTextLength());

  outstr[2*pub.CipherTextLength()] = 0;

  n.setName("format");
  n.setProperty("encryption","default");
  n.setProperty("sessionkey",outstr);
#else

  n.setName("signon");
  n.setProperty("user",m_conf.child("user").property("username"));
  n.setProperty("password",m_conf.child("user").property("password"));
#endif

  sendData(r+string(n));
}

void KitProtocol::recievedTag(XMLNode &n)
{
  // check for format reply

#ifdef HAVE_CRYPTO
  if (n.name()=="encr")
    {
      cout << n.data() << endl;
      string out;
      try {
      StringSource(n.data().c_str(),true,new Base64Decoder(new DefaultDecryptorWithMAC(m_sessionKey,m_sessionKeyLen,new Gunzip(new StringSink(out)))));
      }
      catch (Exception &ex)
	{
	  cout << ex.what() << endl;
	  return;
	}
      cout << out << endl;
      toParse += out;
      return;
    }
#endif

  if (n.name() == "format")
  {
    if (n.property("encryption") == "yes")
#ifdef HAVE_CRYPTO
      m_connectionEncrypted=1;
#else
      m_connectionEncrypted=0;
#endif
    else
    {
      debug() << "*** Warning *** <br> Couldnt enable encryption! Everything will be sent in plaintext!";
      m_connectionEncrypted=0;
    }
    string s;
    // now that we've set encryption, login.
    XMLNode signon;
    signon.setName("signon");
    signon.setProperty("user",m_conf.child("user").property("username"));
    signon.setProperty("password",m_conf.child("user").property("password"));
    sendData(signon);

  }
  else if (n.name() == "signon")
  {
    if (n.property("status") == "success")
    {
      debug() <<"woho!";
      m_state = S_online;

      XMLNode getcontacts;
      getcontacts.setName("getvar");
      getcontacts.setProperty("name","config");
      sendData (getcontacts);

      eventLoggedIn();
      eventStateChange(m_state);
    }
    else
    {
      m_state=S_offline;
      m_kitstate=S_none;
      debug() <<"Invalid login/password combination";
      eventError(1,"Invalid login/password combination");
      eventStateChange(m_state);
    }
  }
  else if (n.name() == "register")
  {
    XMLNode stat;
    stat.setName("register");
    if (n.property("status") == "success")
      stat.setProperty("status","success");
    else
      stat.setProperty("status","failure");
    eventGotCustom(stat);
  }
  else if (n.name() == "pubkey")
  {
    debug() << "got pubkey\n";
    eventGotPubkey(n.property("value"));
  }

  // if we're not logged in return
  if (m_state != S_online)
    return;

      // Switch between the different types of nodes we know about
      // and either handle them directly or call a function that
      // knows how to handle the data appropriatly

      // got a message
  if (n.name()=="message")
  {
    debug() <<("got a msg\n");

    if (n.property("sender") == "")
      return;
    if (m_buddies.find(n.property("sender"))==m_buddies.end())
      {
        // if we got a message from an unknown user, add him
        Contact c;
        c.setProtocol(protocol());
        c.setServerId(n.property("sender"));
        c.setNick(n.property("sender"));
        m_buddies[n.property("sender")]=c;
        eventRecvdMessage(c,n.data());
        debug() << "data;" << n.data() << endl;
      }
    else
      {
        debug() << "data;" << n.data() << endl;
        eventRecvdMessage(m_buddies[n.property("sender")],n.data());
      }

  }
  if (n.name()=="presence")
  {
      if (n.property("status") == "online")
      {
        m_buddies[n.property("user")].setStatus(Contact::Online);
      }
      else if (n.property("status") == "offline")
      {
        m_buddies[n.property("user")].setStatus(Contact::Offline);
      }
      else if (n.property("status") == "away")
      {
        m_buddies[n.property("user")].setStatus(Contact::Away);
      }
      else if (n.property("status") == "dnd")
      {
        m_buddies[n.property("user")].setStatus(Contact::Dnd);
      }
      else if (n.property("status") == "na")
      {
        m_buddies[n.property("user")].setStatus(Contact::Na);
      }
      else
      {
        m_buddies[n.property("user")].setStatus(Contact::Custom);
        m_buddies[n.property("user")].info()["customstatus"]=n.property("status");
      }
      eventStatusChange(m_buddies[n.property("user")]);
  }
  // isonline reply - we need to update the contact list
  if (n.name()=="isonline")
  {
    // add all contacts into the right list
    for (int i=1;i<n.numChildren("contact")+1;i++)
    {
      XMLNode p = n.child("contact",i);

      // ignore unknown users
      if (m_buddies.find(p.property("user"))==m_buddies.end())
        continue;
      if (p.property("status") == "online")
      {
        m_buddies[p.property("user")].setStatus(Contact::Online);
      }
      else if (p.property("status") == "offline")
      {
        m_buddies[p.property("user")].setStatus(Contact::Offline);
      }
      else if (p.property("status") == "away")
      {
        m_buddies[p.property("user")].setStatus(Contact::Away);
      }
      else if (p.property("status") == "dnd")
      {
        m_buddies[p.property("user")].setStatus(Contact::Dnd);
      }
      else if (p.property("status") == "na")
      {
        m_buddies[p.property("user")].setStatus(Contact::Na);
      }
      else
      {
        m_buddies[p.property("user")].setStatus(Contact::Custom);
        m_buddies[p.property("user")].info()["customstatus"]=p.property("status");
      }
      eventStatusChange(m_buddies[p.property("user")]);
    }
  }
  if (n.name() == "result")
  {
  }

  if (n.name() == "getvar")
  {
    debug() <<("got some contacts!");
    if (n["name"]=="config")
    {
      string in,out;
      in = n["value"];
      XMLNode conf;

#ifdef HAVE_CRYPTO
      try {
      StringSource(in,true,new Base64Decoder(new Gunzip(new StringSink(out))));
      }
      catch (Exception ex)
      {
      }
      conf << out;
//    cout << "Got config:";
//    cout << out;
//    cout << endl;
#else
#endif

      if (conf.name()!="config")
        conf.setName("config");
      //   cout << "Sending conf..";
      eventGotCustom(conf);
    }
#if 0
    for (buddy_t::iterator i=m_buddies.begin();i!=m_buddies.end();i++)
    {
      XMLNode n;
      n.setName("subscribe");
      n.setProperty("user",i->second.serverId());
      sendData(n);
      i->second.setStatus(Contact::Offline);
      eventStatusChange(i->second);
    }

    for (int i=1;i<n.numChildren("contact")+1;i++)
    {
      XMLNode contact=n.child("contact",i);

      // Add to our buddy list
      {
        Contact c;
        c.setProtocol(contact.property("protocol"));
        c.setServerId(contact.property("user"));
        // If the user doesnt have a nick, just use the login name
        if (contact.property("nick")!="")
          c.setNick(contact.property("nick"));
        eventGotBuddy(c);
      }
    }
#endif
  }
}

void KitProtocol::parserError(string msg,int line)
{
  debug() <<(msg.c_str());
  reset();
}

void KitProtocol::connectionError(Network *net,int error)
{
  m_state = S_offline;

  switch (error)
    {
    case 0:
      debug() <<("Connection to kit server was refused.\n");
      eventError(0,"Connection to server was refused.");
      break;
    case 1:
      debug() <<("kit server not found\n");
      eventError(1,"Server not found (DNS Error)");
      break;
    case 2:
      debug() <<("Error reading from socket");
      eventError(2,"Error reading from socket");
      break;
    }

  logout();

}


#ifdef TEST

int main()
{
  XMLNode n;
  KitProtocol proto(n);
  debug() << "hej" << endl;
}
#endif
