// This file is part of sshlib. See the file 'ssh.h'
// for copyright and license information.


#define WINVER 0x0400
#include <winsock2.h>
#include <windows.h>
#include "hex.h"
#include "sshwin.h"

NAMESPACE_BEGIN(SSH)

using namespace std;

void DecodeString(BufferedTransformation &in, std::wstring &wstr)
{
	string str;
	DecodeString(in, str);
	wstr = WidenString(str);
}

void EncodeString(BufferedTransformation &out, const std::wstring &wstr)
{
	string str = NarrowString(wstr);
	EncodeString(out, str);
}



// 
// TcpipChannel
///////////////////////////////////////////////

TcpipChannel::TcpipChannel(word32 inInitialWindowSize, word32 inMaxPacketSize)
: Channel(inInitialWindowSize, inMaxPacketSize)
{
}

TcpipChannel::~TcpipChannel()
{
	if (m_socketSink.GetSocket() != INVALID_SOCKET)
	{
		try { m_socketSink.ShutDown(SD_SEND); } catch (...) {}
		try { m_socketSink.CloseSocket(); } catch (...) {}
	}
}

void TcpipChannel::ProcessReceivedData()
{
	try
	{
		// We need a loop because, in the current implementation, TimedFlush() may return
		// before all data has been sent even though there is opportunity to send it.
		while (m_socketSink.TimedFlush(0)) {}

		if (m_socketSink.GetCurrentBufferSize())
		{
			// Since m_socketSink.GetCurrentBufferSize() != 0, we conclude that the
			// flush stopped because it is not possible to send any more data at this time.

			// Since TimedFlush() uses SendReady() to see whether the socket is ready
			// for sending, no call is made to Send() that would result in EWOULDBLOCK.
			// We therefore need a WSAEventSelect() to make sure an FD_WRITE will be
			// triggered when the socket again becomes ready for sending.
			// Note also that m_socketSink represents the same socket as m_socketSource.

			WSAEventSelect(m_socketSink.GetSocket(), m_event, FD_WRITE|FD_READ|FD_CLOSE);
		}
	}
	catch (Socket::Err)
	{
		ForceClose();
	}
}

void TcpipChannel::ProcessDataToBeSent()
{
	try
	{
		m_socketSource.GeneralPump(OutWindowSpaceRemaining(), 0);

		if (m_socketSource.EofReceived())
			ScheduleEof();
	}
	catch (Socket::Err)
	{
		// ForceClose() will also cause m_receiveStopped to be set to 'true',
		// so the channel won't hang with indefinite calls to ProcessLingeringData().
		ForceClose();
	}

	// This causes data that has already been read from the socket to be pushed into
	// the SSH2 channel. If this throws, that indicates an SSH2 error, therefore
	// a try/catch block is not warranted.
	m_socketSource.Flush(true);
}

void TcpipChannel::OnEofReceived()
{
	// Need to call ProcessLingeringData() to ensure EOF is forwarded to the socket -
	// ProcessLingeringData() won't be called by sshlib if all data has been flushed already.
	ProcessLingeringData();
}

void TcpipChannel::OnCloseReceived()
{
	// No action necessary: ProcessLingeringData() will be called if there is still data
	// to process. The channel will be closed automatically after ProcessLingeringData()
	// cleans up, if it needs to.
}

void TcpipChannel::ProcessLingeringData()
{
	ProcessReceivedData();

	// If everything has been flushed, forward EOF to the socket.
	if (!CurrentRecvBufferSize() && (m_socketSink.GetSocket() != INVALID_SOCKET))
	{
		try { m_socketSink.ShutDown(SD_SEND); }
		catch (...) {}
	}
}

void TcpipChannel::ProcessWaitObjects()
{
	ResetEvent(m_event);
}



// 
// TunnelingResponder
///////////////////////////////////////////////

