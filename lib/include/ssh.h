// --*-c++-*--
/*
    $Id: 

    wxSSH - Cross platform ssh2 implementation
    Copyright (C) 002  Jesse Lovelace
 
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

// DH codes
#define SSH_MSG_KEX_DH_GEX_REQUEST_OLD  30
#define SSH_MSG_KEX_DH_GEX_REQUEST      34
#define SSH_MSG_KEX_DH_GEX_GROUP        31
#define SSH_MSG_KEX_DH_GEX_INIT         32
#define SSH_MSG_KEX_DH_GEX_REPLY        33

/// KIM Supported SSH2 methods

#define MAC_ALGORITHMS "hmac-sha1, hmac-sha1-96, hmac-md5, hmac-md5-96, none"
#define KEYX_METHODS "diffie-hellman-group1-sha1"
#define PK_CERT_FORMATS "ssh-rsa, ssh-dss, x509v3-sign-rsa, x509v3-sign-dss, spki-sign-rsa, spki-sign-dss, pgp-sign-rsa, pgp-sign-dss"
#define CIPHERS "twofish256-cbc, 3des-cbc, blowfish-cbc, twofish-cbc, twofish192-cbc, twofish128-cbc, aes256-cbc, aes192-cbc, aes128-cbc, serpent256-cbc, serpent192-cbc, serpent128-cbc, arcfour, idea-cbc, cast128-cbc"
#define COMPRESSION "zlib, none"
#define LANGUAGE ""

#define MAX_PACKET_LENGTH 35000
#define MAX_PAYLOAD_LENGTH 32768

#include "crypto/misc.h"
#include "protocol.h"
#include "contact.h"
#include <string>

using namespace std;
using namespace CryptoPP;

typedef unsigned char byte;
typedef word32 uint32;
typedef word64 unit64;

SecByteBlock sshString(const string& str);

class SSH2
{
public:
    SSH2(Protocol * proto, Contact contact);
    ~SSH2();
    
    SecByteBlock MakeConnection();

    /* Decrypts inbound traffic */

    void Receive(const SecByteBlock& inbound);

    /* Encrypts outbound traffic */
    SecByteBlock Send(const char * outbound);
    SecByteBlock Send(const string& outbound);
    SecByteBlock Send(const SecByteBlock& outbound);

    void DropConnection();

protected:

    // only for non-encrypted packet
    SecByteBlock MakeBinaryPacket(const SecByteBlock& payload,
            const SecByteBlock& random_padding,
            const SecByteBlock& mac);

    SecByteBlock MakeBinaryPacketNoMAC(const SecByteBlock& payload,
        const SecByteBlock& random_padding);

    SecByteBlock EncryptPacket(const SecByteBlock& packet_minus_mac);
    SecByteBlock DecryptPacket(const SecByteBlock& cipher_text);

    SecByteBlock MakeNegotiationPacket(bool nextIsGuess = true);

    SecByteBlock MakeDisconnectPacket(unsigned long reason_code, const char * description, 
                                              const char * lang_code);
    SecByteBlock MakeServiceRequestPacket(const char * service_name);

    SecByteBlock MakeMAC(const SecByteBlock& packet);
    bool VerifyMAC(const SecByteBlock& packet, const SecByteBlock& hmac);

    SecByteBlock CompressPayload(const SecByteBlock& packet);
    SecByteBlock DecompressPayload(const SecByteBlock& packet);

    void ProcessKexPacket(const SecByteBlock& packet);

    unsigned long IncrementSequence() { 
        if (m_sequenceNumber == 4294967296) // check with ietf, should be 2^32
            m_sequenceNumber = 0;
        return m_sequenceNumber++;
    }

    unsigned long IncrementRemoteSequence()
    {
        if (m_remoteSequenceNumber == 4294967296) // check with ietf
            m_remoteSequenceNumber = 0;
        return m_remoteSequenceNumber++;
    }

private:

    enum name{ CLIENT, SERVER, NONE };
    enum status{ NO_CONNECTION, KEX, 
        DH_GEX_REQUEST, DH_GEX_GROUP, DH_GEX_INIT, DH_GEX_REPLY,
        READY };


    name m_name;
    status m_status;

    uint32 m_remoteSequenceNumber;
    uint32 m_sequenceNumber;

    int m_remoteMacType;
    int m_macType;

    int m_remoteCipherType;
    int m_cipherType;

    SecByteBlock m_sessionKey;

    SecByteBlock m_remoteIV;
    SecByteBlock m_IV;

    Protocol * m_proto;
    Contact m_contact;

protected:


    // public key types
	enum { RSA = 0, DSA, ELGAMAL, NR, BLUMGOLDWASSER,
		RABIN, RW, LUC, LUCELG, ECURVE, DH, UNIFIED_DH, ECNR,
		EC_DHC, EC_MQVC, EC_DHAES};


	// block cipher types
	enum { _IDEA = 100, _DES, DES_EDE2, DES_EDE3, DESX, RC2,
		RC5, BLOWFISH, DIAMOND2, _TEA, SAFER_K64, SAFER_K128,
		SAFER_SK64, SAFER_SK128, _3WAY, GOST, SHARK, CAST_128,
		CAST_256, SQUARE, RC6, MARS, RIJNDAEL, TWOFISH, SERPENT,
		SKIPJACK };

	enum { DEFAULT_PK = 0 };
	enum { DEFAULT_BLOCK = 107 };
	enum { DEF_BLOCKSIZE = 16 };
	enum { DEF_KEYSIZE = 16 };
	enum { DEFAULT_COMPRESS_LEVEL = 5 };

	enum { DEF_PKSIZE = 1024 };

	// stream cipher methods
	enum { ARC4 = 200, SEAL, WAKE, Sapphire, BlumBlumShub,
		_PANAMA_STREAM };

	// macs
	enum { MD5_MAC = 300, _HMAC, XOR_MAC, CBC_MAC, DMAC };

    // ssh2 hmacs
    enum { NO_HMAC, HMAC_SHA1, HMAC_SHA1_96, HMAC_MD5, HMAC_MD5_96 };

    // ssh2 ciphers
    enum { THREE_DES_CBC, BLOWFISH_CBC, TWOFISH256_CBC, TWOFISH192_CBC, 
        TWOFISH128_CBC, AES256_CBC, AES192_CBC, AES128_CBC, 
        SERPENT256_CBC, SERPENT192_CBC, SERPENT128_CBC, ARCFOUR,
        IDEA_CBC, CAST128_CBC, NO_CIPHER, MARS256_CBC };

	// one-way hash functions

	enum { _SHA = 400, SHA_256, SHA_384, SHA_512, MD_2, MD_5, _HAVAL,
		HAVAL_3, HAVAL_4, HAVAL_5, RIPEMD_160, _TIGER, PANAMA_HASH };

	// modes

	enum { CBC_RAW = 500, CBC_PADDED, CBC_CTS, CFB, COUNTER, PGP_CFB };

        
};

