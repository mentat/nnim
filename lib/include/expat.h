/* This file is (C) Seth HartBecke, modified by kim authors */
#ifndef KIM_EXPAT_H
#define KIM_EXPAT_H

#include <vector>
#include <stack>

#include <stdio.h>

#include <xmlnode.h>
#include "../common/expat/include/xmlparse.h"

using namespace std;

/**
 *  This class helps us interface with expat.
  * This is the coolest class in the whole project.  This class creates a nice clean interface
  * between expat (which is C) and C++.
  *
  * @author Seth Hartbecke (gandalf@netins.net)
  * @short Expat interface
  */

class Expat
{
public:
	Expat();
	virtual ~Expat();

        /**
         * This feeds more data into the parser.
         */
	void parse(string data);

        /**
         * Deletes the expat datastructures and prepares a new one.  Nice for soft
         * resets.  Used by XMLStream
         */
	void reset();


protected:
        /**
         * This method is called whenever a new tag is recieved.  You should ovveride this.
         */
	virtual void recievedTag( XMLNode &n);
        /**
         * This method is called when the first complete tag is recieved.
         */
	virtual void rootTagRecieved(XMLNode &root);
        /**
         * This method is called when first tag recieved is ended.  This usually means
         * that the document is EOF.
         */
	virtual void rootTagEnded();
        /**
         * This is any errors that expat produces.  The class will not work anymore after an
         * error has been encountered unless you call reset.
         */
	virtual void parserError(string errorMsg, int line );

private:
	static void startElement(void *userData, const XML_Char *name, const XML_Char **atts);
	static void endElement(void *userData, const XML_Char *name);
	static void charData(void *userData, const char *data, int len);

	bool rootRecieved;

	void init();
	void cleanup();

	vector<XMLNode> newNodes;		// Nodes that need to be sent out
						// via recievedTag
	stack<XMLNode> fillingNodes;		//Nodes that are currently being filled

	XML_Parser parser;
        XMLNode root;
};

#endif