bool TunnelingResponder::HandleOpen(Manager &manager, BufferedTransformation &data, BufferedTransformation &response)
{
	DecodeString(data, m_targetHost);
	DecodeWord32(data, m_targetPort);
	DecodeString(data, m_originatorHost);
	DecodeWord32(data, m_originatorPort);

	if (m_type == "direct-tcpip")
		TR_ServerVerifyForwardingTarget(manager);
	else if (m_type == "forwarded-tcpip")
		TR_ClientRetrieveForwardingTarget(manager);
	else
		assert("invalid channel type" == 0);

	m_event.AttachHandle(CreateEvent(NULL, true, false, NULL), true);
	if (!m_event.HandleValid())
	{
		TR_OnEventCreationFailure(manager);
		throw ChannelOpenFailure(SSH_OPEN_RESOURCE_SHORTAGE);
	}

	try
	{
		m_socketSource.Create();
		WSAEventSelect(m_socketSource.GetSocket(), m_event, FD_CONNECT);

		m_connectStart = time(NULL);
		if (m_socketSource.Connect(m_targetHost.c_str(), m_targetPort))
		{
			m_connectCompleted = true;
			m_connectSucceeded = true;
			return ContinueHandleOpen(manager, response);
		}
	}
	catch (...)
	{
		TR_OnConnectFailure(manager);
		throw ChannelOpenFailure(SSH_OPEN_CONNECT_FAILED);
	}

	return false;
}

bool TunnelingResponder::ContinueHandleOpen(Manager &manager, BufferedTransformation &response)
{
	// See whether the connection attempt has completed.
	if (!m_connectCompleted)
	{
		if ((time(NULL) - m_connectStart) > 30)
			throw ChannelOpenFailure(SSH_OPEN_CONNECT_FAILED);

		return false;
	}

	// Connection attempt completed. See whether it was successful.
	if (!m_connectSucceeded)
	{
		TR_OnConnectFailure(manager);
		throw ChannelOpenFailure(SSH_OPEN_CONNECT_FAILED);
	}

	// Connection attempt succeeded.
	m_socketSource.Attach(new ChannelSwitch(manager, MyChannelString()));
	m_socketSink.AttachSocket(m_socketSource.GetSocket());
	m_socketSink.SetMaxBufferSize(m_inWindowSize);
	TR_OnSuccess(manager);
	return true;
}

void TunnelingResponder::ProcessWaitObjects()
{
	if (m_connectCompleted)
	{
		// Must not call WSAEnumNetworkEvents() after the connection is completed -
		// see note in ssh.h for explanation (Windows 98 compatibility issue)
		TcpipChannel::ProcessWaitObjects();
	}
	else
	{
		WSANETWORKEVENTS events;
		WSAEnumNetworkEvents(m_socketSource.GetSocket(), m_event, &events);

		if ((events.lNetworkEvents & FD_CONNECT) != 0)
		{
			m_connectCompleted = true;
			m_connectSucceeded = (events.iErrorCode[FD_CONNECT_BIT] == 0);

			if (m_connectSucceeded)
			{
				// This call to WSAEventSelect() must be here for
				// compatibility with Windows 98; see note in ssh.h.
				WSAEventSelect(m_socketSource.GetSocket(), m_event, FD_WRITE|FD_READ|FD_CLOSE);
			}
		}
	}
}



// 
// TunnelingInitiator
///////////////////////////////////////////////

TunnelingInitiator::TunnelingInitiator()
{
	m_event.AttachHandle(CreateEvent(NULL, true, false, NULL), true);
}

void TunnelingInitiator::ClientAccept(Socket &listeningSocket, const std::string &targetHost, word32 targetPort)
{
	sockaddr_in addr;
	int addrLen = sizeof(addr);
	listeningSocket.Accept(m_socketSource, (struct sockaddr *)&addr, &addrLen);

	WSAEventSelect(m_socketSource.GetSocket(), m_event, FD_WRITE|FD_READ|FD_CLOSE);

	m_originatorHost = inet_ntoa(addr.sin_addr);
	m_originatorPort = ntohs(addr.sin_port);

	m_targetHost = targetHost;
	m_targetPort = targetPort;
}

