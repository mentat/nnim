#ifndef NNIMPROTOCOL_H
#define NNIMPROTOCOL_H

#include "peerprotocol.h"

class NNIMProtocol: public PeerProtocol
{
public:

	NNIMProtocol(const XMLNode &conf,ProtocolManager *manager,
                   const string &name="NNIM");

	void handleIncoming(NetworkServer * srv, Network * newNet);


};


#endif
