// -*- C++ -*-
/*
    $Id: contact.h,v 1.3 2002/06/27 11:52:51 thementat Exp $

    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2002  Henrik Abelsson <henrik@abelsson.com>

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
 // Contact(const Contact& cont) { *this = cont; }
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
