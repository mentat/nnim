// --*-c++-*--
/*
    $Id: NContactEdit.cpp,v 1.1 2002/06/06 17:21:29 thementat Exp $
 
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
    $Log: NContactEdit.cpp,v $
    Revision 1.1  2002/06/06 17:21:29  thementat
    Initial revision

 */

#include "NInclude.h"
#include "NMain.h"

#include "wx/treebase.h"
#include "wx/generic/treectlg.h"

#include "NContactEdit.h"

class MyTreeItemData : public wxTreeItemData
{
public:
  MyTreeItemData(const wxString& desc) : m_desc(desc) { }

  void ShowInfo(wxTreeCtrl *tree);

  const wxChar *GetDesc() const { return m_desc.c_str(); }
  int GetStatus() const { return m_status; }
  void SetDesc(const wxString& text) { m_desc = text; }
  void SetStatus(int status) { m_status = status; }

private:
  int m_status;
  wxString m_desc;
};

void guiPrefs::ViewContact(wxTreeItemId& id)
{
  m_loadDone = false;
  bool localChange = m_aChange;
  AuthLoad* pMyLoader = wxGetApp().AccessLoader();
  
  if (!pMyLoader)
    return;
/*
  wxTextCtrl * pName = (wxTextCtrl *)FindWindow( ID_CON_EDIT_NAME );
  
  wxComboBox * pInfoChoice = (wxComboBox *)FindWindow( ID_CON_EDIT_INFO_LIST );
  
  wxTextCtrl * pInfoText = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_INFO_TEXT ),
    * pNetScreenName = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_NETS_SCREEN_NAME );
  
  wxListBox * pUserNets = (wxListBox *)FindWindow ( ID_CON_EDIT_NETS_USER ),
    * pAllNets = (wxListBox *)FindWindow ( ID_CON_EDIT_NETS_ALL );
  
  wxChoice * pPrefChoice = (wxChoice *)FindWindow ( ID_CON_EDIT_NET_PREF );
*/

  if (!id.IsOk())
  {
    wxLogDebug("id is invalid in void guiPrefs::ViewContact()");
    return;
  }

  wxString l_name = GetTree().GetItemText(id);
  m_sxName = l_name;

  if (!pMyLoader->UserExists(l_name.c_str()) )
  {
    wxLogDebug("User does NOT exist in void guiPrefs::ViewContact()");
    return;
  }

  vector<string> all_nets;

  if (!pMyLoader->GetAvailableNets(all_nets))
  {
    wxLogDebug("GetAvailableNets() returns false in void guiPrefs::ViewContact()");
    return;
  }

  if (!pMyLoader->GetInfo(l_name.c_str(), m_itemsStorage))
  {
    wxLogDebug("GetInfo returned false in void guiPrefs::ViewContact()");
    return;
  }

  if (!pMyLoader->GetNets(l_name.c_str(), m_netsStorage))
  {
    wxLogDebug("GetNets returned false in void guiPrefs::ViewContact()");
    return;
  }

#ifdef __WXDEBUG__

  if (!pName || !pInfoChoice || !pInfoText || !pUserNets || !pAllNets || !pNetScreenName || !pPrefChoice )
  {
    wxLogDebug("A pointer from FindWindow is NULL!");
    return;
  }

#endif
  
  pName->SetValue(l_name);
  map<string, string>::const_iterator it;
  
  pInfoChoice->Clear();

  for (it=m_itemsStorage.begin(); it!=m_itemsStorage.end();it++)
  {
    pInfoChoice->Append(it->first.c_str());
  }

  if (pInfoChoice->GetCount() > 0)
  {
    pInfoText->SetValue(m_itemsStorage.begin()->second.c_str());
    pInfoChoice->SetSelection(0);
    m_iLastItemSelection = 0;
  }
  else
      m_iLastItemSelection = -1;

  pUserNets->Clear();

  for (it=m_netsStorage.begin(); it!=m_netsStorage.end();it++)
  {
    pUserNets->Append(it->first.c_str());
  }

  if(pUserNets->GetCount() > 0)
  {
    pUserNets->SetSelection(0);
    pNetScreenName->SetValue(m_netsStorage.begin()->second.c_str());
    m_iLastNetSelection = 0;
  }
  else
    m_iLastNetSelection = -1;

  pAllNets->Clear();

  for (unsigned int i = 0; i < all_nets.size(); i++)
  {
	  pAllNets->Append(all_nets[i].c_str());
  }

  if (!localChange)
  {
//    ActivateSave(false);
  }
  m_loadDone = true;
}

