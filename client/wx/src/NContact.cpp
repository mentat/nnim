// --*-c++-*--
/*
    $Id: NContact.cpp,v 1.1 2002/06/19 16:27:18 thementat Exp $
 
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
#include "NXpms.h"

#include "wx/notebook.h"
#include "wx/treebase.h"
#include "wx/generic/treectlg.h"
#include "wx/wizard.h"

#include "NEvent.h"
#include "NContactTree.h"
#include "NContact.h"
#include "NPrefs.h"
#include "NChat.h"
#include "NDerivedClasses.h"
#include "NAbout.h"
#include "NMain.h"
#include "NProtocolManager.h"
#include "NContactWiz.h"
#include "NContactTreeXML.h"

#include "contact.h"
#include "authload.h"
#include "protocol.h"

#include "bitmaps/wiz_obsd_fish2.xpm"

wxMenuBar *myContactsMenuBar();
wxSizer *Contacts( wxWindow *parent, bool call_fit, bool set_sizer , XMLNode node);

DECLARE_APP(wxNNIM)
/*
bool RemoveChat(wxWindow * window)
{
  for(map<string, guiChat *>::iterator it=gChatWindows.begin(); it != gChatWindows.end(); it++)
    if ((wxWindow *)it->second == window)
    {
      gChatWindows.erase(it->first);
      return true;
    }

  return false;
}

bool AddChat(const string& name, guiChat * window)
{
  for(map<string, guiChat *>::iterator it=gChatWindows.begin(); it != gChatWindows.end(); it++)
    if (it->first == name)
      return false;

  gChatWindows[name] = window;
  return true;

}*/

wxWindow *
InitContactView(wxWindow * pParent, bool newUser)
{
  /* Get window placement/size prefs here */
  
  guiContact * pMyContact = new guiContact(newUser, pParent, -1, wxT("GM: NNIM"), wxPoint(100, 100), wxSize(200, 400));
  pMyContact->Show(TRUE);
  return pMyContact;
}

void ResetContactTree()
{
//  if (!gTreeView)
  //  return;
  
 // gTreeView->Rebuild();

}

BEGIN_EVENT_TABLE(guiContact, wxFrame)
	EVT_IDLE(guiContact::OnIdle)
	EVT_CLOSE(guiContact::OnCloseWindow)
	EVT_CUSTOM(gmEVT_MESSAGE ,-1, guiContact::OnGetMessage)
	EVT_CUSTOM(gmEVT_STATUS_CHANGE, -1, guiContact::OnChangeStatus)
//	EVT_CUSTOM(gmEVT_LIST_ADD, -1, guiContact::OnListAdd)
    EVT_BUTTON(wxNNIM::ID_CONTACTS_BUTT, guiContact::OnStart)
	// frame menu items
    EVT_MENU(wxNNIM::ID_CONTACTS_ABOUT, guiContact::OnAbout)
	EVT_MENU(wxNNIM::ID_CONTACTS_QUIT, guiContact::OnQuit)
	EVT_MENU(wxNNIM::ID_CONTACTS_MENU_AWAY, guiContact::OnSetAway)
	EVT_MENU(wxNNIM::ID_CONTACTS_MENU_SET_INFO, guiContact::OnSetInfo)
	EVT_MENU(wxNNIM::ID_CONTACTS_MENU_LOGOUT, guiContact::OnLogout)
END_EVENT_TABLE()

guiContact::guiContact(bool newUser, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, 
                       const wxSize& size, long style, const wxString& name)
       : wxFrame(parent, id, title, pos, size, style, name)
{

  wxIcon appIcon;
  appIcon.CopyFromBitmap(wxBitmap(nnim_i_xpm));
  SetIcon(appIcon);  

  SetMenuBar(myContactsMenuBar());
 
  wxPanel * pTopPanel = new wxPanel(this);

  Contacts(pTopPanel, true, true, wxGetApp().AccessLoader().C().GetConfig());

}

guiContact::~guiContact()
{

}

void guiContact::OnIdle(wxIdleEvent& event)
{
 
}


