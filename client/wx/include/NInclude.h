// --*-c++-*--
/*
    $Id: NInclude.h,v 1.1 2002/06/06 17:21:42 thementat Exp $
 
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

#define SOCKET_ID 6767
#define SERVER_ID	8888

#define ID_LOG_QUIT                           250
#define ID_LOG_ENTER                          251

//#define ID_PREF_TREE_CTRL                     301
#define ID_PREF_GENERAL                       401
#define ID_PREF_SECURITY                      402
#define ID_PREF_AIM                           403
#define ID_PREF_YAHOO                         404
#define ID_PREF_MSN                           405
#define ID_PREF_ICQ                           406
#define ID_PREF_KIT                           407
#define ID_PREF_PEER                          408
#define ID_PREF_NETWORKS                      409
#define ID_PREF_CANCEL                        600
#define ID_PREF_SAVE                          601
#define ID_PREF_CONTACTS                      602
#define ID_PREF_UPDATED                       603
#define ID_PREF_CONTACT_LIST                  604
#define ID_PREF_CONTACT_COMBO                 605
#define ID_PREF_ADDUSER                       606
          
//--------------- event IDs -------------------------------
#define ID_TREE_CONTROL                       1000
#define ID_TREE_ABOUT                         1001
#define ID_TREE_CHANGE_STATUS                 1002
#define ID_TREE_DELETE                        1003
#define ID_TREE_EDIT                          1004
#define ID_TREE_ADD_CONTACT                   1005
#define ID_TREE_ADD_FOLDER                    1006
#define ID_TREE_RENAME                        1007

#define ID_MENU_QUIT                          100
#define ID_MENU_ABOUT                         101
#define ID_MENU_CHECKVERSION                  102
#define ID_MENU_OPTIONS                       103
#define ID_MENU_CHATWINDOW                    104
#define ID_MENU_COMMIT                        105
#define ID_MENU_LOAD                          106
#define ID_MENU_CLEAR_CONTACTS                107
#define ID_MENU_LOGOUT                        108
#define ID_MENU_CONTACT_EDIT                  109
#define ID_MENU_START                         110

/* Note to self: these must be dynamic one day... */
#define ID_MENU_POPUP_KIT                     150
#define ID_MENU_POPUP_AIM                     151
#define ID_MENU_POPUP_ICQ                     152
#define ID_MENU_POPUP_PEER                    153

#define ID_MENU_POPUP_CONNECT                 180
#define ID_MENU_POPUP_DISCONNECT              181
#define ID_MENU_POPUP_SERVICES                182
#define ID_MENU_POPUP_PREFS                   183
#define ID_MENU_POPUP_WIZARD                  184
#define ID_MENU_POPUP_ADD_CONTACT             185
#define ID_MENU_POPUP_QUIT                    186

#define ID_PREF_TREE_CTRL                   10014
#define ID_PREF_SPLIT                       10015
#define ID_PREF_BUTT_OK                     10016
#define ID_PREF_BUTT_CANCEL                 10017
#define ID_PREF_BUTT_APPLY                  10100

#define ID_CONTACTS_MENU_POPUP_CONNECT      14000
#define ID_CONTACTS_MENU_POPUP_DISCONNECT   14001
#define ID_CONTACTS_MENU_POPUP_SERVICES     14002
#define ID_CONTACTS_MENU_POPUP_PREFS        14003
#define ID_CONTACTS_MENU_POPUP_WIZARD       14004
#define ID_CONTACTS_MENU_POPUP_ADD_CONTACT  14005
#define ID_CONTACTS_MENU_POPUP_QUIT         14006
#define ID_CONTACTS_MENU_POPUP_TESTCRASH    14007
#define ID_CONTACTS_MENU_POPUP_ADD_FOLDER   14008
#define ID_CONTACTS_TASKBAR_RESTORE         13000
#define ID_CONTACTS_TASKBAR_EXIT            13001
#define ID_CONTACTS_TASKBAR_HIDE            13002
#define ID_CONTACTS_MENU_POPUP_UPDATE_DEBUG 13003
#define ID_CONTACTS_INCOMMING_MESSAGE       13004
#define ID_CONTACTS_STATUS_CHANGE           13005

#define ID_LOGIN_LOGO                       10000
#define ID_LOGIN_USERNAME                   10002
#define ID_LOGIN_PASSWORD                   10004
#define ID_LOGIN_LOGIN                      10005
#define ID_LOGIN_MENU_NEWUSER               10075
#define ID_LOGIN_MENU_QUIT                  10076
#define ID_LOGIN_MENU_ABOUT                 10077

#define ID_NETWORK_REFRESH_CONTACTS         14000
#define ID_NETWORK_REFRESH_NETWORK          14001

#define ID_AWAY_WINDOW                      15000

#define ID_NEW_USER_WIZARD                  16000

#define STATUS_OFFLINE                        100
#define STATUS_ONLINE                         101
#define STATUS_AWAY                           102
#define STATUS_DND                            103
#define STATUS_NA                             104
#define STATUS_FFC                            105
#define STATUS_FOLDER                         199
#define STATUS_CUSTOM                         200

#define PREF_TREEITEM_CONTACT               12000
#define PREF_TREEITEM_FOLDER                12001
#define PREF_TREEITEM_CONTACTROOT           12002
#define PREF_TREEITEM_SECURITY              12003
#define PREF_TREEITEM_NETWORKROOT           12004
#define PREF_TREEITEM_PLUGINS               12005
#define PREF_TREEITEM_GENERAL               12006

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

#define CHAT_QUIT                             700
#define CHAT_ABOUT                            701
#define CHAT_SEND                             702
#define SEND_BUTT                             703
#define ID_NOTEBOOK                           704
#define TEXT_ENTRY                            705
#define P2_BROWSE_BUTT                        706
#define P2_FILESEND_BUTT                      707


#define Login_Quit                            800
#define Login_About                           801
#define Login_Login                           802
#define Login_NewUser                         802



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
    Revision 1.1  2002/06/06 17:21:42  thementat
    Initial revision

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