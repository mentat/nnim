// -*- C++ -*-
#ifndef __DSTRING_H
#define __DSTRING_H

#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#include "platform.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
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
  };

  myvector &operator+= (const myvector &m)
  {
    insert(end(), m.begin(), m.end());
    return *this;
  };

  myvector &append(T _t)
  {
    insert(end(), _t);
    return *this;
  };

  int length() const { return size();};

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
typedef myvector<uint8> dstring;

#endif
