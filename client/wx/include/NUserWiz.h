#ifndef __GUI_UWIZ_H__
#define __GUI_UWIZ_H__

#include "wx/wizard.h"
#include "crypto/misc.h"
#include <vector>
#include <string>

using namespace std;
using namespace CryptoPP;
class wxListEvent;
class UserWizardP2;
class UserWizardP1 : public wxWizardPageSimple
{
public:

  friend class UserWizardP2;
    // directions in which we allow the user to proceed from this page
    enum
    {
        Forward, Backward, Both, Neither
    };

    UserWizardP1(wxWizard *parent);

    // wizard event handlers
    void OnWizardCancel(wxWizardEvent& event){}

    void OnWizardPageChanging(wxWizardEvent& event){}

    virtual bool TransferDataFromWindow();

private:

  wxTextCtrl * name;
  wxTextCtrl * p1;

    //DECLARE_EVENT_TABLE()
};

class UserWizardP2 : public wxWizardPageSimple
{
public:
    // directions in which we allow the user to proceed from this page
    enum
    {
        AIM, MSN, YAHOO, ICQ, GNU
    };
    enum
    {
        Forward, Backward, Both, Neither
    };

    UserWizardP2(wxWizard *parent);

    // wizard event handlers
    void OnWizardCancel(wxWizardEvent& event)
    {

    }

    void OnWizardPageChanging(wxWizardEvent& event);

    virtual bool TransferDataFromWindow()
    {
      return true;
    }

    void OnAddNet(wxCommandEvent &event);
    void OnRemNet(wxCommandEvent &event);
    void OnSelected(wxListEvent & event);

private:

  long m_selected;
  vector<SecByteBlock> m_pwords;
  DECLARE_EVENT_TABLE()
};

#endif
