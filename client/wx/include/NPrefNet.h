// --*-c++-*--
/*
    $Id: NPrefNet.h,v 1.1 2002/06/06 17:21:42 thementat Exp $
 
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
    $Log: NPrefNet.h,v $
    Revision 1.1  2002/06/06 17:21:42  thementat
    Initial revision

*/

#ifndef __NPREFNET_H__
#define __NPREFNET_H__

using namespace std;
class NPrefNet: public wxScrolledWindow
{
public:

  enum
    {
        AIM, MSN, YAHOO, ICQ, GNU
    };

  NPrefNet(wxWindow * parent, const wxPoint& pos = wxDefaultPosition);
  ~NPrefNet() {}


  void OnAddNet(wxCommandEvent &event);
  void OnRemNet(wxCommandEvent &event);
  void OnSelected(wxListEvent & event);
  void SaveSettings();
private:

  void LoadSettings();

  int EnumNet(const string& name);
  
  long m_selected;
  vector<wxString> m_pwords;

  DECLARE_EVENT_TABLE()
};

#endif
