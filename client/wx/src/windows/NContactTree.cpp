// --*-c++-*--
/*
    $Id: NContactTree.cpp,v 1.1 2002/06/06 17:21:30 thementat Exp $
 
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
    $Log: NContactTree.cpp,v $
    Revision 1.1  2002/06/06 17:21:30  thementat
    Initial revision

    Revision 1.11  2002/01/17 20:00:50  mentat
    Moved dirs back to normal.

    Revision 1.1  2001/12/18 04:10:39  mentat
    Restructuring all sources. Beta 1.

    Revision 1.9  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.8  2001/12/12 19:25:25  mentat
    Fixes for gcc 2.95.3 and new makefile to build (for now until I sort out automake).
    Builds in FreeBSD4.4 with gcc 2.95.3 and wxWindows 2.3.2

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/
/* Many thanks to Julian Smart for the treectrl sample which this class has been
    inspired by. */

#include "NInclude.h"
#include "math.h"

#include "wx/log.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treebase.h"
#include "wx/generic/treectlg.h"

#include "NContact.h"
#include "NContactTree.h"
#include "NMain.h"

#include "authload.h"

#include "bitmaps/extra/dnd.xpm"
#include "bitmaps/extra/online.xpm"
#include "bitmaps/extra/offline.xpm"
#include "bitmaps/extra/gnome-message.xpm"
#include "bitmaps/extra/stalker.xpm"
#include "bitmaps/extra/xa.xpm"
#include "bitmaps/extra/chat.xpm"
#include "bitmaps/extra/glade-group.xpm"
#include "bitmaps/extra/glade-message.xpm"
#include "bitmaps/extra/gnome-gpg.xpm"
#include "bitmaps/extra/gnome-editgroups.xpm"

BEGIN_EVENT_TABLE(guiContactTree, wxGenericTreeCtrl)
    EVT_TREE_BEGIN_DRAG(ID_TREE_CONTROL, guiContactTree::OnBeginDrag)
    EVT_TREE_BEGIN_RDRAG(ID_TREE_CONTROL, guiContactTree::OnBeginRDrag)
    EVT_TREE_END_DRAG(ID_TREE_CONTROL, guiContactTree::OnEndDrag)
    EVT_TREE_DELETE_ITEM(ID_TREE_CONTROL, guiContactTree::OnDeleteItem)
    EVT_TREE_SET_INFO(ID_TREE_CONTROL, guiContactTree::OnSetInfo)
    EVT_TREE_ITEM_EXPANDED(ID_TREE_CONTROL, guiContactTree::OnItemExpanded)
    EVT_TREE_ITEM_EXPANDING(ID_TREE_CONTROL, guiContactTree::OnItemExpanding)
    EVT_TREE_ITEM_COLLAPSED(ID_TREE_CONTROL, guiContactTree::OnItemCollapsed)
    EVT_TREE_ITEM_COLLAPSING(ID_TREE_CONTROL, guiContactTree::OnItemCollapsing)
    EVT_TREE_ITEM_RIGHT_CLICK(ID_TREE_CONTROL, guiContactTree::OnItemRightClick)

    EVT_RIGHT_UP(guiContactTree::OnRMouseUp)
    EVT_TREE_SEL_CHANGED(ID_TREE_CONTROL, guiContactTree::OnSelChanged)
    EVT_TREE_SEL_CHANGING(ID_TREE_CONTROL, guiContactTree::OnSelChanging)
    EVT_TREE_KEY_DOWN(ID_TREE_CONTROL, guiContactTree::OnTreeKeyDown)
    EVT_TREE_ITEM_ACTIVATED(ID_TREE_CONTROL, guiContactTree::OnItemActivated)
    EVT_RIGHT_DCLICK(guiContactTree::OnRMouseDClick)

    EVT_MENU(ID_TREE_ADD_CONTACT, guiContactTree::OnAddContact)
END_EVENT_TABLE()

//IMPLEMENT_DYNAMIC_CLASS(guiContactTree, wxGenericTreeCtrl)

