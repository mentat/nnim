// --*-c++-*--
/*
    $Id: NContactTreeXML.cpp,v 1.3 2002/06/16 04:08:28 thementat Exp $
 
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

#include "boost/smart_ptr.hpp"

#include "NInclude.h"
#include "NEvent.h"
#include "NMain.h"
#include "NContactWiz.h"

#include "wx/treebase.h"
#include "wx/treectrl.h"
#include "wx/generic/treectlg.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/font.h"

#include "xmlnode.h"
#include "authload.h"

#include "NContactTreeXML.h"

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

#define ID_NEW_CONTACT_WIZARD   18000

using namespace std;

DECLARE_APP(wxNNIM)

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

BEGIN_EVENT_TABLE(NContactTreeXML, wxGenericTreeCtrl)

EVT_TREE_BEGIN_DRAG(wxNNIM::ID_TREE_CONTROL, NContactTreeXML::OnBeginDrag)
EVT_TREE_END_DRAG(wxNNIM::ID_TREE_CONTROL, NContactTreeXML::OnEndDrag)
//EVT_TREE_DELETE_ITEM(ID_TREE_CONTROL, NContactTreeXML::OnDeleteItem)
EVT_RIGHT_UP(NContactTreeXML::OnRMouseUp)
EVT_TREE_ITEM_ACTIVATED(wxNNIM::ID_TREE_CONTROL, NContactTreeXML::OnItemActivated)  
EVT_TREE_ITEM_EXPANDING(wxNNIM::ID_TREE_CONTROL, NContactTreeXML::OnItemExpanding)	
EVT_TREE_ITEM_EXPANDED(wxNNIM::ID_TREE_CONTROL, NContactTreeXML::OnItemExpanded)
EVT_TREE_END_LABEL_EDIT(wxNNIM::ID_TREE_CONTROL, NContactTreeXML::OnEndLabelEdit)
EVT_MENU(wxNNIM::ID_TREE_DELETE, NContactTreeXML::OnDeleteItem)
EVT_MENU(wxNNIM::ID_TREE_ADD_CONTACT, NContactTreeXML::OnAddContact)
EVT_MENU(wxNNIM::ID_TREE_ADD_FOLDER, NContactTreeXML::OnAddFolder)
EVT_MENU(wxNNIM::ID_TREE_DELETE, NContactTreeXML::OnTreeDelete)
EVT_MENU(wxNNIM::ID_TREE_RENAME, NContactTreeXML::OnTreeRename)

END_EVENT_TABLE()

NContactTreeXML::NContactTreeXML(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style, XMLNode xml, int type)
                       : wxGenericTreeCtrl(parent, id, pos, size, style)
{

    m_type = type;
    if (type == ContactTree)
    {
        CreateImageList();
        SetIndent(5);
        SetSpacing(0);
    }


    wxTreeItemId rootId = AddRoot("", ICON_SECURE, ICON_SECURE, new MyTreeItemData("Root Node"));
    ((MyTreeItemData *)GetItemData(rootId))->SetStatus(STATUS_FOLDER);

    SetWindowStyle(wxTR_NO_BUTTONS | wxTR_FULL_ROW_HIGHLIGHT | wxTR_NO_LINES | wxTR_HIDE_ROOT);

    SetIndent(5);
    SetSpacing(0);

    m_xml = xml;
    LoadFromXML();

}

NContactTreeXML::~NContactTreeXML()
{

}

void NContactTreeXML::LoadFromXML()
{
/*
	wxLogDebug(wxT("In LoadFromXML"));
    if (m_type == ContactTree)
        PlaceContactsInTree(GetRootItem(), m_xml);
    else
        PlaceItemsInTree(GetRootItem(), m_xml);
   */ 
}

/*void NContactTreeXML::OnMenuDeleted(wxCommandEvent& event)
{
	wxTreeItemId id = GetSelection();
	Delete(id);

}*/

void NContactTreeXML::PlaceItemsInTree(wxTreeItemId &base,  XMLNode& xml)
{

	wxLogDebug(wxT("Placing items in tree."));
    wxTreeItemId temp_root;

    for (int i = 1; i <= xml.numChildren("item"); i++)
    {
        XMLNode current(xml.child("item", i));
        temp_root = AddItem(current.property("name").c_str(), current.data().c_str(), base);
        PlaceItemsInTree(temp_root, xml.child("item", i));
    }

}

