// --*-c++-*--
/*
    $Id: NEvent.cpp,v 1.3 2002/06/20 16:21:56 thementat Exp $
 
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
    : wxEvent(id) 
{
	SetEventType(commandType);
}

gmContactListEvent::gmContactListEvent(wxEventType commandType, int id)
    : wxEvent(id)
{
	SetEventType(commandType);
}
