// --*-c++-*--
/*
    $Id: NDerivedClasses.h,v 1.1 2002/06/06 17:21:42 thementat Exp $
 
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
    $Log: NDerivedClasses.h,v $
    Revision 1.1  2002/06/06 17:21:42  thementat
    Initial revision

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i

    Revision 1.4  2001/11/10 00:15:35  mentat
    New sources.

    Revision 1.1  2001/11/09 00:21:36  thementat
    new tree

    Revision 1.1  2001/10/06 19:55:50  thementat
    Added derived class files.


*/

#ifndef GUI_DERIVED_H
#define GUI_DERIVED_H

// this class is meant to group together my derived classes
// that are small and dont need their own .h/.cpp file

#include <wx/treectrl.h>
#include <wx/scrolwin.h>


class wxTreeItemData;

class TreeData : public wxTreeItemData
{
public:
  TreeData(int id) {m_id = id;}
  ~TreeData() {}

  int GetID() { return m_id; }
   
private:
  int m_id;

};

/**
 * Tree Item Data
 * 
 * Holds the data for each tree node.
 *
 * @author Jesse Lovelace
 * @version 0.1
 */
/*class MyTreeItemData : public wxTreeItemData
{
public:
  MyTreeItemData(const wxString& desc,  int status) 
    : m_chatID(0), m_desc(desc), m_status(status), m_needsRefresh(false) { }

  void SetItemStatus(int status) { m_status = status; }
  void SetChatID(unsigned long ID) { m_chatID = ID; }
  void SetNeedsRefresh(bool needsref) { m_needsRefresh = needsref; }
  void SetDesc(const wxString& desc) { m_desc = desc; }
 
  bool NeedsRefresh() { return m_needsRefresh; }
  int GetStatus() { return m_status; }
  unsigned long GetChatID() { return m_chatID; }

  const char *GetDesc() const { return m_desc.c_str(); }

private:
  bool m_needsRefresh;
  unsigned long m_chatID;
  wxString m_desc;
  int m_status;

};*/

#endif