#pragma warning(disable:4786)
#include <xmlnode.h>
#include <contact.h>

#include "contact.h"

Contact::Contact():
  m_crypto(NULL),m_user(),m_status(Contact::Offline)
{

}
Contact::Contact(XMLNode &config):
        m_crypto(NULL),m_user(config),m_status(Contact::Offline)
{
}

void Contact::operator=(const Contact &other)
{
  if (m_crypto)
    delete m_crypto;

  m_user=other.m_user;
  m_status=other.m_status;
  m_crypto=other.m_crypto;
}

bool operator==(Contact lhs,Contact rhs)
{
  return lhs.protocol() == rhs.protocol() && lhs.serverId() == rhs.serverId();
};

bool operator!=(Contact lhs,Contact rhs)
{
  return lhs.protocol() != rhs.protocol() || lhs.serverId() != rhs.serverId();
};

