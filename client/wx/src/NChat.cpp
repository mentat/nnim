// --*-c++-*--
/*
    $Id: NChat.cpp,v 1.2 2002/06/19 19:14:43 thementat Exp $
 
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

#include "NInclude.h"
#include "NXpms.h"
#include "NContact.h"
#include "NChat.h"
#include "NMain.h"
#include "NAbout.h"

#include "wx/html/htmlwin.h"
#include "wx/filedlg.h"
#include "wx/statline.h"
#include "wx/dnd.h"
#include "wx/notebook.h"

#include "authload.h"
#include "contact.h"
#include "manager.h"

#include "bitmaps/nnim_button_trans2.xpm"
#include "bitmaps/nnim_button_trans_bw2.xpm"


guiChat *
InitChatView(wxWindow * parent, const wxString& name, Contact c)
{
  guiChat * pMyChat = new guiChat(name, 20, 20, 250, 200, parent, c);
  pMyChat->Show(TRUE);
  return pMyChat;
}



DECLARE_APP(wxNNIM)

class DnDFile : public wxFileDropTarget
{
public:
    DnDFile(guiChat *pOwner) { m_pOwner = pOwner; }

    virtual bool OnDropFiles(wxCoord x, wxCoord y,
                             const wxArrayString& filenames);

private:
    guiChat *m_pOwner;
};

BEGIN_EVENT_TABLE(guiChat, wxFrame)
    EVT_MENU(wxNNIM::ID_CHAT_QUIT, guiChat::OnQuit)
    EVT_MENU(wxNNIM::ID_CHAT_ABOUT, guiChat::OnAbout)
    EVT_CLOSE(guiChat::OnCloseWindow)
    EVT_BUTTON(wxNNIM::ID_CHAT_SEND_BUTT, guiChat::OnSend)
    EVT_BUTTON(wxNNIM::ID_CHAT_P2_FILESEND_BUTT, guiChat::OnFileSend)
    EVT_BUTTON(wxNNIM::ID_CHAT_P2_BROWSE_BUTT, guiChat::OnP2Browse)
    EVT_IDLE(guiChat::OnIdle)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(myTextCtrl, wxTextCtrl)
  EVT_CHAR(myTextCtrl::OnChar)	
  EVT_IDLE(myTextCtrl::OnIdle)
END_EVENT_TABLE()

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
    size_t nFiles = filenames.GetCount();
    wxString str;
    str.Printf( _T("%d files dropped<BR>"), nFiles);
    m_pOwner->DisplayText(str);
    for ( size_t n = 0; n < nFiles; n++ ) {
        m_pOwner->DisplayText(filenames[n] + wxString("<BR>"));
        m_pOwner->m_file_p2->AppendText(filenames[n]);
    }

    return TRUE;
}

guiChat::guiChat(const wxString& title, int x, int y, int w, int h, wxWindow * parent, Contact cont)
       : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(w, h))
{

	m_Contact = cont;

	SetSizeHints(300,200);
	m_window_closing = false;
	m_notebook = NULL;
	m_panel = NULL;
	m_html = NULL;
	m_outText = NULL;
	m_htmlInternalText = "";

	wxIcon appIcon;
	appIcon.CopyFromBitmap(wxBitmap(nnim_i_xpm));

	SetIcon(appIcon);  // Set the icon

	CreateStatusBar(1);

	m_panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxCLIP_CHILDREN);

	m_panel->SetDropTarget(new DnDFile(this));

	wxLayoutConstraints* c = new wxLayoutConstraints;
	c->left.SameAs(this, wxLeft);
	c->right.SameAs(this, wxRight);
	c->top.SameAs(this, wxTop);
	c->bottom.SameAs(this, wxBottom);
	m_panel->SetConstraints(c);

    m_notebook = new wxNotebook(m_panel, wxNNIM::ID_CHAT_NOTEBOOK);

	c = new wxLayoutConstraints;
	c->left.SameAs(m_panel, wxLeft, 1);
	c->right.SameAs(m_panel, wxRight, 1);
	c->top.SameAs(m_panel, wxTop, 1);
	c->bottom.SameAs(m_panel, wxBottom, 1);

	m_notebook->SetConstraints(c);

	InitTabView(m_notebook, m_panel);

	m_panel->SetAutoLayout(TRUE);
	SetAutoLayout(TRUE);

	Centre(wxBOTH);

	Show(TRUE);
	m_html->SetPage(wxString("<HTML><BODY>") + m_htmlInternalText + wxString("</BODY></HTML>"));
	
}

void guiChat::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	new guiAbout(wxT("About NNIM"), wxPoint(20,20), wxDefaultSize, this);

}

void guiChat::OnQuit(wxCommandEvent&)
{
	Close(TRUE);
}

void guiChat::OnCloseWindow(wxCloseEvent& event)
{ 
    Destroy();
}

guiChat::~guiChat()
{
  
//  RemoveChat(this);
  
}

void guiChat::OnSend(wxCommandEvent& WXUNUSED(event))
{

  // called from a menu or button to process outbound text
  if (m_outText == NULL)
  {
    wxLogError(wxT("m_outText is null"));
    return;
  }

  wxString temp = m_outText->GetValue();

  if (temp.IsEmpty() == true)
    return;

  ProtocolManager& MyMan = wxGetApp().AccessManager();

  AuthLoad& MyLoad = wxGetApp().AccessLoader();

  wxString sxToSend = "<HTML><BODY BGCOLOR=\"#ffffff\">";
  sxToSend += temp;
  sxToSend += "</BODY></HTML>";
  sxToSend.Replace(wxT("\n"), wxT("<BR>"));



  //MyMan.sendMessage(m_Contact.protocol(), m_Contact, sxToSend);
  
  // todo: add local formated colors
  temp.Replace(wxT("\n"), wxT("<BR>"));
  temp += wxT("<BR>");
  temp.Prepend(wxString(wxT("<b><FONT COLOR=\"#0000FF\">")) + 
	  wxString(MyLoad.GetActiveLogin().c_str()) + wxString(wxT(": </FONT></b>")));

  m_htmlInternalText.Prepend(temp);
  m_html->SetPage(wxString(wxT("<HTML><BODY>")) + m_htmlInternalText + wxString(wxT("</BODY></HTML>")));


  m_outText->Clear();

}

void guiChat::OnSendInternal(const wxString& text)
{

  // Called from idle event to process outbound text

  wxString temp(text);

  if (temp.IsEmpty() == true)
    return;

  temp.Replace(wxT("\n"), wxT("<BR>"));
  temp += wxT("<BR>");
  m_htmlInternalText.Prepend(temp);
  m_html->SetPage(wxString("<HTML><BODY>") + m_htmlInternalText + wxString("</BODY></HTML>"));

}

void guiChat::InitTabView(wxNotebook* notebook, wxPanel* window)
{
 // Set up tab view and define constraints

  wxLayoutConstraints *c;  

  // Add some panels
  wxPanel *panel1 = new wxPanel(notebook, -1);
  panel1->SetAutoLayout(TRUE);
  panel1->SetBackgroundColour(wxColour("RED"));
  
  m_html = new wxHtmlWindow(panel1, -1);

  c = new wxLayoutConstraints;
  c->left.SameAs(panel1, wxLeft, 4);
  c->right.SameAs(panel1, wxRight, 4);
  c->top.SameAs(panel1, wxTop, 4);
  c->bottom.SameAs(panel1, wxBottom, 60);
  m_html->SetConstraints(c);

  m_outText = new myTextCtrl(panel1, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  m_outText->SetFocus();
  c = new wxLayoutConstraints;
  c->left.SameAs(panel1, wxLeft, 4);
  c->right.SameAs(panel1, wxRight, 60);
  c->top.SameAs(m_html, wxBottom, 4);
  c->bottom.SameAs(panel1, wxBottom, 4);
  m_outText->SetConstraints(c);


  wxBitmapButton * l_butt = new wxBitmapButton(panel1, wxNNIM::ID_CHAT_SEND_BUTT, 
    wxBitmap(nnim_button_trans2_xpm),wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
  l_butt->SetBitmapSelected(wxBitmap(nnim_button_trans_bw2_xpm));

  c = new wxLayoutConstraints;
  c->left.SameAs(m_outText, wxRight, 1);
  c->right.SameAs(panel1, wxRight, 1);
  c->top.SameAs(m_html, wxBottom, 4);
  c->bottom.SameAs(panel1, wxBottom, 4);
  l_butt->SetConstraints(c);

  notebook->AddPage(panel1, "Chat");

  wxPanel *panel2 = new wxPanel(notebook, -1);
  panel2->SetAutoLayout(TRUE);
  //panel2->SetBackgroundColour(wxColour("BLUE"));
/*
  wxStaticText *l_note_p2 = new wxStaticText(panel2, -1, "Note: You may drag a file onto this\nwindow to select it for transfer.", 
    wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

  c = new wxLayoutConstraints;
  c->right.SameAs(panel2, wxRight, 4);
  c->width.AsIs();
  c->top.SameAs(panel2, wxTop, 4);
  c->height.AsIs();
  l_note_p2->SetConstraints(c); 
*/
  wxStaticText *l_label_p2 = new wxStaticText(panel2, -1, wxT("File to send"), 
    wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );

  c = new wxLayoutConstraints;
  c->left.SameAs(panel2, wxLeft, 4);
  c->width.AsIs();
  c->top.SameAs(panel2, wxTop, 4);
  c->height.AsIs();
  l_label_p2->SetConstraints(c); 


  m_file_p2 = new wxTextCtrl(panel2, -1, "", wxDefaultPosition, wxDefaultSize);
 
  c = new wxLayoutConstraints;
  c->width.PercentOf(panel2, wxWidth, 75);
  c->left.SameAs(panel2, wxLeft, 4);
  c->top.SameAs(l_label_p2, wxBottom, 4);
  c->height.AsIs();
  m_file_p2->SetConstraints(c);

  wxButton *l_browse_p2 = new wxButton(panel2, wxNNIM::ID_CHAT_P2_BROWSE_BUTT, wxT("Browse"));

  c = new wxLayoutConstraints;
  c->right.SameAs(panel2, wxRight, 4);
  c->left.SameAs(m_file_p2, wxRight, 4);
  c->top.SameAs(l_label_p2, wxBottom, 3);
  c->height.AsIs();
  l_browse_p2->SetConstraints(c);


  wxButton *l_sendfile_p2 = new wxButton(panel2, wxNNIM::ID_CHAT_P2_FILESEND_BUTT, wxT("Send File"));

  c = new wxLayoutConstraints;
  c->left.SameAs(panel2, wxLeft, 4);
  c->width.AsIs();
  c->top.SameAs(m_file_p2, wxBottom, 5);
  c->height.AsIs();
  l_sendfile_p2->SetConstraints(c);

  wxCheckBox *l_enc_p2 = new wxCheckBox(panel2, -1, wxT("Encrypt"));

  c = new wxLayoutConstraints;
  c->right.SameAs(panel2, wxRight, 4);
  c->width.AsIs();
  c->top.SameAs(m_file_p2, wxBottom, 5);
  c->height.AsIs();
  l_enc_p2->SetConstraints(c);

  wxCheckBox *l_comp_p2 = new wxCheckBox(panel2, -1, wxT("Compress"));

  c = new wxLayoutConstraints;
  c->width.AsIs();
  c->right.SameAs(l_enc_p2, wxLeft, 4);
  c->top.SameAs(m_file_p2, wxBottom, 5);
  c->height.AsIs();
  l_comp_p2->SetConstraints(c);
