// --*-c++*--
// $Id: securecomm.h,v 1.1 2002/06/06 17:21:53 thementat Exp $

#ifndef GM_SECURECOMM_H
#define GM_SECURECOMM_H

/**
   A very highlevel crypto interface
 */

class CryptoSession
{
public:
  CryptoSession();
  virtual ~CryptoSession();
  enum Status { None,Negotiation1,Reply1,Negoatiation2,Reply2,Connected};

  //  virtual string requestCertificate();
  /* alice sends bob her certificate and a msg with a session key */
  string authenticate();
  // string encryptMsg(const string &msg);

  //  void parseRequest(const string &request);

  void setPeer(const Certificate & peer) { this->peer = peer;};
  void setPrivkey(const string &key) { this->privKey = key; };
  void setCert(const Certificate& mycert) { this->mycert = mycert;};
// private:
  /* bob parses alices certificate and her packet */
   void parseAuthenticateReply(const string &msg);
  /* bob makes a reply to alice */
   string authenticate2();
  /* alice parses bobs reply */
   void parseAuthenticate2Reply(const string &msg);
  /* and sends another packet */
   string authenticate3();
  /* which bob parses */
  void parseAuthenticate3Reply(const string &msg);
  //private:
  enum {
    IDENTITY,
    SESSIONKEY,
    RANDOMDATA,
    RANDOMDATA2,
    SESSIONKEY2,

    CERTIFICATE,
    MSG,
    SIGNATURE,
  };

  byte random[16];
  byte random2[16];

  string privKey;
  byte *sessionKey;
  int sessionKeyLen;
  byte *sessionKey2;
  int sessionKey2Len;
  byte *finalKey;
  int finalKeyLen;
  Status status;
  Certificate peer,mycert;
};

#endif
