// --*-c++-*--
/*
    $Id: NAbout.h,v 1.1 2002/06/06 17:21:37 thementat Exp $
 
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
    $Log: NAbout.h,v $
    Revision 1.1  2002/06/06 17:21:37  thementat
    Initial revision

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.


*/

#ifndef __NNIM_GUI_ABOUT__
#define __NNIM_GUI_ABOUT__

class guiAbout: public wxFrame
{
public:
    guiAbout(const wxString& title, const wxPoint& pos, const wxSize& size, wxWindow * parent);

};

#endif
