#ifdef WIN32
    #pragma warning(disable:4786)
#endif

#include "crypto/config.h"
#include "crypto/misc.h"
#include "crypto/sha.h"
#include "crypto/hex.h"
#include "crypto/files.h"

#include <authload.h>
#include <cryptography.h>
#include <contact.h>
#include <globals.h>

#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace CryptoPP;


bool
AuthLoad::User::GetNet(const string& net, XMLNode& node)
{
	if (m_user().child("netconf").hasChild(net))
	{
		node =  m_user().child("netconf").child(net);
		return true;
	}

	return false;

}

 
bool 
AuthLoad::User::SetProfile(const string& info)
{
	m_user().child("profile").setData(info);

	return true;
}

string 
AuthLoad::User::GetProfile()
{
	return m_user().child("profile").data();
}

bool 
AuthLoad::User::SetAwayMessage(const string& label, const string& message)
{	
	XMLNode searcher;

	if (AuthLoad::Search(label, "item", m_user().child("away"), searcher))	
		searcher.setData(message);
	else
		m_user().child("away").addChild("item").setProperty("name", label).setData(message);

	return true;

}

string 
AuthLoad::User::GetAwayMessage(const string& label)
{
	XMLNode searcher;

	if (AuthLoad::Search(label, "item", m_user().child("away"), searcher))
		return searcher.data();
	return "";
}

bool 
AuthLoad::User::DeleteAwayMessage(const string& label)
{
	XMLNode searcher;
	int num(0);

	if (AuthLoad::BaseSearch(label, "item", m_user().child("away"), searcher, num))
	{
		searcher.delChild("item", num);
		return true;
	}

	return false;
}

bool 
AuthLoad::User::SetNet(XMLNode &net)
{
	if (m_user().child("netconf").hasChild(net.name()))
	{
		m_user().child("netconf").delChild(net.name());
		m_user().child("netconf").addChild(net);
	}
	else
	{
		m_user().child("netconf").addChild(net);
	}
	return true;
}

bool
AuthLoad::User::DeleteNet(const string& netname)
{
	if (m_user().child("netconf").hasChild(netname))
	{
		m_user().child("netconf").delChild(netname);
		return true;
	}
	return false;
}
