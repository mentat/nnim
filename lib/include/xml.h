#ifndef _XMLPARSER_H
#define _XMLPARSER_H

#include <string>
#include <map>
#include <vector>
#include <stack>

#include <xmlnode.h>



#include "expat.h"
/**
 * An XML Parser
 *
 * This class a string and parsers it to build a XML tree.
 *
 * Usually you dont need to use this class directly, you can parse strings
 * and convert XMLNodes to string directly with the XMLNode class.
 *
 * @author Henrik Abelsson
 * @version 0.1
 */
using namespace std;

class XMLParser: public Expat
{
public:
    enum Type {File,String};
    /**
     * Construct a new parser
     *
     * @param data Data to read, either a ready string or a filename
     * @param type Specifies w XMLParser::String - read from an in memory string, or XMLParser::File, a file.
     */
    XMLParser(const string &data="",Type type=XMLParser::String);

    /**
     * Set a XMLNode as root
     */
    XMLParser(XMLNode &root);
    ~XMLParser();

    /**
     * Write the XML Tree to  to line.
     *
     * @param line Otherwise store the tree in this string.
     */
    int tree_to_xml(string &line);
    int tree_to_file(const string &filename);
    /**
     * Get all tags in the tree whose name are key.
     *
     *
     * @param key What tags to return
     * @return all the tags that match key
     * @see tree_to_xml()
     */
    vector<XMLNode> get_tags(const string &key);

    /**
       Return the root tag.
    */
    XMLNode &get_root() { return root;};

    int lock(const string &fname);
    int release(const string &fname);
    int bad() { return status;};
    int good() { return status == 0;};

    static int printTag(const XMLNode &node, string &ret,int indent=0);
    int getParsed() const { return 0; };
protected:
  void recievedTag(XMLNode &n);
  void rootTagRecieved(XMLNode &n);
  void parserError(string errorMsg,int line);

private:
  XMLNode root;
  int status;
};

#endif
