#ifndef __NCONTACTWIZ_H__
#define __NCONTACTWIZ_H__

#include "wx/wizard.h"
class wxListEvent;
class NContactTreeXML;

void RunContactWizard(NContactTreeXML * parent, wxTreeItemId &id);

class CWizardP1: public wxWizardPageSimple
{
public:
	CWizardP1(wxWizard * parent, NContactTreeXML * owner, wxTreeItemId& id);

	void OnChecked(wxCommandEvent & event);

	virtual bool TransferDataFromWindow();

	wxString m_name;
	wxCheckBox * m_checkbox;
	bool m_single;

  private:

	  wxTreeItemId m_baseId;
	  NContactTreeXML * m_owner;
	  wxWizardPage * m_next;

	  DECLARE_EVENT_TABLE()

};

class CWizardP2: public wxWizardPageSimple
{
public:
  CWizardP2(wxWizard * parent, NContactTreeXML * owner);

  void OnWizardPageChanging(wxWizardEvent& event){}

  void OnAddNet(wxCommandEvent &event);
  void OnRemNet(wxCommandEvent &event);
  void OnSelected(wxListEvent & event);

private:

	wxWizardPage * m_next;
	wxWizardPage * m_prev;

	NContactTreeXML * m_owner;
	long m_selected;
	vector<string> m_pwords;

	DECLARE_EVENT_TABLE()

};

class CWizardP3: public wxWizardPageSimple
{
public:
	CWizardP3(wxWizard * parent, NContactTreeXML * owner, wxTreeItemId& id);

	void OnWizardPageChanging(wxWizardEvent& event);

	virtual bool TransferDataFromWindow();

	void OnAddInfo(wxCommandEvent &event);

private:

	wxTreeItemId m_baseId;
	wxWizardPage * m_prev;
	NContactTreeXML * m_owner;

	DECLARE_EVENT_TABLE()

};

class CWizard: public wxWizard
{
public:
	CWizard() : wxWizard() {}
	CWizard(wxWindow *parent,
             int id = -1,
             const wxString& title = wxEmptyString,
             const wxBitmap& bitmap = wxNullBitmap,
			 const wxPoint& pos = wxDefaultPosition): wxWizard(parent, id, title, bitmap, pos) {}

	wxString m_name;

};

#endif
