// --*-c++-*--
/*
    $Id: globals.h,v 1.2 2002/06/06 18:43:02 thementat Exp $
 
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
    $Log: globals.h,v $
    Revision 1.2  2002/06/06 18:43:02  thementat
    Added copyrights, fixed cryptography compile errors, lib builds in vc7

    Revision 1.1.1.1  2002/06/06 17:21:53  thementat
    Checkin of new sources BETA 2

    Revision 1.6  2001/12/12 01:40:02  mentat
    Fixed compilation error with gcc3.0.2 and CryptoPP also added new #defines

    Revision 1.5  2001/10/06 16:43:18  thementat
    Added GNU text.


*/

#ifndef GM_GLOBALS_H
#define GM_GLOBALS_H

typedef unsigned char byte;

#define GM_VER            "Beta 1"
#define GM_WEB_UPDATE_STR "http://not.yet"

#define MAX_SESSIONS    10

// Disk IO error messages

#define DISK_NOERROR    100
#define DISK_NOFILE     101
#define DISK_READERROR  102
#define DISK_WRITEERROR 103
#define DISK_NO_USER    104
#define DISK_NO_PWORD   105

// Contact Network Preference Levels

#define CONTACT_NET_USEFIRST  333
#define CONTACT_NET_USELAST   334
#define CONTACT_NET_USENEVER  335
#define CONTACT_NET_USEANY    336

#define ISO_TIME_LENGTH	8

// Cryptographic types
#define _HASH           600
#define _BLOCK          601
#define _PK             602


//////////////////////////// SSH2 Emulation ///////////////////////////

#define SSH_DISCONNECT_HOST_NOT_ALLOWED_TO_CONNECT      1
#define SSH_DISCONNECT_PROTOCOL_ERROR                   2
#define SSH_DISCONNECT_KEY_EXCHANGE_FAILED              3
#define SSH_DISCONNECT_RESERVED                         4
#define SSH_DISCONNECT_MAC_ERROR                        5
#define SSH_DISCONNECT_COMPRESSION_ERROR                6
#define SSH_DISCONNECT_SERVICE_NOT_AVAILABLE            7
#define SSH_DISCONNECT_PROTOCOL_VERSION_NOT_SUPPORTED   8
#define SSH_DISCONNECT_HOST_KEY_NOT_VERIFIABLE          9
#define SSH_DISCONNECT_CONNECTION_LOST                 10
#define SSH_DISCONNECT_BY_APPLICATION                  11
#define SSH_DISCONNECT_TOO_MANY_CONNECTIONS            12
#define SSH_DISCONNECT_AUTH_CANCELLED_BY_USER          13
#define SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE  14
#define SSH_DISCONNECT_ILLEGAL_USER_NAME               15
#define SSH_MSG_DISCONNECT             1
#define SSH_MSG_IGNORE                 2
#define SSH_MSG_UNIMPLEMENTED          3
#define SSH_MSG_DEBUG                  4
#define SSH_MSG_SERVICE_REQUEST        5
#define SSH_MSG_SERVICE_ACCEPT         6

#define SSH_MSG_KEXINIT                20
#define SSH_MSG_NEWKEYS                21

  /* Numbers 30-49 used for kex packets.
     Different kex methods may reuse message numbers in
     this range. */

#define SSH_MSG_KEXDH_INIT             30
#define SSH_MSG_KEXDH_REPLY            31

// These are the general authentication message codes:

#define SSH_MSG_USERAUTH_REQUEST            50
#define SSH_MSG_USERAUTH_FAILURE            51
#define SSH_MSG_USERAUTH_SUCCESS            52
#define SSH_MSG_USERAUTH_BANNER             53

#endif
