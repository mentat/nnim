// -*- C++ -*-
#ifndef __TOCPROTOCOL_H
#define __TOCPROTOCOL_H

#include "protocol.h"

class Network;
class XMLNode;

/**
   AIM/Toc protocol.

   Implements the protocol needed to connect to AOL's aim service.

   The configuration XML looks like this:
   \begin{verbatim}
    <toc>
    <user password="passwd" username="username"/>
    <server host="toc.oscar.aol.com" port="23"/>
    <loginserver host="login.oscar.aol.com" port="23"/>
    </toc>
   \end{verbatim}
 */

class TocProtocol : public Protocol
{
public:

	TocProtocol(const XMLNode &, ProtocolManager *manager);
	virtual ~TocProtocol();

	const string screenName() { return m_screenName; };

	virtual void update();

	virtual void login();
	virtual void logout();
	virtual void sendMessage(const Contact &recipient, const string &message);
	virtual void addBuddy(const Contact &c);
	virtual void delBuddy(const Contact &c);
	virtual void newUser();
	virtual void getPubkey();

	virtual void handleData(Network *net);
	virtual void handleData(Network *net, const string& data);
	virtual void connectionError(Network *net,int error);
	virtual void connectedToServer(Network *net);

	virtual void setAway(const string& msg = "");
	virtual void setInfo(const string& info = "");

private:

	void send_flap(int type, const string& data);
	string return_flap(int type, const string& data);
	void toc_send_keep_alive();
	void signup();
	string aim_encode(const string& s);
	string aim_normalize(const string& s);
	string roast_password(const string& pass);
	void sendMessageAuto(const Contact &c, const string &message);
	void tocParseConfig(const string& config);

private:

	enum { TYPE_SIGNON=1,TYPE_DATA ,TYPE_ERROR, TYPE_SIGNOFF,TYPE_KEEPALIVE};

	bool m_paused;

	bool m_bufferEmpty;
	//vector<char> m_buffer;

	string m_screenName;
	string m_buffer;
	string m_awayMessage;

	int m_realLength;

	map <string, string> m_buddiesFromServer;

	bool m_away;
  
protected:

	void handleRealData(Network *net, const string& data);
    Network *m_net;

};

#endif
