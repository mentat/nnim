// --*-c++-*--
/*
    $Id: NLogin.cpp,v 1.3 2002/06/23 14:50:01 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2002  Jesse Lovelace
 
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

#include "wx/image.h"

#include "NMain.h"
#include "NXpms.h"
#include "NLogin.h"
#include "NAbout.h"
#include "NUserWiz.h"

#include "bitmaps/nnim_logo_botch.xpm"
#include "bitmaps/wiz_obsd_fish2.xpm"

#include "authload.h"


using namespace std;

DECLARE_APP(wxNNIM)

wxMenuBar *myLoginMenuBar();
wxSizer *NNIMLogin( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

wxWindow * 
InitLoginView(wxWindow * parent)
{
  guiLogin * pMyLogin = new guiLogin(wxT("NNIM Login"), 20, 20, parent);
  pMyLogin->Show(TRUE);
  pMyLogin->Maximize(FALSE);
  return pMyLogin;

}

BEGIN_EVENT_TABLE(guiLogin, wxFrame)
    EVT_MENU(wxNNIM::ID_LOGIN_MENU_QUIT, guiLogin::OnQuit)
    EVT_MENU(wxNNIM::ID_LOGIN_MENU_ABOUT, guiLogin::OnAbout)
    EVT_MENU(wxNNIM::ID_LOGIN_MENU_NEWUSER, guiLogin::OnNewUser)
    EVT_CLOSE(guiLogin::OnCloseWindow)
    EVT_BUTTON(wxNNIM::ID_LOGIN_LOGIN, guiLogin::OnLogin)
END_EVENT_TABLE()

guiLogin::guiLogin(const wxString& title, int x, int y, wxWindow * parent)
    : wxFrame(parent, -1, title, wxPoint(x,y),wxSize(-1,-1), wxDEFAULT_FRAME_STYLE)
{

  wxIcon appIcon;
  appIcon.CopyFromBitmap(wxBitmap(nnim_i_xpm));

  SetIcon(appIcon);  // Set the icon

  SetMenuBar(myLoginMenuBar());
  
  wxPanel * panel = new wxPanel(this);

  // for some reason this won't show in win98 if not using a wxBitmapButton
  wxStaticBitmap * bit_butt = new wxStaticBitmap(panel, wxNNIM::ID_LOGIN_LOGO, 
    wxBitmap(nnim_logo_botch_xpm), wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
  
  wxSizer * sizer = NNIMLogin(panel, true,  true);
  
  sizer->SetSizeHints(this);
 
  SetSize(sizer->GetMinSize());
  
  m_username = static_cast<wxTextCtrl *> (FindWindow(wxNNIM::ID_LOGIN_USERNAME));
	
  m_password = static_cast<wxTextCtrl *> (FindWindow(wxNNIM::ID_LOGIN_PASSWORD));

/* Generic settings saving via wxWindows config */
#if 0
  SetVendorName("A. S. Logic Systems Co.");
  SetAppName("NNIM"); // not needed, it's the default value

  wxConfigBase *pConfig = wxConfigBase::Get();
#endif
}

void guiLogin::OnCloseWindow(wxCloseEvent& event)
{

	wxGetApp().Shutdown();

}

void guiLogin::OnQuit(wxCommandEvent&)
{
	Close(TRUE);
}

void guiLogin::OnAbout(wxCommandEvent&)
{
	new guiAbout(wxT("About GM: NNIM"), wxPoint(20,20), wxDefaultSize, this);
}

guiLogin::~guiLogin()
{

}

