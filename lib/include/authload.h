// --*-c++-*--
/*
    $Id: authload.h,v 1.2 2002/06/08 18:34:21 thementat Exp $
 
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

#ifndef AUTHLOAD_H
#define AUTHLOAD_H

#include <string>

#include "crypto/config.h"
#include "crypto/misc.h"
#include "gmException.h"

#include "globals.h"
#include "xml.h"

using namespace std;
using namespace CryptoPP;

class Contact;
class AuthLoad
{
public:
 
    enum aStatus{ ONLINE, OFFLINE };
  
    AuthLoad(const string& directory = "");
    ~AuthLoad();

	/// Creates new XML config file on disk for specified user
    bool CreateNew(const string & username, SecByteBlock & password);

	/// Tries to log the specified user in, returns false if non-fatal error
    bool Login(const string & username, SecByteBlock &password);

	/// Logs current user off of system
	bool Logoff();

	/// Static XML searching algo, assigns a node by reference as return
    static bool Search(const string& dataToFind, const string& tagName, XMLNode& parentNode, XMLNode& returnNode);

	/// Same as regular search but the parent of the search term is returned
    static bool BaseSearch(const string& dataToFind, const string& tagName, XMLNode& parentNode, XMLNode& realParent, int& num);
    
	/// Counts all tags of certain type in a XML node, recursive underbelly
	static int TagCount(const string& findType, XMLNode& parentNode);

	/// Returns a vector of all tags
    static void GetAllTags(const string& findType, XMLNode& parentNode, vector<XMLNode> &result_vector);

	/// Returns a string of the XML tree in text form
    static string PrintTree(const XMLNode& base);

    /// Writes configuration to disk
	bool CommitToFile();

	/// Checks to see if a given user has a configuration
    bool Exists(const string& username);

	/// Sets active password for loged-in user
    bool SetActivePassword(SecByteBlock& password);

	/// Returns loged-in username
    string GetActiveLogin();

	/// Returns statues of authload system
    aStatus GetStatus() const { return m_status; }

protected:
	class Contacts
	{
	public:
		Contacts(AuthLoad* auth):m_auth(auth) {}
		XMLNode &GetConfig() { return m_contact(); }
		enum Type { FOLDER, BASEFOLDER, CONTACT, CONTACTBASE, PROTOCOL };

		/// Deletes all net tags in XML
		bool DeleteAllNets(const string& contactname);

		/// Deletes all info tags
		bool DeleteAllInfo(const string& contactname);

		/// Returns the number of contacts in the tree
		unsigned long GetCount();

		/// Returns the public key for the given contact
		string GetPublicKey(const string& contactname);

		/// Renames a contact if possible
		bool Rename(const string& oldname, const string& newname);

		/// Adds a contact if possible to given folder
		bool Add(const string& contactname, const string& folder = "");

		/// Deletes a contact from the tree
		bool Delete(const string& contactname);

		/// Moves a contact to a new base folder
		bool Move(const string& name, const string& newbase);

		/// Sets info of type infoname to the given user
		bool SetInfo(const string& username, const string& infoname, const string& data, const string& childof = "");

		/// Deletes a info tag named infoname for the given contact
		bool DeleteInfo(const string& username, const string& infoname);
		string GetInfo(const string& username, const string& infoname);

		bool GetInfoXML(const string& username, XMLNode& xml);
		bool SetInfoXML(const string& username, const XMLNode& xml);

		bool SetNet(const string& username, const string& netname, const string& login);
		bool DeleteNet(const string& username, const string& netname);
		bool GetAllNets(const string& username, map<string, string> &result_map);
		void GetContactsOfProtocol(const string& protocol, vector<Contact>& contacts);

		bool RenameFolder(const string& oldname, const string& newname);
		bool AddFolder(const string& folder_name, const string& base = "");
		bool DeleteFolder(const string& folder_name);
		bool MoveFolder(const string& name, const string& newbase);
    
		bool FolderExists(const string& name);
		bool Exists(const string& name);
 
	private:
		XMLNode &m_contact() 
		{ 
			if (!m_auth->m_config) 
				throw gmException("Pointer Error", gmException::gFATAL); 
			else 
				return m_auth->m_config->child("contacts").child("folder");
		}

		AuthLoad* m_auth;
	};

	class Globals
	{
	public:
		Globals(AuthLoad* auth):m_auth(auth) {}
		bool GetWindowPos(int &x, int &y, int &w, int &h, const string& windowname);
		bool GetValue(const string& value, string &result_string);
		bool SetValue(const string& value, const string& setting);
		bool SetWindowPos(int x, int y, int w, int h, const string& windowname);

	private:
		XMLNode &m_global() 
		{ 
			if (!m_auth->m_config) 
				throw gmException("Pointer Error", gmException::gFATAL); 
			else 
				return m_auth->m_config->child("global");
		}

		AuthLoad* m_auth;

	};

	class User
	{
	public:
		User(AuthLoad* auth):m_auth(auth) {}

		void SetPrivateKey(const string & key);
		void SetPublicKey(const string & key);
		bool GetNet(const string& net, XMLNode& node);
		bool SetProfile(const string& info);
		string GetProfile();

		bool SetAwayMessage(const string & label, const string& message);
		string GetAwayMessage(const string& label);
		bool DeleteAwayMessage(const string& label);

		bool SetNet(XMLNode & net);
		bool DeleteNet(const string& netname);
      
	private:
		XMLNode &m_user() 
		{ 
			if (!m_auth->m_config) 
				throw gmException("Pointer Error", gmException::gFATAL); 
			else 
				return m_auth->m_config->child("user");
		}
		AuthLoad* m_auth;
		
	};

    /// Replace HTML formated unfriendly sequences with original sequence
    string Encode(const string & inbound);
    /// Replace HTML/XML unfriendlies with HTML codes.
    string Decode(const string & inbound);

    SecByteBlock Hash(SecByteBlock & password);

    bool Encrypt(const string& filename, const SecByteBlock &key, const string& data);
    string Decrypt(const string& filename, const SecByteBlock &key);

    virtual void LogText(const string& text) { }


private:

	/// The base config XML node
	XMLNode * m_config;
 
	Contacts *m_contacts;
	User *m_user;
	Globals *m_global;

	/// The directory that the config is stored in
	string m_directory;

	/// The filename of the current user's config
	string m_filename;

	// the password has used to encrypt/decrypt to disk
	SecByteBlock m_diskPass;

	aStatus m_status;

public:

 	Contacts& C() { return *m_contacts; }
	User& U() { return *m_user; }
	Globals& G() { return *m_global; }

	const XMLNode ReturnXML() { return *m_config; }

};

#endif

/*
    -----
    $Log: authload.h,v $
    Revision 1.2  2002/06/08 18:34:21  thementat
    Added comments and fixed VC7 project dirs

    Revision 1.1.1.1  2002/06/06 17:21:53  thementat
    Checkin of new sources BETA 2

    Revision 1.19  2001/12/17 18:33:42  mentat
    Importing changes for NNIM Beta 1 client.

    Revision 1.18  2001/12/16 20:14:39  mentat
    opps, checked in a wrong toc ver.

    Revision 1.17  2001/12/16 19:46:50  mentat
    Updates to TOC protocol and authload class.

    Revision 1.16  2001/12/08 21:45:27  mentat
    Added setAway and setAllAway to Protocol Manager along with modifying some protocol code.

    Revision 1.15  2001/12/06 04:46:40  mentat
    Added setAway() and setAllAway(...) to manager class and to toc protocol, also added changes to toc so that will log in with wx client.

    Revision 1.14  2001/11/26 20:30:03  mentat
    Adding gmstorage.h (used to be storage.h) modified sign.h to include the right file and added classes to authload.h/cpp

    Revision 1.13  2001/10/06 16:46:34  thementat
    Added GNU text.


*/
