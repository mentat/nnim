// --*-c++-*--
/*
    $Id: authload.cpp,v 1.8 2002/06/21 19:03:15 thementat Exp $
 
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

#include "boost/smart_ptr.hpp"
#include "crypto/cryptlib.h"
#include "crypto/misc.h"

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
using namespace boost;

AuthLoad::AuthLoad(const string& directory)
: m_contacts(this), m_user(this), m_global(this)
{
	// set initial status to offline - no user logged in
    m_status = OFFLINE;

	// set initial user directory to whatever is passed to constructor
    m_directory = directory;

}

AuthLoad::~AuthLoad()
{
}

bool AuthLoad::Logoff()
{
	// save data
    CommitToFile();

	// set status to offline
    m_status = OFFLINE;

	// set the config to a empty xml node
    m_config.reset();

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

    m_config.reset( new XMLNode() );

	// create a new file to hold user data
    m_filename = gmCrypto::Encode(username) + string(".kim");

	// hash the user's password to use as file key
    m_diskPass.CleanNew(gmCrypto::GetDigestSize(gmCrypto::SHA_384));
    m_diskPass = gmCrypto::Hash(password, gmCrypto::SHA_384);

	// set up initial values in the xml node
    m_config->setName("config").setProperty("username", username);

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


    m_config.reset( new XMLNode() );

    m_filename = gmCrypto::Encode(username) + string(".kim");

    m_diskPass.CleanNew(gmCrypto::GetDigestSize(gmCrypto::SHA_384));
    m_diskPass = gmCrypto::Hash(password, gmCrypto::SHA_384);


    // plain text read in of config for testing
	ifstream in;
    string tmp, inbound;
    in.open("test.xml");
    while (getline(in, tmp))
        inbound += tmp;

    *m_config << inbound;
#if 0
    *m_config << gmCrypto::DecryptFile(m_directory + m_filename, m_diskPass);

	ofstream out;

	out.open("hi.txt");

	out << *m_config;

    if (!(m_config->property("username") == username))
    {
		throw(gmException("Critical error, username does not match but file was decrypted!", gmException::gUSER));
    } 
	else
        LogText("Username matches.");
#endif
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
        gmCrypto::EncryptFile(m_directory + m_filename, m_diskPass, data.c_str());
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

    if (in.fail() || in.bad())
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
	
    m_diskPass.CleanNew(gmCrypto::GetDigestSize(gmCrypto::SHA_384));
	// set new password
    m_diskPass = gmCrypto::Hash(password, gmCrypto::SHA_384);

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


/*
    -----
    $Log: authload.cpp,v $
    Revision 1.8  2002/06/21 19:03:15  thementat
    NNIM compiles and links in gcc 2.96 20000731

    Revision 1.7  2002/06/16 04:54:37  thementat
    Disabled file encryption for testing, temp saves to test.xml

    Revision 1.6  2002/06/16 04:08:28  thementat
    Hopefully fixed Authload and related classes.

    Revision 1.5  2002/06/13 16:38:50  thementat
    Major work on the SSH2 protocol and authload changes.

    Revision 1.4  2002/06/10 13:28:04  thementat
    Fixed file digest bug.

    Revision 1.3  2002/06/09 20:28:46  thementat
    Tried to fix referencing in authload, moved crypto out of authload, more auto_ptrs

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
