// --*-c++-*--
/*
    $Id: auth_contact.cpp,v 1.2 2002/06/08 18:34:21 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2001  Jesse Lovelace
 
    This program is free software you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#pragma warning(disable:4786)

#include <string>
#include <iostream>

#include "crypto/config.h"
#include "crypto/misc.h"

#include "authload.h"
#include "xmlnode.h"
#include "contact.h"

using namespace std;
using namespace CryptoPP;

bool 
AuthLoad::Contacts::DeleteAllNets(const string& contactname)
{
    XMLNode searcher;

    if (!AuthLoad::Search(contactname, "contact", m_contact(), searcher))
        return false;

    searcher.delChild("nets");
    searcher.addChild("nets");

    return true;
    
}
bool 
AuthLoad::Contacts::DeleteAllInfo(const string& contactname)
{
    XMLNode searcher;

    if (!AuthLoad::Search(contactname, "contact", m_contact(), searcher))
        return false;

    searcher.delChild("info");
    searcher.addChild("info");

    return true;
    
}

unsigned long 
AuthLoad::Contacts::GetCount()
{
    return AuthLoad::TagCount("contact", m_contact());
}

bool 
AuthLoad::Contacts::Rename(const string& oldname, const string& newname)
{
    XMLNode searcher;

    if (!AuthLoad::Search(oldname, "contact", m_contact(), searcher))
        return false;

    if (Exists(newname))
        return false;

    searcher.setName(newname);

    return true;
}

string 
AuthLoad::Contacts::GetPublicKey(const string& contactname)
{

    XMLNode searcher;

    if (!AuthLoad::Search(contactname, "contact", m_contact(), searcher))
        return "";

    return searcher.child("key").data();

}

bool 
AuthLoad::Contacts::Add(const string& contactname, const string& folder)
{
    if (Exists(contactname))
	{ 
		m_auth->LogText("Contact exists.");
		return false; 
	}

    XMLNode searcher;
        
    if (folder == "")
        searcher = m_contact();
    else 
        if (!AuthLoad::Search(folder, "folder", m_contact(), searcher))
            return false;

    searcher.addChild("contact").setProperty("name", contactname);

    return true;

}

bool 
AuthLoad::Contacts::Delete(const string& contactname)
{
    int num;
    XMLNode baseSearch;
    
    if (!AuthLoad::BaseSearch(contactname, "contact", m_contact(), baseSearch, num))
        return false;

    baseSearch.delChild("contact", num);
    return true;
}


bool 
AuthLoad::Contacts::Move(const string& name, const string& newbase)
{
    XMLNode searcher;
    
    if (!AuthLoad::Search(name, "contact", m_contact(), searcher))
        return false;

    int num;
    XMLNode oldBase;
    
    if (!AuthLoad::BaseSearch(name, "contact", m_contact(), oldBase, num))
        return false;

    XMLNode newBase;
    
    if (newbase == "")
        newBase = m_contact();
    else
        if (!AuthLoad::Search(newbase, "folder", m_contact(), newBase))
            return false;

    newBase.addChild(searcher);
    oldBase.delChild("contact", num);

    return true;
}

bool 
AuthLoad::Contacts::SetInfo(const string& username, const string& infoname, const string& data, const string& childof)
{
    // see if this contact exists
    XMLNode user;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), user))
        return false;

    XMLNode item;
    
    // see if the item exists
    if (AuthLoad::Search(infoname, "item", user.child("info"), item))
    {
        item.setProperty("data",data);
        return true;
    }

    // otherwise, see if a parent node was specified
    if (childof != "")
    {
        XMLNode childOfNode;
        
        if (!AuthLoad::Search(childof, "item", user.child("info"), childOfNode))
            return false;
        // the parent node specified does not exist, return false


        // the parent node does exist, add this new child and return true
        childOfNode.addChild("item").setProperty("name", infoname).setProperty("data",data);

        return true;

    }

    // no parent node specified, just add to base <info/> tag
    user.child("info").addChild("item").setProperty("name", infoname).setProperty("data",data);
    return true;

}

bool 
AuthLoad::Contacts::DeleteInfo(const string& username, const string& infoname)
{
    XMLNode contact;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), contact))
        return false;

    int num;

    XMLNode info;
    
    if (!AuthLoad::BaseSearch(infoname, "item", contact.child("info"), info, num))
        return false;

    info.delChild("item", num);

    return true;



}

string 
AuthLoad::Contacts::GetInfo(const string& username, const string& infoname)
{

    XMLNode contact;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), contact))
        return "";


    XMLNode info;
    
    if (!AuthLoad::Search(infoname, "item", contact.child("info"), info))
        return "";

    return info.data();
}

bool 
AuthLoad::Contacts::SetNet(const string& username, const string& netname, const string& login)
{
    // see if this contact exists
    XMLNode user;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), user))
        return false;

    XMLNode item;
    
    // see if the item exists
    if (AuthLoad::Search(netname, "item", user.child("nets"), item))
    {
		cout << "Found this item\n";
        item.setProperty("login",login);
        return true;
    }


    // no parent node specified, just add to base <info/> tag
    user.child("nets").addChild("item").setProperty("name", netname).setProperty("login",login);
    return true;


}

bool 
AuthLoad::Contacts::DeleteNet(const string& username, const string& netname)
{
    XMLNode contact;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), contact))
        return false;

    int num;

    XMLNode info;
    
    if (!AuthLoad::BaseSearch(netname, "item", contact.child("nets"), info, num))
        return false;

    info.delChild("item", num);

    return true;


}

bool 
AuthLoad::Contacts::GetAllNets(const string& username, map<string, string> &result_map)
{
    result_map.clear();

    XMLNode contact;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), contact))
        return false;

    for (int i = 1; i <= contact.child("nets").numChildren("item"); i++)
    {
        result_map[contact.child("nets").child("item",i).property("name")] = contact.child("nets").child("item",i).property("data");
    }

    return true;

}
  //void AuthLoad::Contacts::RecursiveListUsersForProtocol(XMLNode& parentNode, const string& proto, vector<XMLNode*>& searcher){}    
 /*   
bool
AuthLoad::Contacts::GetFoldersOfBase(vector<string> &result_vector, const string& base)
{
    result_vector.clear();

    XMLNode xmlbase;

    if (base == "")
        xmlbase = m_contact();
    else 
        if (!Search(base, "folder", m_contact(), xmlbase))
            return false;

    for (int i = 1; i <= xmlbase.numChildren("folder"); i++)
        result_vector.push_back(xmlbase.child("folder", i).property("name"));

    return true;


}*/
/*
bool
AuthLoad::Contacts::GetContactsOfBase(vector<string> &result_vector, const string& base)
{
    result_vector.clear();

    XMLNode xmlbase;

    if (base == "")
        xmlbase = m_contact();
    else 
        if (!Search(base, "folder", m_contact(), xmlbase))
            return false;

    for (int i = 1; i <= xmlbase.numChildren("contact"); i++)
        result_vector.push_back(xmlbase.child("contact", i).property("name"));

    return true;

}
*/
void 
AuthLoad::Contacts::GetContactsOfProtocol(const string& protocol, vector<Contact>& contacts)
{
    contacts.clear();

    vector<XMLNode> allContacts;
    AuthLoad::GetAllTags("contact", m_contact(), allContacts);
    
    for (unsigned int i = 0; i < allContacts.size(); i++)
    {
        XMLNode temp;
        if (Search(protocol, "item", allContacts[i], temp))
        {
            Contact c;

            c.setProtocol(protocol);
            c.setServerId(temp.data());
            c.setNick(allContacts[i].property("name"));
            contacts.push_back(c);
        }
    }
}



