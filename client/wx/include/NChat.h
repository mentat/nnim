// --*-c++-*--
/*
    $Id: NChat.h,v 1.2 2002/06/27 11:52:51 thementat Exp $
 
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
    $Log: NChat.h,v $
    Revision 1.2  2002/06/27 11:52:51  thementat
    More event handling fixes.

    Revision 1.1.1.1  2002/06/06 17:21:42  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i

*/

#ifndef GUI_CHAT_H
#define GUI_CHAT_H

class wxNotebook;
class wxHtmlWindow;
class DnDFile;
class guiChat;
#include "contact.h"

guiChat *
InitChatView(wxWindow * parent, const wxString& name, Contact cont);


/**
 * Chat Window Class
 * 
 * This is the basic chat window with an area to type
 * text and an area to read incoming text.
 *
 * @author Jesse Lovelace
 * @version 0.1
 */

class myTextCtrl: public wxTextCtrl
{
public:

  myTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value = "", 
    const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0)
    : wxTextCtrl(parent, id,value, pos, size, style) { m_parent = (guiChat *)parent; m_sendReady = false; }

  void OnChar(wxKeyEvent& event);
  void OnIdle(wxIdleEvent& event);

  bool m_sendReady;

private:

  guiChat * m_parent;

  DECLARE_EVENT_TABLE()

};

class guiChat: public wxFrame
{
public:

  friend class DnDFile;

  /**
    * Construct a new chat window
    *
    * @param title The window title.
    * @param x Left/right position on screen, top left is (0,0)
    * @param y Up/down position on screen.
    * @param w Window width
    * @param l Window length
  */
	guiChat(const wxString& title, int x, int y, int w, int h, wxWindow * parent, Contact cont);
	
  virtual ~guiChat();

  /// Called when window is sent the quit command, ie Windows X button
	void OnQuit(wxCommandEvent& event);

  void OnIdle(wxIdleEvent& event);
  /// Shows About information
	void OnAbout(wxCommandEvent& event);

  /// Called when window is closing, also called by OnQuit()
  void OnCloseWindow(wxCloseEvent& event);

  /// Called when send button is pressed
  void OnSend(wxCommandEvent& event);
  void OnFileSend(wxCommandEvent& event);
  void OnSendInternal(const wxString& text);

  void InitTabView(wxNotebook* notebook, wxPanel* window);

  void DisplayText(const wxString& text);

  void OnP2Browse(wxCommandEvent& event);

  wxString GetRealName() { return m_sxRealName;}
  wxString GetServerId() { return m_sxServerId;}
  wxString GetProtocol() { return m_sxProtocol;}

  void SetRealName(const wxString& name) { m_sxRealName = name; }
  void SetServerId(const wxString& serverId) { m_sxServerId = serverId; }
  void SetProtocol(const wxString& protocol) { m_sxProtocol = protocol; }

  Contact m_Contact;

private:

	wxString m_sxRealName;
	wxString m_sxServerId;
	wxString m_sxProtocol;

	bool m_window_closing;

	wxNotebook* m_notebook;

	wxString m_htmlInternalText;

	wxPanel *m_panel;

	// Chat tab items
	wxHtmlWindow *m_html;
	myTextCtrl *m_outText;
	
  // File tab items

  wxTextCtrl *m_file_p2;
  wxGauge *m_transfer_gauge_p2;

  // Security tab items

  // All wxWindows classes that use events must 
  // define this macro
	DECLARE_EVENT_TABLE()
};

#endif
