/*  
   * General Errors *
   901   - $1 not currently available
   902   - Warning of $1 not currently available
   903   - A message has been dropped, you are exceeding
	   the server speed limit

   * Admin Errors  *
   911   - Error validating input
   912   - Invalid account
   913   - Error encountered while processing request
   914   - Service unavailable

   * Chat Errors  *
   950   - Chat in $1 is unavailable.

   * IM & Info Errors *
   960   - You are sending message too fast to $1
   961   - You missed an im from $1 because it was too big.
   962   - You missed an im from $1 because it was sent too fast.

   * Dir Errors *
   970   - Failure
   971   - Too many matches
   972   - Need more qualifiers
   973   - Dir service temporarily unavailable
   974   - Email lookup restricted
   975   - Keyword Ignored
   976   - No Keywords
   977   - Language not supported
   978   - Country not supported
   979   - Failure unknown $1

   * Auth errors *
   980   - Incorrect nickname or password.
   981   - The service is temporarily unavailable.
   982   - Your warning level is currently too high to sign on.
   983   - You have been connecting and
	   disconnecting too frequently.  Wait 10 minutes and try again.
	   If you continue to try, you will need to wait even longer.
   989   - An unknown signon error has occurred $1
*/

// flap frame types


#define SIGNON      1
#define DATA        2
#define ERROR       3
#define SIGNOFF     4
#define KEEP_ALIVE  5


string TocProtocol::aim_normalize(const string& s)
{
  string buf;

  for (int i=0;i<s.length();i++)
  {
    char c=s[i];
    if (c != ' ' && c!='\t' && c!='\n')
      buf+=tolower(c);
  }
  return buf;
}

unsigned char *TocProtocol::roast_password(const char *pass)
{
  /* Trivial "encryption" */
  static char rp[256];
  static char *roast =  "Tic/Toc";
  int pos = 2;
  int x;

  strcpy(rp, "0x");
  for (x = 0; (x < 150) && pass[x]; x++)
    pos += sprintf(&rp[pos], "%02x", pass[x] ^ roast[x % strlen(roast)]);
  rp[pos] = '\0';
  return (unsigned char *) rp;
}

void TocProtocol::handleData(Network *net,const char *__data,int avail)
{

  int sequence, length, type;

  // flap header
  struct
  {
    unsigned char astrisk[1];
    unsigned char frameType[1];
    unsigned char sequence[2];
    unsigned char dataLength[2];
  } flapHeader;

  /// if not enough data for header, return
  if ((avail < sizeof(flapHeader)))
    return;

  for (int it = 0; it < sizeof(flapHeader); it++)
  {
    flapHeader = __data[it];
  }

  // if first char is not atrisk then partial packet or rogue packet
  if ((char)flapHeader.astrisk[0] != '*')
  {
    if (m_buffer != 0)
      MergePackets(__data, avail);
    else
      debug << "Unknown packet received, not FLAP type or incomplete.\n";

    return;
  }

  sequence = (int)flapHeader.sequence[0];
  sequence |= (int)flapHeader.sequence[1];

  length = (int)flapHeader.dataLength[0];
  length |= (int)flapHeader.dataLength[1];

  type = (int)frameType[0];

  // check to see if we have a complete packet.
  if (length > (avail - sizeof(flapHeader)))
  {
    m_origLength = length;
    m_origType = type;
    m_buffer = __data;
    return;
  }

  string toProcess(__data + 6, length);

  switch (type)
  {
  case(SIGNON): DoSignOn(toProcess); break;
  case(DATA): DoGetData(toProcess); break;
  case(ERROR): DoGetError(toProcess); break;
  case(SIGNOFF): DoSignOff(toProcess); break;
  case(KEEP_ALIVE): DoKeepAlive(toProcess); break;
  default:
    debug() << "Unknown frame type received!\n";
  }

  // if there are more packets in here, call recursively
  if (length > (avail - sizeof(flapHeader)))
    handleData(net, (__data + 6 + length), (avail - 6 - length));


}

void TocProtocol::DoSignOn(const string& process) 
{
  string normUserName = aim_normalize(m_conf.child("user").property("username"));
  
  string outBound;

  debug() << "doing FLAP SIGNON" ;
 
  outBound = ReturnFLAP(TYPE_SIGNON,normUserName.length());

  /*
   * toc_signon <authorizer host> <authorizer port> <User Name> <Password>
   *            <language> <version>
   */

  char buff[2048];
  sprintf(buff, "toc_signon %s %d %s %s %s \"%s\"",
          m_conf.child("loginserver").property("host").c_str(),
          m_conf.child("loginserver").intProperty("port"),
          normalized_username.c_str(),
          roast_password(m_conf.child("user").property("password").c_str()),
          "english",
          "GNU Messenger");

  outBound += ReturnFLAP(TYPE_DATA, buff);

  m_net->sendData(outBound.c_str(),outBound.length());
  m_state=S_online;

}

string TocProtocol::ReturnFLAP(int type, const string& data)
{

  char buff[2048];

  struct
  {
    unsigned char astrisk[1];
    unsigned char frameType[1];
    unsigned char sequence[2];
    unsigned char dataLength[2];
  } flapHeader;

  flapHeader.astrisk = '*';
  flapHeader.frameType = (unsigned char)type;

  short int dataLength = htons((short)data.length());

  flapHeader.dataLength[0] = (unsigned char) (dataLength & 0xff);
  flapHeader.dataLength[1] = (unsigned char) ((dataLength >> 8) & 0xff);

  short int locSequence = htons(m_sequence++);

  flapHeader.sequence[0] = (unsigned char) (locSequence & 0xff);
  flapHeader.sequence[1] = (unsigned char) ((locSequence >> 8) & 0xff);

  int it = 0;
  for (it = 0; it < sizeof(flapHeader); it++)
    buff[it] = (char)flapHeader[it];

  for (it = sizeof(flapHeader); it < (data.length() + sizeof(flapHeader)); it++)
    buff[it] = data[it - sizeof(flapHeader)];

  return string(buff, data.length() + sizeof(flapHeader));

}

void TocProtocol::DoGetData(const string& process) {}

void TocProtocol::DoGetError(const string& process) {}

void TocProtocol::DoSignOff(const string& process) {}

void TocProtocol::DoKeepAlive(const string& process) {}


void TocProtocol::MergePackets(const char* packet, int size)
{

  m_buffer += string(packet, size);

  if (m_buffer.length() < m_origLength)
    return;
  else
  {
    newString = m_buffer;
    int newSize;
    if (m_buffer.length() > m_origLength)
      newSize = m_origLength + (m_origLength - m_buffer.length());
    else
      newSize = m_origLength;

    m_origLength = 0;
    m_buffer = "";

    handleData(NULL, newString.c_str(), newSize);
  }

}