void TunnelingInitiator::ServerAccept(Socket &listeningSocket)
{
	sockaddr_in addr;
	int addrLen = sizeof(addr);
	listeningSocket.Accept(m_socketSource, (struct sockaddr *)&addr, &addrLen);

	WSAEventSelect(m_socketSource.GetSocket(), m_event, FD_WRITE|FD_READ|FD_CLOSE);

	m_originatorHost = inet_ntoa(addr.sin_addr);
	m_originatorPort = ntohs(addr.sin_port);

	sockaddr_in local;
	addrLen = sizeof(local);
	listeningSocket.GetSockName((struct sockaddr *)&local, &addrLen);
	m_targetHost = inet_ntoa(local.sin_addr);
	m_targetPort = ntohs(local.sin_port);
}

void TunnelingInitiator::ChannelSpecificOpenData(Manager &manager, BufferedTransformation &data)
{
	m_socketSource.Attach(new ChannelSwitch(manager, MyChannelString()));
	m_socketSink.AttachSocket(m_socketSource.GetSocket());
	m_socketSink.SetMaxBufferSize(m_inWindowSize);

	EncodeString(data, m_targetHost);
	EncodeWord32(data, m_targetPort);
	EncodeString(data, m_originatorHost);
	EncodeWord32(data, m_originatorPort);
}



// 
// CJobMethods
///////////////////////////////////////////////

// (Below applies only to SSH2 applications which spawn child processes -
//  which is, primarily, servers.)
// 
// We want sshlib to be able to run on any Windows platform, yet we want to have
// the ability to manage child processes through jobs on the platform where this
// is available - which is Windows 2000 only. We cannot call job methods directly
// even if the call is conditional on a platform check, because the compiler will
// still generate code to check for corresponding entry points in the DLL.
// Therefore, we have to bypass these checks by calling the methods manually.
//
// Managing child processes through jobs is better because when we terminate a job
// that includes one of our child processes, we are sure we are also terminating
// any sub-processes that the child process has created. If we terminate the child
// process directly (not by terminating its job), we will only terminate that
// child process, but not any grand-child-processes it may have spawned.

typedef HANDLE __declspec(dllimport) (__stdcall * CreateJobObjectA_Type)(LPSECURITY_ATTRIBUTES, LPCSTR);
typedef BOOL __declspec(dllimport) (__stdcall * TerminateJobObject_Type)(HANDLE, UINT);
typedef BOOL __declspec(dllimport) (__stdcall * AssignProcessToJobObject_Type)(HANDLE, HANDLE);

class CJobMethods
{
public:
	CJobMethods()
	{
		DWORD version = GetVersion();
		if (((version & 0x80000000) == 0) && ((version & 0xff) >= 5))
			m_hDll = LoadLibrary("kernel32.dll");
		else
			m_hDll = NULL;
	}

	~CJobMethods()
	{
		if (m_hDll != NULL)
			FreeLibrary(m_hDll);
	}

	bool Available() const { return m_hDll != NULL; }

	HANDLE CreateJobObject()
		{ return ((CreateJobObjectA_Type) GetProc("CreateJobObjectA"))(NULL, NULL); }

	BOOL TerminateJobObject(HANDLE hJob, UINT uExitCode)
		{ return ((TerminateJobObject_Type) GetProc("TerminateJobObject"))(hJob, uExitCode); }

	BOOL AssignProcessToJobObject(HANDLE hJob, HANDLE hProcess)
		{ return ((AssignProcessToJobObject_Type) GetProc("AssignProcessToJobObject"))(hJob, hProcess); }

protected:
	HMODULE m_hDll;

	FARPROC GetProc(LPCSTR zName)
	{
		assert(Available());
		FARPROC pProc = GetProcAddress(m_hDll, zName);
		if (pProc == NULL) throw Exception(string("entrypoint ") + string(zName) + string(" missing in kernel32.dll"));
		return pProc;
	}
};

