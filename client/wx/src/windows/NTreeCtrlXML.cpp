#include "NInclude.h"

#include "wx/treebase.h"
#include "wx/generic/treectlg.h"

#include "xmlnode.h"

#include "NTreeCtrlXML.h"

NTreeCtrlXML::NTreeCtrlXML(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style, XMLNode& xml)
                       :wxGenericTreeCtrl(parent, id, pos, size, style), m_xml(xml)
{
}

NTreeCtrlXML::~NTreeCtrlXML()
{

}
    
XMLNode NTreeCtrlXML::GetXML()
{

    return m_xml;

}

void NTreeCtrlXML::SetXML(XMLNode &xml)
{
    m_xml = xml;

    Refresh();
}

void NTreeCtrlXML::Refresh()
{

}