// --*-c++-*--
/*
    $Id: NTaskbar.h,v 1.2 2002/06/19 19:14:43 thementat Exp $
 
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
    $Log: NTaskbar.h,v $
    Revision 1.2  2002/06/19 19:14:43  thementat
    Working towards GCC 3.0.4 compile, many modifications and new automake-1.5 files.

    Revision 1.1.1.1  2002/06/06 17:21:41  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.2  2001/12/12 19:25:25  mentat
    Fixes for gcc 2.95.3 and new makefile to build (for now until I sort out automake).
    Builds in FreeBSD4.4 with gcc 2.95.3 and wxWindows 2.3.2

    Revision 1.1  2001/12/05 05:26:59  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

*/
#ifndef NNIM_GUI_TASKBAR_H
#define NNIM_GUI_TASKBAR_H
#ifdef _WIN32
/**
 * Contact window task bar icon class
 * 
 * Controlled task bar icon behavior in MSW
 *
 * @author Jesse Lovelace
 * @version 0.1
 */
#include <wx/msw/taskbar.h>

class NNIMTaskBarIcon: public wxTaskBarIcon
{
public:

  NNIMTaskBarIcon() {}

  virtual void OnRButtonUp(wxEvent&);
  virtual void OnLButtonDClick(wxEvent&);

  void OnCommandEvent(wxCommandEvent &event);
  void OnMenuCommand(int id);

private:

  DECLARE_EVENT_TABLE()
};
#endif
#endif
