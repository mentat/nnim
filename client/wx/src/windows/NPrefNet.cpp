// --*-c++-*--
/*
    $Id: NPrefNet.cpp,v 1.1 2002/06/06 17:21:30 thementat Exp $
 
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
    $Log: NPrefNet.cpp,v $
    Revision 1.1  2002/06/06 17:21:30  thementat
    Initial revision

    */

#include "NInclude.h"
#include "NWizards_wdr.h"
#include "NPrefNet.h"
#include "NMain.h"

BEGIN_EVENT_TABLE(NPrefNet, wxPanel)
EVT_BUTTON(ID_U_ADDNET, NPrefNet::OnAddNet)
EVT_BUTTON(ID_U_REMNET, NPrefNet::OnRemNet)
EVT_LIST_ITEM_SELECTED(ID_U_CONFNETS, NPrefNet::OnSelected)	
END_EVENT_TABLE()

NPrefNet::NPrefNet(wxWindow * parent, const wxPoint& pos)
: wxScrolledWindow(parent, -1, pos, wxDefaultSize)
{

  m_selected = -1;  

  Show(FALSE);
  uWiz2(this, true, true);
  LoadSettings();
}

int NPrefNet::EnumNet(const string& name)
{
  if (name == "toc") return AIM;
  if (name == "msn") return MSN;
  if (name == "icq") return ICQ;
  if (name == "kit") return GNU;
  if (name == "yahoo") return YAHOO;

  return -1;

}

void NPrefNet::LoadSettings()
{/*
  AuthLoad &myLoad = AccessLoader();
  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);

  listctrl->InsertColumn(0, _T("Screen-name"));
  listctrl->InsertColumn(1, _T("Network"));

  wxListBox * listbox = (wxListBox *)FindWindow(ID_U_ALLNETS);

  listbox->Clear();
  listbox->Append("AIM");
  
  listbox->Append("MSN");
  
  listbox->Append("Yahoo");
  
  listbox->Append("ICQ");
  
  listbox->Append("GNU");

  vector<string> myNets;
  map<string, string> netSettings;


  myLoad.GetAvailableNets(myNets);

  for (unsigned int i = 0; i < myNets.size(); i++)
  {

    myLoad->GetAvailableNetSettings(myNets[i], netSettings);

    // see if user is configured for this network
    map<string,string>::const_iterator it=netSettings.find("user");
    if (it!=netSettings.end())
    {
      string usr = netSettings["user"];

      if (usr != "")
      {
        listctrl->InsertItem(0, usr.c_str());

        listctrl->SetItemData(0, EnumNet(myNets[i]));
        listctrl->SetItem(0,1, myNets[i].c_str());
        m_pwords.insert(m_pwords.begin(), netSettings["password"]);
      }
    }
  }
*/
}

void NPrefNet::SaveSettings()
{/*
  AuthLoad * myLoader = AccessLoader();

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);
  wxListBox * listbox = (wxListBox *)FindWindow(ID_U_ALLNETS);
  
  for (long int it = 0; it < listctrl->GetItemCount(); it++)
  {
    wxString netname;
    switch(listctrl->GetItemData(it)) {
    case (AIM): netname = "toc"; break;
    case(MSN): netname = "msn"; break;
    case(YAHOO): netname = "yahoo"; break;
    case(ICQ): netname = "icq"; break;
    case(GNU): netname = "kit"; break;
    default: wxASSERT(false); break;
    }
    
    wxMessageBox(listctrl->GetItemText(it) + wxString(m_pwords[it].c_str()));
    myLoader->AddAvailableNetSetting(netname.c_str(), "user", listctrl->GetItemText(it).c_str());
    myLoader->AddAvailableNetSetting(netname.c_str(), "password", m_pwords[it]);
      
  }

  myLoader->Commit();
*/
}

void NPrefNet::OnAddNet(wxCommandEvent &event)
{

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);
  wxListBox * listbox = (wxListBox *)FindWindow(ID_U_ALLNETS);

  if (listbox->GetSelection() < 0)
    return;

  wxString prompt;

  if (listbox->GetStringSelection() == wxT("ICQ"))
    prompt = wxT("What is this contact's UIN\non ICQ Network");
  else
    prompt.Printf(wxT("What is this contact's screen-name\non %s Network"), listbox->GetStringSelection());
  
  wxString screenname = wxGetTextFromUser(prompt, "Contact Information","", this);

  if (screenname == "")
    return;

  long fingstr = listctrl->FindItem(-1, screenname);

  if (fingstr >= 0)
  {
    if (listctrl->GetItemData(fingstr) == listbox->GetSelection())
    {
      wxMessageBox(wxT("Sorry, you can't have the same name/network twice."), wxT("Duplicate Info"), wxOK, this);
      return;
    }
  }

  wxString prompt2;
  prompt2.Printf(wxT("Please enter the password for %s\non network %s."), screenname, listbox->GetStringSelection());
  wxString pw = wxGetPasswordFromUser(prompt2);

  m_pwords.insert(m_pwords.begin(), pw);

  listctrl->InsertItem(0, screenname);
  listctrl->SetItemData(0, listbox->GetSelection());
  listctrl->SetItem(0,1, listbox->GetStringSelection());

}

void NPrefNet::OnRemNet(wxCommandEvent &event)
{

  if (m_selected == -1 ) return;

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);

  vector<wxString>::iterator it;

  it = m_pwords.begin();

  for (int i = 0; i < m_selected; i++)
  {
    it++;

  }

  m_pwords.erase(it);
  listctrl->DeleteItem(m_selected);
}

void NPrefNet::OnSelected(wxListEvent & event)
{
  m_selected = event.GetIndex();

}