// --*-c++-*--
/*
    $Id: authload.cpp,v 1.2 2002/06/06 18:43:02 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2002  Jesse Lovelace
 
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

#ifdef WIN32
    #pragma warning(disable:4786)
#endif

const static int IV_SEED_LENGTH = 8;
const static int IV_LENGTH	= 16;

#include "crypto/config.h"
#include "crypto/misc.h"
#include "crypto/sha.h"
#include "crypto/hex.h"
#include "crypto/files.h"
#include "crypto/mars.h"
#include "crypto/cbc.h"
#include "crypto/osrng.h"
#include "crypto/zlib.h"
#include "crypto/hmac.h"
#include "crypto/base64.h"
#include "crypto/ripemd.h"

#include "authload.h"
#include "cryptography.h"
#include "contact.h"
#include "globals.h"
#include "gmException.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace CryptoPP;

template<class T> inline void destroy(T*& p) { delete p; p = NULL; }


AuthLoad::AuthLoad(const string& directory)
: m_config(NULL)
{
	// set initial status to offline - no user logged in
    m_status = OFFLINE;

	// set initial user directory to whatever is passed to constructor
    m_directory = directory;

	m_contacts = new Contacts(this);
	m_user = new User(this);
	m_global = new Globals(this);
}

AuthLoad::~AuthLoad()
{
	destroy(m_contacts);
	destroy(m_user);
	destroy(m_global);
	destroy(m_config);
}

SecByteBlock AuthLoad::Hash(SecByteBlock & password)
{
    SecByteBlock retBlock;

	// this hash is for storage and authentication
    retBlock.Resize(SHA384::DIGESTSIZE);
    SHA384().CalculateDigest(retBlock, password, password.Size());

    return retBlock;
}

bool AuthLoad::Logoff()
{
	// save data
    CommitToFile();

	// set status to offline
    m_status = OFFLINE;

	// set the config to a empty xml node
    destroy(m_config);

	// resize the disk password to zero length
    m_diskPass.CleanNew(0);

	// erase old file name
    m_filename = "";

    return true;

}

bool AuthLoad::CreateNew(const string& username, SecByteBlock &password)
{

	// if username exists, return false
    if (Exists(username))
        return false;

	Logoff();

    m_config = new XMLNode();

	// create a new file to hold user data
    m_filename = gmCrypto::Encode(username) + string(".kim");

	// hash the user's password to use as file key
    m_diskPass = Hash(password);

	// set up initial values in the xml node
    m_config->setName("config").setProperty("username", username);

#if 0
	/* This extra store of the password with a different hash
	might be unneeded because file is encrypted. */

	SecByteBlock thehash = Hash(password);

	string myhash((const char *)(void *)thehash, thehash.Size());

    m_config->setProperty("password", myhash);
#endif

	// set status to online - user logged in
    m_status = ONLINE;

	// do initial save to disk
    CommitToFile();
    return true;

}

bool AuthLoad::Login(const string & username, SecByteBlock &password)
{
    // returns false if user does not exist
    if (!Exists(username))
    	return false;


	destroy(m_config);
    m_config = new XMLNode();

    m_filename = gmCrypto::Encode(username) + string(".kim");

    m_diskPass = Hash(password);

#if 0
	ifstream in;
    string tmp, inbound;
    in.open("test.xml");
    while (getline(in, tmp))
        inbound += tmp;
#endif

    *m_config << Decrypt(m_directory + m_filename, m_diskPass);

	ofstream out;

	out.open("hi.txt");

	out << *m_config;

    if (!(m_config->property("username") == username))
    {
		throw(gmException("Critical error, username does not match but file was decrypted!", gmException::gUSER));
    } 
	else
        LogText("Username matches.");

    m_status = ONLINE;

    return true;

}

bool
AuthLoad::Search(const string& toFind, const string& findType, XMLNode& parentNode, XMLNode& returnNode)
{
    for (int iter2 = 1; iter2 <= parentNode.numChildren(findType); iter2++)
    {
        if (parentNode.child(findType, iter2).property("name") == toFind)
        {
            returnNode = parentNode.child(findType, iter2);
            return true;
        }
    }

    /* search through folders */
    for (unsigned int iter = 0; iter < parentNode.children().size(); iter++)
    {
        if (Search(toFind, findType, parentNode.children()[iter], returnNode))
            return true;
    }

    return false;
}

bool
AuthLoad::BaseSearch(const string& toFind, const string& findType, XMLNode& parentNode, XMLNode& returnNode, int& num)
{ 
    /* search through things of type findType */

    for (int iter2 = 1; iter2 <= parentNode.numChildren(findType); iter2++)
    {
        if (parentNode.child(findType, iter2).property("name") == toFind)
        {
            num = iter2;
            returnNode = parentNode;
            return true;
        }
    }

    /* search through folders */
    for (unsigned int iter = 0; iter < parentNode.children().size(); iter++)
    {
        if (BaseSearch(toFind, findType, parentNode.children()[iter], returnNode, num))
            return true;
    }

    return false;
}

int
AuthLoad::TagCount(const string& findType, XMLNode& parentNode)
{ 
    /* search through things of type findType */

    int levelCount(parentNode.numChildren(findType));

    /* search through folders */
    for (unsigned int iter = 0; iter <parentNode.children().size(); iter++)
    {
        levelCount += TagCount(findType, parentNode.children()[iter]);
    }

    return levelCount;
}

