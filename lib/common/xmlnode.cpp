// --*-c++-*--
// $Id: xmlnode.cpp,v 1.1 2002/06/06 17:21:54 thementat Exp $
#pragma warning(disable:4786)
#ifndef WIN32
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include <map>
#include <list>
#include <string>
#include <stack>

#include <strstream>

#ifdef _UNICODE
#undef string
#define string basic_string<wchar_t>
#endif

using namespace std;

#include "xmlnode.h"
#include "xml.h"

XMLNode::XMLNode()
{
  d=new XMLNodeData;
  d->data_is_cdata=false;
  d->oneline=false;
}

XMLNode::XMLNode(const XMLNode &other)
{
  other.d->ref();
  d=other.d;

}

XMLNode::~XMLNode()
{
  if (d->deref())
  {
    delete d;
    d=0;
  }
}

bool XMLNode::hasChild(const string &name, int n) const
{
  vector<XMLNode>::const_iterator it;

  for(it=d->m_children.begin();it!=d->m_children.end();it++)
    {
      if (it->d->m_name == name)
	n--;
      if (n == 0)
	return true;
    }

  return false;
}

int XMLNode::numChildren(const string &name) const
{
  vector<XMLNode>::const_iterator it;
  int n=0;

  for(it=d->m_children.begin();it!=d->m_children.end();it++)
    {
      if (it->d->m_name == name)
	n++;
    }

  return n;
}

string XMLNode::property(const string &name) const
{
 map<string,string>::const_iterator it=d->m_properties.find(name);
 if (it==d->m_properties.end())
   return "";
 else
   return it->second;

};

int XMLNode::intProperty(const string &name) const
{
 map<string,string>::const_iterator it=d->m_properties.find(name);
 if (it==d->m_properties.end())
   return 0;
 else
   return atoi(it->second.c_str());
};

XMLNode &XMLNode::child(const string &name, int n)
{
  vector<XMLNode>::iterator it;
  for(it=d->m_children.begin();it!=d->m_children.end();it++)
    {
      if (it->d->m_name == name)
	n--;
      if (n == 0)
	return *it;
    }
  XMLNode newNode;
  newNode.setName(name);
  return *d->m_children.insert(d->m_children.end(),newNode);
}

XMLNode &XMLNode::setProperty(const string &name, const string &value)
{
  d->m_properties[name] = value;
  return *this;
}

XMLNode &XMLNode::setProperty(const string &name, int value)
{
  strstream foo;
  string s;
  foo << value;
  foo >> s;
  return setProperty(name, s);
}

XMLNode &XMLNode::addChild(const string &name, const string &data)
{
  XMLNode tmp;
  tmp.d->m_name = name;
  tmp.d->m_data = data;

  return *d->m_children.insert(d->m_children.end(), tmp);
}

XMLNode &XMLNode::delChild(const string &name, int n)
{
  vector<XMLNode>::iterator it;

  for(it=d->m_children.begin();it!=d->m_children.end();it++)
    {
      if (it->d->m_name == name)
	n--;
      if (n == 0)
	{
	  d->m_children.erase(it);
	  return *this;
	}
    }
  // On second thought, don't throw.. Just return silently instead...
  //throw "XMLNode::delChild(): child not found";

  return *this; //To get rid of warning
}


XMLNode::operator string() const
{
  string str;

  XMLParser::printTag(*this,str);

  return str;
}


string &XMLNode::operator[](const string &name)
{
  return d->m_properties[name];
};

const string XMLNode::operator[](const string &name) const
{
  map<string,string>::const_iterator it=d->m_properties.find(name);
  if (it==d->m_properties.end())
    return "";
  else
    return it->second;
};

ostream &operator<<(ostream &o, XMLNode &n)
{
  string str;

  XMLParser::printTag(n,str);
  o << str;
  return o;
}

XMLNode &XMLNode::operator<<(const string &str)
{
  XMLParser p(str);

  *this = p.get_root();
  return *this;
}

XMLNode &XMLNode::operator=(const XMLNode &other)
{
  if (other.d == d)
    return *this;

  other.d->ref();
  if (d->deref())
  {
    delete d;
    d=0;
  }
  d=other.d;
  return *this;
}

