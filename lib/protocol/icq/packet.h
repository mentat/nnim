/*
    $Id: packet.h,v 1.2 2002/06/24 12:07:40 thementat Exp $
 
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
    $Log: packet.h,v $
    Revision 1.2  2002/06/24 12:07:40  thementat
    Toc fixes.

    Revision 1.1.1.1  2002/06/06 17:21:52  thementat
    Checkin of new sources BETA 2

    Revision 1.5  2001/10/28 20:21:31  estyrke
    Added code for nick discovery in icq

    Revision 1.4  2001/10/27 18:29:13  estyrke
    Fixed icq parser bug.. #475544

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __PACKET_H
#define __PACKET_H

#include "iceq.h"
#include "util.h"
#include "dstring.h"

#include <map>

class Header
{
public:
  Header();
  Header(const dstring &data);
  ~Header() {};

  //Conversion operators
  operator dstring();

  uint8 channel() const {return _channel;};
  void setChannel(uint8 c) {_channel = c;};
  uint16 seq() const {return _seq;};
  void setSeq(uint16 s) {_seq = s;};
  uint16 len() const {return _len;};
  void setLen(uint16 l) {_len = l;};
private:
  uint8 _id;
  uint8 _channel;
  uint16 _seq;
  uint16 _len;
};

class TLV
{
public:
  enum type_e { TLV_NONE=0x0000,
		TLV_UIN=0x0001,
		TLV_ICQREQ=0x0001,
		TLV_ICQRES=0x0001,
		TLV_ENCPASS=0x0002,
		TLV_CLIENTSTRING=0x0003,
		TLV_ERRORMSG=0x0004,
		TLV_ADDR=0x0005,
		TLV_COOKIE=0x0006,

		TLV_ERRORCODE=0x0008,

		TLV_COUNTRY=0x000e,
		TLV_LANGUAGE=0x000f,

		TLV_EMAIL=0x0011,     //Unused in ICQ?

		TLV_REGSTATUS=0x0013, //Unused in ICQ?
		TLV_BUILD2=0x0014,

		TLV_MAJOR2=0x0016,
		TLV_MAJOR=0x0017,
		TLV_MINOR=0x0018,
		TLV_MINOR2=0x0019,
		TLV_BUILD=0x001a };
  TLV() {_type=TLV_NONE;};
  TLV(type_e t, dstring value);
  TLV(type_e t, uint8 value);
  TLV(type_e t, uint16 value);
  TLV(type_e t, uint32 value);
  virtual ~TLV() {};

  virtual uint16 type() const {return _type;};
  virtual uint16 length() const {return (uint16)_value.length();};
  virtual const dstring &value() const {return _value;};

  virtual void setType(type_e t) {_type=t;};
  virtual void setType(uint16 t) {_type=(type_e)t;};
  virtual void setValue(const dstring &v) {_value=v;};

  virtual operator dstring();

  friend ostream &operator<< (ostream &o, const TLV &t);
//private:
  type_e _type;
  //  int _len;  //Just for testing
  dstring _value;
};

class UserInfo
{
public:
  UserInfo() {};
  virtual ~UserInfo() {};

  const string &name() const {return _name;};
  uint16 warnLevel() const {return _warnlevel;};
  const map<uint16,TLV> &const_TLVs() const {return _tlvs;};
  map<uint16,TLV> &TLVs() {return _tlvs;};

  void setName(const string &name) {_name=name;};
  void setWarnLevel(uint16 lvl) {_warnlevel=lvl;};
private:
  string _name;
  uint16 _warnlevel;
  map<uint16, TLV> _tlvs;
};

class PascalString
{
public:
  PascalString(const dstring &d=dstring())
  {
    _len = d.length();
    _data = d;
  };
  operator dstring()
  {
    return _data;
  };
  operator dstring() const
  {
    return _data;
  };
private:
  int _len;
  dstring _data;
};

class Packet
{
public:
  Packet() {
    reset();
  };
  Packet(const Packet &p)
  {
    _hdr = p._hdr;
    _data = p._data;
    reset();
  };
  Packet(const dstring &data);
  Packet(Header hdr, const dstring &data=dstring());
  Packet(uint16 seq, uint8 channel, const dstring &data=dstring());
  virtual ~Packet();

  //virtual void reset() { _cur=_data.begin();};
  virtual void reset() { _cur=0;};
  virtual void dump() const;
  virtual const string breadCrumb() const {return "Packet ";};

  //Conversion operators
  virtual operator dstring();

  //Accessors
  virtual const Header &hdr() const {return _hdr;};
  virtual const dstring &data() const {return _data;};
  virtual const bool EOP() const {return _cur == _data.length();};
  //Overloaded operators
  virtual Packet &operator<< (const dstring &data);
  virtual Packet &operator<< (uint8 data);
  virtual Packet &operator<< (uint16 data);
  virtual Packet &operator<< (uint32 data);
  virtual Packet &operator<< (const PascalString &data);
  virtual dstring getString (int len);
  virtual Packet &operator>> (uint8 &data);
  virtual Packet &operator>> (uint16 &data);
  virtual Packet &operator>> (uint32 &data);
  virtual Packet &operator>> (PascalString &data);
  virtual Packet &operator>> (TLV &t);
  virtual int getTLV(TLV &t);
  virtual Packet &operator>> (UserInfo &ui);

  //friends
  friend ostream &operator<< (ostream &o, const Packet &p);

//protected:
  Header _hdr;
  dstring _data;
  //dstring::iterator _cur;
  uint16 _cur;
};

class CPacket : public Packet
{
public:
  CPacket(uint16 seq, uint8 channel, const dstring &data=dstring())
    : Packet(seq, channel, data) {};
  virtual ~CPacket() {};

  virtual const string breadCrumb() const {
    return "  Packet ";
  };
};

class SPacket : public Packet
{
public:
  SPacket(const Packet &p) : Packet(p)
  {
    _ok = true; // It's best to assume ok.. (I think)
  };
  virtual ~SPacket() {};
  virtual bool ok() const {return _ok;};

  template<class PT>
  static bool isType(const Packet &p)
  {
    PT test(p);
    return test.ok();
  };

protected:
  bool _ok;
};


#endif