bool 
AuthLoad::Contacts::RenameFolder(const string& oldname, const string& newname)
{
    XMLNode fold;
    
    if (!AuthLoad::Search(oldname, "folder", m_contact(), fold))
        return false;

    fold.setProperty("name", newname);

    return true;
}

bool 
AuthLoad::Contacts::AddFolder(const string& folder_name, const string& base)
{
    if (FolderExists(folder_name))
        return false;

    XMLNode searcher;
        
    if (base == "")
        searcher = m_contact();
    else 
        if (!AuthLoad::Search(base, "folder", m_contact(), searcher))
            return false;

    searcher.addChild("folder").setProperty("name", folder_name);

    return true;
}

bool
AuthLoad::Contacts::DeleteFolder(const string& folder_name)
{
    int num;
    XMLNode base;
    
    if (!AuthLoad::BaseSearch(folder_name, "folder", m_contact(), base, num))
        return false;

    base.delChild("folder", num);

    return true;

}

bool 
AuthLoad::Contacts::MoveFolder(const string& name, const string& newbase)
{
    int num;

    XMLNode origbase;
    
    if (!AuthLoad::BaseSearch(name, "folder", m_contact(), origbase, num))
        return false;
    // folder of that name not found

    XMLNode tobase;
    
    if (!AuthLoad::Search(name, "folder", m_contact(), tobase))
        return false;

    tobase.addChild(origbase.child("folder", num));

    origbase.delChild("folder", num);

    return true;

}
    

bool 
AuthLoad::Contacts::FolderExists(const string& name)
{
    XMLNode temp;

    if (AuthLoad::Search(name, "folder", m_contact(), temp))
        return true;
    return false;

}

bool 
AuthLoad::Contacts::Exists(const string& name)
{
    
    XMLNode temp;
    if (AuthLoad::Search(name, "contact", m_contact(), temp))
        return true;

    return false;
}
  
/*bool 
AuthLoad::Contacts::NetExist(const string& netname)
{
return false;
}

string 
AuthLoad::Contacts::ServerIdExists(const string& serverId, const string& protocol)
{
return "";
}*/


bool 
AuthLoad::Contacts::GetInfoXML(const string& username, XMLNode& xml)
{
    XMLNode contact;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), contact))
        return false;

    xml = contact.child("info");
    return true;

}

bool 
AuthLoad::Contacts::SetInfoXML(const string& username, const XMLNode& xml)
{
    XMLNode contact;
    
    if (!AuthLoad::Search(username, "contact", m_contact(), contact))
        return false;

    contact.child("info") = xml;
    return true;

}
