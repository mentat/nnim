// --*-c++-*--
/*
    $Id: NContactTree.h,v 1.1 2002/06/06 17:21:42 thementat Exp $
 
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
    $Log: NContactTree.h,v $
    Revision 1.1  2002/06/06 17:21:42  thementat
    Initial revision

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.7  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.6  2001/12/12 19:25:25  mentat
    Fixes for gcc 2.95.3 and new makefile to build (for now until I sort out automake).
    Builds in FreeBSD4.4 with gcc 2.95.3 and wxWindows 2.3.2

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/

#ifndef GUI_CONTACT_TREE_H
#define GUI_CONTACT_TREE_H

class guiContact;

/**
 * Tree Control
 * 
 * Controls the action of the tree structure.
 *
 * @author Jesse Lovelace
 * @version 0.1
 */

class guiContactTree : public wxGenericTreeCtrl
{
public:
  
  enum
  {
    ICON_DND = 0,
    ICON_ONLINE = 1,
    ICON_OFFLINE = 2,
    ICON_AWAY = 3,
    ICON_STALKER = 4,
    ICON_XAWAY = 5,
    ICON_CHAT = 6,
    ICON_GROUP = 7,
    ICON_MESSAGE = 8,
    ICON_SECURE = 9,
    ICON_EDITGROUP = 10
  };


  guiContactTree() { }

  void Rebuild();

  guiContactTree(wxWindow *parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style);
  virtual ~guiContactTree();
  
  bool SetItemTreeStatus(const wxString& name, int status);
  bool SetItemTreeText(const wxString& name, const wxString& text);

  wxTreeItemId Append(const wxString& name, const wxTreeItemId& idParent);
  wxTreeItemId Prepend(const wxString& name, const wxTreeItemId& idParent);

  wxTreeItemId InsertAfter(const wxString& name, const wxTreeItemId& afterthis, const wxTreeItemId& idParent);
  wxTreeItemId InsertAt(const wxString& name, int index, const wxTreeItemId& idParent);

  wxTreeItemId AddFolder(const wxString& name, const wxTreeItemId& idParent);
  wxTreeItemId InsertFolderAfter(const wxString& name, const wxTreeItemId& afterthis, const wxTreeItemId& idParent);

  void OnBeginDrag(wxTreeEvent& event);
  void OnBeginRDrag(wxTreeEvent& event);
  void OnEndDrag(wxTreeEvent& event);
  void OnBeginLabelEdit(wxTreeEvent& event);
  void OnEndLabelEdit(wxTreeEvent& event);
  void OnDeleteItem(wxTreeEvent& event);
  void OnRMouseUp(wxMouseEvent& event);
  void OnGetInfo(wxTreeEvent& event);
  void OnTreeRMouseClick(wxTreeEvent& event);
  void OnItemRightClick(wxTreeEvent& event);
  void OnSetInfo(wxTreeEvent& event);
  void OnItemExpanded(wxTreeEvent& event);
  void OnItemExpanding(wxTreeEvent& event);
  void OnItemCollapsed(wxTreeEvent& event);
  void OnItemCollapsing(wxTreeEvent& event);
  void OnSelChanged(wxTreeEvent& event);
  void OnSelChanging(wxTreeEvent& event);
  void OnTreeKeyDown(wxTreeEvent& event);
  void OnItemActivated(wxTreeEvent& event);
  void OnRMouseDClick(wxMouseEvent& event);

  void OnAddContact(wxCommandEvent& event);

  void GetItemsRecursively(const wxTreeItemId& idParent, long cookie);

  void CreateImageList(int size = 16);
 // void CreateButtonsImageList(int size = 11);

  void AddTestItemsToTree(size_t numChildren, size_t depth);

  void DoSortChildren(const wxTreeItemId& item, bool reverse = FALSE)
        { m_reverseSort = reverse; wxGenericTreeCtrl::SortChildren(item); }
  void DoEnsureVisible() { EnsureVisible(m_lastItem); }

  void DoToggleIcon(const wxTreeItemId& item);

  void ShowMenu(wxTreeItemId id, const wxPoint& pt);

  int ImageSize(void) const { return m_imageSize; }

  guiContact& GetOwner() { return *m_owner; }

protected:

  void PrivateDoSetStatus(wxTreeItemId &id, int status);
  virtual int OnCompareItems(const wxTreeItemId& i1, const wxTreeItemId& i2);

  // is this the test item which we use in several event handlers?
  bool IsTestItem(const wxTreeItemId& item)
  {
    // the test item is the first child folder
    return GetParent(item) == GetRootItem() && !GetPrevSibling(item);
  }

private:

  guiContact * m_owner;

  wxTreeItemId SearchItemsRecursively(const wxTreeItemId& idParent, long cookie, const wxString& name, wxTreeItemId& found);

  void AddItemsRecursively(const wxTreeItemId& idParent,
                             size_t nChildren,
                             size_t depth,
                             size_t folder);

  int          m_imageSize;               // current size of images
  bool         m_reverseSort;             // flag for OnCompareItems
  wxTreeItemId m_lastItem,                // for OnEnsureVisible()
               m_draggedItem;             // item being dragged right now

  // NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
  //     if you want your overloaded OnCompareItems() to be called.
  //     OTOH, if you don't want it you may omit the next line - this will
  //     make default (alphabetical) sorting much faster under wxMSW.
//DECLARE_DYNAMIC_CLASS(guiContactTree)
  DECLARE_EVENT_TABLE()
};

class MyTreeItemData : public wxTreeItemData
{
public:
  MyTreeItemData(const wxString& desc) : m_desc(desc) { }

  void ShowInfo(wxGenericTreeCtrl *tree);

  const wxChar *GetDesc() const { return m_desc.c_str(); }
  int GetStatus() const { return m_status; }
  void SetDesc(const wxString& text) { m_desc = text; }
  void SetStatus(int status) { m_status = status; }

private:
  int m_status;
  wxString m_desc;
};

#endif