void AuthLoad::GetAllTags(const string& findType, XMLNode& parentNode, vector<XMLNode> &result_vector)
{
    for (int iter2 = 1; iter2 <= parentNode.numChildren(findType); iter2++)
    {
        result_vector.push_back(parentNode.child(findType, iter2));
    }

    /* search through folders */
    for (unsigned int iter = 0; iter < parentNode.children().size(); iter++)
    {
        GetAllTags(findType, parentNode.children()[iter], result_vector);

    }
}

string AuthLoad::PrintTree(const XMLNode& base)
{
    return base;
}

bool AuthLoad::CommitToFile()
{
	// if user is not logged in, then cannot commit to disk
    if (m_status == OFFLINE)
	{
		LogText("Cannot Commit to file while not logged in.");
        return false;
	}

	// add xml encoding information for expat
    string data;//("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n");

	// convert xmlnode to a string
	data += string(*m_config);

	// try to encrypt to disk
    try
    {
        Encrypt(m_directory + m_filename, m_diskPass, data.c_str());
    }
    catch (CryptoPP::Exception &e)
    {
		//catch cryptopp exception and throw gmException
		throw(gmException(e.GetWhat(), gmException::gDISK));
    }
	catch (gmException &e)
	{
		throw e;
	}
	
	// write out debug xml
    ofstream out;
    out.open("test.xml");
    out << string(data);

    return true;
}

string AuthLoad::Encode(const string & inbound)
{

    string outbound;
    unsigned int i(0);

    // first pass - remove # and & and ;
    for (i = 0; i < inbound.size(); i++)
    {
        switch (inbound[i])
        {
        case('#'): outbound += "&#35;"; break;
        case('&'): outbound += "&#38;"; break;
        case(';'): outbound += "&#59;"; break;
        case('\''): outbound += "&#39;"; break; // i dont know why i'm doing this one
        case('\"'): outbound += "&#34;"; break;
        default: outbound += inbound[i];
        }
    }

    return outbound;
}

string AuthLoad::Decode(const string & inbound)
{

    string outbound;
    string code;

    int STATE(0);
    unsigned int i(0);
    /*
    States:
    0 - not in HTML code
    1 - in HTML code
    2 - found ending ';' in HTML code
    */
    for (i = 0; i < inbound.size(); i++)
    {
        switch (inbound[i])
        {
        case('&'): STATE = 1; code = '&'; break;
        case(';'): STATE = 2; code += ';'; break;
        default:
            {
                if ( STATE > 0 )
                    code += inbound[i];
                else
                    outbound += inbound[i];
            }
        }
        if (STATE == 2)
        {
            STATE = 0;
            if (code == "&#35;")
                outbound += '#';
            if (code == "&#38;")
                outbound += '&';
            if (code == "&#59;")
                outbound +=';';
            if (code == "&#39;")
                outbound += '\'';
            if (code == "&#34;")
                outbound += '\"';
        }
    }

    return outbound;

}

bool 
AuthLoad::Exists(const string& username)
{
    ifstream in;

    string filename = gmCrypto::Encode(username);

    string toOpen = m_directory + filename + string(".kim");
    in.open(toOpen.c_str());

    if (in.fail() == true)
    {
        return false;
    }
    return true;
}

bool
AuthLoad::SetActivePassword(SecByteBlock& password)
{

    if (AuthLoad::GetStatus() == OFFLINE)
        return false;

    //m_config.setProperty("password", (const char*)(void *)Hash(password));
	
	// set new password
	m_diskPass = Hash(password);

	// save with new password
	CommitToFile();

    return true;

}

string 
AuthLoad::GetActiveLogin()
{
    if (AuthLoad::GetStatus() == OFFLINE)
        return "";

    return m_config->property("username");
}

bool 
AuthLoad::Encrypt(const string& filename, const SecByteBlock& key, const string& data)
{

	LogText("PlainText:");
	LogText(data.c_str());
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

	LogText("mac enc:");
	LogText((const char *)(const unsigned char *)myMac);

	LogText("Ciphertext:");
	LogText(gmCrypto::Encode(cipherText, cipherText.Size()));

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

string 
AuthLoad::Decrypt(const string& filename, const SecByteBlock &key)
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
	LogText("mac:");
	LogText((const char *)(const unsigned char *)hmac);

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

/*
    -----
    $Log: authload.cpp,v $
    Revision 1.2  2002/06/06 18:43:02  thementat
    Added copyrights, fixed cryptography compile errors, lib builds in vc7

    Revision 1.1.1.1  2002/06/06 17:21:53  thementat
    Checkin of new sources BETA 2

    Revision 1.21  2001/12/17 18:33:42  mentat
    Importing changes for NNIM Beta 1 client.

    Revision 1.20  2001/12/16 20:14:39  mentat
    opps, checked in a wrong toc ver.

    Revision 1.19  2001/12/16 19:46:50  mentat
    Updates to TOC protocol and authload class.

    Revision 1.18  2001/12/12 02:10:46  mentat
    Added #include <sys/time.h> to tocprotocol.cpp to resolve FreeBSD compile error, fixed small error in authload.cpp

    Revision 1.17  2001/12/08 21:45:27  mentat
    Added setAway and setAllAway to Protocol Manager along with modifying some protocol code.

    Revision 1.16  2001/12/06 04:46:40  mentat
    Added setAway() and setAllAway(...) to manager class and to toc protocol, also added changes to toc so that will log in with wx client.

    Revision 1.15  2001/11/26 20:34:15  mentat
    Added functions to Authload

    Revision 1.14  2001/10/06 16:54:20  thementat
    Added GNU text.

*/
