// --*-c++-*--
/////////////////////////////////////////////////////////////////////////////
// Name:        cryptography.h
// Purpose:     cryptography source
// Author:      Jesse Lovelace
// Modified by:
// Created:
// RCS-ID:      $Id: cryptography.cpp,v 1.4 2002/06/19 19:14:45 thementat Exp $
// Copyright:   (c) Jesse Lovelace
// Licence:     LGPL licence
/////////////////////////////////////////////////////////////////////////////


#ifdef WIN32
#pragma warning(disable:4786)
#endif


#include <string>
#include <memory>
#include <math.h>

#include "crypto/config.h"
#include "crypto/misc.h"
#include "crypto/idea.h"
#include "crypto/rng.h"
#include "crypto/osrng.h"
#include "crypto/files.h"
#include "crypto/rsa.h"
#include "crypto/queue.h"
#include "crypto/randpool.h"
#include "crypto/hex.h"
#include "crypto/osrng.h"
#include "crypto/rijndael.h"
#include "crypto/mars.h"
#include "crypto/des.h"
#include "crypto/3way.h"
#include "crypto/idea.h"
#include "crypto/serpent.h"
#include "crypto/rc2.h"
#include "crypto/rc5.h"
#include "crypto/blowfish.h"
#include "crypto/diamond.h"
#include "crypto/rc6.h"
#include "crypto/tea.h"
#include "crypto/cbc.h"
#include "crypto/sha.h"
#include "crypto/md2.h"
#include "crypto/md5.h"
//#include "crypto/haval.h"
#include "crypto/ripemd.h"
#include "crypto/tiger.h"
#include "crypto/default.h"
#include "crypto/base64.h"
#include "crypto/panama.h"
#include "crypto/gzip.h"
#include "crypto/zlib.h"
#include "crypto/filters.h"
#include "gmException.h"

using namespace CryptoPP;
using namespace std;

#include "cryptography.h"



void gmCrypto::CompressFile(const string& data, const string& filename)
{

	try
	{
		StringSource f(data.c_str(), true, new Gzip(new FileSink(filename.c_str())));
	}
	catch (CryptoPP::Exception &e)
	{
		throw gmException(e.what(), gmException::gDISK);
	}
	
}

string gmCrypto::DecompressFile(const string& infile)
{

	string decryptedFile;
	try
	{
		FileSource f(infile.c_str(), true, new Gunzip(new StringSink(decryptedFile)));
	}
	catch (CryptoPP::Exception &e)
	{
		throw gmException(e.what(), gmException::gDISK);
	}
	return decryptedFile;

}


bool gmCrypto::EncryptFile(const string& filename, const SecByteBlock& key, const string& data)
{

    // generating random seed for iv
    SecByteBlock ivSeed(IV_SEED_LENGTH);
    SecByteBlock realIv(IV_LENGTH);

    AutoSeededRandomPool rng;
    rng.GenerateBlock(ivSeed, IV_SEED_LENGTH);

    //creating actual iv, thanks to denis bider for this method
    SecByteBlock ivHash(SHA256::DIGESTSIZE);
    SHA256().CalculateDigest(ivHash, ivSeed, IV_SEED_LENGTH);

    for (unsigned int i = 0; i != IV_LENGTH; ++i)
        realIv[i] = ivHash[i] ^ ivHash[i+16];

    //compress 

    ZlibCompressor z(NULL,9);
    z.Put((const unsigned char *)data.c_str(), data.length());
    z.MessageEnd();

    SecByteBlock compressedPlainText(z.MaxRetrievable());
    z.Get(compressedPlainText, compressedPlainText.Size());

    //encrypting
    SecByteBlock cipherText(compressedPlainText.Size());

    MARSEncryption mars(key, SHA384::DIGESTSIZE);
    CBC_CTS_Encryptor encryptor(mars, realIv, new ArraySink(cipherText, compressedPlainText.Size()));
    encryptor.Put(compressedPlainText, compressedPlainText.Size());
    encryptor.MessageEnd();

    // create HMAC

    SecByteBlock myMac(HMAC<SHA>::DIGESTSIZE);
    HMAC<SHA> themac(key, SHA384::DIGESTSIZE);

    themac.Update(cipherText, cipherText.Size());
    themac.Final(myMac);

    // output all to file

	SecByteBlock testout;
	// setup output file and encode the file as base64
    Base64Encoder encoder(new FileSink(filename.c_str(), false));

	encoder.Put(realIv, IV_LENGTH);
    encoder.Put(cipherText, cipherText.Size());
    encoder.Put(myMac, HMAC<SHA>::DIGESTSIZE);
    encoder.MessageEnd();

    return true;

}