void guiContact::OnGetMessage(gmEvent& event)
{
 /* wxLogDebug(wxString(wxT("Got event: ")) + event.getProtocol());
  
  wxString newSearch = event.getServerId().c_str();
  newSearch += ':';
  newSearch += event.getProtocol().c_str();
  newSearch = newSearch.Lower();

  wxLogDebug(wxString(wxT("Looking for window: ")) + newSearch);

  for(map<string, guiChat *>::iterator it=gChatWindows.begin(); it != gChatWindows.end(); it++)
  {
    if (it->first == newSearch.c_str())
    {
      it->second->DisplayText(event.getMessage().c_str());
      return;
    }
  }

  // no window with that name open.

  guiChat * myChat = LaunchChat(event.getServerId().c_str(), event.getProtocol().c_str());

  if (myChat)
    myChat->DisplayText(event.getMessage().c_str());
  else
    wxLogDebug(wxT("Oh shit, myChat is NULL in OnGetMessage"));
*/
}

void guiContact::OnChangeStatus(gmEvent& event)
{
  int treeStatus;
  switch(event.contact.status())
  {
  case (Contact::Online):
    treeStatus = STATUS_ONLINE;
    break;
  case (Contact::Offline):
    treeStatus = STATUS_OFFLINE;
    break;
  case (Contact::Away):
    treeStatus = STATUS_AWAY;
    break;
  case (Contact::Dnd):
    treeStatus = STATUS_DND;
    break;
  case (Contact::Na):
    treeStatus = STATUS_NA;
    break;
  case (Contact::Ffc):
    treeStatus = STATUS_FFC;
    break;
  case (Contact::Custom):
    treeStatus = STATUS_CUSTOM;
    break;
  }
//  gTreeView->SetItemTreeStatus(event.contact.nick().c_str(), treeStatus);

}


void guiContact::OnLogout(wxCommandEvent& event)
{
    wxLogDebug(wxT("OnLogout"));

    wxGetApp().Logout();
}

void guiContact::OnCheckVersion(wxCommandEvent& event)
{
  wxLogDebug(wxT("OnCheckVersion"));
}

void guiContact::OnQuit(wxCommandEvent& event)
{
    wxLogDebug(wxT("OnQuit"));
    Close(TRUE);
}

void guiContact::OnCloseWindow(wxCloseEvent& event)
{
	wxGetApp().AccessLoader().CommitToFile();
	wxGetApp().Shutdown();

}

void guiContact::OnAbout(wxCommandEvent& event)
{
    new guiAbout(wxT("About GM: NNIM"), wxPoint(20,20), wxDefaultSize, this);
}

void guiContact::OnStart(wxCommandEvent& event)
{
    wxLogDebug(wxT("OnStart"));
    wxMenu popUp;

    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_CONNECT, wxT("Connect"), wxT("Go online!"));
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_UPDATE_DEBUG, wxT("Update"));
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_DISCONNECT, wxT("Disconnect"), wxT("Go offline."));
    popUp.AppendSeparator();
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_ADD_CONTACT, wxT("Add Contact"));
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_ADD_FOLDER, wxT("Add Folder"));
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_PREFS, wxT("Preferences"));
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_TESTCRASH, wxT("Test Crash"));
    popUp.AppendSeparator();
    popUp.Append(wxNNIM::ID_CONTACTS_MENU_POPUP_QUIT, wxT("Quit"));

    wxSize size = GetClientSize();
    wxSize l_size = wxButton::GetDefaultSize();

    PopupMenu(&popUp, wxPoint(l_size.GetWidth()/2,(size.y - (l_size.GetHeight()))));

}

void guiContact::OnSetAway(wxCommandEvent& event)
{
/*
  ProtocolManager* pMyManager = AccessManager();
  if (!pMyManager)
  {
    wxLogError("guiContact::OnSetAway: pMyManager is NULL!");
    return;
  }

  wxString sxAwayText = wxGetTextFromUser("Away Message", "Away Message", "", this);
  pMyManager->setAllAway(sxAwayText.c_str());

  if (sxAwayText != "")
  {
    m_away = true;
    wxDialog * myAway = new wxDialog( this, ID_AWAY_WINDOW, "Away", wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER );
    AwayScreen((wxPanel *)myAway, TRUE);    
    
    myAway->CentreOnParent();
    myAway->SetEventHandler(this);
    myAway->Show(TRUE);

  }
  else
  {
    m_away = false;
  }*/

/*
  sxAwayText.Prepend("<HTML><BODY BGCOLOR=\"#ffffff\">");
  sxAwayText += "</BODY></HTML>";
*/

}

