// --*-c++-*--
/*
    $Id: NLogin.h,v 1.1 2002/06/06 17:21:43 thementat Exp $
 
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

#ifndef GUI_LOGIN_H
#define GUI_LOGIN_H

using namespace std;

class wxWindow;

wxWindow * InitLoginView(wxWindow * parent = NULL);

/**
 * Login Window
 * 
 * Displays the login screen and calls the user manager
 * for validation of hashes.
 *
 * @author Jesse Lovelace
 * @version 0.1
 */
class guiLogin: public wxFrame
{
public:

  /**
    * Construct a new login window
    *
    * @param title The window title.
    * @param x Left/right position on screen, top left is (0,0)
    * @param y Up/down position on screen.
    * @param w Window width
    * @param l Window length
  */
	guiLogin(const wxString& title, int x, int y, wxWindow * parent);
	
  virtual ~guiLogin();
	 
  /// Called when window is sent the quit command, ie Windows X button
  void OnQuit(wxCommandEvent& event);

  /// Shows About information
	void OnAbout(wxCommandEvent& event);

  /// Called when the login button is pressed or <ENTER> received
	void OnLogin(wxCommandEvent& event);

  /// Called when window is closing, also called by OnQuit()
	void OnCloseWindow(wxCloseEvent& event);

  /// Runs the new user wizard
	void OnNewUser(wxCommandEvent& event);

  void NewUser();
private:

	wxTextCtrl *m_username;
	wxTextCtrl *m_password;

  // All wxWindows classes that use events must 
  // define this macro
	DECLARE_EVENT_TABLE()
};


#endif
/*
    -----
    $Log: NLogin.h,v $
    Revision 1.1  2002/06/06 17:21:43  thementat
    Initial revision

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i

*/