/*
  wxStaticLine *l_line_p2 = new wxStaticLine(panel2, -1);

  c = new wxLayoutConstraints;
  c->right.SameAs(panel2, wxRight, 4);
  c->left.SameAs(panel2, wxLeft, 4);
  c->top.SameAs(l_sendfile_p2, wxBottom, 8);
  c->height.AsIs();
  l_line_p2->SetConstraints(c);

  wxCheckBox *l_prompt_rec_p2 = new wxCheckBox(panel2, -1, "Prompt before recieving files");

  c = new wxLayoutConstraints;
  c->width.AsIs();
  c->left.SameAs(panel2, wxLeft, 4);
  c->top.SameAs(l_line_p2, wxBottom, 4);
  c->height.AsIs();
  l_prompt_rec_p2->SetConstraints(c);

  wxStaticText *l_label2_p2 = new wxStaticText(panel2, -1, "Directory to recieve files to", 
    wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );

  c = new wxLayoutConstraints;
  c->left.SameAs(panel2, wxLeft, 4);
  c->width.AsIs();
  c->top.SameAs(l_prompt_rec_p2, wxBottom, 4);
  c->height.AsIs();
  l_label2_p2->SetConstraints(c); 

  wxTextCtrl *l_dir_rec_p2 = new wxTextCtrl(panel2, -1, "", wxDefaultPosition, wxDefaultSize);
 
  c = new wxLayoutConstraints;
  c->width.PercentOf(panel2, wxWidth, 75);
  c->left.SameAs(panel2, wxLeft, 4);
  c->top.SameAs(l_label2_p2, wxBottom, 4);
  c->height.AsIs();
  l_dir_rec_p2->SetConstraints(c);

  wxButton *l_browse_dir_p2 = new wxButton(panel2, -1, "Browse");

  c = new wxLayoutConstraints;
  c->right.SameAs(panel2, wxRight, 4);
  c->left.SameAs(l_dir_rec_p2, wxRight, 4);
  c->top.SameAs(l_label2_p2, wxBottom, 3);
  c->height.AsIs();
  l_browse_dir_p2->SetConstraints(c);*/

  wxStaticText *l_label_progress_p2 = new wxStaticText(panel2, -1, wxT("Transfer Progress"), 
    wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );

  c = new wxLayoutConstraints;
  c->left.SameAs(panel2, wxLeft, 4);
  c->width.AsIs();
  c->height.AsIs();
  c->bottom.SameAs(panel2, wxBottom,4);
  l_label_progress_p2->SetConstraints(c); 

  m_transfer_gauge_p2 = new wxGauge(panel2, -1, 30);
  m_transfer_gauge_p2->SetValue(0);

  c = new wxLayoutConstraints;
  c->left.SameAs(l_label_progress_p2, wxRight, 4);
  c->right.SameAs(panel2, wxRight, 4);
  c->height.SameAs(l_label_progress_p2, wxHeight);
  c->top.SameAs(l_label_progress_p2, wxTop, 1);
  m_transfer_gauge_p2->SetConstraints(c); 


  l_sendfile_p2->SetDefault();

  notebook->AddPage(panel2, wxT("File"));

  
  wxPanel *panel3 = new wxPanel(notebook, -1);
  //panel4->SetBackgroundColour(wxColour("WHITE"));
 
   wxStaticBox * l_box1_p3 = new wxStaticBox(panel3, -1, wxT("Network"));

  c = new wxLayoutConstraints;
  c->left.SameAs(panel3, wxLeft, 4);
  c->width.PercentOf(panel3, wxWidth,50);
  c->top.SameAs(panel3, wxTop,4);
  c->bottom.SameAs(panel3, wxBottom, 4);
  l_box1_p3->SetConstraints(c); 

  wxStaticText *l_packin_p3 = new wxStaticText(panel3, -1, wxT("Packets In"), 
    wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );

  c = new wxLayoutConstraints;
  c->left.SameAs(l_box1_p3, wxLeft, 5);
  c->width.AsIs();
  c->top.SameAs(l_box1_p3, wxTop,20);
  c->height.AsIs();
  l_packin_p3->SetConstraints(c); 


  wxStaticText *l_packout_p3 = new wxStaticText(panel3, -1, wxT("Packets Out"), 
    wxDefaultPosition, wxDefaultSize );

  c = new wxLayoutConstraints;
  c->left.SameAs(l_box1_p3, wxLeft, 5);
  c->width.AsIs();
  c->top.SameAs(l_packin_p3, wxBottom,2);
  c->height.AsIs();
  l_packout_p3->SetConstraints(c); 

  wxStaticText *l_net_p3 = new wxStaticText(panel3, -1, wxT("Remote IP"), 
    wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT  );
 
  c = new wxLayoutConstraints;
  c->left.SameAs(l_box1_p3, wxLeft, 5);
  c->width.AsIs();
  c->top.SameAs(l_packout_p3, wxBottom,2);
  c->height.AsIs();
  l_net_p3->SetConstraints(c); 

  wxStaticText *l_rhost_p3 = new wxStaticText(panel3, -1, wxT("Remote Host"), 
    wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT  );

   c = new wxLayoutConstraints;
  c->left.SameAs(l_box1_p3, wxLeft, 5);
  c->width.AsIs();
  c->top.SameAs(l_net_p3, wxBottom,2);
  c->height.AsIs();
  l_rhost_p3->SetConstraints(c); 

  wxStaticText *l_cipher_p3 = new wxStaticText(panel3, -1, wxT("Cipher"), 
    wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT  );

   c = new wxLayoutConstraints;
  c->left.SameAs(l_box1_p3, wxLeft, 5);
  c->width.AsIs();
  c->top.SameAs(l_rhost_p3, wxBottom,2);
  c->height.AsIs();
  l_cipher_p3->SetConstraints(c); 


  panel3->SetAutoLayout(TRUE);
  notebook->AddPage(panel3, wxT("Statistics"));

  notebook->SetSelection(0);

}

