// --*-c++-*--
/*
    $Id: NPrefs.cpp,v 1.1 2002/06/19 16:27:18 thementat Exp $
 
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
*/


#include "NInclude.h"

#include "wx/splitter.h"
#include "wx/scrolwin.h"
#include "wx/treebase.h"
#include "wx/generic/treectlg.h"

#include "authload.h"

#include "NXpms.h"
#include "NWizards_wdr.h"

#include "NPrefs.h"
#include "NContact.h"
#include "NMain.h"
#include "NDerivedClasses.h"

#include "NPrefGen.h"
#include "NPrefNet.h"
#include "NPrefSec.h"

static bool gPrefsOpen = false;

void 
InitPrefView(wxWindow * parent, bool newUser)
{
  if (!gPrefsOpen)
    new guiPrefs("Preferences - General", 20, 20, 500, 300, parent, newUser);

  gPrefsOpen = true;

}

void PlaceFoldersInTree(wxGenericTreeCtrl * tree, wxTreeItemId &base, const string& remote_base);

wxSizer *Preferences( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );
wxSizer *PrefTree( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

class MySplitterWindow : public wxSplitterWindow
{
public:
  MySplitterWindow(wxWindow* parent, wxWindowID id, const wxPoint& point = wxDefaultPosition, 
    const wxSize& size = wxDefaultSize, long style=wxSP_3D, const wxString& name = "splitterWindow");

  virtual bool OnSashPositionChange(int newSashPosition);
};

BEGIN_EVENT_TABLE(guiPrefs, wxFrame)
  EVT_TREE_SEL_CHANGED(wxNNIM::ID_PREF_TREE_CTRL, guiPrefs::OnTreeChange)
  EVT_BUTTON(wxNNIM::ID_PREF_BUTT_CANCEL, guiPrefs::OnCancel)
  EVT_BUTTON(wxNNIM::ID_PREF_BUTT_APPLY, guiPrefs::OnApply)
  EVT_BUTTON(wxNNIM::ID_PREF_BUTT_OK, guiPrefs::OnOK) 
  EVT_CLOSE(guiPrefs::OnCloseWindow)
END_EVENT_TABLE()


guiPrefs::guiPrefs(const wxString& title, int x, int y, int w, int h, wxWindow* parent, bool newUser)
    : wxFrame(parent, -1, title, wxPoint(x,y), wxSize(w,h),  wxDEFAULT_FRAME_STYLE	),
    m_apply(NULL), m_treeCtrl(NULL), m_aChange(false)
{
  // set important values: title and callback parent

  m_parent = parent;
  m_newUser = newUser;
  m_loadDone = false;

  // set the icon
 
  MakeModal(TRUE);

  wxIcon appIcon;
  appIcon.CopyFromBitmap(wxBitmap(nnim_i_xpm));
  SetIcon(appIcon);
  
  // create initial panels
  wxPanel * panel = new wxPanel(this);
  wxSizer * sizer = Preferences(panel, true, true);

  // Find splitter window by id
  m_splitterWindow = (MySplitterWindow *)FindWindow( wxNNIM::ID_PREF_SPLIT );

  // create new panels for splitter window
  wxPanel * myLeft = new wxPanel(m_splitterWindow);

  m_prefNet = new NPrefNet(m_splitterWindow,wxPoint(m_splitterWindow->GetSize().GetWidth() - 5, 5));
  m_prefGen = new NPrefGen(m_splitterWindow,wxPoint(m_splitterWindow->GetSize().GetWidth() - 5, 5));
  m_prefSec = new NPrefSec(m_splitterWindow,wxPoint(m_splitterWindow->GetSize().GetWidth() - 5, 5));

  // assign the panels
  m_splitterWindow->SplitVertically(myLeft, m_prefNet, 120);
  
  PrefTree(myLeft, true, true);

  // find controls
  m_apply = (wxButton *)FindWindow( wxNNIM::ID_PREF_BUTT_APPLY );
  
  m_treeCtrl = (wxGenericTreeCtrl *)FindWindow( wxNNIM::ID_PREF_TREE_CTRL );
  m_treeRoot = m_treeCtrl->AddRoot("root");
  m_treeCtrl->SetWindowStyle(wxTR_FULL_ROW_HIGHLIGHT | wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_HIDE_ROOT);

  // load settings from AuthLoad
  LoadSettings();

  sizer->SetSizeHints(this);
  SetSize(w,h);
  m_splitterWindow->SetSashPosition(120);

  // initially disable save button
  ActivateSave(false);
  Show(TRUE);
  Maximize(FALSE);
  
}

guiPrefs::~guiPrefs()
{
  MakeModal(FALSE);
  gPrefsOpen = false;

}

wxGenericTreeCtrl& guiPrefs::GetTree()
{
  return *m_treeCtrl;
}

void guiPrefs::LoadSettings()
{

  wxTreeItemId genId = m_treeCtrl->AppendItem(m_treeRoot, "General", -1, -1, new TreeData(wxNNIM::PREF_TREEITEM_GENERAL));

  m_treeCtrl->AppendItem(m_treeRoot, "Security", -1, -1, new TreeData(wxNNIM::PREF_TREEITEM_SECURITY));
  m_treeCtrl->AppendItem(m_treeRoot, "Networks", -1, -1, new TreeData(wxNNIM::PREF_TREEITEM_NETWORKROOT));
  
  m_treeCtrl->AppendItem(m_treeRoot, "Plug-ins", -1, -1, new TreeData(wxNNIM::PREF_TREEITEM_PLUGINS));

  m_treeCtrl->SelectItem(genId);
}

void guiPrefs::OnOK(wxCommandEvent &event)
{

  SaveSettings();
  ActivateSave(false);
  Close(FALSE);

}

void guiPrefs::OnTreeChange(wxTreeEvent& event)
{

  wxTreeItemId id = event.GetItem();
  wxTreeItemId oldId = event.GetOldItem();

  if ( (!m_splitterWindow) || (!m_treeCtrl->GetItemData(id)) || (id == oldId))
    return;

  int TYPE = ((TreeData *)m_treeCtrl->GetItemData(id))->GetID();

  wxScrolledWindow  * CURRENT = (wxScrolledWindow  *)m_splitterWindow->GetWindow2();

  wxASSERT(CURRENT);

  CURRENT->Show(FALSE);

  switch ( TYPE )
  {
  case wxNNIM::PREF_TREEITEM_NETWORKROOT:
    m_splitterWindow->ReplaceWindow(CURRENT, m_prefNet); m_prefNet->Show(TRUE);
    break;
  case wxNNIM::PREF_TREEITEM_SECURITY:
    m_splitterWindow->ReplaceWindow(CURRENT, m_prefSec); m_prefSec->Show(TRUE);
    break;
  case wxNNIM::PREF_TREEITEM_PLUGINS:
    m_splitterWindow->ReplaceWindow(CURRENT, m_prefGen); m_prefGen->Show(TRUE);
    break;
  default: 
    m_splitterWindow->ReplaceWindow(CURRENT, m_prefGen); m_prefGen->Show(TRUE);
    break;
  }

}

void guiPrefs::OnCancel(wxCommandEvent& event)
{
  // clear these or program will try to save changes.

  Close(TRUE);
}

void guiPrefs::OnApply(wxCommandEvent& event)
{

  SaveSettings();
  ActivateSave(false);

}

void guiPrefs::OnCloseWindow(wxCloseEvent& event)
{
  Destroy();
}

void guiPrefs::OnQuit(wxCommandEvent& event)
{
	Close(TRUE);
}

void guiPrefs::SaveSettings()
{

  m_prefNet->SaveSettings();
  m_prefGen->SaveSettings();
  m_prefSec->SaveSettings();

}

void guiPrefs::ActivateSave(bool activate)
{
  if (activate)
  {
    m_apply->Enable(TRUE);
    m_aChange = true;
  }
  else
  {
    m_apply->Enable(FALSE);
    m_aChange = false;

  }

}

MySplitterWindow::MySplitterWindow(wxWindow* parent, wxWindowID id, const wxPoint& point, const wxSize& size, long style, const wxString& name)
  : wxSplitterWindow(parent, id, point, size, style, name) 
{}

bool MySplitterWindow::OnSashPositionChange(int newSashPosition)
{
  guiPrefs * owner = (guiPrefs *)GetParent();

  if (!owner)
  {
    wxLogDebug(wxT("OnSashPositionChange: No owner"));
    return false;
  }

  wxPanel * pan = (wxPanel *)GetWindow2();

  if (!pan)
  {
    wxLogDebug(wxT("OnSashPositionChange: No Panel"));
    return false;
  }

  wxSizer * size = pan->GetSizer();

  if (!size)
  {
    wxLogDebug(wxT("OnSashPositionChange: No Sizer"));
    return false;
  }

  if (size->GetMinSize().GetWidth() > (GetSize().GetWidth() - newSashPosition) )
  {
    wxLogDebug(wxT("OnSashPositionChange: Formula rejects!"));
    return false;
  }

  if (newSashPosition < 20)
  {
    wxLogDebug(wxT("OnSashPositionChange: Less than 20!"));
    return false;
  }

  return true;
}


wxSizer *Preferences( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 0 );

    wxSplitterWindow *item1 = new MySplitterWindow( parent, wxNNIM::ID_PREF_SPLIT, wxDefaultPosition, wxSize(260,160), wxSP_3D|wxCLIP_CHILDREN );
    item1->SetMinimumPaneSize( 20 );
    item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer *item2 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item3 = new wxButton( parent, wxNNIM::ID_PREF_BUTT_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->SetDefault();
    item3->Enable( FALSE );
    item2->Add( item3, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item4 = new wxButton( parent, wxNNIM::ID_PREF_BUTT_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item2->Add( item4, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item5 = new wxButton( parent, wxNNIM::ID_PREF_BUTT_APPLY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0);
    item2->Add( item5, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item2, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

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

wxSizer *PrefTree( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 0 );

    wxGenericTreeCtrl *item1 = new wxGenericTreeCtrl( parent, wxNNIM::ID_PREF_TREE_CTRL, wxDefaultPosition, wxSize(120,160), wxSUNKEN_BORDER );
    item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 0 );

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
    $Log: NPrefs.cpp,v $
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

    Revision 1.7  2001/12/12 19:25:25  mentat
    Fixes for gcc 2.95.3 and new makefile to build (for now until I sort out automake).
    Builds in FreeBSD4.4 with gcc 2.95.3 and wxWindows 2.3.2

    Revision 1.6  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.


*/
