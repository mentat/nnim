#ifndef __NEVENT_H__
#define __NEVENT_H__

#include "wx/event.h"
#include "contact.h"
#include "wx/treebase.h"

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(gmEVT_STATUS_CHANGE, 13000)
	DECLARE_EVENT_TYPE(gmEVT_STATE_CHANGE, 13001)
	DECLARE_EVENT_TYPE(gmEVT_ERROR, 13002)
	DECLARE_EVENT_TYPE(gmEVT_MESSAGE, 13003)
	DECLARE_EVENT_TYPE(gmEVT_REFRESH_CONTACTS, 13004)
	DECLARE_EVENT_TYPE(gmEVT_REFRESH_NETWORK, 13005)
	DECLARE_EVENT_TYPE(gmEVT_SETTING_CHANGED, 13006)
	DECLARE_EVENT_TYPE(gmEVT_LIST_ADD, 13007)
    DECLARE_EVENT_TYPE(gmEVT_MESSAGE_ANONY, 13008)
END_DECLARE_EVENT_TYPES()

class gmEvent : public wxEvent
{
public:
  gmEvent(wxEventType commandType = wxEVT_NULL, int id = 0);
  gmEvent(const gmEvent& event) : wxEvent(event)
  {
    m_status = event.m_status;
    m_serverId = event.m_serverId;
    m_message = event.m_message;
    m_protocol = event.m_protocol;
    contact = event.contact;
  }


  void setProtocol(const wxString& proto) { m_protocol = proto; }
  void setServerId(const wxString& id) { m_serverId = id; }
  void setMessage(const wxString& msg) { m_message = msg; }
  void setStatus(const int & status) { m_status = status; }

  wxString getProtocol() { return m_protocol; }
  wxString getServerId() { return m_serverId; }
  wxString getMessage() { return m_message; }
  int getStatus() { return m_status;}

  Contact contact;

  virtual wxEvent *Clone() const { return new gmEvent(*this); }

public:
  int m_status;
  wxString m_protocol;
  wxString m_serverId;
  wxString m_message;

private:
  DECLARE_DYNAMIC_CLASS(gmEvent)
};

class gmContactListEvent: public wxEvent
{
public:

  enum { ADDCONTACT, REMOVECONTACT, LAUNCHCHAT };

  gmContactListEvent(wxEventType commandType = wxEVT_NULL,int id = 0);
  gmContactListEvent(const gmContactListEvent& event):
    wxEvent(event),
    m_contact(event.m_contact),
    m_type(event.m_type)
    {}


  int GetType() { return m_type; }

  void SetType(int type) { m_type = type; }

  wxTreeItemId & GetItem() { return m_contact; }
  void SetItem(wxTreeItemId& id) { m_contact = id; }

  virtual wxEvent *Clone() const { return new gmContactListEvent(*this); }

private:

  DECLARE_DYNAMIC_CLASS(gmContactListEvent)
  wxTreeItemId m_contact;
  int m_type;


};

#define EVT_GM_NETWORK(id, fn)  DECLARE_EVENT_TABLE_ENTRY( gmEVT_MESSAGE, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, (wxObject *) NULL  ),

#endif
