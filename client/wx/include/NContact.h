// --*-c++-*--
/*
    $Id: NContact.h,v 1.6 2002/06/27 15:26:56 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2001  Jesse Lovelace
 
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

#ifndef GUI_CONTACT_H
#define GUI_CONTACT_H

#include "NEvent.h"
#include "wx/treebase.h"
#include <map>
#include "boost/smart_ptr.hpp"

using namespace std;
using namespace boost;

class NContactTreeXML;
class guiChat;

/**
 * Contact Window
 * 
 * Prolly the most important frame, holds the contact tree
 * and is were most of the user/program interface is.
 *
 * @author Jesse Lovelace
 * @version 0.1
 */
class guiContact: public wxFrame
{
public:

	guiContact(bool newUser, wxWindow* parent, wxWindowID id, const wxString& title, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxDEFAULT_FRAME_STYLE, const wxString& name = "frame");
	
    virtual ~guiContact();

	void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);

    /// Called when window is idle
    void OnIdle(wxIdleEvent& event);
 
	void OnLogout(wxCommandEvent& event);
    void OnConnect(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
	void OnCheckVersion(wxCommandEvent& event);
    void OnCommit(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnSetAway(wxCommandEvent& event);
    void OnSetInfo(wxCommandEvent& event);
    void OnGetMessage(gmEvent& event);
    void OnGetMessageAnony(gmEvent& event);
    void OnChangeStatus(gmEvent& event);
    void OnLeaveAway(wxCommandEvent& event);

protected:

    bool RemoveChat(guiChat* window);
    bool AddChat(const string& name, guiChat * window);


private:
    friend class guiChat;
    map<string, shared_ptr<guiChat> > m_chatWindows;

	NContactTreeXML *m_tree;
    wxBitmapButton *m_start; 
  
	DECLARE_EVENT_TABLE()

};

wxFrame *
InitContactView(wxWindow * pParent = NULL, bool newUser = false);

/*
    -----
    $Log: NContact.h,v $
    Revision 1.6  2002/06/27 15:26:56  thementat
    Contact fixes and debug fixes, works better!

    Revision 1.5  2002/06/27 11:52:51  thementat
    More event handling fixes.

    Revision 1.4  2002/06/26 04:27:07  thementat
    Event fixes.

    Revision 1.3  2002/06/21 19:38:21  thementat
    Trying to get online...

    Revision 1.2  2002/06/21 19:03:14  thementat
    NNIM compiles and links in gcc 2.96 20000731

    Revision 1.1.1.1  2002/06/06 17:21:37  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.8  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/

#endif