void guiPrefs::SaveUser()
{

  if (FindWindow( ID_CON_EDIT_NAME ) == NULL)
    return;

  AuthLoad* pMyLoader = AccessLoader();
  
  if (!pMyLoader)
    return;

  wxTextCtrl * pName = (wxTextCtrl *)FindWindow( ID_CON_EDIT_NAME );
  wxComboBox * pInfoChoice = (wxComboBox *)FindWindow( ID_CON_EDIT_INFO_LIST );
  
  wxTextCtrl * pInfoText = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_INFO_TEXT );
  wxListBox * availableNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_ALL),
    * activeNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_USER);

  wxTextCtrl * pNetScreenName = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_NETS_SCREEN_NAME );

  // save current net screenname
  int nCurSelection = activeNets->GetSelection();
  if (nCurSelection != -1)
    m_netsStorage[activeNets->GetString(nCurSelection).c_str()] = pNetScreenName->GetValue().Lower().c_str();
  
  wxLogDebug(wxString("pNetScreenName is ") + pNetScreenName->GetValue());
  // save current item text
  nCurSelection = pInfoChoice->GetSelection();
  if (nCurSelection != -1)
    m_itemsStorage[pInfoChoice->GetString(nCurSelection).c_str()] = pInfoText->GetValue().c_str();
     
  if (m_sxName == "")
    return;

  if (pName->GetValue() != "")
  {
    if (pMyLoader->ChangeName(m_sxName.c_str(), pName->GetValue().c_str()))
      m_sxName = pName->GetValue();
  }
  
  map<string, string>::const_iterator it;
  
  for (it=m_itemsStorage.begin(); it!=m_itemsStorage.end();it++)
  {
    pMyLoader->AddInfo(m_sxName.c_str(),it->first, it->second);
  }

  pMyLoader->RemoveAllContactNets(m_sxName.c_str());

  wxLogDebug(wxString("sxName is") + m_sxName + wxString(" nets ------"));
  for (it=m_netsStorage.begin(); it!=m_netsStorage.end();it++)
  {
    wxLogDebug(it->first.c_str());
    wxLogDebug(it->second.c_str());
    pMyLoader->AddNet(m_sxName.c_str(), it->first, it->second, 0);
  }
  wxLogDebug("end nets ----------");
  pMyLoader->Commit();
}


void guiPrefs::OnAddToActive(wxCommandEvent& event)
{
  wxListBox * availableNets,
    * activeNets;

  availableNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_ALL);
  activeNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_USER);

  if ((!availableNets) || (!activeNets))
  {
    wxLogError("guiPrefs::OnAddToActive: Windows are NULL.");
    return;
  }

  wxString l_net = availableNets->GetStringSelection();
  if (l_net == "")
    return;

  wxLogMessage(wxString("OnAddToActive(): Selection is ")+l_net);

  if (activeNets->FindString(l_net) != -1)
    return;

  activeNets->Append(l_net);

  if (activeNets->GetCount() == 1)
  {
    activeNets->SetSelection(0);
    m_iLastNetSelection = 0;
  }
   
  m_netsStorage[l_net.c_str()] = "";
  ActivateSave(true, true);

}

void guiPrefs::OnNetRemove(wxCommandEvent& event)
{

  wxListBox * availableNets,
    * activeNets;

  availableNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_ALL);
  activeNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_USER);

  if ((!availableNets) || (!activeNets))
  {
    wxLogError("guiPrefs::OnNetRemove: Windows are NULL.");
    return;
  }

  if (activeNets->GetCount() == 0)
  {
    wxLogError("Returning early, count 0.");
    return;
  }

  wxString l_net = activeNets->GetStringSelection();

  if (l_net == "")
    return;

  activeNets->Delete(activeNets->GetSelection());

  if (activeNets->GetCount() > 0)
    activeNets->SetSelection(0);

  if (availableNets->FindString(l_net) == -1)
  {
    wxLogMessage("returning early, not in available networks");
    return; // i dont know why this would be the case, however....
  }

  m_netsStorage.erase(l_net.c_str());

  ActivateSave(true, true);
  

}


void guiPrefs::OnNetSelectionChange(wxCommandEvent& event)
{

  wxListBox * availableNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_ALL),
    * activeNets = (wxListBox *)FindWindow(ID_CON_EDIT_NETS_USER);

  wxTextCtrl * pNetScreenName = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_NETS_SCREEN_NAME );

  if ((!availableNets) || (!activeNets) || (!pNetScreenName))
  {
    wxLogError("guiPrefs::OnNetSelectionChange: Windows are NULL.");
    return;
  }

  int nCurSelection = activeNets->GetSelection();

  if (nCurSelection == m_iLastNetSelection)
  {
    // User selected the same item that they were on
    return;
  }
  else
  {
    if (m_iLastNetSelection != -1)
    {
      if (m_netsStorage[activeNets->GetString(m_iLastNetSelection).c_str()] != pNetScreenName->GetValue().c_str())
      {
        ActivateSave();
        m_netsStorage[activeNets->GetString(m_iLastNetSelection).c_str()] = pNetScreenName->GetValue().Lower().c_str();
      }
    }

    m_iLastNetSelection = nCurSelection;
    pNetScreenName->SetValue(m_netsStorage[activeNets->GetString(nCurSelection).c_str()].c_str());
  }

}


