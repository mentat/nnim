#ifndef __NTREECTRLXML_H__
#define __NTREECTRLXML_H__


#include "wx/treebase.h"
#include "wx/generic/treectlg.h"

class NTreeCtrlXML: public wxGenericTreeCtrl
{
public:
    NTreeCtrlXML(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style, XMLNode& xml);

    virtual ~NTreeCtrlXML();
    
    XMLNode GetXML();

    virtual void SetXML(XMLNode &xml);

    virtual void Refresh();


private:

    XMLNode m_xml;

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
