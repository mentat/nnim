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
  myvector(): vector<T>() ,m_data(0),m_datalen(0) {};
  myvector(const string &e): vector<T>() ,m_data(0),m_datalen(0)
  {
    for (unsigned int i=0;i<e.length();i++)
      append(e[i]);
  }

  myvector(const T* e,int n):vector<T>() , m_data(0),m_datalen(0)
  {
    for (int i=0;i<n;i++)
      append(e[i]);
  }

  ~myvector()
  {
    myvector<T>::iterator it;
//    for (int i=0;i<m_datalen;i++)
//      m_data[i]=0;
//    delete[] m_data;

    for (it=begin();it!=end();it++)
      (*it)=0;
  }


  const T *data() const
  {
    int i=0;
    const_iterator it;

//    for (int i=0;i<m_datalen;i++)
//      m_data[i]=0;
//    delete[] m_data;

    m_datalen = size();
    m_data = new T[m_datalen];
    for (it=this->begin();it!=this->end();it++)
      m_data[i++]=*it;

    return m_data;
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

    if (n=-1 ||n > size())
      n = size();

    b=begin();
    for (i=0;i<pos;i++)
      b++;

    e=b;
    for (i=pos;i<n;i++)
      e++;

    myvector tmp;
    tmp.insert(tmp.begin(), b, e);
    return tmp;
  };
  operator string() const
  {
    return string((char*)data(),sizeof(T)*size());
  }
private:
  mutable T* m_data;
  mutable int m_datalen;
};

/// defines an 8-bit clean string for use in data storage
typedef myvector<uint8> dstring;

#endif
