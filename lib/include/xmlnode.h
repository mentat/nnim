#ifndef _XMLNODE_H
#define _XMLNODE_H

#include <string>
#include <map>
#include <vector>
#include <stack>

using namespace std;
class XMLNode;


/**
  An XML Tag represented as a single class.


  This is a parsed xml tag. It stores all properties and all nested nodes and
  the text between the start tag. It can be used as a combined tree, array and
  associative map data structure without any artificial size limits.

  Terminology:
  \begin{itemize}
  \item tag - everything betweem <name> and </name> including the tag name and properties
  \item node - same as tag
  \item property - A value stored in the tag start <tag property="value">
  \item child nodes - nested nodes <tag> <child> </child> </tag>
  \item data - Text between start and end tag
  \end{itemize}
  Example:
  \begin{verbatim}
  <tag property1="value" property2="a value">
  .. some text here ..
  </tag>
  \end{verbatim}

  @author Henrik Abelsson
  @version 0.1
 */
class XMLNode
{
public:

  XMLNode();
  XMLNode(const XMLNode &other);
  ~XMLNode();
  const string &name() const { return d->m_name; };
  const string &data() const { return d->m_data; };

  /**
   Return all properties
  */
  map<string,string> &properties() { return d->m_properties; };

  /**
     Return all properties (read only)
   */
  const map<string,string> &const_properties() const { return d->m_properties; };

  /**
     Return a single property
  */
  string property(const string &name) const;

  /**
     Return property as an integer if possible, otherwise 0.
   */
  int intProperty(const string &name) const;

  /**
     true if the node has a child with the specified name
     @param name name of tag to look for
     @param n how many tags are required (n=2 means there has to be two children with the specified name)
   */
  bool hasChild(const string &name, int n=1) const;

  /**
     Return how many children of a specified name there is in the node
   */
  int numChildren(const string &name) const;

  /**
     Return all children
   */
  vector<XMLNode> &children() { return d->m_children; };

  /**
     Return all children (read only)
   */
  const vector<XMLNode> &const_children() const { return d->m_children; };

  /**
     Return a single child
     @param name name of tag to look for
     @param n which tag to return if there are more than one

   */
  XMLNode &child(const string &name, int n=1);


  /**
     Set tag name
  */
  XMLNode &setName(const string &name) { d->m_name = name; return *this; };

  /**
     Set data
  */
  XMLNode &setData(const string &data) { d->m_data = data; return *this; };

  /**
     Set data as cdata sections
   */
  XMLNode &setCData(const string &data) { d->data_is_cdata=true; d->m_data = data; return *this; };

  /*
    Set a property
   */
  XMLNode &setProperty(const string &name, const string &value);
  ///
  XMLNode &setProperty(const string &name, int value);

  /**
    Add a child node to the tree.
   */
  XMLNode &addChild( XMLNode &node) { d->m_children.push_back(node); return node;};
  /// - " -
  XMLNode &addChild(const string &name, const string &data="");

  /**
     Delete a child node
     @param name name of tag to look for
     @param n which tag to delete if there are more than one
   */
  XMLNode &delChild(const string &name, int n=1);

  friend ostream& operator<<(ostream &o,XMLNode &n);

  /**
     Parse a string into a XMLNode directly.
  */
  XMLNode &operator<<(const string &str);

  XMLNode &operator=(const XMLNode &other);


  /**
     Implicitly convert to string.
   */
  operator string() const;

  /**
     Set and get tag properties using operator[]
   */
  string &operator[](const string &name);
  /// - " -
  const string operator[](const string &name) const;

  friend class XMLParser;
private:
  class XMLNodeData
  {
  public:
    XMLNodeData() { refCount=1;};
    void ref() {refCount++;};
    int deref() {return --refCount == 0;};
    string m_name;
    string m_data;
    bool data_is_cdata;
    map <string,string> m_properties;
    vector<XMLNode> m_children;
    bool oneline;
  private:
    int refCount;
  };

  XMLNodeData *d;
};

#endif
