/*
    $Id: stdpackets.h,v 1.1 2002/06/06 17:21:52 thementat Exp $
 
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

    -----
    $Log: stdpackets.h,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __STDPACKETS_H
#define __STDPACKETS_H

#include "packet.h"

///////////////////////////////
// Server originated packets //
///////////////////////////////

class PktSConnAck : public SPacket
{
public:
  PktSConnAck(const Packet &p);
  virtual ~PktSConnAck(); 
private:
  uint32 _ver;
};

class PktSCookieReply : public SPacket
{
public:
  PktSCookieReply(const Packet &p);
  virtual ~PktSCookieReply();
  
  const string &addr() const {return _addr;};
  uint16 port() const {return _port;};
  const dstring &cookie() const {return _cookie;};
private:
  string _addr;
  uint16 _port;
  dstring _cookie;
};

///////////////////////////////
// Client originated packets //
///////////////////////////////

class PktCCookieRequest : public CPacket
{
public:
  PktCCookieRequest(int seq, const string &uin, const string &pass);
  virtual ~PktCCookieRequest();
};

class PktCSignon : public CPacket
{
public:
  PktCSignon(uint16 seq, const dstring &cookie);
  virtual ~PktCSignon() {};
};

class PktCSignoff : public CPacket
{
public:
  PktCSignoff(uint16 seq); //Just an empty packet
  virtual ~PktCSignoff() {};
};

#endif
