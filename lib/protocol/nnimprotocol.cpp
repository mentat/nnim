#include "nnimprotocol.h"

NNIMProtocol::NNIMProtocol(const XMLNode &conf,ProtocolManager *manager,
                   const string &name)
				   : PeerProtocol(conf, manager, name)
{

}

void NNIMProtocol::handleIncoming(NetworkServer * srv, Network * newNet)
{
	m_nets.push_back(newNet);

}
