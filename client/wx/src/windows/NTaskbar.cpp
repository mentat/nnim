#include "NInclude.h"
#include "NMain.h"

#include "NTaskbar.h"


#ifdef _WIN32

BEGIN_EVENT_TABLE(NNIMTaskBarIcon, wxTaskBarIcon)
  EVT_MENU(-1, NNIMTaskBarIcon::OnCommandEvent)
END_EVENT_TABLE()

void NNIMTaskBarIcon::OnRButtonUp(wxEvent& event)
{
  wxMenu menu("NNIM Menu");

  menu.Append(ID_CONTACTS_TASKBAR_RESTORE, "&Restore GM: NNIM");
  menu.Append(ID_CONTACTS_TASKBAR_HIDE, "&Hide");
  menu.AppendSeparator();
  menu.Append(ID_CONTACTS_TASKBAR_EXIT, "E&xit");

  PopupMenu(&menu);
}

void NNIMTaskBarIcon::OnLButtonDClick(wxEvent& event)
{
 /* wxLogDebug("OnLButtonDClick");
  wxFrame * pMyWindow = NULL;

  if (gNContactView)
    pMyWindow = (wxFrame *)gNContactView;
  else
  {
    if (gNLoginView)
      pMyWindow = (wxFrame *)gNLoginView;
    else 
      return;
  }

  if (pMyWindow->IsIconized())
    pMyWindow->Iconize(FALSE);
  else
    pMyWindow->Iconize(TRUE);
*/
}

void NNIMTaskBarIcon::OnCommandEvent(wxCommandEvent &event)
{
  OnMenuCommand(event.GetId());
}

void NNIMTaskBarIcon::OnMenuCommand(int id)
{
/*  wxFrame * pMyWindow = NULL;

  if (gNContactView)
    pMyWindow = (wxFrame *)gNContactView;
  else
  {
    if (gNLoginView)
      pMyWindow = (wxFrame *)gNLoginView;
    else 
      return;
  }

  switch(id)
  {
  case(ID_CONTACTS_TASKBAR_RESTORE):
    pMyWindow->Show(TRUE);
    break;
  case(ID_CONTACTS_TASKBAR_HIDE):
    pMyWindow->Show(FALSE);
    break;
  case(ID_CONTACTS_TASKBAR_EXIT):
    pMyWindow->Close(TRUE);
    wxGetApp().ProcessIdle(); 
    break;
  }*/
}

#endif