string gmCrypto::DecryptFile(const string& filename, const SecByteBlock &key)
{
  // read from file
    FileSource inputFile(filename.c_str(), true);
    SecByteBlock dataFromFile(inputFile.MaxRetrievable());
    inputFile.Get(dataFromFile, dataFromFile.Size());

    // decode from base64
    Base64Decoder decoder64;
    decoder64.Put(dataFromFile, dataFromFile.Size());
    decoder64.MessageEnd();

	SecByteBlock decodedData(decoder64.MaxRetrievable());
    decoder64.Get(decodedData, decodedData.Size());

    //seperate data
    SecByteBlock iv(decodedData, IV_LENGTH);
    SecByteBlock payload(decodedData + IV_LENGTH, decodedData.Size() - IV_LENGTH - HMAC<SHA>::DIGESTSIZE);
    SecByteBlock hmac(decodedData + IV_LENGTH + payload.Size(), HMAC<SHA>::DIGESTSIZE);

    // decrypt, dat becomes new compressed cipher text.
    MARSDecryption marsDec(key, SHA384::DIGESTSIZE);
    CBC_CTS_Decryptor cbcDecryptor(marsDec, iv);
    cbcDecryptor.Put(payload, payload.Size());
    cbcDecryptor.MessageEnd();

	// verify hmac digest

    if (!HMAC<SHA>(key, SHA384::DIGESTSIZE).VerifyDigest(hmac, payload, payload.Size()))
		throw gmException("File digest not verified.", gmException::gDISK);

    // create object to store compressed plain text
    SecByteBlock compressedPlaintext(cbcDecryptor.MaxRetrievable());

	// put decrypted data in
    cbcDecryptor.Get(compressedPlaintext, compressedPlaintext.Size());

    ZlibDecompressor zDecompressor;
    zDecompressor.Put(compressedPlaintext, compressedPlaintext.Size());
    zDecompressor.MessageEnd();

	SecByteBlock plainText(zDecompressor.MaxRetrievable());
    zDecompressor.Get(plainText, plainText.Size());

	return (const char *)(const unsigned char *)plainText;

}


void gmCrypto::GenerateRSAKeys(unsigned short keyLength)
{
	return ;

}

string gmCrypto::EncryptRSA(const string& message)
{

	return "";

}

string gmCrypto::DecryptRSA(const string& ciphertext)
{

	return "";

}

gmCrypto::gmCrypto()
{
	// Default constructor uses RSA (1024) and IDEA
	m_HexEncoding = true;

	m_pub_key_mode = DEFAULT_PK;
	m_pub_key_length = DEF_PKSIZE;
	m_cipher_mode = DEFAULT_BLOCK;
	m_block_key_length = DEF_KEYSIZE;
	m_block_size = DEF_BLOCKSIZE;

}


gmCrypto::~gmCrypto()
{

}

string gmCrypto::HashEncode(SecByteBlock& toHash, const int hash_function, bool hexOR64)
{
	SecByteBlock block(Hash(toHash, hash_function));
	auto_ptr<Filter> encoding;

	string outString;

	if (hexOR64)
		encoding.reset(new HexEncoder(new StringSink(outString)));
	else
		encoding.reset(new Base64Encoder(new StringSink(outString), false));
	
	encoding->Put(block, block.Size());
	encoding->MessageEnd();

	return outString;
}