guiContactTree::guiContactTree(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
          : wxGenericTreeCtrl(parent, id, pos, size, style)
{

  m_owner = (guiContact *) parent;

  m_reverseSort = FALSE;

  CreateImageList();

  wxTreeItemId rootId = AddRoot("root", ICON_SECURE, ICON_SECURE, new MyTreeItemData("Root Node"));
  ((MyTreeItemData *)GetItemData(rootId))->SetStatus(STATUS_FOLDER);

  SetWindowStyle(wxTR_NO_BUTTONS | wxTR_FULL_ROW_HIGHLIGHT | wxTR_NO_LINES | wxTR_HIDE_ROOT);

  SetIndent(5);
  SetSpacing(0);
}

void guiContactTree::Rebuild()
{ 
 
}

void guiContactTree::CreateImageList(int size)
{
  if ( size == -1 )
  {
    SetImageList(NULL);
    return;
  }
    
  if ( size == 0 )
    size = m_imageSize;
  else
    m_imageSize = size;

  // Make an image list containing small icons
  wxImageList *images = new wxImageList(size, size, TRUE);

  // should correspond to TreeCtrlIcon_xxx enum
  wxBusyCursor wait;
  wxIcon icons[11];

#ifdef __WXMSW__
  wxBitmap b1(dnd_xpm);
  wxBitmap b2(online_xpm);
  wxBitmap b3(offline_xpm);
  wxBitmap b4(gnome_message_xpm);
  wxBitmap b5(stalker_xpm);
  wxBitmap b6(xa_xpm);
  wxBitmap b7(chat_xpm);
  wxBitmap b8(glade_group_xpm);
  wxBitmap b9(glade_message_xpm);
  wxBitmap b10(gnome_gpg_xpm);
  wxBitmap b11(gnome_userinfo_xpm);

  icons[0].CopyFromBitmap(b1);
  icons[1].CopyFromBitmap(b2);
  icons[2].CopyFromBitmap(b3);
  icons[3].CopyFromBitmap(b4);
  icons[4].CopyFromBitmap(b5);
  icons[5].CopyFromBitmap(b6); 
  icons[6].CopyFromBitmap(b7);
  icons[7].CopyFromBitmap(b8);
  icons[8].CopyFromBitmap(b9);
  icons[9].CopyFromBitmap(b10);
  icons[10].CopyFromBitmap(b11);  
#else
  icons[0] = wxICON(dnd);
  icons[1] = wxICON(online);
  icons[2] = wxICON(offline);
  icons[3] = wxICON(gnome_message);
  icons[4] = wxICON(stalker);
  icons[5] = wxICON(xa);
  icons[6] = wxICON(chat);
  icons[7] = wxICON(glade_group);
  icons[8] = wxICON(glade_message);
  icons[9] = wxICON(gnome_gpg);
  icons[10] = wxICON(gnome_userinfo);
       
#endif

  int sizeOrig = icons[0].GetWidth();
  for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
  {
    if ( size == sizeOrig )
    {
      images->Add(icons[i]);
    }
    else
    {
      images->Add(wxImage(icons[i]).Rescale(size, size).
                                    ConvertToBitmap());
    }
  }

  AssignImageList(images);
}

guiContactTree::~guiContactTree()
{
}

void guiContactTree::PrivateDoSetStatus(wxTreeItemId &id, int status)
{
  switch ( status )
  {
  case STATUS_ONLINE: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_ONLINE);
    SetItemBackgroundColour(id, COLOUR_ONLINE); 
    SetItemImage(id, ICON_ONLINE); 
    SetItemImage(id, ICON_ONLINE, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_OFFLINE: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_OFFLINE);
    SetItemBackgroundColour(id, COLOUR_OFFLINE); 
    SetItemImage(id, ICON_OFFLINE);
    SetItemImage(id, ICON_OFFLINE, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_AWAY: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_AWAY);
    SetItemBackgroundColour(id, COLOUR_AWAY); 
    SetItemImage(id, ICON_AWAY); 
    SetItemImage(id, ICON_AWAY, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_DND: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_DND);
    SetItemBackgroundColour(id, COLOUR_DND); 
    SetItemImage(id, ICON_DND); 
    SetItemImage(id, ICON_DND, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_NA: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_NA);
    SetItemBackgroundColour(id, COLOUR_NA); 
    SetItemImage(id, ICON_XAWAY); 
    SetItemImage(id, ICON_XAWAY, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_FFC: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_FFC);
    SetItemBackgroundColour(id, COLOUR_FFC); 
    SetItemImage(id, ICON_CHAT); 
    SetItemImage(id, ICON_CHAT, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_CUSTOM: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_CUSTOM);
    SetItemBackgroundColour(id, COLOUR_CUSTOM); 
    SetItemImage(id, ICON_SECURE); 
    SetItemImage(id, ICON_SECURE, wxTreeItemIcon_Selected);
  }
  break;
  case STATUS_FOLDER:
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_FOLDER);
    SetItemBackgroundColour(id, COLOUR_FOLDER); 
    SetItemImage(id, ICON_GROUP); 
    SetItemImage(id, ICON_EDITGROUP, wxTreeItemIcon_Expanded);
    SetItemImage(id, ICON_GROUP, wxTreeItemIcon_Selected); 
    SetItemImage(id, ICON_EDITGROUP, wxTreeItemIcon_SelectedExpanded);
  }
  break;

  default: 
  {
    ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_OFFLINE);
    SetItemBackgroundColour(id, COLOUR_OFFLINE); 
    SetItemImage(id, ICON_OFFLINE); 
    SetItemImage(id, ICON_OFFLINE, wxTreeItemIcon_Selected);
  }
  break;
  }

}