CJobMethods g_jobMethods;



// 
// ProcessChannel
///////////////////////////////////////////////

ProcessChannel::ProcessChannel(
	Manager& manager,
	bool reportExitCodeToRemote,
	word32 inInitialWindowSize,
	word32 inMaxPacketSize)
:
	Channel(inInitialWindowSize, inMaxPacketSize),
	m_manager(manager),
	m_reportExitCodeToRemote(reportExitCodeToRemote),
	m_processStarted(false)
{
	if (g_jobMethods.Available())
	{
		m_job.AttachHandle(g_jobMethods.CreateJobObject(), true);
		if (!m_job.HandleValid())
			throw ChannelOpenFailure(SSH_OPEN_RESOURCE_SHORTAGE, "could not create job object");
	}
}

ProcessChannel::~ProcessChannel()
{
	if (m_stdinSink.HandleValid())
		m_stdinSink.CloseHandle();

	if (m_job.HandleValid())
		g_jobMethods.TerminateJobObject(m_job, 0);

	if (m_process.HandleValid())
		TerminateProcess(m_process, 0);	// in case we're not running on Windows 2000
}

static bool CreateAsynchronousPipe(RandomNumberGenerator &rng, WindowsHandle &read, WindowsHandle &write, bool writeAsync)
{
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = true;
	saAttr.lpSecurityDescriptor = NULL;

	string pipeName("\\\\.\\pipe\\");
	RandomNumberSource(rng, 16, true, new HexEncoder(new StringSink(pipeName)));

	HANDLE server, client;

	server = CreateNamedPipeA(pipeName.c_str(), (writeAsync ? PIPE_ACCESS_OUTBOUND : PIPE_ACCESS_INBOUND) | FILE_FLAG_OVERLAPPED, 0, 1, 0, 0, 0, NULL);

	if (server == INVALID_HANDLE_VALUE)
		return false;

	OVERLAPPED overlapped;
	overlapped.hEvent = CreateEvent(NULL, true, false, NULL);

	ConnectNamedPipe(server, &overlapped);

	client = CreateFileA(pipeName.c_str(), (writeAsync ? GENERIC_READ : GENERIC_WRITE), 0, &saAttr, OPEN_EXISTING, 0, 0);

	if (client == INVALID_HANDLE_VALUE)
		return false;

	read.AttachHandle(writeAsync ? client : server, true);
	write.AttachHandle(writeAsync ? server : client, true);
	return true;
}

void ProcessChannel::CreatePipes()
{
	if (!CreateAsynchronousPipe(m_manager.AccessRNG(), m_stdinRead, m_stdinSink, true)
		|| !CreateAsynchronousPipe(m_manager.AccessRNG(), m_stdoutSource, m_stdoutWrite, false)
		|| !CreateAsynchronousPipe(m_manager.AccessRNG(), m_stderrSource, m_stderrWrite, false))
	{
		throw ChannelOpenFailure(SSH_OPEN_RESOURCE_SHORTAGE, "could not create named pipes");
	}

	m_stdinSink.SetMaxBufferSize(m_inWindowSize);
	m_stdoutSource.Attach(new ChannelSwitch(m_manager, MyChannelString()));
	m_stderrSource.Attach(new ChannelSwitch(m_manager, MyChannelString()+Word32ToString(SSH_EXTENDED_DATA_STDERR)));
}

void ProcessChannel::ProcessStarted(PROCESS_INFORMATION &piProcInfo)
{
	m_processStarted = true;

	CloseHandle(piProcInfo.hThread);
	m_process.AttachHandle(piProcInfo.hProcess, true);
	if (g_jobMethods.Available())
		g_jobMethods.AssignProcessToJobObject(m_job, m_process);

	m_stdinRead.CloseHandle();
	m_stdoutWrite.CloseHandle();
	m_stderrWrite.CloseHandle();
}

