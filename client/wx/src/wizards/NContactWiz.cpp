// --*-c++-*--
/*
    $Id: NContactWiz.cpp,v 1.3 2002/06/16 04:08:28 thementat Exp $
 
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

#include "NWizards_wdr.h"
#include "NContactWiz.h"
#include "NMain.h"
#include "NContactTreeXML.h"

#include "bitmaps/wiz_obsd_fish2.xpm"

DECLARE_APP(wxNNIM)

CWizardP1 * page1(NULL);
CWizardP2 * page2(NULL);
CWizardP3 * page3(NULL);

void RunContactWizard(NContactTreeXML * parent, wxTreeItemId &id)
{
	CWizard * wizard = new CWizard(parent, -1, wxT("New Contact Wizard"), wxBitmap(wiz_obsd_fish2_xpm));
	wizard->SetBackgroundColour(*wxWHITE);

	page1 = new CWizardP1(wizard, parent, id);
	page2 = new CWizardP2(wizard, parent);
	page3 = new CWizardP3(wizard, parent, id);
	page1->SetNext(page2);
	page2->SetNext(page3);
	page2->SetPrev(page1);
	page3->SetPrev(page2);

	wizard->RunWizard(page1);
	wizard->Destroy();

}
BEGIN_EVENT_TABLE(CWizardP1, wxWizardPageSimple)
EVT_CHECKBOX(-1, CWizardP1::OnChecked)	
END_EVENT_TABLE()

CWizardP1::CWizardP1(wxWizard * parent, NContactTreeXML * owner, wxTreeItemId& id)
: wxWizardPageSimple(parent)
{
	m_owner = owner;
	m_baseId = id;
	
	SetBackgroundColour(*wxWHITE);
	cWiz1(this, false, true);

	m_checkbox = (wxCheckBox *)FindWindow(ID_C_WIZ_EDIT);
	m_checkbox->Enable(TRUE);

}

void CWizardP1::OnChecked(wxCommandEvent &event)
{
   bool m_useWizard = m_checkbox->GetValue();

   wxButton *btn = (wxButton *)GetParent()->FindWindow(wxID_FORWARD);
   if ( btn )
   {
      if ( m_useWizard )
	  {
		SetNext(page2);
		btn->SetLabel(_("&Next >"));
	  }
      else
	  {
		  SetNext(NULL);
		btn->SetLabel(_("&Finish"));
	  }
   }

}

bool CWizardP1::TransferDataFromWindow()
{
	wxLogDebug(wxT("TransferData"));

	wxTextCtrl * cname = (wxTextCtrl *)FindWindow(ID_C_NAME);

	AuthLoad &myLoader = wxGetApp().AccessLoader();

	m_name = cname->GetValue();

	if (m_name == "")
	{
		wxMessageBox(wxT("Sorry, the contact name cannot be blank."), wxT("Name Error"), wxOK, this);
		return false;
	}

	if ((m_name.Find('<') > -1) || (m_name.Find('>') > -1) || (m_name.Find('\"') > -1) || (m_name.Find('\'') > -1))
	{
		wxMessageBox(wxT("Sorry, the name can't contain <>\"\'"), wxT("Name Error"), wxOK, this);
		return false;
	}
  
	if (myLoader.C().Exists(m_name.mb_str(wxConvUTF8).data()))
	{
		wxMessageBox(wxT("Sorry, this contact name already exists."), wxT("Name Error"), wxOK, this);
		return false;
	}

	return true;
}

BEGIN_EVENT_TABLE(CWizardP2, wxWizardPageSimple)
	EVT_BUTTON(ID_C_ADDNET, CWizardP2::OnAddNet)
	EVT_BUTTON(ID_C_REMNET, CWizardP2::OnRemNet)
	EVT_LIST_ITEM_SELECTED(ID_U_CONFNETS, CWizardP2::OnSelected)	
END_EVENT_TABLE()

CWizardP2::CWizardP2(wxWizard * parent, NContactTreeXML * owner)
: wxWizardPageSimple(parent)
{

	m_owner = owner;

	m_selected = -1; 

	SetBackgroundColour(*wxWHITE);
	cWiz2(this, false, true);

	wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_C_CONFNET);

	listctrl->InsertColumn(0, wxT("Screen-name"));
	listctrl->InsertColumn(1, wxT("Network"));

	wxListBox * listbox = (wxListBox *)FindWindow(ID_C_ALLNET);

	listbox->Clear();
	listbox->Append("AIM");
  
	listbox->Append("MSN");
  
	listbox->Append("Yahoo");
  
	listbox->Append("ICQ");
  
	listbox->Append("GNU");
}

void CWizardP2::OnAddNet(wxCommandEvent &event)
{

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_C_CONFNET);
  wxListBox * listbox = (wxListBox *)FindWindow(ID_C_ALLNET);

  if (listbox->GetSelection() < 0)
    return;

  wxString prompt;

  if (listbox->GetStringSelection() == "ICQ")
    prompt = wxT("What is this contact's UIN\non ICQ Network");
  else
    prompt.Printf(wxT("What is this contact's screen-name\non %s Network"), listbox->GetStringSelection());
  
  wxString screenname = wxGetTextFromUser(prompt, wxT("Contact Information"),"", this);

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
  listctrl->InsertItem(0, screenname);
  listctrl->SetItemData(0, listbox->GetSelection());
  listctrl->SetItem(0,1, listbox->GetStringSelection());

}

void CWizardP2::OnRemNet(wxCommandEvent & event)
{

  if (m_selected == -1 ) return;

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_C_CONFNET);
  listctrl->DeleteItem(m_selected);
}

void CWizardP2::OnSelected(wxListEvent & event)
{
	  m_selected = event.GetIndex();
}


BEGIN_EVENT_TABLE(CWizardP3, wxWizardPageSimple)
	EVT_BUTTON(ID_USER_INFO_ADD, CWizardP3::OnAddInfo)
	EVT_WIZARD_PAGE_CHANGING(-1, CWizardP3::OnWizardPageChanging)
END_EVENT_TABLE()

CWizardP3::CWizardP3(wxWizard * parent,NContactTreeXML *owner, wxTreeItemId& id)
: wxWizardPageSimple(parent)
{
	m_owner = owner;
	m_baseId = id;

  SetBackgroundColour(*wxWHITE);
  UserTree(this, false, true);
}

void CWizardP3::OnAddInfo(wxCommandEvent & event)
{

}

bool CWizardP3::TransferDataFromWindow()
{
    return true;
}

void CWizardP3::OnWizardPageChanging(wxWizardEvent& event)
{

	if ( !event.GetDirection()  )
		return;


	wxLogDebug(wxT("In OnWizardPageChanging"));

	wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_C_CONFNET);
	wxListBox * listbox = (wxListBox *)FindWindow(ID_C_ALLNET);

	wxString base = m_owner->GetItemText(m_baseId);
    wxString name = ((CWizardP1 *)GetPrev()->GetPrev())->m_name;

	AuthLoad &myLoader = wxGetApp().AccessLoader();

	wxLogDebug(wxString("Trying to add ") + name + wxString(" ") + base);

	if (!myLoader.C().Add(name.mb_str(wxConvUTF8).data(),base.mb_str(wxConvUTF8).data()))
	{
		wxLogError(wxT("Couldn't add this user to specified folder."));
		wxWizardEvent event(wxEVT_WIZARD_CANCEL);
		((CWizard *)GetParent())->ProcessEvent(event);
		return;
	}

	m_owner->AddTreeContact(name, m_baseId);

}