void guiLogin::OnLogin(wxCommandEvent&)
{
	AuthLoad& pMyLoader = wxGetApp().AccessLoader();
	
	wxString l_user(m_username->GetValue());
	wxString l_pass(m_password->GetValue());
	
	SecByteBlock pass((const unsigned char *)l_pass.c_str(), 
		l_pass.length());

	try
	{
		if (pMyLoader.Login(m_username->GetValue().c_str(), pass))
		{
			wxGetApp().Login();
			return;
		}
	}
	catch(gmException &e)
	{

		switch(e.whatType())
		{
		case (gmException::gDISK): 
			wxMessageBox(e.what(), wxT("Disk Error"), wxOK, this); 
			wxLogError(wxString(e.what()));
			return; 
			break;
		case (gmException::gUSER): 
			wxLogMessage(wxString(e.what()));
			break;
		case (gmException::gFATAL): 
			wxMessageBox(e.what(), wxT("Fatal Error"), wxOK, this); 
			wxLogError(wxString(e.what()));
			return; 
			break;
		default: wxLogMessage(wxString(e.what())); return; break;
		}

		return;
	}
	
	int choice = wxMessageBox(wxT("This username was not found, create new user?\n")
        wxT("(it may just be that NNIM cannot find your datafile, goto File->Global Options\n")
        wxT("and set a new home directory if you think this is the case)"), wxT("Create New User?"),  wxYES_NO |  wxICON_QUESTION	, this);
 
	if (choice == wxYES)
	{
		NewUser();
	}
}

void guiLogin::NewUser()
{
  
  wxWizard * wizard = new wxWizard(this, wxNNIM::ID_NEW_USER_WIZARD, wxT("New User Wizard"), wxBitmap(wiz_obsd_fish2_xpm));
  wizard->SetBackgroundColour(*wxWHITE);
  UserWizardP1 * page1= new UserWizardP1(wizard);
  UserWizardP2 * page2 = new UserWizardP2(wizard);
  page1->SetNext(page2);
  page2->SetPrev(page1);

  try
  {
	if (wizard->RunWizard(page1))
	{
		wizard->Destroy();
		wxGetApp().Login(true);
	}
	else
		wizard->Destroy();
  }
  catch(exception &e)
  {
	  wxLogError(wxString(e.what()));
  }

}

void guiLogin::OnNewUser(wxCommandEvent& event)
{

	NewUser();
}

wxSizer *NNIMLogin( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxWindow *item1 = parent->FindWindow( wxNNIM::ID_LOGIN_LOGO );
    wxASSERT( item1 );
    item0->Add( item1, 0, wxALIGN_CENTRE, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 2, 0, 0 );
    item2->AddGrowableCol( 1 );

    wxStaticText *item3 = new wxStaticText( parent, -1, wxT("Username:"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item3, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item4 = new wxTextCtrl( parent, wxNNIM::ID_LOGIN_USERNAME, "", wxDefaultPosition, wxSize(100,-1), 0 );
    item2->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, -1, wxT("Password:"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item5, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxTextCtrl *item6 = new wxTextCtrl( parent, wxNNIM::ID_LOGIN_PASSWORD, "", wxDefaultPosition, wxSize(100,-1), wxTE_PASSWORD );
    item2->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item2, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    wxButton *item7 = new wxButton( parent, wxNNIM::ID_LOGIN_LOGIN, wxT("Login"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->SetDefault();
    item0->Add( item7, 0, wxALIGN_CENTRE|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

wxMenuBar *myLoginMenuBar()
{
    wxMenuBar *item0 = new wxMenuBar;
    
    wxMenu* item1 = new wxMenu;
    item1->Append( wxNNIM::ID_LOGIN_MENU_NEWUSER, wxT("New User Wizard\tCtrl-n"), wxT("Run New User Wizard" ));
    item1->AppendSeparator();
    item1->Append( wxNNIM::ID_LOGIN_MENU_QUIT, wxT("Quit\tCtrl-x"), "" );
    item0->Append( item1, wxT("File"));
    
    wxMenu* item2 = new wxMenu;
    item2->Append( wxNNIM::ID_LOGIN_MENU_ABOUT, wxT("About NNIM\tCtrl-a"), wxT("About NNIM/KiT") );
    item0->Append( item2, wxT("Help") );
    
    return item0;
}
/*
    -----
    $Log: NLogin.cpp,v $
    Revision 1.3  2002/06/23 14:50:01  thementat
    Work on TOC protocol and new buffer class.

    Revision 1.2  2002/06/20 01:25:00  thementat
    Removed unicode for the time being to fix linux build.

    Revision 1.1  2002/06/19 16:27:18  thementat
    Restructured directories.

    Revision 1.2  2002/06/14 22:02:24  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.1.1.1  2002/06/06 17:21:30  thementat
    Checkin of new sources BETA 2

    Revision 1.10  2002/01/17 20:00:51  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.

    Revision 1.8  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/