void guiPrefs::OnChangeItem(wxCommandEvent& event)
{

  wxComboBox * pInfoChoice = (wxComboBox *)FindWindow( ID_CON_EDIT_INFO_LIST );
  
  wxTextCtrl * pInfoText = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_INFO_TEXT );
  
  if (!pInfoChoice || !pInfoText)
  {
    wxLogError("guiPrefs::OnChangeItem::pInfoChoice is NULL");
    return;
  }

  int nCurSelection = pInfoChoice->GetSelection();

  if (nCurSelection == m_iLastItemSelection)
  {
    // User selected the same item that they were on
    return;
  }
  else
  {
    if (m_iLastItemSelection != -1)
    {
      if (m_itemsStorage[pInfoChoice->GetString(m_iLastItemSelection).c_str()] != pInfoText->GetValue().c_str())
      {
        ActivateSave();
        m_itemsStorage[pInfoChoice->GetString(m_iLastItemSelection).c_str()] = pInfoText->GetValue().c_str();
      }
    }

    m_iLastItemSelection = nCurSelection;
    pInfoText->SetValue(m_itemsStorage[pInfoChoice->GetString(nCurSelection).c_str()].c_str());
  }
}

void guiPrefs::OnAddInfo(wxCommandEvent& event)
{
  wxComboBox * pInfoChoice = (wxComboBox *)FindWindow( ID_CON_EDIT_INFO_LIST );
  
  wxTextCtrl * pInfoText = (wxTextCtrl *)FindWindow ( ID_CON_EDIT_INFO_TEXT );
  
  if (!pInfoChoice || !pInfoText)
  {
    wxLogError("guiPrefs::OnAddInfo::pInfoChoice is NULL");
    return;
  }

  // getting name from tree
  wxTreeItemId id = m_treeCtrl->GetSelection();
  if (!id.IsOk())
  {
    wxLogDebug("id is invalid in void guiPrefs::OnAddInfo(wxCommandEvent& event)");
    return;
  }

  // getting text from user
  wxString sxNewCatagory = wxGetTextFromUser("New Catagory", "Enter catagory name","", this, -1,-1,TRUE);
  
  if (pInfoChoice->FindString(sxNewCatagory) > -1)
  {
    wxMessageBox("Sorry, this information title already exists.");
    return;
  }

  pInfoChoice->Append(sxNewCatagory);
  m_itemsStorage[sxNewCatagory.c_str()];

  int nCurSelection = pInfoChoice->FindString(sxNewCatagory);
  pInfoChoice->SetSelection(nCurSelection);

  if (m_iLastItemSelection != -1)
    m_itemsStorage[pInfoChoice->GetString(m_iLastItemSelection).c_str()] = pInfoText->GetValue().c_str();

  m_iLastItemSelection = nCurSelection;
  pInfoText->SetValue(m_itemsStorage[pInfoChoice->GetString(nCurSelection).c_str()].c_str());
  
  ActivateSave();
}


void guiPrefs::OnAddUser(wxCommandEvent& event)
{
  AuthLoad* pMyLoader = AccessLoader();
  
  if (!pMyLoader)
    return;

  if (!m_treeRoot.IsOk())
  {
    wxLogDebug("m_treeRoot is not OK in guiPrefs::OnAddUser()");
    return;
  }

  wxString sxNewUser = wxGetTextFromUser("New Contact", "Enter contact name","", this, -1,-1,TRUE);

  if (sxNewUser == "")
  {
    wxLogDebug("guiPrefs::OnAddUser: sxNewUser is \"\"");
    return;
  }

  
  if (!pMyLoader->AddUser(sxNewUser.c_str()))
  {
    wxMessageBox("That contact name already exists, please choose another.");
    return;
  }

  // refresh pref tree
  GetTree().CollapseAndReset(m_contactRoot);
  PlaceFoldersInTree(m_contactRoot, "root");
 
  // refresh main tree (in contact window)
  // this might be better done in idle time on contact window (TODO)
  ResetContactTree();

  pMyLoader->Commit();

}