void guiContact::OnSetInfo(wxCommandEvent& event)
{
  /*
  ProtocolManager* pMyManager = AccessManager();
  if (!pMyManager)
  {
    wxLogError("guiContact::OnSetAway: pMyManager is NULL!");
    return;
  }

  wxString sxInfoText = wxGetTextFromUser("Info Message", "Info Message", "", this);

  pMyManager->setAllInfo(sxInfoText.c_str());
*/
}

/*void guiContact::OnRename(wxCommandEvent& event)
{

  wxTreeItemId id = gTreeView->GetSelection();
  DoRename(id);

}*/

/*void guiContact::OnPopUpProcess(wxCommandEvent& event)
{
  switch (event.GetId())
  {
  case ID_CONTACTS_MENU_POPUP_PREFS:
    wxLogDebug("Menu->Popup->Prefs");
    InitPrefView(this);
    break;
  case ID_CONTACTS_MENU_POPUP_QUIT:
    Close(TRUE);
    break;
  case ID_CONTACTS_MENU_POPUP_TESTCRASH:
    wxGetApp().SetError();
    Close(TRUE);
    break;
  default:
    wxLogDebug(wxT("Unknown ID passed to OnPopUpProcess"));
    break;
  }

}*/
/*
guiChat * guiContact::LaunchChat(const wxString& name, const wxString& protocol)
{
  guiChat * pMyChatWindow = NULL;
  
  Contact c;
  wxString NewNameForMap;
  
  AuthLoad* pMyLoader = AccessLoader();
  
  if (!pMyLoader)
    return NULL;

  ProtocolManager* pMyManager = AccessManager();
  if (!pMyManager)
  {
    wxLogError("guiContact::OnSetAway: pMyManager is NULL!");
    return NULL;
  }
*/
  /* check which protocol is most priority and logged in, 
     set window to that protocol */
/*
  if ((protocol == "") && pMyLoader->UserExists(name.c_str()))
  {
    map<string, string> validUserNets;
    pMyLoader->GetNets(name.c_str(), validUserNets);
    if (validUserNets.empty())
    {
      wxMessageBox("This contact has no networks associated with it, please configure a network.");
      return NULL;
    }

    // Ok, we've found this contact, now we get its configured nets
    pMyLoader->GetNets(name.c_str(), validUserNets);

    map<string, string>::const_iterator it;
  
    bool suiteableFound = false;

    // we run through the nets and see if any are online, if so, we use it
    for (it=validUserNets.begin(); it!=validUserNets.end();it++)
    {
      wxLogDebug("Trying ----");
      wxLogDebug(it->first.c_str());
      wxLogDebug(wxString("status ") + WXITOA((long int)pMyManager->getState(it->first)));
      
      if (pMyManager->getState(it->first) == Protocol::S_online)
      {
        wxLogDebug(wxString("Connecting to user via ") + wxString(it->first.c_str()));
        
        c.setProtocol(it->first);
        c.setServerId(it->second);
        c.setNick(name.c_str());

        pMyChatWindow = InitChatView(this, name + wxString(" (") + wxString(it->second.c_str()) + wxString(')'), c);
         
        NewNameForMap = wxString(it->second.c_str()) + wxString(':') + wxString(it->first.c_str());
        NewNameForMap = NewNameForMap.Lower();
        suiteableFound = true;
      }
      else
      {
        wxLogDebug("Protocol not suitable.");
      }

      // if we have an ok network, get out of this damn loop
      if (suiteableFound)
        break;
    }

    if (!suiteableFound)
    {
      wxLogMessage("No suitable network found to communicate with contact over.");
      return NULL;
    }
    
  }
  else
  {
    // otherwise message is comming in, see if our boy in listed in authload under the protocol//
    if (protocol == "")
    {
      // whoops, no protocol, so we cant find him/her
      wxLogDebug("protocol is \"\" in LaunchChat");
      return NULL;
    }

    // trying to find contact's real name association by searching
    wxString whichRealName = pMyLoader->ServerIdExists(name.Lower().c_str(), protocol.c_str()).c_str();

    if (whichRealName == "")
    {
      // we didnt find it, ask if user wants to accept strange contact
      int choice = wxMessageBox(name + wxString(" on protocol ") + protocol + wxString(" wants to send you a message.  Do you accept?"), "Accept message?", wxYES_NO, this);
      if (choice == wxYES)
      {
        c.setProtocol(protocol.c_str());
        c.setServerId(name.c_str());
        c.setNick(name.c_str());
        pMyChatWindow = InitChatView(this, name + wxString(": ") + protocol, c);
        NewNameForMap = name + wxString(':') + protocol;
        NewNameForMap = NewNameForMap.Lower();
      }
      else // if not, return 
        return NULL;
    }
    else
    {
      // real name found, set it to window title, ext, so we arent confused
      c.setProtocol(protocol.c_str());
      c.setServerId(name.c_str());
      c.setNick(whichRealName.c_str());

      pMyChatWindow = InitChatView(this, whichRealName + wxString(" (") + name + wxString(')'), c);
      NewNameForMap = name + wxString(':') + protocol;
      NewNameForMap = NewNameForMap.Lower();
    }

  }

  //guiChat * pMyChatWindow = InitChatView(this, name, NULL);

  if (!pMyChatWindow)
    return NULL;

  wxLogDebug(wxString("New window called ") + NewNameForMap);
  if (!AddChat(NewNameForMap.c_str(), pMyChatWindow))
  {
    wxLogMessage("Can't add chat window, maybe right one open. Deleting new.");
    pMyChatWindow->Destroy();
    return NULL;
  }

  return pMyChatWindow;


return NULL;
}*/


