// --*-c++*--
// $Id: gmstorage.h,v 1.1 2002/06/06 17:21:52 thementat Exp $

#ifndef GM_STORAGE_H
#define GM_STORAGE_H

#include "dstring.h"
class Storage
{
public:
  Storage();
  ~Storage();
  typedef pair<int,pair<byte *,int> > sectionType; // (type, (data, len))
  typedef pair<byte *,int> dataType;               // (data,len)

  static sectionType empty;
  static uint32 char2int(unsigned char *buf);
  static void int2char(unsigned char *buf,uint32 num);

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
