// -*- C++ -*-
/*
    $Id: gmstorage.h,v 1.2 2002/06/23 18:35:51 thementat Exp $

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
#ifndef GM_STORAGE_H
#define GM_STORAGE_H

#include "crypto/misc.h"
#include "dstring.h"

using namespace std;
using namespace CryptoPP;

class Storage
{
public:
  Storage();
  ~Storage();
  typedef pair<int,pair<byte *,int> > sectionType; // (type, (data, len))
  typedef pair<byte *,int> dataType;               // (data,len)

  static sectionType empty;
  static word32 char2int(unsigned char *buf);
  static void int2char(unsigned char *buf,word32 num);

  int addSection(byte t,int len,const byte *data);
  int addSection(byte t,unsigned int data);
  int addSection(byte t,byte data);
  int addSection(byte t,const string &data);

  sectionType readSection(unsigned int n); // n = the nth section in the file
  vector<sectionType> readSections();
  const dstring getData() const { return m_data;};
  void setData(dstring data) { m_data = data;};
  void setData(int len, byte *data);


private:
  dstring m_data;
};

#endif
