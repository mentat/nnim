//#define TEST
     /*
#ifndef byte
typedef unsigned char byte;
#endif
#ifndef uint32
typedef unsigned int uint32;
#endif
*/
  /*
#ifndef WIN32
#include <sys/time.h>
#include <netinet/in.h>
#else
#include <windows.h>
#endif    */

#include <string>
#include <stdio.h>
#include <list>
#include <utility>
#include "crypto/misc.h"

using namespace std;
#include "gmstorage.h"

Storage::sectionType Storage::empty;


int dumphex (byte *data,int length)
{
    int i;
  for (i=0;i<length;i++)
        {
            printf("0x%x ",data[i]);
        }
  printf ("\n");
  return 0;
}

int dumphex (const dstring &data)
{

    int i;
  for (i=0;i<data.length();i++)
        {
            printf("0x%x ",data.data()[i]);
        }
  printf ("\n");
  return 0;
}

word32 Storage::char2int(unsigned char *buf)
{
  word32 i;

  i= buf[0];
  i <<= 8;
  i+= buf[1];
  i <<= 8;
  i+= buf[2];
  i <<= 8;
  i+= buf[3];
  i=byteReverse(i);

  return i;
}

void Storage::int2char(unsigned char *buf,word32 num)
{
  num=byteReverse(num);

  buf[0] = ( unsigned char ) ((num)>>24)& 0x000000FF;
  buf[1] = ( unsigned char ) ((num)>>16)& 0x000000FF;
  buf[2] = ( unsigned char ) ((num)>>8)& 0x000000FF;
  buf[3] = ( unsigned char ) (num) & 0x000000FF;
}

Storage::Storage():
  m_data()
{
}

Storage::~Storage()
{

}

//
// Add data at the end of the file.
//
int Storage::addSection(byte t,int len,const byte *newData)
{
  byte slen[4];
  int i;

  m_data.append(t);

  int2char(slen,len);
  for (i=0;i<4;i++)
    m_data.append(slen[i]);

  for (i=0;i<len;i++)
    m_data.append(newData[i]);

#ifdef TEST
  string tmp= (string)m_data;
  dumphex(m_data);
#endif
  return 0;
}

int Storage::addSection(byte t,unsigned int data)
{
  byte buf[4];
  int2char(buf,data);
  return addSection(t,4,buf);
}

int Storage::addSection(byte t,byte data)
{
  return addSection(t,1,&data);
}
int Storage::addSection(byte t,const string &data)
{
  return addSection(t,data.length()+1,(byte*)data.c_str());
}

pair<int, pair < byte *,int> > Storage::readSection(unsigned int n)
{
  unsigned int current =0;
  byte type;
  unsigned int len;
  unsigned int sectionLen;
  unsigned int offset=0;
  byte *data;

  data=(byte*)m_data.data();
  len=m_data.length();
  // look for the right section
  while (current < n)
  {
    // read length and skip to next type entry
    type=data[offset++];
#ifdef TEST
    cout << "type:" << (int)type << endl;
    dumphex(data+offset,4);
#endif
    sectionLen=char2int(data+sizeof(byte)*offset);
    offset+=4;
#ifdef TEST
    cout << "len:" << (int)sectionLen << endl;
    cout << "---Type: " << type << " Len: " << sectionLen << endl;
#endif
    if (sectionLen+offset>len)
      return sectionType(0,dataType(0,0));
    offset+=sectionLen;
    current++;
  }
  type=data[offset++];

  sectionLen=char2int(data+sizeof(byte)*offset);
  offset+=4;
  if (sectionLen+offset>len)
    return sectionType(0,dataType(0,0));

#ifdef TEST
  cout << "---Type: " << (int)type << " Len: " << sectionLen << endl;
#endif

  return sectionType(type,dataType(data+sizeof(byte)*offset,sectionLen));
}

vector<Storage::sectionType> Storage::readSections()
{
	vector<Storage::sectionType> foo;
	return foo;
}

void Storage::setData(int len, byte *data)
{
  for (int i=0;i<len;i++)
    m_data.append(data[i]);
}

#ifdef TESTs
int main()
{
  Storage s;
  Storage::sectionType t;
  s.addSection(1,7,(const byte*)"henrik");
  s.addSection(3,5,(const byte*)"abcd");

  t=s.readSection(0);
  cout <<(char*) t.second.first << endl;
  t=s.readSection(1);
  cout <<(char*) t.second.first << endl;
  t=s.readSection(2);
  cout <<(char*) t.second.first << endl;

}
#endif

