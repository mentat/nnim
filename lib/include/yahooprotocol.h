// -*- C++ -*-
#ifndef KIM_YAHOO_PROTOCOL_H
#define KIM_YAHOO_PROTOCOL_H

#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#define YAHOO_PROTO_ID 5
#define YAHOO_PACKET_HEADER_SIZE 104
#define YAHOO_PROTOCOL_HEADER "YPNS2.0"

#define YAHOO_SERVICE_LOGON		1
#define YAHOO_SERVICE_LOGOFF		2
#define YAHOO_SERVICE_ISAWAY		3
#define YAHOO_SERVICE_ISBACK		4
#define YAHOO_SERVICE_IDLE		5
#define YAHOO_SERVICE_MESSAGE		6
#define YAHOO_SERVICE_IDACT		7
#define YAHOO_SERVICE_IDDEACT		8
#define YAHOO_SERVICE_MAILSTAT	9
#define YAHOO_SERVICE_USERSTAT	10
#define YAHOO_SERVICE_NEWMAIL		11
#define YAHOO_SERVICE_CHATINVITE	12
#define YAHOO_SERVICE_CALENDAR	13
#define YAHOO_SERVICE_NEWPERSONALMAIL		14
#define YAHOO_SERVICE_NEWCONTACT	15
#define YAHOO_SERVICE_ADDIDENT	16
#define YAHOO_SERVICE_ADDIGNORE	17
#define YAHOO_SERVICE_PING		18
#define YAHOO_SERVICE_GROUPRENAME	19
#define YAHOO_SERVICE_SYSMESSAGE	20
#define YAHOO_SERVICE_PASSTHROUGH2	22
#define YAHOO_SERVICE_CONFINVITE 24
#define YAHOO_SERVICE_CONFLOGON	25
#define YAHOO_SERVICE_CONFDECLINE 26
#define YAHOO_SERVICE_CONFLOGOFF		27
#define YAHOO_SERVICE_CONFADDINVITE 28
#define YAHOO_SERVICE_CONFMSG 29
#define YAHOO_SERVICE_CHATLOGON	30
#define YAHOO_SERVICE_CHATLOGOFF	31
#define YAHOO_SERVICE_CHATMSG 32
#define YAHOO_SERVICE_GAMELOGON 40
#define YAHOO_SERVICE_GAMELOGOFF 41
#define YAHOO_SERVICE_FILETRANSFER 70

#include <vector>
#include <string>

#include "protocol.h"
#include "manager.h"
#include "basenetwork.h"
#include "expat.h"
class XMLNode;
class YahooProtocol;

class HttpClient
{
public:
    typedef map<string,string> httpArgMap;
    HttpClient();
    void setOwner(YahooProtocol *parent);
    ~HttpClient();
    void connect(const string &host, int port);
    void get(const string &url, const httpArgMap &args);
    void setCookie(const string &c);
    void clearCookie();
    static vector<string> lineSplit(const string &s);
    static string urlEncode(const string &s);
    Network *net() {return m_net;};
private:
    YahooProtocol *m_parent;
    Network *m_net;
    int m_cid;
    string m_host;
    string m_cookie;
};

/**
   A protocol template

   The configuration XML looks like this:
   \begin{verbatim}
    ...
   \end{verbatim}

 */

class YahooPacket
{
public:
    char version[8];			/* 7 chars and trailing null */
    unsigned char len[4];		/* length - little endian */
    unsigned char service[4];	/* service - little endian */

/* 3 X 4bytes - host, port, ip_address */
/* not sure what diff is between host and ip addr */
    unsigned char connection_id[4];	/* connection number - little endian */
    unsigned char magic_id[4];	/* magic number used for http session */
    unsigned char unknown1[4];

    unsigned char msgtype[4];
    char nick1[36];
    char nick2[36];
    char content[1];			/* was zero, had problems with aix xlc */
};

class YahooProtocol: public Protocol

{

 public:

  YahooProtocol(XMLNode &,ProtocolManager *manager);
  virtual ~YahooProtocol();

  const string screenName() { return ""; };

  virtual void update();

  virtual void login();
  virtual void logout();
  virtual void sendMessage(const Contact &recipient, const string &message);
  virtual void addBuddy(const Contact &c);
  virtual void delBuddy(const Contact &c);
  virtual void newUser();
  virtual void getPubkey();

  virtual void handleData(Network *n);
  virtual void handleData(Network *n, const string& buf);
  virtual void connectionError(Network *n, int error);
  virtual void connectedToServer(Network *n);
protected:
    friend class HttpClient;
    Network *addNet() {return Protocol::addNet();};
    void removeNet(Network *n) {Protocol::removeNet(n);};
    void sendCmd(int cmd, const string &data, int msgtype);
    void storeInt(unsigned char *dest, unsigned int src);
private:
  enum YahooState { S_none=YAHOO_PROTO_ID*65536, S_getCookie, S_connect,
                    S_getConfig, S_logon };
    Network *m_net;
    YahooState m_istate;
    string m_login_cookie;
    unsigned int m_magicId;
    HttpClient httpClient;

};

#endif
