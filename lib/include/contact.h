#ifndef KIT_CONTACT_H
#define KIT_CONTACT_H

#include <string>
#include "cryptography.h"
#include "xml.h"

using namespace std;


/**
  All contacts (buddies) in the various protocols are represented by this

 Each contact has a status associated with it, and the client should display the
 user differently depending on the status here.
 \begin{itemize}
 \item Contact::Offline - User is offline
 \item Contact::Online - User is connected and available
 \item Contact::Away - User is connected but away
 \item Contact::Dnd - User is connected but doesnt want to be disturbed
 \item Contact::Na - User is connected but is not available
 \item Contact::Ffc - User is connected and free for chat
 \item Contact::Custom - A custom status. retrieve info()["customstatus"] to find out more.
 \end{itemize}
 */

class Contact
{
 public:

  Contact();
  Contact(XMLNode &config);

  ~Contact() {};

  // Contact status.
  //Custom is a string set and retrieved by info()["customstatus"]
  enum status { Offline, Online, Away, Occupied, Dnd, Na, Ffc, Custom };

  ////////////////////////// set info /////////////////////////////////////////

  void    setProtocol(const string &p) { m_user["protocol"]=p;}
  void    setServerId(const string &n) { m_user["serverid"]=n;}
  void    setNick(const string &n)     { m_user["nick"]=n;}
  void    setStatus(const int status)  { m_status=status;}

  ////////////////////////// get info //////////////////////////////////////

  // Access
  string protocol() const { return m_user["protocol"];}
  string serverId() const { return m_user["serverid"];}
  string nick() const     { if (m_user.property("nick")=="") return m_user["serverid"]; return m_user["nick"];}
  int    status() const   { return m_status;}

  /// Get xml node (for extending)
  XMLNode &info()  { return m_user;}
  void operator=(const Contact &other);
 private:

 gmCrypto * m_crypto;
  XMLNode m_user;
  int m_status;
};

/// Comparation operators
bool operator==(Contact lhs,Contact rhs);

bool operator!=(Contact lhs,Contact rhs);

#endif
