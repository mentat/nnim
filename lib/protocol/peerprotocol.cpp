/*
    $Id: peerprotocol.cpp,v 1.1 2002/06/06 17:21:48 thementat Exp $

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
    $Log: peerprotocol.cpp,v $
    Revision 1.1  2002/06/06 17:21:48  thementat
    Initial revision

    Revision 1.4  2001/10/05 14:11:00  abelsson
    Added debug() macro for debug output.

    Revision 1.3  2001/10/02 22:51:57  estyrke
    Added some more boilerplates...

*/

#pragma warning(disable:4786)
#include <string>
#include <map>

#include "basenetwork.h"

#include "protocol.h"
#include "manager.h"
#include "peerprotocol.h"
#include "log.h"
#include "xml.h"

using namespace std;

PeerProtocol::PeerProtocol(const XMLNode &conf,ProtocolManager *manager,
                   const string &name)
: Protocol(conf, manager, name)
{
}