void NContactTreeXML::OnTreeDelete(wxCommandEvent & event)
{

}

void NContactTreeXML::OnTreeRename(wxCommandEvent &event)
{
	wxTreeItemId id = GetSelection();

	if (!id.IsOk())
		return;

	Edit(id);
	
}

void NContactTreeXML::OnItemExpanding(wxTreeEvent& event){ event.Skip();}
void NContactTreeXML::OnItemExpanded(wxTreeEvent& event) { event.Skip(); }

void NContactTreeXML::PlaceContactsInTree(wxTreeItemId &base, XMLNode& xml)
{
	wxLogDebug(wxT("Placing contacts in tree."));
    wxTreeItemId temp_root;

    int i(0);

    for (i = 1; i <= xml.numChildren("contact"); i++)
    {
        XMLNode current(xml.child("contact", i));
        AddTreeContact(current.property("name").c_str(), base);
		wxLogDebug(wxT("adding contact"));
    }
    
    for (i = 1; i <= xml.numChildren("folder"); i++)
    {
        XMLNode current(xml.child("folder", i));
        temp_root = AddTreeFolder(current.property("name").c_str(), base);
        PlaceContactsInTree(temp_root, xml.child("folder", i));
		wxLogDebug(wxT("adding folder"));
    }
}


void NContactTreeXML::SetXML(XMLNode xml)
{
   m_xml = xml;
   LoadFromXML();
}

void NContactTreeXML::RefreshTree()
{
	wxLogDebug(wxT("In Refresh"));
    DeleteAllItems();

    wxTreeItemId rootId = AddRoot(wxT("root"), ICON_SECURE, ICON_SECURE, new MyTreeItemData(wxT("Root Node")));
    ((MyTreeItemData *)GetItemData(rootId))->SetStatus(STATUS_FOLDER);


    LoadFromXML();

}

void NContactTreeXML::OnBeginDrag(wxTreeEvent& event)
{}

void NContactTreeXML::OnEndDrag(wxTreeEvent& event)
{}

void NContactTreeXML::OnEndLabelEdit(wxTreeEvent& event)
{
	wxString name;
	wxTreeItemId id = event.GetId();

	if (!id.IsOk())
	{
		wxLogDebug(wxT("Item not ok"));
		event.Veto();
	}
	name = GetItemText(id);

	if ((name.Find('<') > -1) || (name.Find('>') > -1) || (name.Find('\"') > -1) || (name.Find('\'') > -1))
	{
		wxMessageBox(wxT("Sorry, the name can't contain <>\"\'"), wxT("Name Error"), wxOK, this);
		event.Veto();
	}
}

void NContactTreeXML::OnDeleteItem(wxCommandEvent& event)
{
	wxTreeItemId id = GetSelection();

	if (!id.IsOk())
		return;

	if (m_type == ContactTree)
	{
		if (((MyTreeItemData *)GetItemData(id))->GetStatus() == STATUS_FOLDER)
			wxGetApp().AccessLoader().C().DeleteFolder(GetItemText(id).mb_str(wxConvUTF8).data());
		else
			wxGetApp().AccessLoader().C().Delete(GetItemText(id).mb_str(wxConvUTF8).data());
	}
	else
	{
		wxGetApp().AccessLoader().C().DeleteInfo(m_user.mb_str(wxConvUTF8).data(), GetItemText(id).mb_str(wxConvUTF8).data());
	}

	RefreshTree();


}

void NContactTreeXML::OnItemActivated(wxTreeEvent& event)
{
	wxLogDebug(wxT("OnItemActivated"));
	wxTreeItemId item = event.GetItem();
	if (!item.IsOk())
	{
		wxLogDebug(wxT("item not ok"));
		return;
	}

	if (((MyTreeItemData *)GetItemData(item))->GetStatus() == STATUS_FOLDER)
	{
		wxLogDebug(wxT("item is folder"));

		if (HasChildren(item))
			wxLogDebug(wxT("item has child"));
		if (IsExpanded(item))
			Collapse(item);
		else
			Expand(item);
	}
}

void NContactTreeXML::OnRMouseUp(wxMouseEvent& event)
{
    wxPoint pt = event.GetPosition();
    ShowMenu(HitTest(pt), pt);
}