bool guiContactTree::SetItemTreeStatus(const wxString& name, int status)
{
  wxLogMessage("SetStatus");

  long cookie = -1;
  wxTreeItemId id = -1;
  SearchItemsRecursively(GetRootItem(), cookie, name, id);

  if (id > 0)
  {
    wxLogDebug(wxString("Found: ") + GetItemText(id));

    MyTreeItemData * data = ( MyTreeItemData *)GetItemData(id);
    if (data == NULL)
    {
      wxLogError("data is null!!");
      return false;
    }
  
    // return if status is the same as before of it folder
    //if ((data->GetStatus() == STATUS_FOLDER) || (data->GetStatus() == status))
    //  return false;

    // update images and colours according to new status

    PrivateDoSetStatus(id, status);
    return true;
  }

  wxLogMessage("Not Found");
    
  return false;

}  

wxTreeItemId guiContactTree::SearchItemsRecursively(const wxTreeItemId& idParent, long cookie, const wxString& name, wxTreeItemId& found)
{
  wxLogMessage(wxString("Searching...") + GetItemText(idParent));
  wxTreeItemId id;

  if( cookie == -1 )
    id = GetFirstChild(idParent, cookie);
  else
    id = GetNextChild(idParent, cookie);

  if(id <= 0)
    return id;

  if (GetItemText(id) == name)
  {
    wxLogMessage("Found in recurse");
    wxString text = GetItemText(id);
    wxLogMessage(text);

    found = id;
  }

  if (ItemHasChildren(id))
    SearchItemsRecursively(id,-1, name, found);

  return SearchItemsRecursively(idParent, cookie, name, found);
}

bool guiContactTree::SetItemTreeText(const wxString& name, const wxString& text)
{

  long cookie = -1;
  wxTreeItemId id = -1;
  SearchItemsRecursively(GetRootItem(), cookie, name, id);


  if (id > 0)
  {
    SetItemText(id, text);
    return true;
  }
  wxLogDebug("Not found in SetItemTreeText");
  return false;

}

int guiContactTree::OnCompareItems(const wxTreeItemId& item1,
                               const wxTreeItemId& item2)
{
    if ( m_reverseSort )
    {
        // just exchange 1st and 2nd items
        return wxGenericTreeCtrl::OnCompareItems(item2, item1);
    }
    else
    {
        return wxGenericTreeCtrl::OnCompareItems(item1, item2);
    }
}

void guiContactTree::GetItemsRecursively(const wxTreeItemId& idParent, long cookie)
{
    wxTreeItemId id;

    if( cookie == -1 )
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if(id <= 0)
        return;

    wxString text = GetItemText(id);
    wxLogMessage(text);

    if (ItemHasChildren(id))
        GetItemsRecursively(id,-1);

    GetItemsRecursively(idParent, cookie);
}

#if 0
void guiContactTree::DoToggleIcon(const wxTreeItemId& item)
{
    int image = GetItemImage(item) == ICON_GROUP ? ICON_OFFLINE
                                                          : ICON_GROUP;

    SetItemImage(item, image);
}
#endif

