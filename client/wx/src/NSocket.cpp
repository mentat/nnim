// --*-c++-*--
/*
    $Id: NSocket.cpp,v 1.4 2002/06/23 18:35:51 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2001-2002  Jesse Lovelace
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
#include <string>
#include <sstream>
#include "NInclude.h"

#include <wx/string.h>
#include <wx/file.h>
#include <wx/app.h>
#include <wx/socket.h>
#include <wx/protocol/protocol.h>

#include "NSocket.h"

#include "protocol.h"
#include "buffer.hpp"

using namespace std;

wxString ConvertString(const byte * data, int len)
{
  stringstream temp;
  for (int i = 0; i < len; i++)
  {
    if ((data[i] > 31) && (data[i] < 127))
      temp << data[i];
    else
    {
      temp << '\\';
      temp << (int)data[i];
    }
  }
  return temp.str().c_str();
}

wxNetwork::wxNetwork(Protocol *proto) :  Network(proto)
{}

// connect to specified host and port
int wxNetwork::connectTo(string host,int port)
{
  const char *hostname = host.c_str();

  wxIPV4address addr;
  addr.Hostname(hostname);
  addr.Service(port);

  if ( (addr.Hostname(host.c_str()) == false) || (addr.Service(port)==false) )
  {
    wxLogMessage(wxT("Invalid Address passed to socket."));
    return 1;
  }

  Connect(addr, FALSE);
  return 0;

}

void wxNetwork::connected()
{
  if (owner)
    owner->connectedToServer(this);
}

bool wxNetwork::isConnected()
{    
  return IsConnected();
}

void wxNetwork::connectionError(int error)
{
  if (owner)
      owner->connectionError(this,error);
}

void wxNetwork::sendData(const char *data,int len)
{
  SetFlags(wxSOCKET_WAITALL);
 
  wxLogMessage(wxString(wxT("Sending: ")) + ConvertString((const byte *)data, len));
  Write(data, len);

  int lCount = LastCount();
  wxLogMessage(wxString(wxT("Send LCount is: ")) + WXITOA(lCount));
}

void wxNetwork::sendData(const vbuf& data)
{
    SetFlags(wxSOCKET_WAITALL);

    wxLogMessage(wxString(wxT("Sending: ")) + ConvertString(data.data(), data.size()));
    Write((const char *)data.data(), data.size());

    int lCount = LastCount();
    wxLogMessage(wxString(wxT("Send LCount is: ")) + WXITOA(lCount));

}

void wxNetwork::disconnect()
{
  if (IsConnected())
    Close();
}

void wxNetwork::socketData(string &data)
{
#define READ_CHUNK_SIZE 300

  if (!IsData())
  {
    wxLogMessage(wxT("IsData is false"));
    return;
  }

  SetTimeout(1);
  SetFlags(wxSOCKET_NONE);

  string dataBuffer;
  unsigned int lCount = 0, it = 0;

  char charBuffer[READ_CHUNK_SIZE];

  Read(charBuffer, READ_CHUNK_SIZE);

  lCount = LastCount();

  dataBuffer += string(charBuffer, lCount);
  wxLogMessage(wxString(wxT("Receive LCount is: ")) + WXITOA(lCount));
  
  while (READ_CHUNK_SIZE == lCount)
  {

    Read(charBuffer, READ_CHUNK_SIZE);
    lCount = LastCount();

    dataBuffer += string(charBuffer, lCount);
    
    wxLogMessage(wxString(wxT("Receive LCount is: ")) + WXITOA(lCount));
  }

  unsigned long int len = dataBuffer.size();

  data = dataBuffer;

#undef READ_CHUNK_SIZE
}

void wxNetwork::socketData(vbuf& data)
{
#define READ_CHUNK_SIZE 300

  if (!IsData())
  {
    wxLogDebug(wxT("IsData is false"));
    return;
  }

  SetTimeout(1);
  SetFlags(wxSOCKET_NONE);

  unsigned int lCount = 0,
        it = 0;

  char charBuffer[READ_CHUNK_SIZE];

  Read(charBuffer, READ_CHUNK_SIZE);

  lCount = LastCount();

  data += vbuf(charBuffer, lCount);

  wxLogMessage(wxString(wxT("Receive LCount is: ")) + WXITOA(lCount));

  while (READ_CHUNK_SIZE == lCount)
  {

    Read(charBuffer, READ_CHUNK_SIZE);
    lCount = LastCount();

    data += vbuf(charBuffer, lCount);

    wxLogMessage(wxString(wxT("Receive LCount is: ")) + WXITOA(lCount));
  }

#undef READ_CHUNK_SIZE

}

void wxNetwork::socketData(char* &data,int &len)
{
#define READ_CHUNK_SIZE 300

  if (!IsData())
  {
    wxLogMessage(wxT("IsData is false"));
    len = 0;
    data = NULL;
    return;
  }

  SetTimeout(1);
  SetFlags(wxSOCKET_NONE);

  vector<char> dataBuffer;
  int lCount = 0, it = 0;

  char charBuffer[READ_CHUNK_SIZE];

  Read(charBuffer, READ_CHUNK_SIZE);

  lCount = LastCount();

  for (it = 0; it < lCount; it++)
  {
    dataBuffer.push_back(charBuffer[it]);
  }
  wxLogMessage(wxString(wxT("Receive LCount is: ")) + WXITOA(lCount));
  
  while (READ_CHUNK_SIZE == lCount)
  {

    Read(charBuffer, READ_CHUNK_SIZE);
    lCount = LastCount();

    for (it = 0; it < lCount; it++)
    {
      dataBuffer.push_back(charBuffer[it]);
    }
    wxLogMessage(wxString(wxT("Receive LCount is: ")) + WXITOA(lCount));
  }

  len = dataBuffer.size();
  data = new char[len];

  for(it = 0; it < len; it++)
  {
    data[it] = dataBuffer[it];
  }

#undef READ_CHUNK_SIZE
}

void wxNetwork::checkForData()
{
  vbuf buf;
  int avail = 0;

  socketData(buf);

  if (buf.empty())
  {
    wxLogMessage(wxT("Incomming is zero."));
    return;
  }

  wxLogDebug(wxString(wxT("Receiving: ")) + ConvertString(buf.data(), buf.size()));


  if (owner)
    owner->handleData(this,buf);
  else
    wxLogError(wxT("No owner in wxNetwork::checkForData()"));

}

void wxNetwork::errClosed()
{
  //FIXME: this is obviously wrong and horrible.
  if (owner)
    owner->logout();
  wxLogError(wxT("Connection to server was lost."));
}
