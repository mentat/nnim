// --*-c++-*--
/*
    $Id: ssh.h,v 1.1 2002/06/06 17:21:53 thementat Exp $
 
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
    $Log: ssh.h,v $
    Revision 1.1  2002/06/06 17:21:53  thementat
    Initial revision

    Revision 1.3  2001/10/06 16:49:09  thementat
    Added GNU text.

*/

#ifndef GM_SSH_H
#define GM_SSH_H

#define SSH_IDENTIFICATION "SSH-2.0-GM_BETA1\013\010"


/// SSH2 Error Codes

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

/// SSH2

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

/// KIM Supported SSH2 methods

#define MAC_ALGORITHMS "hmac-sha1, hmac-sha1-96, hmac-md5, hmac-md5-96, none"
#define KEYX_METHODS "diffie-hellman-group1-sha1"
#define PK_CERT_FORMATS "ssh-dss, ssh-rsa, x509v3-sign-rsa, x509v3-sign-dss, spki-sign-rsa, spki-sign-dss, pgp-sign-rsa, pgp-sign-dss"
#define CIPHERS "3des-cbc, blowfish-cbc, twofish256-cbc, twofish-cbc, twofish192-cbc, twofish128-cbc, aes256-cbc, aes192-cbc, aes128-cbc, serpent256-cbc, serpent192-cbc, serpent128-cbc, arcfour, idea-cbc, cast128-cbc"
#define COMPRESSION "none, zlib"

#endif
