#include "NInclude.h"

#include <wx/string.h>
#include <wx/file.h>
#include <wx/app.h>
#include <wx/socket.h>
#include <wx/protocol/protocol.h>

#include "NSocket.h"

#include "protocol.h"

wxString ConvertString(const char * data, int len)
{
  wxString temp;
  for (int i = 0; i < len; i++)
  {
    if ((data[i] > 31) && (data[i] < 127))
      temp += data[i];
    else
    {
      temp += '\\';
      temp += WXITOA((int)((unsigned char)data[i]));
    }
  }
  return temp;
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
 
  wxLogMessage(wxString(wxT("Sending: ")) + ConvertString(data, len));
  Write(data, len);

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

  unsigned long int len = dataBuffer.size();

  for(it = 0; it < len; it++)
  {
    data[it] = dataBuffer[it];
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
  string buf;
  int avail = 0;

  socketData(buf);

  if (buf.empty())
  {
    wxLogMessage(wxT("Incomming is zero."));
    return;
  }

  wxLogMessage(wxString(wxT("Receiving: ")) + wxString(buf.c_str(), wxConvUTF8));


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
