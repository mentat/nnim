// This file is part of sshlib. See the file 'ssh.h'
// for copyright and license information.


#ifndef SSHWIN_H
#define SSHWIN_H

// std
#include <time.h>

// Crypto++
#include "socketft.h"
#include "winpipes.h"

// mulib
#include "Utf8.h"
#include "datavect.h"

// sshlib
#include "ssh.h"


NAMESPACE_BEGIN(SSH)

using namespace CryptoPP;

class WaitObjectContainer : public datavect<HANDLE> {};

void DecodeString(BufferedTransformation &in, std::wstring &wstr);
void EncodeString(BufferedTransformation &out, const std::wstring &wstr);



// 
// TcpipChannel
///////////////////////////////////////////////

// Parent class for TunnelingInitiator and TunnelingResponder.
// For port forwarding, you should use one of the two derived classes instead
// of using TcpipChannel directly.

class TcpipChannel : public Channel
{
public:
	// inMaxPacketSize must be some number of bytes less than SSHLIB_IN_MAX_PACKETLEN
	TcpipChannel(
		word32 inInitialWindowSize = CHANNEL_MAX_IN_PACKET_SIZE,
		word32 inMaxPacketSize = CHANNEL_MAX_IN_PACKET_SIZE);

	~TcpipChannel();

	void ProcessReceivedData();
	void ProcessDataToBeSent();
	void OnEofReceived();
	void OnCloseReceived();
	void ProcessLingeringData();
	word32 MaxRecvBufferSize() { return m_socketSink.GetMaxBufferSize(); }
	word32 CurrentRecvBufferSize() { return m_socketSink.GetCurrentBufferSize(); }
	BufferedTransformation* DataSink() { return &m_socketSink; }

	unsigned int MaxWaitObjects() const { return 1; }
	void GetWaitObjects(WaitObjectContainer &container) { container.push_back(m_event); }
	void ProcessWaitObjects();

	WindowsHandle m_event;
	SocketSource m_socketSource;  // m_socketSource and m_socketSink represent the same socket
	SocketSink m_socketSink;
};



// 
// TunnelingResponder
///////////////////////////////////////////////

// This class can be used by an SSH2 client to initiate "direct-tcpip" port forwarding,
// or by an SSH2 server to initiate "forwarded-tcpip" port forwarding.

class TunnelingResponder : public TcpipChannel
{
public:
	TunnelingResponder() : m_connectCompleted(false), m_connectSucceeded(false) {}

	bool HandleOpen(Manager &manager, BufferedTransformation &data, BufferedTransformation &response);
	bool ContinueHandleOpen(Manager &manager, BufferedTransformation &response);

	void ProcessWaitObjects();

	// This method should be implemented by clients to investigate m_targetHost and
	// m_targetPort and replace them with the host to actually connect to.
	virtual void TR_ClientRetrieveForwardingTarget(Manager &manager)
		{ assert("should be implemented when used on client, should not be called when used on server" == 0); }

	// This method should be implemented by servers to investigate m_targetHost and
	// m_targetPort and throw a ChannelOpenFailure exception if they are unacceptable.
	virtual void TR_ServerVerifyForwardingTarget(Manager &manager)
		{ assert("should be implemented when used on client, should not be called when used on server" == 0); }

	// Use these methods for any logging that might be performed.
	virtual void TR_OnSuccess(Manager &manager) = 0;
	virtual void TR_OnEventCreationFailure(Manager &manager) = 0;
	virtual void TR_OnConnectFailure(Manager &manager) = 0;

	// Note that, when used on client-side (i.e., for "forwarded-tcpip" channels),
	// HandleOpen() initializes m_targetHost and m_targetPort with the interface and
	// port of the listening socket that was connected to on the server's side.
	// The TR_ClientRetrieveForwardingTarget() method should use this data to determine
	// what target host and port to connect to, and should replace m_targetHost and
	// m_targetPort with the connection parameters for that host.
	std::string m_targetHost, m_originatorHost;
	word32 m_targetPort, m_originatorPort;

protected:
	bool m_connectCompleted;
	bool m_connectSucceeded;
	time_t m_connectStart;
};



// 
// TunnelingInitiator
///////////////////////////////////////////////

// This class can be used by an SSH2 client to respond to a "forwarded-tcpip" channel
// open request, or by an SSH2 server to respond to a "direct-tcpip" channel open request.

// Note that HandleOpenConfirmation() and HandleOpenFailure() still have to be
// implemented by the consumer.
class TunnelingInitiator : public TcpipChannel
{
public:
	TunnelingInitiator();

	// Client's accept method ("direct-tcpip" channel type): uses targetHost and targetPort
	// when sending the channel open request. Should be called after the channel is created,
	// but before Manager::OpenChannel() is called.
	void ClientAccept(Socket &listeningSocket, const std::string &targetHost, word32 targetPort);

	// Server's accept method ("forwarded-tcpip" channel type): uses the listening socket's
	// interface and port when sending the channel open request. Should be called after the
	// channel is created, but before Manager::OpenChannel() is called.
	void ServerAccept(Socket &listeningSocket);

	void ChannelSpecificOpenData(Manager &manager, BufferedTransformation &data);

	std::string m_targetHost, m_originatorHost;
	word32 m_targetPort, m_originatorPort;
};



// 
// ProcessChannel
///////////////////////////////////////////////

// Used for SSH2 terminal sessions

class ProcessChannel : public Channel
{
public:
	ProcessChannel(
		Manager& manager,
		bool reportExitCodeToRemote,
		word32 inInitialWindowSize = CHANNEL_MAX_IN_PACKET_SIZE,
		word32 inMaxPacketSize = CHANNEL_MAX_IN_PACKET_SIZE);

	~ProcessChannel();

	void CreatePipes();
	void ProcessStarted(PROCESS_INFORMATION &piProcInfo);

	void ProcessReceivedData();
	void ProcessDataToBeSent();
	void OnEofReceived();
	void OnCloseReceived();
	void ProcessLingeringData();
	word32 MaxRecvBufferSize() {return m_stdinSink.GetMaxBufferSize();}
	word32 CurrentRecvBufferSize() {return m_stdinSink.GetCurrentBufferSize();}
	unsigned int MaxWaitObjects() const {return 4;}
	void GetWaitObjects(WaitObjectContainer &container);
	void ProcessWaitObjects() {}
	BufferedTransformation* DataSink();

	void ReportExitCodeToRemote();

	Manager& m_manager;
	bool m_reportExitCodeToRemote;
	bool m_processStarted;
	WindowsHandle m_job, m_process, m_stdinRead, m_stdoutWrite, m_stderrWrite;
	WindowsPipeSink m_stdinSink;
	WindowsPipeSource m_stdoutSource, m_stderrSource;
};

NAMESPACE_END

#endif