wxSizer *Contacts( wxWindow *parent, bool call_fit, bool set_sizer, XMLNode node )
{
    wxFlexGridSizer *item0 = new wxFlexGridSizer( 1, 0, 0 );
    item0->AddGrowableCol( 0 );
    item0->AddGrowableRow( 0 );

    NContactTreeXML *item1 = new NContactTreeXML( parent, wxNNIM::ID_TREE_CONTROL, wxDefaultPosition, wxSize(120,160), wxTR_HAS_BUTTONS|wxSUNKEN_BORDER, node );
    item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxBoxSizer *item2 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item3 = new wxButton( parent, wxNNIM::ID_CONTACTS_BUTT, wxT("NNIM"), wxDefaultPosition);
    wxASSERT( item3 );
    item2->Add( item3, 0, wxALIGN_CENTRE, 5 );

    item0->Add( item2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

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
wxMenuBar *myContactsMenuBar()
{
    wxMenuBar *item0 = new wxMenuBar;
    
    wxMenu* item1 = new wxMenu;
    item1->Append( wxNNIM::ID_CONTACTS_MENU_AWAY, wxT("Away Message"), wxT("Sets an away message for all networks.") );
    item1->Append( wxNNIM::ID_CONTACTS_MENU_SET_INFO, wxT("Set Info"), wxT("Sets permanent info such as AOL directory information.") );
    item1->AppendSeparator();
    item1->Append( wxNNIM::ID_CONTACTS_LOGOUT, wxT("Logout"), wxT("Logs you off of all of your networks.") );
    item1->Append( wxNNIM::ID_CONTACTS_DIRECT, wxT("Direct connect"), "" );
    item1->AppendSeparator();
    item1->Append( wxNNIM::ID_CONTACTS_COMMIT, wxT("Commit"), wxT("(For debug use) Commits configuration file to disk.") );
    item1->Append( wxNNIM::ID_CONTACTS_RESET_TREE, wxT("Reset Tree"), wxT("(For debug use) Resets contact tree.") );
    item1->AppendSeparator();
    item1->Append( wxNNIM::ID_CONTACTS_QUIT, wxT("Quit"), wxT("Exits NNIM") );
    item0->Append( item1, wxT("NNIM") );
    
    wxMenu* item2 = new wxMenu;
    item2->Append( wxNNIM::ID_CONTACTS_ABOUT, wxT("About NNIM"), "" );
    item0->Append( item2, wxT("Help") );
    
    return item0;
}
/*
   -----
    $Log: NContact.cpp,v $
    Revision 1.1  2002/06/19 16:27:18  thementat
    Restructured directories.

    Revision 1.3  2002/06/16 04:08:28  thementat
    Hopefully fixed Authload and related classes.

    Revision 1.2  2002/06/14 22:02:24  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.1.1.1  2002/06/06 17:21:30  thementat
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
