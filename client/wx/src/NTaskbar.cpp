// --*-c++-*--
/*
    $Id: NTaskbar.cpp,v 1.1 2002/06/19 16:27:18 thementat Exp $
 
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
#include "NMain.h"

#include "NTaskbar.h"


#ifdef _WIN32

BEGIN_EVENT_TABLE(NNIMTaskBarIcon, wxTaskBarIcon)
  EVT_MENU(-1, NNIMTaskBarIcon::OnCommandEvent)
END_EVENT_TABLE()

void NNIMTaskBarIcon::OnRButtonUp(wxEvent& event)
{
  wxMenu menu("NNIM Menu");

  menu.Append(wxNNIM::ID_CONTACTS_TASKBAR_RESTORE, "&Restore GM: NNIM");
  menu.Append(wxNNIM::ID_CONTACTS_TASKBAR_HIDE, "&Hide");
  menu.AppendSeparator();
  menu.Append(wxNNIM::ID_CONTACTS_TASKBAR_EXIT, "E&xit");

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
  case(wxNNIM::ID_CONTACTS_TASKBAR_RESTORE):
    pMyWindow->Show(TRUE);
    break;
  case(wxNNIM::ID_CONTACTS_TASKBAR_HIDE):
    pMyWindow->Show(FALSE);
    break;
  case(wxNNIM::ID_CONTACTS_TASKBAR_EXIT):
    pMyWindow->Close(TRUE);
    wxGetApp().ProcessIdle(); 
    break;
  }*/
}

#endif


