// -*- C++ -*-
/*
    $Id: msnprotocol.h,v 1.2 2002/06/23 18:35:51 thementat Exp $

    GNU Messenger - The secure instant messenger

	Copyright (C) 1999-2002  Henrik Abelsson <henrik@abelsson.com>

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
#ifndef KIM_MSN_PROTOCOL_H
#define KIM_MSN_PROTOCOL_H

#ifdef _WIN32
#pragma warning(disable:4786)
#endif


#include <vector>
#include <string>

#include "protocol.h"
#include "basenetwork.h"
#include "expat.h"
class XMLNode;

/**
   The msn protocol (not functional)

   The configuration XML looks like this:
   \begin{verbatim}
    ...
   \end{verbatim}

 */

class MsnProtocol: public Protocol

{

 public:

  MsnProtocol(const XMLNode &,ProtocolManager *manager);
  virtual ~MsnProtocol();

  const string screenName() { return ""; };

  virtual void update();

  virtual void login();
  virtual void logout();
  virtual void sendMessage(const Contact &recipient, const string &message);
  virtual void addBuddy(const Contact &c);
  virtual void delBuddy(const Contact &c);
  virtual void newUser();
  virtual void getPubkey();

  virtual void handleData(Network *net);
  virtual void handleData(Network *net,const string& data);
  virtual void connectionError(Network *net,int error);
  virtual void connectedToServer(Network *net);

private:
	
	const string trId() {return "123";};
	void setProtocol();
	void sendMD5Auth(string challenge);
	void authenticateUser();
	void getPolicy();
	void handleMsg(string sender,string nick,string msg,int pos);
	void handleStatus(Contact c,string status );
	void parseMessage(const string &msg);
	string deSpaceify(string s);
	void requestSwitchboard();
	void sendCmd(Network *net,string cmd, string args="");
	void sendCmd(Network *net,string cmd, vector<string> args);
	vector<string> splitStr(string str,char delim=' ');
	void handleNS(Network *net,vector<string> &cmd);
	void handleSW(Network *net,vector<string> &cmd);
	void sendPendingMsgs(Network *net);
	
	class message 
	{
	public:
		// timestamp
		int year;int month;int day;int hour;int minute;int sec;
		// msg
		string msg;
		string sender;
		Network *fd;
	};


	enum MsnState { S_none=3*65536,
					S_connecting,
					S_xfer1,
					S_settingProtocol,
					S_gettingPolicy,
					S_establishingChat,
					S_authenticatingUser1,
					S_authenticatingUser2,
					S_synchronizing
	};

	class Conversation
	{
	public:
		enum status {online,negotiating1,negotiating2,answering,offline};
		vector<string> pendingMsgs;
		Contact user;
		string cookie;
		string sessionid;
		Network *net;
		int status;
	};

	MsnProtocol::Conversation &findConv(Network *net);

	vector<string> chatUsers;
	vector<string> allowList;
	vector<string> blockList;
	vector<string> reverseList;
	MsnState m_msnstate;
    Network *m_net;
	vector<Conversation> chatSessions;
	Network *notifyServer;

};

#endif