void guiChat::DisplayText(const wxString& text)
{/*
#ifdef WIN32

  FlashWindow(GetHWND(), true);
#endif*/
  wxString newText = text, newText2;

  newText.Replace(wxT("<HTML>"), wxT(""));
  newText.Replace(wxT("</HTML>"), wxT(""));
  newText.Replace(wxT("<BODY"), wxT("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\""));

  newText.Replace(wxT("</BODY>"), wxT("</TD></TR></TABLE>"));

  newText2 = newText.BeforeFirst('>');
  newText2 += wxT("><TR><TD><b><FONT COLOR=\"#FF0000\">");
  newText2 += wxString(m_Contact.nick().c_str(), wxConvUTF8);
  newText2 += wxT(": </FONT></b>");
  newText2 += newText.AfterFirst('>');

  wxLogDebug(wxT("Trying to render ") + newText2);


  // Used for incomming text (from remote user)
  m_htmlInternalText.Prepend(newText2);
  
  m_html->SetPage(wxString(wxT("<HTML><BODY>")) + m_htmlInternalText + wxString(wxT("</BODY></HTML>")));

}

void guiChat::OnIdle(wxIdleEvent& event)
{

}

void guiChat::OnFileSend(wxCommandEvent& event)
{

  int value = m_transfer_gauge_p2->GetValue();

  if (value == m_transfer_gauge_p2->GetRange())
  {
     m_transfer_gauge_p2->SetValue(0);
     value = 0;
  }

   m_transfer_gauge_p2->SetValue(++value);

}

