// --*-c++-*--
/*
    $Id: NUserWiz.cpp,v 1.3 2002/06/21 19:03:15 thementat Exp $
 
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
#include "NWizards_wdr.h"

#include "NUserWiz.h"
#include "NMain.h"

#include "xmlnode.h"

DECLARE_APP(wxNNIM)

UserWizardP1::UserWizardP1(wxWizard *parent)
 : wxWizardPageSimple(parent)
{
  SetBackgroundColour(*wxWHITE);
  uWiz1(this, false, true);

  name = (wxTextCtrl *)FindWindow(ID_U_NAME);
  p1 = (wxTextCtrl *)FindWindow(ID_U_PASS1);

  name->SetFocus();

}

bool UserWizardP1::TransferDataFromWindow()
{

  wxTextCtrl * p2 = (wxTextCtrl *)FindWindow(ID_U_PASS2);

  AuthLoad & myLoader = wxGetApp().AccessLoader();

  if ((name->GetValue() == "") || (p1->GetValue() == ""))
  {
    wxMessageBox(wxT("Sorry, all fields must be completed before you move on."), wxT("Empty fields."), wxOK, this);
    return FALSE;
  }

  if (myLoader.Exists(name->GetValue().c_str()))
  {
    wxMessageBox(wxT("Sorry, this user name already exists, please choose another."), wxT("User Exists"), wxOK, this);
    return FALSE;
  }

  if (p1->GetValue() != p2->GetValue())
  {
    wxMessageBox(wxT("Sorry, your passwords do not match, please enter them again."), wxT("Password Error"), wxOK, this);
    p1->Clear();
    p2->Clear();
    return FALSE;
  }

  if (p1->GetValue().length() < 5)
  {
    wxMessageBox(wxT("Your password needs to be at least 5 characters."), wxT("Password Error"), wxOK, this);
    p1->Clear();
    p2->Clear();
    return FALSE;
  }

  return TRUE;
}

BEGIN_EVENT_TABLE(UserWizardP2, wxWizardPageSimple)
EVT_BUTTON(ID_U_ADDNET, UserWizardP2::OnAddNet)
EVT_BUTTON(ID_U_REMNET, UserWizardP2::OnRemNet)
EVT_LIST_ITEM_SELECTED(ID_U_CONFNETS, UserWizardP2::OnSelected)	
EVT_WIZARD_PAGE_CHANGING(-1, UserWizardP2::OnWizardPageChanging)
// EVT_WIZARD_CANCEL(-1, wxRadioboxPage::OnWizardCancel)
END_EVENT_TABLE()

UserWizardP2::UserWizardP2(wxWizard *parent)
 : wxWizardPageSimple(parent)
{

  m_selected = -1;  

  SetBackgroundColour(*wxWHITE);
  uWiz2(this, false, true);

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);

  listctrl->InsertColumn(0, wxT("Screen-name"));
  listctrl->InsertColumn(1, wxT("Network"));

  wxListBox * listbox = (wxListBox *)FindWindow(ID_U_ALLNETS);

  listbox->Clear();
  listbox->Append("AIM");
  
  listbox->Append("MSN");
  
  listbox->Append("Yahoo");
  
  listbox->Append("ICQ");
  
  listbox->Append("GNU");

}

void UserWizardP2::OnAddNet(wxCommandEvent &event)
{

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);
  wxListBox * listbox = (wxListBox *)FindWindow(ID_U_ALLNETS);

  if (listbox->GetSelection() < 0)
    return;

  wxString prompt;

  if (listbox->GetStringSelection() == "ICQ")
    prompt = wxT("What is this your UIN\non ICQ Network");
  else
    prompt.Printf(wxT("What is your screen-name\non %s Network"), listbox->GetStringSelection());
  
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

  wxString prompt2;
  prompt2.Printf(wxT("Please enter the password for %s\non network %s."), screenname, listbox->GetStringSelection());
  wxString pw = wxGetPasswordFromUser(prompt2);

  m_pwords.insert(m_pwords.begin(), SecByteBlock((const unsigned char *)pw.c_str(), pw.length()));
  pw = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

  listctrl->InsertItem(0, screenname);
  listctrl->SetItemData(0, listbox->GetSelection());
  listctrl->SetItem(0,1, listbox->GetStringSelection());

}

void UserWizardP2::OnRemNet(wxCommandEvent &event)
{

  if (m_selected == -1 ) return;

  wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);

  vector<SecByteBlock>::iterator it;

  it = m_pwords.begin();

  for (int i = 0; i < m_selected; i++)
  {
    it++;
  }

  m_pwords.erase(it);
  listctrl->DeleteItem(m_selected);
}

void UserWizardP2::OnSelected(wxListEvent & event)
{
  m_selected = event.GetIndex();

}

void UserWizardP2::OnWizardPageChanging(wxWizardEvent& event)
{
	if ( !event.GetDirection()  )
		return;

	wxListCtrl * listctrl = (wxListCtrl *)FindWindow(ID_U_CONFNETS);
	wxListBox * listbox = (wxListBox *)FindWindow(ID_U_ALLNETS);

	wxString uname = ((UserWizardP1 *)GetPrev())->name->GetValue();
	wxString pword = ((UserWizardP1 *)GetPrev())->p1->GetValue();

	SecByteBlock pass((const unsigned char *)pword.c_str(), 

		pword.length());
    
	AuthLoad &myLoader = wxGetApp().AccessLoader();

	if (!myLoader.CreateNew(uname.c_str(), pass))
	{
		throw("UserWizardP2::OnWizardPageChanging: Could not create new user");
	}

	wxMessageBox(uname + pword);

	//try to get password value out of memory, dont know of the actual value of this
	pword = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	((UserWizardP1 *)GetPrev())->p1->SetValue("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  

  // load defaults
	XMLNode toc, msn, yahoo, kit, icq;
	
	toc <<
    "<toc>"
    "<user password=\"\" username=\"\"/>"
    "<server host=\"" TOC_SERVER "\" port=\"" TOC_SERVER_PORT "\"/>"
	"<loginserver host=\"" TOC_LOGIN_SERVER "\" port=\"" TOC_LOGIN_PORT "\"/>"
    "</toc>";

	msn <<
    "<msn>"
    "<user password=\"\" username=\"\"/>"
    "<server host=\"" MSN_SERVER "\" port=\"" MSN_PORT "\"/>"
	"</msn>";

	yahoo <<
    "<yahoo>"
    "<user password=\"\" username=\"\"/>"
    "<server host=\"" YAHOO_DATA_SERVER "\" port=\"" YAHOO_DATA_PORT "\"/>"
	"<loginserver host=\"" YAHOO_AUTH_SERVER "\" port=\"" YAHOO_AUTH_PORT "\"/>"
    "</yahoo>";

	kit <<
    "<kit>"
    "<user password=\"\" username=\"\"/>"
    "<server host=\"" GNU_SERVER "\" port=\"" GNU_PORT "\"/>"
	"</kit>";

	icq <<
    "<icq>"
    "<user password=\"\" username=\"\"/>"
    "<server host=\"" ICQ_SERVER "\" port=\"" ICQ_PORT "\"/>"
	"</icq>";


	for (long int it = 0; it < listctrl->GetItemCount(); it++)
	{
		XMLNode tmp;
		switch(listctrl->GetItemData(it)) {
		case (AIM): tmp = toc; break;
		case(MSN): tmp = msn; break;
		case(YAHOO): tmp = yahoo; break;
		case(ICQ): tmp = icq; break;
		case(GNU): tmp = kit; break;
		default: wxASSERT(false); break;
		}

		tmp.child("user").setProperty("username", listctrl->GetItemText(it).c_str());
		tmp.child("user").setProperty("password", (const char *)(void *)m_pwords[it]);
      
	}

    try
    {
	    myLoader.U().SetNet(icq);	
	    myLoader.U().SetNet(toc);
	    myLoader.U().SetNet(msn);
	    myLoader.U().SetNet(yahoo);
	    myLoader.U().SetNet(kit);
    }
    catch(...)
    {
        throw gmException("Pointer Error", gmException::gMEM);
    }
  
	myLoader.CommitToFile();

}
/*
    -----
    $Log: NUserWiz.cpp,v $
    Revision 1.3  2002/06/21 19:03:15  thementat
    NNIM compiles and links in gcc 2.96 20000731

    Revision 1.2  2002/06/20 01:25:00  thementat
    Removed unicode for the time being to fix linux build.

    Revision 1.1  2002/06/19 16:27:18  thementat
    Restructured directories.

    Revision 1.3  2002/06/16 04:08:28  thementat
    Hopefully fixed Authload and related classes.

    Revision 1.2  2002/06/14 22:02:24  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.1.1.1  2002/06/06 17:21:30  thementat
    Checkin of new sources BETA 2

*/
