#ifndef NCONTACTTREEXML_H
#define NCONTACTTREEXML_H

#include "wx/treebase.h"
#include "wx/generic/treectlg.h"

#include "NInclude.h"

using namespace std;

class NContactTreeXML: public wxGenericTreeCtrl
{
public:
    enum 
    {
        ContactTree, ItemTree
    };
    
    NContactTreeXML(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style, XMLNode xml, int type = ContactTree);

    virtual ~NContactTreeXML();

    void SetXML(XMLNode xml);
    void RefreshTree();

    void OnBeginDrag(wxTreeEvent& event);
    void OnEndDrag(wxTreeEvent& event);

    void OnEndLabelEdit(wxTreeEvent& event);
    void OnDeleteItem(wxCommandEvent& event);
	void OnItemActivated(wxTreeEvent& event);
	void OnItemExpanding(wxTreeEvent& event);
	void OnItemExpanded(wxTreeEvent& event);

    void OnRMouseUp(wxMouseEvent& event);

    void OnChangeStatus(gmEvent& event);

    void OnAddContact(wxCommandEvent& event);
    void OnAddFolder(wxCommandEvent& event);
    void OnAddItem(wxCommandEvent& event);
	void OnTreeDelete(wxCommandEvent & event);
	void OnTreeRename(wxCommandEvent & event);

	wxTreeItemId AddTreeFolder(const wxString& name, const wxTreeItemId& idParent);
    wxTreeItemId AddTreeContact(const wxString& name, const wxTreeItemId& idParent);
    wxTreeItemId AddItem(const wxString& name, const wxString& data, const wxTreeItemId& idParent);

protected:
	wxString NewFolder(const wxString& base);

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
    void PlaceItemsInTree(wxTreeItemId &base,  XMLNode& xml);
    void PlaceContactsInTree(wxTreeItemId &base,  XMLNode& xml);

    void SetStatus(wxTreeItemId &id, int status);

    void LoadFromXML();
    void CreateImageList(int size = 16);
    void ShowMenu(wxTreeItemId id, const wxPoint& pt);

private:

	wxString m_user;
    int m_type;
    XMLNode m_xml;
    DECLARE_EVENT_TABLE()


};

#endif