void NContactTreeXML::CreateImageList(int size)
{
    
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

void NContactTreeXML::ShowMenu(wxTreeItemId id, const wxPoint& pt)
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
	Unselect();
  }

  wxMenu menu(title);
  menu.Append(wxNNIM::ID_TREE_ABOUT, wxT("&About..."));
  menu.AppendSeparator();
  menu.Append(wxNNIM::ID_TREE_DELETE, wxT("&Delete"));
  menu.Append(wxNNIM::ID_TREE_EDIT, wxT("&Edit"));
  menu.Append(wxNNIM::ID_TREE_RENAME, wxT("&Rename"));
  menu.AppendSeparator();
  menu.Append(wxNNIM::ID_TREE_ADD_CONTACT, wxT("Add Contact"));
  menu.Append(wxNNIM::ID_TREE_ADD_FOLDER, wxT("Add Folder"));
  //menu.Append(ID_TREE_EXPAND, wxT("Expand"));

  PopupMenu(&menu, pt);

}

wxTreeItemId NContactTreeXML::AddTreeFolder(const wxString& name, const wxTreeItemId& idParent)
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
  SetStatus(id, STATUS_FOLDER);
  return id;
}

wxTreeItemId NContactTreeXML::AddTreeContact(const wxString& name, const wxTreeItemId& idParent)
{
  wxTreeItemId realParent, id;

  if (((MyTreeItemData *)GetItemData(idParent))->GetStatus() != STATUS_FOLDER)
  {
    realParent = GetParent(idParent);
    id = AppendItem(realParent, name, 0, 0, new MyTreeItemData(wxString(wxT("Contact: ")) + name));
  }
  else
  {
    id = AppendItem(idParent, name,0, 0, new MyTreeItemData(wxString(wxT("Contact: ")) + name));
  }

  SetStatus(id, STATUS_OFFLINE);

  return id;
}


void NContactTreeXML::SetStatus(wxTreeItemId &id, int status)
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
  wxFont Cirth;
  Cirth.SetFaceName("Dwarf Runes");

  SetItemFont(id, Cirth);

}
wxString NContactTreeXML::NewFolder(const wxString& base)
{
	wxString name = wxGetTextFromUser(wxT("New Folder Name"), wxT("New Folder"), "", this);	if (name == wxT(""))		return wxT("");	AuthLoad &myLoader = wxGetApp().AccessLoader();	if (myLoader.C().FolderExists(name.mb_str(wxConvUTF8).data()))	{		wxMessageBox(wxT("Sorry, this folder already exists."), wxT("Folder Exists"));		return wxT("");	}	if (base == wxT(""))	{		if (!myLoader.C().AddFolder(name.mb_str(wxConvUTF8).data()))		{			wxLogError(wxT("Folder not added with no base."));			return "";		}		else			return name;	}		if (!myLoader.C().AddFolder(name.mb_str(wxConvUTF8).data(), base.mb_str(wxConvUTF8).data()))	{		wxLogError(wxT("This folder wasn't added."));		return wxT("");	}

	return name;
}

void NContactTreeXML::OnAddContact(wxCommandEvent& event)
{
    wxTreeItemId id = GetSelection();
	
	if (!id.IsOk())
	{
		id = GetRootItem();
	}
	else
	{
		if (((MyTreeItemData *)GetItemData(id))->GetStatus() != STATUS_FOLDER)
			id = GetParent(id);
	}

	RunContactWizard(this, id);

}

void NContactTreeXML::OnAddFolder(wxCommandEvent& event)
{
	wxLogDebug(wxT("OnAddFolder"));
	wxTreeItemId id = GetSelection();
	wxString root;

	if (!id.IsOk())
	{
		root="";
	}
	else
	{
		if (((MyTreeItemData *)GetItemData(id))->GetStatus() == STATUS_FOLDER)
			root = GetItemText(id);
		else
		{
			root = GetItemText(GetParent(id));
			id = GetParent(id);
		}
	}
	
	wxLogDebug(wxString("Root is: ") + root);
	
	wxString name(NewFolder(root));

	if (name != "")
	{
		AddTreeFolder(name, id);
	}

}

void NContactTreeXML::OnChangeStatus(gmEvent& event)
{
}

wxTreeItemId NContactTreeXML::AddItem(const wxString&name, const wxString&data, const wxTreeItemId & idParent)
{
	return wxTreeItemId();
}