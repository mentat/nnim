#ifndef __NEVENT_H__
#define __NEVENT_H__

#include "wx/event.h"
#include "contact.h"
#include "wx/treebase.h"

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(gmEVT_STATUS_CHANGE, 50)
	DECLARE_EVENT_TYPE(gmEVT_STATE_CHANGE, 51)
	DECLARE_EVENT_TYPE(gmEVT_ERROR, 52)
	DECLARE_EVENT_TYPE(gmEVT_MESSAGE, 53)
	DECLARE_EVENT_TYPE(gmEVT_REFRESH_CONTACTS, 54)
	DECLARE_EVENT_TYPE(gmEVT_REFRESH_NETWORK, 55)
	DECLARE_EVENT_TYPE(gmEVT_SETTING_CHANGED, 56)
	DECLARE_EVENT_TYPE(gmEVT_LIST_ADD, 57)
END_DECLARE_EVENT_TYPES()

class gmEvent : public wxEvent
{
public:
  gmEvent(wxEventType commandType = wxEVT_NULL, int id = 0);

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
