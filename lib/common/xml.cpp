// --*-c++-*--
// $Id: xml.cpp,v 1.1 2002/06/06 17:21:53 thementat Exp $
#pragma warning(disable:4786)
#ifndef WIN32
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include <map>
#include <list>
#include <string>
#include <stack>

#include <strstream>

using namespace std;

#include "xml.h"


XMLParser::XMLParser(const string &line,Type type)
{
  switch (type)
  {
  case String:
    parse(line);
    break;
  case File:
  {
    string str;
    ifstream in(line.c_str());
    if (!in)
      return;
    while (in.good())
    {
      getline(in,str);
      parse(str);
    }
    in.close();
  }
  break;
  };
}

XMLParser::XMLParser(XMLNode &_root)
{

  root=_root;
}

XMLParser::~XMLParser()
{
  reset();
}


int XMLParser::tree_to_xml(string &line)
{
  printTag(root,line);
  return 0;
}

int XMLParser::tree_to_file(const string &filename)
{
  string line;
  ofstream out(filename.c_str());

  if (!out)
    return -1;

  printTag(root,line);
  out << line;

  out.close();
  return 0;
}

vector<XMLNode> XMLParser::get_tags(const string &key)
{
    vector<XMLNode> ret;
    return ret;
}

int XMLParser::printTag(const XMLNode &node,string &ret,int indent)
{
	int s(0);
    
	// print indent
	for (s=0; s<indent; s++) ret += "\t";

	// print tag name
    ret += "<" + node.d->m_name;
    
	// print properties
	for (map<string,string>::const_iterator i=node.d->m_properties.begin();i!=node.d->m_properties.end();i++)
    {
        if (i->second == "")
            ret+= " " + i->first + "=\"\"";
        else
            ret+= " " + i->first + "=\"" + i->second + "\"";
    }

	// if tag has no children, close and return
    if (node.d->m_data == "" && node.d->m_children.empty())
    {
		ret+="/>\n";
		return 0;
    }
    
	// if has data/children
	ret+=">";
    
	// print children
	bool childprinted = false;
	for (vector<XMLNode>::const_iterator it=node.d->m_children.begin();it!=node.d->m_children.end();it++)
    {
		ret += '\n';
		childprinted = true;
        string tmp;
        printTag(*it,tmp,indent+1);
        ret+=tmp;
    }
  
	// indent for data
	if (childprinted && (node.d->m_data.length()>0))
		for (s=0; s<indent + 1; s++) 
			ret += "\t";

	if (node.d->data_is_cdata)
        ret+="<![CDATA[";

    if (node.d->m_data.length()>0)
        ret+=node.d->m_data;

    if (node.d->data_is_cdata)
		ret+="]]>";

	if (childprinted && (node.d->m_data.length()>0))
		ret+='\n';

    // print indent
	if (childprinted)
		for (s=0; s<indent; s++) ret += "\t";

	ret += "</" + node.d->m_name + ">\n";
    
	return 0;
}


void XMLParser::recievedTag(XMLNode &n)
{
  root.addChild(n);
}

void XMLParser::rootTagRecieved(XMLNode &_root)
{
  root=_root;
}

void XMLParser::parserError(string errorMsg,int line)
{
  cout << "error:" << errorMsg<<":" << line << endl;
  reset();
}


