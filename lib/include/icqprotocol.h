// -*- C++ -*-
#ifndef __ENGINE_H
#define __ENGINE_H

//#include "network.h"
#include "dstring.h"
#include "contact.h"
#include "protocol.h"
#include "basenetwork.h"

#define ICQ_SETSTATUS_ONLINE	0x00030000
#define ICQ_SETSTATUS_AWAY	0x00030001
#define ICQ_SETSTATUS_NA	0x00030005
#define ICQ_SETSTATUS_OCCUPIED	0x00030011
#define ICQ_SETSTATUS_DND	0x00030013
#define ICQ_SETSTATUS_INVISIBLE	0x00030100

#define ICQ_PROTO_ID		0x0001

#include <stdarg.h>
#include <stdio.h>

#include <string>
#include <list>

/**
   The ICQ protocol.

   @author Emil Styrke

*/

class Packet;
class FSPacket;

class IcqProtocol : public Protocol
{
public:
	IcqProtocol(const XMLNode &,ProtocolManager *manager);
	virtual ~IcqProtocol();

	const int protocolId() {return ICQ_PROTO_ID;}
	const string screenName() {return "";}

	virtual void login();
	virtual void logout();
  
	//  virtual void icq_setStatus(uint32 status = ICQ_SETSTATUS_ONLINE);
	virtual void sendMessage(const Contact &recipient, const string &message);
	virtual void addBuddy(const Contact &contact);
	virtual void delBuddy(const Contact &contact);
	virtual void newUser() {}

	virtual void handleData(Network *net) {}
	virtual void handleData(Network *net,const string& data);
	virtual void connectedToServer(Network *net);
	virtual void connectionError(Network *net,int error);

private:
	/*enum e_state { DISCONNECTING, CONNECTING,
    BUSY, E_ERROR, IDLE, OFFLINE };*/
  
	enum IState { AUTHORIZER, BOS };
	enum IcqErrors { E_Authorizer = ICQ_PROTO_ID * 65536 };

	int parsePacket(const Packet &p);
	int parseFnac(const Packet &p);
	int parseFnacGen(const FSPacket &p);
	int parseFnacLoc(const FSPacket &p);
	int parseFnacBud(const FSPacket &p);
	int parseFnacMsg(const FSPacket &p);
	int parseFnacBos(const FSPacket &p);
	int parseFnacIcq(const FSPacket &p);

	void packetError(const Packet *p, const char *fmt, ...);
	//uint32 _status;
	//state_e _state;
	IState _istate;
	//string _uin;
	//string _pass;
	dstring _cookie;
	int _seq;
	dstring _recvbuf;
	//ContactList _contacts;
	//list<string> _contacts;
    Network *m_net;
};

#endif