void ProcessChannel::ProcessReceivedData()
{
	if (m_process.HandleValid())
	{
		try
		{
			// We need a loop because, in the current implementation, TimedFlush() may return
			// before all data has been sent even though there is opportunity to send it.
			while (m_stdinSink.TimedFlush(0)) {}
		}
		catch (WindowsPipe::Err const&)
		{
			// If the child process closes its stdin, we will see it as a
			// WindowsPipe::Err exception with a variety of possible error codes,
			// including ERROR_BROKEN_PIPE, ERROR_NO_DATA, etc
			StopReceive();
		}
	}

	if (m_process.HandleValid() && WaitForSingleObject(m_process, 0) != WAIT_TIMEOUT)
	{
		ReportExitCodeToRemote();
		m_process.CloseHandle();	// process has terminated
	}

	if (m_processStarted && !m_process.HandleValid())
	{
		// ForceClose() will also cause m_receiveStopped to be set to 'true',
		// so the channel won't hang with indefinite calls to ProcessLingeringData().
		ForceClose();
	}
}

void ProcessChannel::ProcessDataToBeSent()
{
	if (m_processStarted)
	{
		if (m_stdoutSource.GeneralPump(OutWindowSpaceRemaining(), 0))
			m_stdoutSource.Flush(true);

		if (m_stderrSource.GeneralPump(OutWindowSpaceRemaining(), 0))
			m_stderrSource.Flush(true);

		if (m_stdoutSource.EofReceived() && m_stderrSource.EofReceived())
			ScheduleEof();

		if (m_process.HandleValid() && WaitForSingleObject(m_process, 0) != WAIT_TIMEOUT)
		{
			ReportExitCodeToRemote();
			m_process.CloseHandle();	// process has terminated
		}

		if (!m_process.HandleValid())
		{
			// ForceClose() will also cause m_receiveStopped to be set to 'true',
			// so the channel won't hang with indefinite calls to ProcessLingeringData().
			ForceClose();
		}
	}
}

void ProcessChannel::OnEofReceived()
{
	// This will arrange for EOF to be sent if it can be sent immediately
	ProcessLingeringData();
}

void ProcessChannel::OnCloseReceived()
{
	// No action necessary - everything will be cleaned up in the destructor,
	// which will be called by sshlib as soon as any lingering data is processed.
}

void ProcessChannel::ProcessLingeringData()
{
	ProcessReceivedData();

	// If all lingering data has been handled, forward EOF to the process
	if (!CurrentRecvBufferSize() && m_stdinSink.HandleValid())
		m_stdinSink.CloseHandle();
}

void ProcessChannel::GetWaitObjects(WaitObjectContainer &container)
{
	if (m_processStarted && m_outWindowSize > 0)
	{
		if (!m_stdoutSource.EofReceived())
			container.push_back(m_stdoutSource.GetEvent());
		if (!m_stderrSource.EofReceived())
			container.push_back(m_stderrSource.GetEvent());
	}

	if (m_process.HandleValid())
	{
		container.push_back(m_process);

		if (m_stdinSink.GetCurrentBufferSize() > 0)
			container.push_back(m_stdinSink.GetEvent());
	}
}

BufferedTransformation* ProcessChannel::DataSink()
{
	return &m_stdinSink;
}

void ProcessChannel::ReportExitCodeToRemote()
{
	if (m_process.HandleValid() && m_reportExitCodeToRemote)
	{
		DWORD dwExitCode;
		if (GetExitCodeProcess(m_process, &dwExitCode) != STILL_ACTIVE)
		{
			BufferedTransformation& bt = *m_manager.AttachedTransformation();
			EncodeByte(bt, SSH_MSG_CHANNEL_REQUEST);
			EncodeWord32(bt, m_yourNumber);
			EncodeString(bt, string("exit-status"));
			EncodeBoolean(bt, false);
			EncodeWord32(bt, dwExitCode);
			bt.MessageEnd();
		}
	}
}

NAMESPACE_END
