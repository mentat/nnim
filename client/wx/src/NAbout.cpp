// --*-c++-*--
/*
    $Id: NAbout.cpp,v 1.1 2002/06/19 16:27:18 thementat Exp $
 
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
#include "NAbout.h"
//#include "NAutoTxt.h"

#include "wx/image.h"

#include "bitmaps/nnim_logo.xpm"

#define ID_CONTACTS_BITMAP_SMALL_LOGO 10030

wxSizer *About( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

guiAbout::guiAbout(const wxString& title, const wxPoint& pos, const wxSize& size, wxWindow * parent)
  : wxFrame(parent, -1, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU |  wxCAPTION)
{
	wxIcon appIcon;
	appIcon.CopyFromBitmap(wxBitmap(nnim_i_xpm));

	SetIcon(appIcon);  // Set the icon
  
	wxPanel * panel = new wxPanel(this);
	panel->SetBackgroundColour(*wxWHITE);

	//wxAutoTextCtrl * tester = new wxAutoTextCtrl(this);

	wxStaticBitmap * bit_butt = new wxStaticBitmap(panel, ID_CONTACTS_BITMAP_SMALL_LOGO, 
		wxBitmap(nnim_logo_xpm), wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
  
	wxSizer * sizer = About(panel, true,  true);
  
	sizer->SetSizeHints(this);
 
	SetSize(sizer->GetMinSize());
	Show(TRUE);

}

wxSizer *About( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 2, 0, 0 );
    item0->AddGrowableCol( 1 );

    wxWindow *item1 = parent->FindWindow( ID_CONTACTS_BITMAP_SMALL_LOGO );
    wxASSERT( item1 );
    item0->Add( item1, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

    wxFlexGridSizer *item2 = new wxFlexGridSizer( 1, 0, 0 );

    wxStaticText *item3 = new wxStaticText( parent, -1, wxT("GM: NNIM"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->SetForegroundColour( *wxBLUE );
    item3->SetFont( wxFont( 17, wxSWISS, wxNORMAL, wxNORMAL ) );
#if defined(__WXMSW__) && !(wxCHECK_VERSION(2,3,0))
    item3->SetSize( item3->GetBestSize() );
#endif
    item2->Add( item3, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxString txt;
    txt.Printf(wxT("A wxWindows client for the\nGNU Messenger system.\n%s"),NNIM_GUI_VER);
    wxStaticText *item4 = new wxStaticText( parent, -1, txt,
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    item2->Add( item4, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, -1, 
        wxT("Copyright (c) 2002 Jesse Lovelace, GPL.") wxT("\n") wxT("Portions Copyright (c) 1995-2001 by Wei Dai.\nAll rights reserved.")
        wxT("\n\nThe blowfish graphic is part of the\nOpenBSD project and is under their copyright."),
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    item2->Add( item5, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item2, 0, wxALIGN_CENTRE|wxALL, 5 );

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
/*

    -----
    $Log: NAbout.cpp,v $
    Revision 1.1  2002/06/19 16:27:18  thementat
    Restructured directories.

    Revision 1.2  2002/06/16 05:14:22  thementat
    Updated about screen.

    Revision 1.1.1.1  2002/06/06 17:21:29  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2002/01/17 20:00:50  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.


*/