SecByteBlock gmCrypto::Hash(SecByteBlock& toHash, const int hash_function)
{
	auto_ptr<HashModule> hash;

	switch (hash_function)
	{
	case(_SHA): hash.reset(new SHA); break;
	case(SHA_256): hash.reset(new SHA256); break;
	case(SHA_384): hash.reset(new SHA384); break;
	case(SHA_512): hash.reset(new SHA512); break;
	case(MD_2): hash.reset(new MD2); break;
	case(MD_5): hash.reset(new MD5); break;
	/*case(_HAVAL): hash.reset(new HAVAL); break;
	case(HAVAL_3): hash.reset(new HAVAL3); break;
	case(HAVAL_4): hash.reset(new HAVAL4); break;
	case(HAVAL_5): hash.reset(new HAVAL5); break;     */
	case(RIPEMD_160): hash.reset(new RIPEMD160); break;
	case(_TIGER): hash.reset(new Tiger); break;
	case(PANAMA_HASH): hash.reset(new PanamaHash<true>); break; // true for big endian
	default: throw gmException("No such hash function available.", gmException::gUSER);
	}

	SecByteBlock retBlock(hash->DigestSize());
	hash->CalculateDigest(retBlock, toHash, toHash.Size());

	return retBlock;

}

// Encrypt Text Public Key
string gmCrypto::PKEncryptText(const string& in_text)
{

	return "";

}


// Decrypt Text Private Key
string gmCrypto::PKDecryptText(const string& in_text)
{

	return "";

}

void gmCrypto::GeneratePK()
{

	switch (m_pub_key_mode)
	{
	case(RSA): GenerateRSAKeys(m_pub_key_length); break;
	}

	return;

}

void gmCrypto::GenerateSessionKey()
{


	return ;

}

string gmCrypto::BlockEncryptText(const string& in_text)
{


	return "";

}


string gmCrypto::BlockDecryptText(const string& ciphertext/*, const string& IV*/)
{


	return "";

}

string gmCrypto::Encode(const string& input, bool hexEnc)
{
	auto_ptr<Filter> encoding;
	string outString;

	if (hexEnc)
		encoding.reset(new HexEncoder(new StringSink(outString)));
	else
		encoding.reset(new Base64Encoder(new StringSink(outString), false));
	
	encoding->Put((const unsigned char *)input.c_str(), input.length());
	encoding->MessageEnd();

	return outString;

}

string gmCrypto::Encode(const SecByteBlock& input, bool hexEnc)
{
	auto_ptr<Filter> encoding;

	string outString;

	if (hexEnc)
		encoding.reset(new HexEncoder(new StringSink(outString)));
	else
		encoding.reset(new Base64Encoder(new StringSink(outString), false));

	
	encoding->Put(input, input.Size());
	encoding->MessageEnd();

	return outString;
}
/*
string gmCrypto::Decode(const string& input, bool hexDec)
{
	string out;

	if (hexDec)
	{
		HexDecoder d(new StringSink(out));
		d.Put((const unsigned char *)input.c_str(), input.length());
		d.MessageEnd();
	}
	else
	{
		Base64Decoder d(new StringSink(out));
		d.Put((const unsigned char *)input.c_str(), input.length());
		d.MessageEnd();
	}

	return out;
}
*/
SecByteBlock gmCrypto::Decode(const string& input, bool hexDec)
{
	auto_ptr<Filter> decoding;

	SecByteBlock out;

	if (hexDec)
		decoding.reset(new HexDecoder);
	else
		decoding.reset(new Base64Decoder);
	
	decoding->Put((const unsigned char *)input.c_str(), input.length());
	decoding->MessageEnd();

	out.Resize(decoding->MaxRetrievable());
	decoding->Get(out, out.Size());

	return out;
}

int gmCrypto::GetDigestSize(int hash_function)
{
	switch (hash_function)
	{
    case(_SHA): return SHA::DIGESTSIZE; break;
    case(SHA_256): return SHA256::DIGESTSIZE; break;
    case(SHA_384): return SHA384::DIGESTSIZE; break;
    case(SHA_512): SHA512::DIGESTSIZE; break;
    case(MD_2): MD2::DIGESTSIZE; break;
    case(MD_5): MD5::DIGESTSIZE; break;
    /*case(_HAVAL): HAVAL::DIGESTSIZE; break;
    case(HAVAL_3): HAVAL3::DIGESTSIZE; break;
    case(HAVAL_4): HAVAL4::DIGESTSIZE; break;
    case(HAVAL_5): HAVAL5::DIGESTSIZE; break;  */
    case(RIPEMD_160): RIPEMD160::DIGESTSIZE; break;
    case(_TIGER): Tiger::DIGESTSIZE; break;
    case(PANAMA_HASH):PanamaHash<true>::DIGESTSIZE; break; // true for big endian
	default: throw gmException("No such hash function available.", gmException::gUSER);
	}
}

