/*
    $Id: errors.h,v 1.1 2002/06/06 17:21:50 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2001  Henrik Abelsson <henrik@abelsson.com>
 
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
    $Log: errors.h,v $
    Revision 1.1  2002/06/06 17:21:50  thementat
    Initial revision

    Revision 1.2  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

class Error
{

public:
  static string strError(int code)
  {
    map<int,string> str;
    str[200]="SyntaxError               ";
    str[201]="InvalidParameter          ";
    str[205]="InvalidUser               ";
    str[206]="FqdnMissing               ";
    str[207]="AlreadyLogin              ";
    str[208]="InvalidUsername           ";
    str[209]="InvalidFriendlyName       ";
    str[210]="ListFull                  ";
    str[215]="AlreadyThere              ";
    str[216]="NotOnList                 ";
    str[218]="AlreadyInTheMode          ";
    str[219]="AlreadyInOppositeList     ";
    str[280]="SwitchboardFailed         ";
    str[281]="NotifyXfrFailed           ";
    str[300]="RequiredFieldsMissing     ";
    str[302]="NotLoggedIn               ";
    str[500]="InternalServer            ";
    str[501]="DbServer                  ";
    str[510]="FileOperation             ";
    str[520]="MemoryAlloc               ";
    str[600]="ServerBusy                ";
    str[601]="ServerUnavailable         ";
    str[602]="PeerNsDown                ";
    str[603]="DbConnect                 ";
    str[604]="ServerGoingDown           ";
    str[707]="CreateConnection          ";
    str[711]="BlockingWrite             ";
    str[712]="SessionOverload           ";
    str[713]="UserTooActive             ";
    str[714]="TooManySessions           ";
    str[715]="NotExpected               ";
    str[717]="BadFriendFile             ";
    str[911]="AuthenticationFailed      ";
    str[913]="NotAllowedWhenOffline     ";
    str[920]="NotAcceptingNewUsers      ";
    return str[code];
  }
  enum {
    SyntaxError               =200,
    InvalidParameter          =201,
    InvalidUser               =205,
    FqdnMissing               =206,
    AlreadyLogin              =207,
    InvalidUsername           =208,
    InvalidFriendlyName       =209,
    ListFull                  =210,
    AlreadyThere              =215,
    NotOnList                 =216,
    AlreadyInTheMode          =218,
    AlreadyInOppositeList     =219,
    SwitchboardFailed         =280,
    NotifyXfrFailed           =281,
    RequiredFieldsMissing     =300,
    NotLoggedIn               =302,
    InternalServer            =500,
    DbServer                  =501,
    FileOperation             =510,
    MemoryAlloc               =520,
    ServerBusy                =600,
    ServerUnavailable         =601,
    PeerNsDown                =602,
    DbConnect                 =603,
    ServerGoingDown           =604,
    CreateConnection          =707,
    BlockingWrite             =711,
    SessionOverload           =712,
    UserTooActive             =713,
    TooManySessions           =714,
    NotExpected               =715,
    BadFriendFile             =717,
    AuthenticationFailed      =911,
    NotAllowedWhenOffline     =913,
    NotAcceptingNewUsers      =920,
  }
private:
};
