// --*-c++-*--
/*
    $Id: cryptography.h,v 1.1 2002/06/06 17:21:53 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2002  Jesse Lovelace - jllovela@eos.ncsu.edu
 
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
    $Log: cryptography.h,v $
    Revision 1.1  2002/06/06 17:21:53  thementat
    Initial revision

    Revision 1.9  2001/12/12 01:40:02  mentat
    Fixed compilation error with gcc3.0.2 and CryptoPP also added new #defines

    Revision 1.8  2001/10/27 18:29:13  estyrke
    Fixed icq parser bug.. #475544

    Revision 1.7  2001/10/06 16:46:04  thementat
    Added GNU text.
*/

#ifndef GM_CRYPTOGRAPHY_H
#define GM_CRYPTOGRAPHY_H

#include <string>
#include "crypto/misc.h"


using namespace std;
using namespace CryptoPP;

/**
 * The cryptography class.
 *
 * This class creates keys, encrypts, and decrypts data.
 *
 * In other words, a dummied down interface to Crypto++.
 *
 * This class is usually associated with an active Contact.
 * The static hashing functions and disk i/o are used by the login/
 * contact loading class.
 *
 * @author Jesse Lovelace
 * @version 1.0
 */

class gmCrypto {
public:

	// public key types
	enum { RSA = 0, DSA, ELGAMAL, NR, BLUMGOLDWASSER,
		RABIN,RW, LUC, LUCELG, ECURVE, DH, UNIFIED_DH, ECNR,
		EC_DHC, EC_MQVC, EC_DHAES};

	enum { DEFAULT_PK = 0 };

	// block cipher types
	enum { _IDEA = 100, _DES, DES_EDE2, DES_EDE3, DESX, RC2,
		RC5, BLOWFISH, DIAMOND2, _TEA, SAFER_K64, SAFER_K128,
		SAFER_SK64, SAFER_SK128, _3WAY, GOST, SHARK, CAST_128,
		CAST_256, SQUARE, RC6, MARS, RIJNDAEL, TWOFISH, SERPENT,
		SKIPJACK };

	enum { DEFAULT_BLOCK = 107 };
	enum { DEF_BLOCKSIZE = 16 };
	enum { DEF_KEYSIZE = 16 };
	enum { DEFAULT_COMPRESS_LEVEL = 5 };

	// stream cipher methods
	enum { ARC4 = 200, SEAL, WAKE, Sapphire, BlumBlumShub,
		_PANAMA_STREAM };

	// macs
	enum { MD5_MAC = 300, _HMAC, XOR_MAC, CBC_MAC, DMAC };

	// one-way hash functions

	enum { _SHA = 400, SHA_256, SHA_384, SHA_512, MD_2, MD_5, _HAVAL,
		HAVAL_3, HAVAL_4, HAVAL_5, RIPEMD_160, _TIGER, PANAMA_HASH };

	enum { DEF_PKSIZE = 1024 };

	// modes

	enum { CBC_RAW = 500, CBC_PADDED, CBC_CTS, CFB, COUNTER, PGP_CFB };
	
	gmCrypto(); 
	virtual ~gmCrypto();

    /* Functions to set variables */
	void SetPKType( unsigned short pub_mode = DEFAULT_PK );
	void SetPKSize( unsigned short pub_length = DEF_PKSIZE );

	void SetBlockCipher( unsigned short cipher_mode = DEFAULT_BLOCK ) { m_cipher_mode = cipher_mode; }
	void SetBlockSize( unsigned short cipher_length = DEF_BLOCKSIZE ) { m_block_size = cipher_length; }
	void SetBlockKeySize( unsigned short cipher_length = DEF_KEYSIZE ) { m_block_key_length = cipher_length; }

	void SetPubKey ( const SecByteBlock& key ) { m_publicKey = key; }
	void SetBlockKey ( const SecByteBlock& key ) { m_sessionKey = key; }

    /* Functions to return variables */
	unsigned short GetBlockCipher() { return m_cipher_mode; }
	unsigned short GetBlockKeySize() { return m_block_key_length; }
	unsigned short GetBlockSize() { return m_block_size; }

	unsigned short GetPKType() { return m_pub_key_mode; }
	unsigned short GetPKSize() { return m_pub_key_length; }

    /* returns encoded public or block keys base64/16 */
	string GetDERPubKey() { return ""; }
	SecByteBlock GetBlockKey() { return m_sessionKey; }

    /* Generates, or re-generates PK/Session Keys */
	void GeneratePK();
	void GenerateSessionKey();

    /* encrypts with current block cipher */
	string BlockEncryptText(const string& in_text/*, const wxString& IV*/);
	string PKEncryptText(const string& in_text);
	string BlockDecryptText(const string& in_text/*, const wxString& IV*/);
	string PKDecryptText(const string& in_text);

    /* random returning functions */
	void returnRandom (unsigned long randLength);
	void ClearRandom();

	void setBlockCipherMode(unsigned short mode) { m_mode = mode; }

protected:

	byte * MakeTransportPacket(byte payload[], const unsigned long payload_length, 
                                   byte random_padding[], const unsigned char random_pad_length,
                                   byte mac[], const unsigned long mac_length);

	byte * MakeNegotiationPacket();

	byte * MakeAuthenticationRequestPacket();
	byte * MakeAuthResponsePacket(bool accepted);
	byte * MakeBannerPacket(const string& msg);

private:

	enum { IV_SEED_LENGTH = 8 };
	enum { IV_LENGTH = 16 };

	unsigned short m_mode;

	bool m_HexEncoding;
    // True if the cryptographic data will be hex encoded, if false, base64

	void GenerateRSAKeys(unsigned short keyLength);
    // Generates new RSA keys of length keyLength

	string EncryptRSA(const string& message);
    // Encrypts a block of data with RSA, returns encrypted-encoded text

	string DecryptRSA(const string& ciphertext);
    // Decrypts a block of data, returns plaintext

	/* key storage */
	SecByteBlock m_publicKey;
    // The local public key

	SecByteBlock m_privateKey;
    // The local private key

	SecByteBlock m_sessionKey;
    // The session key (symmetric block mode)

	unsigned short m_pub_key_mode;  // all in bytes
	unsigned short m_pub_key_length;

	unsigned short m_cipher_mode;
	unsigned short m_block_key_length;
	unsigned short m_block_size;

public:

	/* base encode/decode functions */
	static string Encode(const string& input, bool hexEnc = true);
	static string Decode(const string& input, bool hexDec = true);

    /* file decryption/enc functions */
    /* Note: need to make multi-cipher functions */

	static bool EncryptFile(const string& filename, const SecByteBlock& key, const string& data);
	static string DecryptFile(const string& filename, const SecByteBlock &key);

	static string DecompressFile(const string & infile);
	static void CompressFile(const string& data, const string& filename);

	/* Static hashing functions, one way hashing.  See globals.h for hash names */
	static string HashEncode(SecByteBlock& toHash, const int hash_function = SHA_256, bool hexOR64 = true);
	static SecByteBlock Hash(SecByteBlock& toHash, const int hash_function = SHA_256);

};

#endif
