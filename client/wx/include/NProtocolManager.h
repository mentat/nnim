// --*-c++-*--
/*
    $Id: NProtocolManager.h,v 1.4 2002/06/27 02:54:07 thementat Exp $
 
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

    -----
    $Log: NProtocolManager.h,v $
    Revision 1.4  2002/06/27 02:54:07  thementat
    Changes to the Event handling.

    Revision 1.3  2002/06/26 04:27:07  thementat
    Event fixes.

    Revision 1.2  2002/06/25 16:48:09  thementat
    Got TOC done! (mostly)

    Revision 1.1.1.1  2002/06/06 17:21:42  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.3  2001/12/05 05:26:59  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.2  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i

    
*/

#ifndef WX_PROTOCOL_H
#define WX_PROTOCOL_H

#include "NInclude.h"
#include "NMain.h"
#include "protocol.h"
#include "NEvent.h"

using namespace std;

DECLARE_APP(wxNNIM)

class AuthLoad;

void
InitProtoManager(AuthLoad & myLoader, ProtocolManager& myMan);

//void 
//AddContacts(const wxString& protocol, AuthLoad &MyLoader, ProtocolManager &Manager);

void 
AddNet(const wxString& network, AuthLoad * pMyLoader, ProtocolManager *pManager);

class wxProtocolManager: public ProtocolManager, public wxObject
{
public:
  virtual ~wxProtocolManager();
  wxProtocolManager(): ProtocolManager(), m_handler(NULL) { SetEventHandler(wxGetApp()); }

  // override the c_* functions you are interested in. hopefully the names should be
  // descriptive enough.

  Network *createNet(Protocol *proto);
  NetworkServer * createServer(Protocol *proto);

  void removeNet(const Network *);

  void c_loggedIn(const string &proto);

  void c_statusChange(const string &proto,const Contact &c);

  virtual void c_recvdMessage(const string &proto,const Contact &c, const string &message);
  virtual void c_recvdMessageAnony(const string& proto, const Contact &c, const string& message);

  void c_error(const string &proto,int err_no,const string &error);
  
  void c_stateChange(const string &proto,int state);

  void SendEvent(gmEvent & event);

  void SetEventHandler(wxEvtHandler& handler, int id = -1);

  private:

  wxEvtHandler *m_handler;
  int m_id;

};

#endif
