// --*-c++-*--
/*
    $Id: NPrefs.h,v 1.1 2002/06/06 17:21:42 thementat Exp $
 
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

    -----
    $Log: NPrefs.h,v $
    Revision 1.1  2002/06/06 17:21:42  thementat
    Initial revision

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.7  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.6  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/

#ifndef GUI_PREFS_H
#define GUI_PREFS_H

void InitPrefView(wxWindow * parent, bool newUser = false);

class AuthLoad;
class MySplitterWindow;
class NPrefNet;
class NPrefSec;
class NPrefGen;

class guiPrefs: public wxFrame
{
public:

	guiPrefs(const wxString& title, int x, int y, int w, int h, wxWindow * parent, bool newUser = false);
	
  virtual ~guiPrefs();
	 
  void OnTreeChange(wxTreeEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnApply(wxCommandEvent& event);
  void OnOK(wxCommandEvent& event);
  void OnCloseWindow(wxCloseEvent& event);
  void OnQuit(wxCommandEvent& event);

  void SaveSettings();
  void LoadSettings();

protected:

  wxGenericTreeCtrl& GetTree();

private:

  /// new user flag
  bool m_newUser;
  /// setting has been changed flag
  bool m_aChange;

  /// dialog information loading done or not
  bool m_loadDone;

  // menu tree ids
  wxTreeItemId m_treeRoot;

  // controls
  wxButton * m_apply;

  wxGenericTreeCtrl * m_treeCtrl;
  MySplitterWindow * m_splitterWindow;

  NPrefNet * m_prefNet;
  NPrefGen * m_prefGen;
  NPrefSec * m_prefSec;

  void ActivateSave(bool activate = true);

	DECLARE_EVENT_TABLE()

};

#endif
