#include "NInclude.h"

#include "wx/event.h"
#include "NEvent.h"

DEFINE_EVENT_TYPE(gmEVT_STATUS_CHANGE)
DEFINE_EVENT_TYPE(gmEVT_STATE_CHANGE)
DEFINE_EVENT_TYPE(gmEVT_ERROR)
DEFINE_EVENT_TYPE(gmEVT_MESSAGE)
DEFINE_EVENT_TYPE(gmEVT_REFRESH_CONTACTS)
DEFINE_EVENT_TYPE(gmEVT_REFRESH_NETWORK)
DEFINE_EVENT_TYPE(gmEVT_SETTINGS_CHANGED)
DEFINE_EVENT_TYPE(gmEVT_LIST_ADD)

IMPLEMENT_DYNAMIC_CLASS(gmContactListEvent, wxEvent)
IMPLEMENT_DYNAMIC_CLASS(gmEvent, wxEvent)

gmEvent::gmEvent(wxEventType commandType, int id)
    : wxEvent(id, commandType) 
{

}

gmContactListEvent::gmContactListEvent(wxEventType commandType, int id)
    : wxEvent(id, commandType)
{

}