void guiContactTree::OnBeginRDrag(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnDeleteItem(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnGetInfo(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnSetInfo(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnItemExpanded(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnItemExpanding(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnItemCollapsed(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnSelChanged(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnSelChanging(wxTreeEvent& event) { event.Skip(); }
void guiContactTree::OnTreeKeyDown(wxTreeEvent& event) { event.Skip(); }

void guiContactTree::OnBeginDrag(wxTreeEvent& event)
{
  // can't drag folders yet
    if ( ( event.GetItem() != GetRootItem() ) && !ItemHasChildren(event.GetItem()) )
    {
        m_draggedItem = event.GetItem();

        wxLogMessage(wxT("OnBeginDrag: started dragging %s"),
                     GetItemText(m_draggedItem).c_str());

        event.Allow();
    }
    else
    {
        wxLogMessage(wxT("OnBeginDrag: this item can't be dragged."));
    }
}

void guiContactTree::OnEndDrag(wxTreeEvent& event)
{
    wxTreeItemId itemSrc = m_draggedItem,
                 itemDst = event.GetItem();
    m_draggedItem = (wxTreeItemId)0l;

    if ( !itemDst.IsOk() )
    {
        wxLogMessage(wxT("OnEndDrag: can't drop here."));

        return;
    }
    if (itemSrc == itemDst)
    {
      wxLogDebug(wxT("Items are equal.");
      return;
    }

    if ( (GetParent(itemSrc) == GetParent(itemDst)) && (!ItemHasChildren(itemDst)) )
    {
      wxLogDebug(wxT("Items parents are equals."));
      return;
    }

    if ( GetParent(itemSrc) == itemDst )
    {
      wxLogDebug(wxT("Won't copy to same folder."));
      return;
    }

    // where to copy the item?
    if ( itemDst.IsOk() && !ItemHasChildren(itemDst) )
    {
        // copy to the parent then
        itemDst = GetParent(itemDst);
    }

    wxString text = GetItemText(itemSrc);
    wxLogMessage(wxT("OnEndDrag: '%s' copied to '%s'."),
                 text.c_str(), GetItemText(itemDst).c_str());

    int image = GetItemImage(itemSrc, wxTreeItemIcon_Normal);

    MyTreeItemData * srcData; 

    srcData = (MyTreeItemData *)GetItemData(itemSrc);

    wxTreeItemId newItem = AppendItem(itemDst, text, image, image, new MyTreeItemData((*srcData).GetDesc()));
    PrivateDoSetStatus(newItem, srcData->GetStatus());
    Delete(itemSrc);
}

void guiContactTree::OnBeginLabelEdit(wxTreeEvent& event)
{
  event.Veto();
}

void guiContactTree::OnEndLabelEdit(wxTreeEvent& event)
{
   event.Veto();
}

void guiContactTree::OnItemCollapsing(wxTreeEvent& event)
{
}

void guiContactTree::OnItemActivated(wxTreeEvent& event)
{
   /* // show some info about this item
    wxTreeItemId itemId = event.GetItem();
    MyTreeItemData *item = (MyTreeItemData *)GetItemData(itemId);



    wxLogMessage(wxT("OnItemActivated"));

    if (item->GetStatus() != STATUS_FOLDER)
    {
      ((guiContact *)m_parent)->LaunchChat(GetItemText(itemId));
    }
    else
    {
      if (IsExpanded(itemId))
        Collapse(itemId);
      else
        Expand(itemId);
    }*/

}

void guiContactTree::OnItemRightClick(wxTreeEvent& event)
{
    ShowMenu(event.GetItem(), event.GetPoint());
}

void guiContactTree::OnRMouseUp(wxMouseEvent& event)
{
    wxPoint pt = event.GetPosition();
    ShowMenu(HitTest(pt), pt);
}

void guiContactTree::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
  wxString title;
  if ( id.IsOk() )
  {
    title << _T("Menu for ") << GetItemText(id);
    SelectItem(id);
  }
  else
  {
    title = _T("Menu for no particular item");
  }

  wxMenu menu(title);
  menu.Append(ID_TREE_ABOUT, _T("&About..."));
  menu.Append(ID_TREE_CHANGE_STATUS, _T("&Change Status"));
  menu.AppendSeparator();
  menu.Append(ID_TREE_DELETE, _T("&Delete"));
  menu.Append(ID_TREE_EDIT, _T("&Edit"));
  menu.Append(ID_TREE_RENAME, _T("&Rename"));
  menu.AppendSeparator();
  menu.Append(ID_TREE_ADD_CONTACT, _T("Add Contact"));
  menu.Append(ID_TREE_ADD_FOLDER, _T("Add Folder"));

  PopupMenu(&menu, pt);

}

void guiContactTree::OnAddContact(wxCommandEvent& event)
{

}


void guiContactTree::OnRMouseDClick(wxMouseEvent& event)
{
}

void MyTreeItemData::ShowInfo(wxGenericTreeCtrl *tree)
{
    wxLogMessage(wxT("Item '%s': %sselected, %sexpanded, %sbold,\n")
                 wxT("%u children (%u immediately under this item).\n")
                 wxT("Status: %u."),
                 m_desc.c_str(),
                 bool2string(tree->IsSelected(GetId())).c_str(),
                 bool2string(tree->IsExpanded(GetId())).c_str(),
                 bool2string(tree->IsBold(GetId())).c_str(),
                 tree->GetChildrenCount(GetId()),
                 tree->GetChildrenCount(GetId(), FALSE),
                 GetStatus());
}

wxTreeItemId guiContactTree::Append(const wxString& name, const wxTreeItemId& idParent)
{
  wxTreeItemId realParent, id;

  if (((MyTreeItemData *)GetItemData(idParent))->GetStatus() != STATUS_FOLDER)
  {
    realParent = GetParent(idParent);
    id = AppendItem(realParent, name, 0, 0, new MyTreeItemData(wxString("Contact: ") + name));
  }
  else
  {
    id = AppendItem(idParent, name,0, 0, new MyTreeItemData(wxString("Contact: ") + name));
  }

  PrivateDoSetStatus(id, STATUS_OFFLINE);

	//wxTreeItemId id = AppendItem(idParent, name, ICON_OFFLINE, ICON_OFFLINE, new MyTreeItemData(wxString("Contact: ") + name));
  //((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_OFFLINE);
  //SetItemBackgroundColour(id, COLOUR_OFFLINE);
  return id;
}

wxTreeItemId guiContactTree::Prepend(const wxString& name, const wxTreeItemId& idParent)
{
	wxTreeItemId id = PrependItem(idParent, name, 0, 0, new MyTreeItemData(wxString("Contact: ") + name));
  ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_OFFLINE);
  SetItemBackgroundColour(id, COLOUR_OFFLINE);
  return id;
}

wxTreeItemId guiContactTree::InsertAfter(const wxString& name, const wxTreeItemId& afterthis, const wxTreeItemId& idParent)
{
  wxTreeItemId id = InsertItem(idParent, afterthis, name, 0, 0, new MyTreeItemData(wxString("Contact: ") + name));
  ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_OFFLINE);
  SetItemBackgroundColour(id, COLOUR_OFFLINE);
  return id;  
}

wxTreeItemId guiContactTree::InsertAt(const wxString& name, int index, const wxTreeItemId& idParent)
{
  wxTreeItemId id = InsertItem(idParent, index, name, 0, 0, new MyTreeItemData(wxString("Contact: ") + name));
  ((MyTreeItemData *)GetItemData(id))->SetStatus(STATUS_OFFLINE);
  SetItemBackgroundColour(id, COLOUR_OFFLINE);
  return id;  
}

wxTreeItemId guiContactTree::AddFolder(const wxString& name, const wxTreeItemId& idParent)
{
  wxTreeItemId realParent, id;

  if (((MyTreeItemData *)GetItemData(idParent))->GetStatus() != STATUS_FOLDER)
  {
    realParent = GetParent(idParent);
    id = AppendItem(realParent, name, 0, 0, new MyTreeItemData(wxString("Folder: ") + name));
  }
  else
  {
    id = AppendItem(idParent, name, 0, 0, new MyTreeItemData(wxString("Folder: ") + name));
  }

  PrivateDoSetStatus(id, STATUS_FOLDER);
  return id;
}

wxTreeItemId guiContactTree::InsertFolderAfter(const wxString& name, const wxTreeItemId& afterthis, const wxTreeItemId& idParent)
{
  //wxTreeItemId id = InsertItem(idParent, afterthis, name, ICON_GROUP, ICON_GROUP, new MyTreeItemData(""));
  wxTreeItemId id = InsertItem(idParent, afterthis, name, 0, 0, new MyTreeItemData(wxString("Folder: ") + name));
  
  PrivateDoSetStatus(id, STATUS_FOLDER);
  return id;
}