void myTextCtrl::OnChar(wxKeyEvent& event)
{
  // Catching the <RETURN> to send the text, <SHIFT><RETURN> inserts new line
    if ( (event.ShiftDown() == false) && (event.GetKeyCode() == WXK_RETURN) )
    {  
        wxCommandEvent myEvent(wxEVT_COMMAND_BUTTON_CLICKED,wxNNIM::ID_CHAT_SEND_BUTT);
      GetParent()->ProcessEvent(myEvent);
    }
    else
    {
       event.Skip();
    }
}

void myTextCtrl::OnIdle(wxIdleEvent& event)
{

}

void guiChat::OnP2Browse(wxCommandEvent& event)
{

  wxString s = wxFileSelector(wxT("Choose a file to send"));
  if (!s.IsEmpty())
  {
   if (m_file_p2)
     m_file_p2->SetValue(s);
  }
}

/*
    -----
    $Log: NChat.cpp,v $
    Revision 1.2  2002/06/19 19:14:43  thementat
    Working towards GCC 3.0.4 compile, many modifications and new automake-1.5 files.

    Revision 1.1  2002/06/19 16:27:18  thementat
    Restructured directories.

    Revision 1.2  2002/06/14 22:02:24  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.1.1.1  2002/06/06 17:21:29  thementat

    Checkin of new sources BETA 2

    Revision 1.11  2002/01/17 20:00:50  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.

    Revision 1.9  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.8  2001/12/12 19:25:25  mentat
    Fixes for gcc 2.95.3 and new makefile to build (for now until I sort out automake).
    Builds in FreeBSD4.4 with gcc 2.95.3 and wxWindows 2.3.2

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/

