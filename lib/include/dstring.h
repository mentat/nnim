// -*- C++ -*-
/*
    $Id: dstring.h,v 1.4 2002/06/24 12:07:40 thementat Exp $

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
#ifndef __DSTRING_H
#define __DSTRING_H

#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#include "crypto/misc.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace CryptoPP;
/// Defines a vector for storing data
template<class T>
class myvector: public vector<T>
{
public:
  myvector(): vector<T>() ,_data(0) {};

  myvector(const string &e): vector<T>() ,_data(0)
  {
    for (unsigned int i=0;i<e.length();i++)
      append(e[i]);
  }

  myvector(const T* e,int n):vector<T>() , _data(0)
  {
    for (int i=0;i<n;i++)
      append(e[i]);
  }
  const T *data() const
  {
    int i=0;
    const_iterator it;
    delete[] _data;

    _data = new T[size()];
    for (it=this->begin();it!=this->end();it++)
      _data[i++]=*it;

    return _data;
  }

  myvector operator+ (const myvector &m)
  {
    myvector tmp(*this);
    tmp.insert(tmp.end(), m.begin(), m.end());
    return tmp;
  }

  myvector &operator+= (const myvector &m)
  {
    insert(end(), m.begin(), m.end());
    return *this;
  }

  myvector &append(T _t)
  {
    insert(end(), _t);
    return *this;
  }

  unsigned long length() const { return (unsigned long)size();}

  myvector substr(int pos,int n=-1) const
  {
    const_iterator b, e;
    int i;

    if (n==-1 ||n > size()-pos)
      n = size()-pos;

    b=begin();
    for (i=0;i<pos;i++)
      b++;

    e=b;
    for (i=0;i<n;i++)
      e++;

    myvector tmp;
    tmp.insert(tmp.begin(), b, e);
    return tmp;
  };

  friend ostream &operator<< (ostream &o, const myvector &m)
  {
    o << string(m);
    return o;
  };

  operator string() const
  {
    return string((char*)data(),sizeof(T)*size());
  }
private:
  mutable T* _data;
};

/// defines an 8-bit clean string for use in data storage
typedef myvector<byte> dstring;

#endif
