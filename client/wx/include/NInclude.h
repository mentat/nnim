// --*-c++-*--
/*
    $Id: NInclude.h,v 1.3 2002/06/14 22:02:23 thementat Exp $
 
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
*/
/*
Datatype prefixes are as follows:

    n  -- int, short
    l  -- long, size_t (specific 32 bit integer)
    ll -- longlong (64 bit integer)
    f  -- float
    d  -- double
    w  -- word
    dw -- dword
    h  -- handle
    u  -- unsigned
    c  -- char/byte (8 bit)
    z  -- char* string
    s  -- string class string
    p  -- pointer
    q  -- smart (counted) pointer
    r  -- reference
    a  -- array
    v  -- user or library defined datatype (can be a struct, typedef, or even
          a class variable).
    sx -- wxString
    ID_ - Event ID (global)

    m_ -- class member variable
    g_ -- global
*/

#ifndef GUI_DEFINES_H
#define GUI_DEFINES_H

// for VC6's long-winded template name warning
#ifdef __WXMSW__
  #pragma warning(disable:4786)
#endif

#include <string>
#include <map>
#include <vector>

#include "crypto/misc.h"

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __WIN32__
	#define NO_VARIABLE_HEIGHT
#endif

#if !wxUSE_DRAG_AND_DROP
    #error This program requires drag and drop support
#endif

// exception class

#include "gmException.h"

#define NNIM_GUI_VER                         wxT("Beta 1.3")

#define STATUS_OFFLINE                        100
#define STATUS_ONLINE                         101
#define STATUS_AWAY                           102
#define STATUS_DND                            103
#define STATUS_NA                             104
#define STATUS_FFC                            105
#define STATUS_FOLDER                         199
#define STATUS_CUSTOM                         200

/* wx Colour Database:

  AQUAMARINE, BLACK, BLUE, BLUE VIOLET, BROWN, CADET BLUE, CORAL, 
  CORNFLOWER BLUE, CYAN, DARK GREY, DARK GREEN, DARK OLIVE GREEN, 
  DARK ORCHID, DARK SLATE BLUE, DARK SLATE GREY DARK TURQUOISE, 
  DIM GREY, FIREBRICK, FOREST GREEN, GOLD, GOLDENROD, GREY, GREEN, 
  GREEN YELLOW, INDIAN RED, KHAKI, LIGHT BLUE, LIGHT GREY, LIGHT STEEL BLUE, 
  LIME GREEN, MAGENTA, MAROON, MEDIUM AQUAMARINE, MEDIUM BLUE, MEDIUM FOREST 
  GREEN, MEDIUM GOLDENROD, MEDIUM ORCHID, MEDIUM SEA GREEN, 
  MEDIUM SLATE BLUE, MEDIUM SPRING GREEN, MEDIUM TURQUOISE, MEDIUM VIOLET RED, 
  MIDNIGHT BLUE, NAVY, ORANGE, ORANGE RED, ORCHID, PALE GREEN, PINK, PLUM, PURPLE, 
  RED, SALMON, SEA GREEN, SIENNA, SKY BLUE, SLATE BLUE, SPRING GREEN, STEEL BLUE, 
  TAN, THISTLE, TURQUOISE, VIOLET, VIOLET RED, WHEAT, WHITE, YELLOW, YELLOW GREEN

*/

#define COLOUR_OFFLINE                        wxT("BLUE VIOLET")
#define COLOUR_ONLINE                         wxT("GOLD")
#define COLOUR_AWAY                           wxT("MEDIUM SEA GREAN")
#define COLOUR_DND                            wxT("WHEAT")
#define COLOUR_NA                             wxT("YELLOW GREEN")
#define COLOUR_FFC                            wxT("MAROON")
#define COLOUR_CUSTOM                         wxT("YELLOW")
#define COLOUR_FOLDER                         wxT("SKY BLUE")

#define HASH_TABLE_SIZE                       2000

#define TOC_SERVER        "toc.oscar.aol.com"
#define TOC_SERVER_PORT   "9898"
#define TOC_LOGIN_SERVER  "login.oscar.aol.com"
#define TOC_LOGIN_PORT    "5190"

#define ICQ_SERVER        "login.icq.com"
#define ICQ_PORT          "5190"

#define MSN_SERVER        "messenger.hotmail.com"
#define MSN_PORT          "1863"

#define GNU_SERVER        "p230.ryd.student.liu.se"
#define GNU_PORT          "4444"

#define YAHOO_DATA_SERVER  "edit.messenger.yahoo.com"
#define YAHOO_DATA_PORT    "80"
#define YAHOO_AUTH_SERVER  "edit.messenger.yahoo.com"
#define YAHOO_AUTH_PORT    "80"


static wxString WXITOA( long int num ) { wxString tmp; tmp.sprintf(wxT("%d"), num);  return tmp; }

using namespace std;
static bool string2bool(const wxString& input)
{
  if (input == wxT("TRUE"))
    return true;
  return false;
}

static wxString bool2string(bool input)
{
  if (input)
    return wxT("TRUE");
  return wxT("FALSE");
}

template<class T> inline void destroy(T*& p) { delete p; p = 0; }


#endif
/*
    -----
    $Log: NInclude.h,v $
    Revision 1.3  2002/06/14 22:02:23  thementat
    Large work on revamping IDs in gui, more SSH2 additions.

    Revision 1.2  2002/06/08 18:34:21  thementat
    Added comments and fixed VC7 project dirs

    Revision 1.1.1.1  2002/06/06 17:21:42  thementat
    Checkin of new sources BETA 2

    Revision 1.1  2001/12/18 04:09:27  mentat
    Restructuring all files.

    Revision 1.8  2001/12/17 18:01:37  mentat
    NNIM Beta 1 import.

    Revision 1.7  2001/12/08 21:46:23  mentat
    Client is working better, configuration now adds defaults for protocols to user's config.

    Revision 1.6  2001/12/05 05:26:58  mentat
    Fixed taskbar crash, fixed logout crash, toc almost works. :) wxNetwork rewritten with events, works well.

    Revision 1.5  2001/12/02 21:40:26  mentat
    Major restructure of all source, preparing for more changes and release.i


*/