/*
    $Id: v7protocol.h,v 1.1 2002/06/06 17:21:52 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2001  Emil Styrke <emil@lysator.liu.se>
 
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
    $Log: v7protocol.h,v $
    Revision 1.1  2002/06/06 17:21:52  thementat
    Initial revision

    Revision 1.4  2001/11/09 08:55:38  henrik
    ICQ cleanups and fixes.

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

// -*- C++ -*-
#ifndef __V7PROTOCOL_H
#define __V7PROTOCOL_H

#define ICQ_PACKET_ID 0x2a

#define ICQ_CHANNEL_OPEN 0x01
#define ICQ_CHANNEL_FNAC 0x02
#define ICQ_CHANNEL_FERROR 0x03
#define ICQ_CHANNEL_CLOSE 0x04

#define FAMILY_GEN 0x0001
#define FAMILY_LOC 0x0002
#define FAMILY_BUD 0x0003
#define FAMILY_MSG 0x0004
#define FAMILY_BOS 0x0009
#define FAMILY_ICQ 0x0015

#define SUB_GEN_ERROR		0x0001
#define SUB_GEN_CLIENTREADY	0x0002
#define SUB_GEN_SERVERREADY     0x0003
#define SUB_GEN_RATEREQUEST     0x0006
#define SUB_GEN_RATERESPONSE    0x0007
#define SUB_GEN_RATERESPONSEACK 0x0008
#define SUB_GEN_REQSELFINFO     0x000e
#define SUB_GEN_SELFINFO	0x000f
#define SUB_GEN_SETSTATUS	0x0011
#define SUB_GEN_MOTD		0x0013
#define SUB_GEN_CLIENTVERSIONS  0x0017
#define SUB_GEN_SERVERVERSIONS  0x0018

#define SUB_LOC_REQRIGHTS	0x0002
#define SUB_LOC_RIGHTS		0x0003
#define SUB_LOC_CAPINFO		0x0004

#define SUB_BUD_REQRIGHTS	0x0002
#define SUB_BUD_RIGHTS		0x0003
#define SUB_BUD_ADDBUDDY	0x0004
#define SUB_BUD_DELBUDDY	0x0005
#define SUB_BUD_ONCOMING	0x000b
#define SUB_BUD_OFFGOING	0x000c

#define BUD_STATUS_ONLINE     0x00030000
#define BUD_STATUS_AWAY       0x00030001
#define BUD_STATUS_NA         0x00030005
#define BUD_STATUS_OCCUPIED   0x00030011
#define BUD_STATUS_DND        0x00030013
#define BUD_STATUS_CHAT       0x00030020
#define BUD_STATUS_INVISIBLE  0x00030100

#define SUB_MSG_ADDPARAM	0x0002
#define SUB_MSG_REQPARMINFO	0x0004
#define SUB_MSG_PARMINFO	0x0005
#define SUB_MSG_SENDMSG		0x0006
#define SUB_MSG_RECVMSG		0x0007

#define SUB_BOS_REQRIGHTS	0x0002
#define SUB_BOS_RIGHTS		0x0003

#define SUB_ICQ_REQUEST		0x0002
#define SUB_ICQ_RESPONSE	0x0003

/*#define CLIENTSTRING "ICQ Inc. - Product of ICQ (TM).2000a.4.30.1.3141.85"
#define MAJOR2 (uint16)0x010a     // 266
#define MAJOR  (uint16)0x0004     // 4
#define MINOR  (uint16)0x001e     // 30
#define MINOR2 (uint16)0x0001     // 1
#define BUILD  (uint16)0x0c45     // 3141
#define BUILD2 (uint32)0x00000055 // 85 */

#define CLIENTSTRING "ICQ Inc. - Product of ICQ (TM).2000b.4.63.1.3279.85"
#define MAJOR2 (uint16)0x010a	  // 266
#define MAJOR  (uint16)0x0004	  // 4
#define MINOR  (uint16)0x003f	  // 63
#define MINOR2 (uint16)0x0001     // 1
#define BUILD  (uint16)0x0ccf     // 3279
#define BUILD2 (uint32)0x00000055 // 85

#define LANGUAGE (string)"en"
#define COUNTRY (string)"us"

#define PASSWORD_MASK     "\xf3\x26\x81\xc4\x39\x86\xdb\x92\x71\xa3\xb9\xe6\x53\x7a\x95\x7c"
#define ICBM_PARAM_STRING "\x00\x00\x00\x00\x00\x03\x1f\x40\x03\xe7\x03\xe7\x00\x00\x00\x00"
#define CLIENT_RDY_STRING "\x00\x01\x00\x03\x01\x10\x02\x8a\x00\x02\x00\x01\x01\x01\x02\x8a\x00\x03\x00\x01\x01\x10\x02\x8a\x00\x15\x00\x01\x01\x10\x02\x8a\x00\x04\x00\x01\x01\x10\x02\x8a\x00\x06\x00\x01\x01\x10\x02\x8a\x00\x09\x00\x01\x01\x10\x02\x8a\x00\x0a\x00\x01\x01\x10\x02\x8a"

#endif
