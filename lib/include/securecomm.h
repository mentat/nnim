// -*- C++ -*-
/*
    $Id: securecomm.h,v 1.2 2002/06/23 18:35:51 thementat Exp $

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
