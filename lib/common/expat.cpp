/* Copyright (C) 2000 by Seth Hartbecke - Modified by kim authors */
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <iostream>
#include "expat.h"


using namespace std;

Expat::Expat()
{
  parser = 0;
  init();
}

Expat::~Expat()
{
  cleanup();
}

void Expat::init()
{
  if (parser != 0)
    cleanup();

  parser = XML_ParserCreate( NULL );
  XML_SetElementHandler(parser, startElement, endElement);
  XML_SetCharacterDataHandler(parser, charData);
  XML_SetUserData(parser, (void *)this);

  rootRecieved = false;
}

void Expat::cleanup()
{
  if (parser == 0)
    return;

  XML_ParserFree( parser );
  parser = 0;

  newNodes.clear();
  while (!fillingNodes.empty())
    fillingNodes.pop();
}

void Expat::reset()
{
  cleanup();
  init();
}

void Expat::parse(string data)
{
  if (!parser)
    parserError("Invernal error",0);

  if (!XML_Parse( parser, (const char *)data.c_str(), data.length(), false ))
  {

    parserError( XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
    reset();
    return;
  }

  // Send out any new tags
  for (int i=0;i<newNodes.size();i++)
    recievedTag(newNodes[i]);
  newNodes.clear();
}

void Expat::recievedTag( XMLNode &n)
{
  string s;
  printf("Recieved root tag:\n");
  cout << n;
}

void Expat::rootTagRecieved(XMLNode &n)
{
  string s;
  printf("Recieved root tag:\n");
  cout << n;
}

void Expat::rootTagEnded()
{
//	printf("Root tag ended.\n");
}

void Expat::parserError(string errorMsg, int line )
{
  cout << "Expat parser error: " << errorMsg << " at "<< line << endl;
}


void Expat::startElement(void *userData, const XML_Char *name, const XML_Char **atts)
{
  /* get the userdata into a usable form */
  Expat *me = (Expat *)userData;

  /* New tag? Well...create one */
  XMLNode t;

  t.setName(name);
  /* now put on it's parameters */
  int i=0;

  if (atts != NULL)
  {
    while (atts[i] != NULL)
    {
      t.setProperty(atts[i],atts[i+1]);
      i+=2;
    }
  }


  if( !me->rootRecieved )
  {
    // This is the first tag...therefore the root tag.
    me->rootTagRecieved(t);
    me->rootRecieved = true;
    return;
  }

  /* Register it as a sub tag of the previous tag */
  if (!me->fillingNodes.empty())
    me->fillingNodes.top().addChild(t);

	/* push this node onto the stack */
  me->fillingNodes.push( t );
}

void Expat::endElement(void *userData, const XML_Char *)
{
/* get the userdata into a usable form */
  Expat *me = (Expat *)userData;

  if (me->fillingNodes.empty())
  {
    // If this is 0 then this is the end of the root tag...
    me->rootTagEnded();
    return;
  }

  if (me->fillingNodes.size() == 1)
  {
    // We used to send them right out from here...but I found out something very nasty can
    // happen...suppose that the parser is deleted because of this node (login error...)
    me->newNodes.push_back(me->fillingNodes.top());

    // We do not delete the node becuse that job belongs to
    // whoever overrid the revieced tag
  }
  me->fillingNodes.pop();
}

void Expat::charData(void *userData, const char *s, int len)
{
  /* get the userdata into a usable form */
  Expat *me = (Expat *)userData;

  char *data;
  data = new char[len + 1];
  memcpy(data, s, len);
  data[len] = '\0';

  if (me->fillingNodes.empty())
  {
    return;
  }

  string newData = me->fillingNodes.top().data()+data;
//  cout << "newData:" << newData<< endl;
  me->fillingNodes.top().setData(newData);

  delete[] data;
}
