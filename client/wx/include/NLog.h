// --*-c++-*--
/*
    $Id: NLog.h,v 1.3 2002/06/26 04:27:07 thementat Exp $
 
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

class wxListCtrl;

wxFrame * InitLogView(wxWindow * parent);

class guiLog: public wxLog, public wxFrame

{
public:

  guiLog(const wxString& title, int x, int y, int w, int h, wxWindow * parent);
  ~guiLog();

  void OnCloseWindow(wxCloseEvent& event);

protected:

  virtual void DoLog(wxLogLevel level, const wxChar *szString, time_t t);

private:

  wxListCtrl * m_listctrl;
  DECLARE_EVENT_TABLE()

};

/*
enum
{
    wxLOG_FatalError, // program can't continue, abort immediately
    wxLOG_Error,      // a serious error, user must be informed about it
    wxLOG_Warning,    // user is normally informed about it but may be ignored
    wxLOG_Message,    // normal message (i.e. normal output of a non GUI app)
    wxLOG_Info,       // informational message (a.k.a. 'Verbose')
    wxLOG_Status,     // informational: might go to the status line of GUI app
    wxLOG_Debug,      // never shown to the user, disabled in release mode
    wxLOG_Trace,      // trace messages are also only enabled in debug mode
    wxLOG_Progress,   // used for progress indicator (not yet)
    wxLOG_User = 100  // user defined levels start here
};
*/

/*
    -----
    $Log: NLog.h,v $
    Revision 1.3  2002/06/26 04:27:07  thementat
    Event fixes.

    Revision 1.2  2002/06/20 01:25:00  thementat
    Removed unicode for the time being to fix linux build.

    Revision 1.1.1.1  2002/06/06 17:21:42  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i

*/
