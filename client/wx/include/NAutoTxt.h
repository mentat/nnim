#ifndef __N_AUTO_TXT_H__
#define __N_AUTO_TXT_H__

#include "wx/wx.h"
#include "wx/hash.h"
#include "wx/textctrl.h"

class BST;
class wxHashTable;
class wxAutoTextCtrl: public wxTextCtrl
{
public:
	wxAutoTextCtrl();
    wxAutoTextCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTextCtrlNameStr);

    bool Create(wxWindow *parent, wxWindowID id = -1,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxTextCtrlNameStr);

	~wxAutoTextCtrl();

	void OnChar(wxKeyEvent &event);

private:

	void DisplayClosest();
	wxHashTable * m_table;
	//BST * m_bst;
	void ReadDict();

	wxString m_curWord;
	wxString m_curFoundWord;

	wxArrayString stringArray;

	DECLARE_EVENT_TABLE()

};

#endif
