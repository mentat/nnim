// This file is part of sshlib. See the file 'ssh.h'
// for copyright and license information.


#include "asn.h"
#include "rsa.h"
#include "base64.h"
#include "hex.h"
#include "ssh.h"
#include "dh.h"
#include "des.h"
#include "aes.h"
#include "blowfish.h"
#include "twofish.h"
#include "cbc.h"
#include "arc4.h"
#include "cast.h"
#include "hmac.h"
#include "md5.h"
#include "sha.h"
#include "trunhash.h"
#include "zlib.h"
#include "channels.h"
#include "babble.h"
#include <sstream>


using namespace std;
using namespace CryptoPP;


NAMESPACE_BEGIN(SSH)


char const* const c_zSshlibVersion = "1.27";


#define BEGIN_MAP() switch (n) {
#define MAP(REASON) case REASON: return #REASON;
#define MAP_EX(REASON, DESC) case REASON: return DESC;
#define END_MAP() default: { ostringstream o; o << n << " (unrecognized)"; return o.str(); } }

string DescribeDisconnectReason(DisconnectReason n)
{
	BEGIN_MAP()
	MAP(SSH_DISCONNECT_HOST_NOT_ALLOWED_TO_CONNECT)
	MAP(SSH_DISCONNECT_PROTOCOL_ERROR)
	MAP(SSH_DISCONNECT_KEY_EXCHANGE_FAILED)
	MAP(SSH_DISCONNECT_RESERVED)
	MAP(SSH_DISCONNECT_MAC_ERROR)
	MAP(SSH_DISCONNECT_COMPRESSION_ERROR)
	MAP(SSH_DISCONNECT_SERVICE_NOT_AVAILABLE)
	MAP(SSH_DISCONNECT_PROTOCOL_VERSION_NOT_SUPPORTED)
	MAP(SSH_DISCONNECT_HOST_KEY_NOT_VERIFIABLE)
	MAP(SSH_DISCONNECT_CONNECTION_LOST)
	MAP(SSH_DISCONNECT_BY_APPLICATION)
	MAP(SSH_DISCONNECT_TOO_MANY_CONNECTIONS)
	MAP(SSH_DISCONNECT_AUTH_CANCELLED_BY_USER)
	MAP(SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE)
	MAP(SSH_DISCONNECT_ILLEGAL_USER_NAME)
	END_MAP()
}

string DescribeChannelOpenFailureReason(ChannelOpenFailureReason n)
{
	BEGIN_MAP()
	MAP(SSH_OPEN_ADMINISTRATIVELY_PROHIBITED)
	MAP(SSH_OPEN_CONNECT_FAILED)
	MAP(SSH_OPEN_UNKNOWN_CHANNEL_TYPE)
	MAP(SSH_OPEN_RESOURCE_SHORTAGE)
	END_MAP()
}

string DescribeMessageType(byte n)
{
	BEGIN_MAP()
	MAP(SSH_MSG_DISCONNECT)
	MAP(SSH_MSG_IGNORE)
	MAP(SSH_MSG_UNIMPLEMENTED)
	MAP(SSH_MSG_DEBUG)
	MAP(SSH_MSG_SERVICE_REQUEST)
	MAP(SSH_MSG_SERVICE_ACCEPT)
	MAP(SSH_MSG_KEXINIT)
	MAP(SSH_MSG_NEWKEYS)
	MAP(SSH_MSG_KEXDH_INIT)
	MAP(SSH_MSG_KEXDH_REPLY)
	MAP(SSH_MSG_USERAUTH_REQUEST)
	MAP(SSH_MSG_USERAUTH_FAILURE)
	MAP(SSH_MSG_USERAUTH_SUCCESS)
	MAP(SSH_MSG_USERAUTH_BANNER)
	MAP_EX(SSH_MSG_USERAUTH_METHODSPECIFIC_1, "SSH_MSG_USERAUTH_METHODSPECIFIC_1 (PK_OK, PASSWD_CHANGEREQ)")
	MAP(SSH_MSG_GLOBAL_REQUEST)
	MAP(SSH_MSG_REQUEST_SUCCESS)
	MAP(SSH_MSG_REQUEST_FAILURE)
	MAP(SSH_MSG_CHANNEL_OPEN)
	MAP(SSH_MSG_CHANNEL_OPEN_CONFIRMATION)
	MAP(SSH_MSG_CHANNEL_OPEN_FAILURE)
	MAP(SSH_MSG_CHANNEL_WINDOW_ADJUST)
	MAP(SSH_MSG_CHANNEL_DATA)
	MAP(SSH_MSG_CHANNEL_EXTENDED_DATA)
	MAP(SSH_MSG_CHANNEL_EOF)
	MAP(SSH_MSG_CHANNEL_CLOSE)
	MAP(SSH_MSG_CHANNEL_REQUEST)
	MAP(SSH_MSG_CHANNEL_SUCCESS)
	MAP(SSH_MSG_CHANNEL_FAILURE)
	END_MAP()
}

#undef BEGIN_MAP
#undef MAP
#undef END_MAP



string GetLine(BufferedTransformation& bt, char cDelim = '\n', unsigned int nGiveUpSize = 16384)
{
	string sRet;
	char acPeekBuf[128];

	while (true)
	{
		unsigned int nPeekLen = bt.Peek((byte*) acPeekBuf, sizeof acPeekBuf);

		char const* p = (char const*) memchr(acPeekBuf, cDelim, nPeekLen);
		if (p)
		{
			unsigned int nToAppend = p - acPeekBuf;
			sRet.append(acPeekBuf, nToAppend);
			bt.Skip(nToAppend + 1);
			return sRet;
		}

		sRet.append(acPeekBuf, nPeekLen);
		bt.Skip(nPeekLen);

		if (nPeekLen < sizeof acPeekBuf)
			return sRet;

		if (sRet.size() >= nGiveUpSize)
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "line too long");
	}
}


string AssertGetLine(BufferedTransformation& bt, char cDelim = '\n', unsigned int nGiveUpSize = 16384)
{
	if (!bt.AnyRetrievable())
		throw Err(BufferedTransformation::INPUT_ERROR, "premature end of data");

	return GetLine(bt, cDelim, nGiveUpSize);
}


string RightTrim(string const& s)
{
	unsigned int i = s.size();

	while (true)
	{
		if (i == 0)
			return string();

		switch (s[i-1])
		{
		case ' ': case '\t': case '\n': case '\r':
			break;
		default:
			return s.substr(0, i);
		}

		--i;
	}
}



void EncodeBoolean(BufferedTransformation& out, bool b)
{
	EncodeByte(out, b);
}

void EncodeByte(BufferedTransformation& out, byte b)
{
	out.Put(b);
}

void EncodeWord32(BufferedTransformation& out, word32 w)
{
	out.PutWord32(w);
}

void EncodeBytes(BufferedTransformation& out, byte const* s, unsigned int len)
{
	out.Put(s, len);
}

void EncodeString(BufferedTransformation& out, byte const* s, unsigned int len)
{
	out.PutWord32(len);
	out.Put(s, len);
}

void EncodeString(BufferedTransformation& out, string const& str)
{
	EncodeString(out, (byte*) str.data(), str.size());
}

void EncodeString(BufferedTransformation& out, SecByteBlock const& str)
{
	EncodeString(out, str.ptr, str.size);
}

unsigned long EncodeString(BufferedTransformation& out, BufferedTransformation const& str)
{
	out.PutWord32(str.MaxRetrievable());
	return str.CopyTo(out);
}

unsigned long EncodeString(BufferedTransformation& out, BufferedTransformation const& str, unsigned int maxLen)
{
	unsigned int len = STDMIN((unsigned long) maxLen, str.MaxRetrievable());
	out.PutWord32(len);
	return str.CopyTo(out, len);
}

void EncodeInteger(BufferedTransformation& out, Integer const& i)
{
	unsigned int len = i.NotZero() ? i.MinEncodedSize(Integer::SIGNED) : 0;
	SecByteBlock buf(len);
	i.Encode(buf, len, Integer::SIGNED);
	EncodeString(out, buf);
}

void DecodeBoolean(BufferedTransformation& out, bool& b)
{
	byte t;
	DecodeByte(out, t);
	b = t != 0;
}

void DecodeByte(BufferedTransformation& out, byte& b)
{
	if (out.Get(b) < 1)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
}

void DecodeWord32(BufferedTransformation& out, word32& w)
{
	if (out.GetWord32(w) < 4)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
}

void DecodeBytes(BufferedTransformation& out, byte* s, unsigned int len)
{
	if (out.Get(s, len) < len)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
}

void DecodeString(BufferedTransformation& in, SecByteBlock& str)
{
	word32 len;
	if (in.GetWord32(len) < 4 || in.MaxRetrievable() < len)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
	str.Resize(len);
	in.Get(str, len);
}

void DecodeString(BufferedTransformation& in, string& str)
{
	SecByteBlock buf;
	DecodeString(in, buf);
	str.assign((char*) buf.ptr, buf.size);
}

void DecodeString(BufferedTransformation& in, BufferedTransformation& str)
{
	word32 len;
	if (in.GetWord32(len) < 4 || in.MaxRetrievable() < len)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
	in.TransferTo(str, len);
}

void DecodeInteger(BufferedTransformation& in, Integer& i)
{
	SecByteBlock buf;
	DecodeString(in, buf);
	i.Decode(buf, buf.size, Integer::SIGNED);
}

static void ExpectMessageNumber(BufferedTransformation& in, byte expected)
{
	byte b;
	DecodeByte(in, b);
	if (b != expected)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected message number " + IntToString(b) + ", should be " + IntToString(expected));
}

string Word32ToString(word32 channel)
{
	return WordToString(channel);
}

word32 StringToWord32(string const& str)
{
	assert(str.size() >= 4);
	return StringToWord<word32>(str);
}

bool MatchCommaDelimitedList(string const& slist, string const& str)
{
	istringstream ss(slist);
	string listElement;
	while (getline(ss, listElement, ','))
	{
		if (str == listElement)
			return true;
	}
	return false;
}

static string MatchAlgorithms(string const& clientAlgs, string const& serverAlgs)
{
	istringstream cs(clientAlgs);
	string clientAlg;
	while (getline(cs, clientAlg, ','))
	{
		if (MatchCommaDelimitedList(serverAlgs, clientAlg))
			return clientAlg;
	}
	return "";
}

static string GetFirstAlgorithm(string const& algs)
{
	return algs.substr(0, algs.find(','));
}

bool IsValidAlgList(string const& slist, string const& supported)
{
	if (!slist.size())
		return false;

	string::size_type nStartPos = 0;
	string::size_type nEndPos = slist.find(',');

	while (true)
	{
		if (nEndPos == string::npos)
			nEndPos = slist.size();

		if (!MatchCommaDelimitedList(supported, slist.substr(nStartPos, nEndPos-nStartPos)))
			return false;

		if (nEndPos == slist.size())
			return true;

		if (nEndPos == slist.size() - 1)
			return false;

		nStartPos = nEndPos + 1;
		nEndPos = slist.find(',', nStartPos);
	}
}



//
// Signer
////////////

Signer::Signer(RandomNumberGenerator& rng, string const& algorithm, unsigned int keybits)
{
	assert(algorithm == "ssh-dss");
	m_key.reset(new DSAPrivateKey(rng, keybits));
}


Signer::Signer(BufferedTransformation& key)
{
	word32 version;
	DecodeWord32(key, version);
	if (version != 0)
		throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "unsupported private key version");
	string algorithm;
	DecodeString(key, algorithm);
	if (algorithm != "ssh-dss")
		throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "unsupported private key algorithm");

	m_key.reset(new GDSASigner<SHA>(key));
}


void Signer::EncodePrivateKey(BufferedTransformation& key) const
{
	EncodeWord32(key, 0);
	EncodeString(key, "ssh-dss");
	m_key->DEREncode(key);
}


string Signer::GetPublicKey() const
{
	string key;
	StringSink out(key);
	EncodeString(out, "ssh-dss");
	EncodeInteger(out, m_key->GetModulus());
	EncodeInteger(out, m_key->GetSubgroupSize());
	EncodeInteger(out, m_key->GetGenerator());
	EncodeInteger(out, m_key->GetPublicResidue());
	return key;
}


string Signer::SignMessage(RandomNumberGenerator& rng, byte const* message, unsigned int messageLen) const
{
	string sig1;
	StringSink out(sig1);
	EncodeString(out, "ssh-dss");
	SecByteBlock sig(m_key->SignatureLength());
	m_key->SignMessage(rng, message, messageLen, sig);
	EncodeString(out, sig);
	return sig1;
}


bool VerifySignature(byte const* key, unsigned int keyLen, 
	byte const* message, unsigned int messageLen,
	byte const* signature, unsigned int signatureLen)
{
	StringStore keyStore(key, keyLen);
	string alg;
	DecodeString(keyStore, alg);
	if (alg != "ssh-dss")
		return false;

	Integer p, q, g, y;
	DecodeInteger(keyStore, p);
	DecodeInteger(keyStore, q);
	DecodeInteger(keyStore, g);
	DecodeInteger(keyStore, y);
	DSAPublicKey pub(p, q, g, y);

	StringStore sigStore(signature, signatureLen);
	SecByteBlock sig;

	if (signatureLen == pub.SignatureLength())
	{
		// Has broken signature implementation
		sig.Resize(pub.SignatureLength());
		sigStore.TransferTo(ArraySink(sig.Begin(), sig.Size()));
	}
	else
	{
		// Regular signature implementation
		DecodeString(sigStore, alg);
		if (alg != "ssh-dss")
			return false;
		DecodeString(sigStore, sig);
	}

	if (sig.size != pub.SignatureLength())
		return false;

	return pub.VerifyMessage(message, messageLen, sig);
}



//
// Version
/////////////

Version::Version(std::string appNameAndVersion)
{
	// Check for invalid characters
	string::const_iterator it;
	for (it=appNameAndVersion.begin(); it!=appNameAndVersion.end(); ++it)
		if ((*it < 32) || (*it > 126))
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "appNameAndVersion contains invalid characters");

	// Trim leading whitespace
	string::size_type nPos = appNameAndVersion.find_first_not_of(" ");
	if (nPos == string::npos)
		throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "appNameAndVersion must not be empty");
	if (nPos > 0)
		appNameAndVersion.erase(0, nPos);

	// Trim trailing whitespace
	nPos = appNameAndVersion.find_last_not_of(" ") + 1;
	if (nPos < appNameAndVersion.size())
		appNameAndVersion.erase(nPos);

	// Set version fields
	m_protocolVersion = "2.0";
	m_softwareVersion = c_zSshlibVersion;
	m_comments = "sshlib: " + appNameAndVersion;

	m_versionString = "SSH-" + m_protocolVersion + "-" + m_softwareVersion + " " + m_comments;

	// The resulting version string including CRLF must not be longer than 255 characters
	if (m_versionString.size() > 253)
		throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "appNameAndVersion is too large");
}


Version::Version(string const& protoversion, string const& softwareversion,
				 string const& comments, string const& versionString) :
	m_protocolVersion(protoversion),
	m_softwareVersion(softwareversion),
	m_comments(comments),
	m_versionString(versionString)
{
}



//
// KexInitPacket
///////////////////

void KexInitPacket::SetKexAlgorithms(string const& algs)
{
	kex_algorithms = algs;
}


void KexInitPacket::SetServerHostKeyAlgorithms(string const& algs)
{
	server_host_key_algorithms = algs;
}


void KexInitPacket::SetEncryptionAlgorithms(string const& algs, Direction dir)
{
	if (dir == BOTH_DIRECTIONS || dir == CLIENT_TO_SERVER)
		encryption_algorithms_client_to_server = algs;
	if (dir == BOTH_DIRECTIONS || dir == SERVER_TO_CLIENT)
		encryption_algorithms_server_to_client = algs;
}


void KexInitPacket::SetMacAlgorithms(string const& algs, Direction dir)
{
	if (dir == BOTH_DIRECTIONS || dir == CLIENT_TO_SERVER)
		mac_algorithms_client_to_server = algs;
	if (dir == BOTH_DIRECTIONS || dir == SERVER_TO_CLIENT)
		mac_algorithms_server_to_client = algs;
}


void KexInitPacket::SetCompressionAlgorithms(string const&algs, Direction dir)
{
	if (dir == BOTH_DIRECTIONS || dir == CLIENT_TO_SERVER)
		compression_algorithms_client_to_server = algs;
	if (dir == BOTH_DIRECTIONS || dir == SERVER_TO_CLIENT)
		compression_algorithms_server_to_client = algs;
}


void KexInitPacket::Encode(BufferedTransformation& t) const
{
	EncodeByte(t, SSH_MSG_KEXINIT);
	EncodeBytes(t, cookie, 16);
	EncodeString(t, kex_algorithms);
	EncodeString(t, server_host_key_algorithms);
	EncodeString(t, encryption_algorithms_client_to_server);
	EncodeString(t, encryption_algorithms_server_to_client);
	EncodeString(t, mac_algorithms_client_to_server);
	EncodeString(t, mac_algorithms_server_to_client);
	EncodeString(t, compression_algorithms_client_to_server);
	EncodeString(t, compression_algorithms_server_to_client);
	EncodeString(t, languages_client_to_server);
	EncodeString(t, languages_server_to_client);
	EncodeBoolean(t, first_kex_packet_follows);
	EncodeWord32(t, 0);
}


void KexInitPacket::Decode(BufferedTransformation& s)
{
	ExpectMessageNumber(s, SSH_MSG_KEXINIT);
	DecodeBytes(s, cookie, 16);
	DecodeString(s, kex_algorithms);
	DecodeString(s, server_host_key_algorithms);
	DecodeString(s, encryption_algorithms_client_to_server);
	DecodeString(s, encryption_algorithms_server_to_client);
	DecodeString(s, mac_algorithms_client_to_server);
	DecodeString(s, mac_algorithms_server_to_client);
	DecodeString(s, compression_algorithms_client_to_server);
	DecodeString(s, compression_algorithms_server_to_client);
	DecodeString(s, languages_client_to_server);
	DecodeString(s, languages_server_to_client);
	DecodeBoolean(s, first_kex_packet_follows);
	word32 w;
	DecodeWord32(s, w);
}



//
// TransportState
////////////////////

TransportState::TransportState(RandomNumberGenerator& rng, Signer* signer, int compressionLevel)
	: m_rng(&rng)
	, m_signer(signer)
	, m_inState(WAITING_FOR_VERSION), m_outState(READY_TO_SEND_VERSION)
	, m_inSequenceNumber(0), m_outSequenceNumber(0)
	, m_kexInitPacketReceived(false), m_kexInitPacketSent(false)
	, m_serviceStarted(false)
	, m_inCipher(new Cipher), m_outCipher(new Cipher(new ByteQueue(1024)))
	, m_inMac(new Mac), m_outMac(new Mac)
	, m_inCompressor(new Compressor), m_outCompressor(new Compressor(new ByteQueue(1024), compressionLevel))
{
	if (signer)
		SetServerHostKey(m_signer->GetPublicKey());
}


void TransportState::AssertIsClient() const
{
	if (!IsClient())
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "received packet meant for client");
}


void TransportState::AssertIsServer() const
{
	if (!IsServer())
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "received packet meant for server");
}


void TransportState::AssertInState(InTransportState state) const
{
	if (GetInState() != state)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "received packet inappropriate for current transport state");
}


void TransportState::AssertOutState(OutTransportState state) const
{
	if (GetOutState() != state)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "received packet inappropriate for current transport state");
}


string TransportState::GetKexAlgorithm() const
{
	return MatchAlgorithms(	GetClientKexInitPacket().kex_algorithms,
							GetServerKexInitPacket().kex_algorithms);
}


string TransportState::GetServerHostKeyAlgorithm() const
{
	return MatchAlgorithms(	GetClientKexInitPacket().server_host_key_algorithms,
							GetServerKexInitPacket().server_host_key_algorithms);
}


string TransportState::GetEncryptionAlgorithm(Direction dir) const
{
	if (dir == CLIENT_TO_SERVER)
		return MatchAlgorithms(	GetClientKexInitPacket().encryption_algorithms_client_to_server,
								GetServerKexInitPacket().encryption_algorithms_client_to_server);
	else
		return MatchAlgorithms(	GetClientKexInitPacket().encryption_algorithms_server_to_client,
								GetServerKexInitPacket().encryption_algorithms_server_to_client);
}


string TransportState::GetMacAlgorithm(Direction dir) const 
{
	if (dir == CLIENT_TO_SERVER)
		return MatchAlgorithms(	GetClientKexInitPacket().mac_algorithms_client_to_server,
								GetServerKexInitPacket().mac_algorithms_client_to_server);
	else
		return MatchAlgorithms(	GetClientKexInitPacket().mac_algorithms_server_to_client,
								GetServerKexInitPacket().mac_algorithms_server_to_client);
}


string TransportState::GetCompressionAlgorithm(Direction dir) const 
{
	if (dir == CLIENT_TO_SERVER)
		return MatchAlgorithms(	GetClientKexInitPacket().compression_algorithms_client_to_server,
								GetServerKexInitPacket().compression_algorithms_client_to_server);
	else
		return MatchAlgorithms(	GetClientKexInitPacket().compression_algorithms_server_to_client,
								GetServerKexInitPacket().compression_algorithms_server_to_client);
}



//
// ChannelTable
//////////////////

ChannelTable::~ChannelTable()
{
	for (Iterator it = m_channels.begin(); it != m_channels.end(); ++it)
		delete *it;
}


Channel& ChannelTable::NewChannel(Channel* channel)
{
	Iterator it = find(m_channels.begin(), m_channels.end(), (Channel*) NULL);
	if (it == m_channels.end())
	{
		m_channels.push_back(channel);
		it = m_channels.end()-1;
	}
	else
		*it = channel;

	(*it)->m_myNumber = it - m_channels.begin();
	return **it;
}


Channel* ChannelTable::GetChannel(word32 myNumber)
{
	if (myNumber >= m_channels.size())
		return NULL;
	else
		return m_channels[myNumber];
}


void ChannelTable::RemoveChannel(word32 myNumber)
{
	if (myNumber < m_channels.size())
	{
		delete m_channels[myNumber];
		m_channels[myNumber] = NULL;
	}
}



//
// Channel
/////////////

void Channel::Open(Manager& manager, BufferedTransformation& t)
{
	assert(m_state == START);
	EncodeByte(t, SSH_MSG_CHANNEL_OPEN);
	EncodeString(t, m_type);
	EncodeWord32(t, m_myNumber);
	EncodeWord32(t, m_inWindowSize);
	EncodeWord32(t, m_inMaxPacketSize);
	ChannelSpecificOpenData(manager, t);
	t.MessageEnd();
	m_state = WAITING_FOR_OPEN_RESPONSE;
}


void Channel::HandleData(Manager& manager, BufferedTransformation& data, BufferedTransformation& response)
{
	word32 len;
	DecodeWord32(data, len);
	if (data.MaxRetrievable() < len)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
	if (len > m_inWindowSize)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "data exceeds window size");

	if (DataSink())
		data.TransferTo(*DataSink(), len);
	else
		data.Skip(len);
	m_inWindowSize -= len;
}


void Channel::HandleExtendedData(Manager& manager, word32 dataType, BufferedTransformation& data, BufferedTransformation& response)
{
	word32 len;
	DecodeWord32(data, len);
	if (data.MaxRetrievable() < len)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected end of packet");
	if (len > m_inWindowSize)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "data exceeds window size");

	if (ExtendedDataSink())
		data.TransferTo(ChannelSwitch(*ExtendedDataSink(), Word32ToString(dataType)), len);
	else
		HandleUnrecognizedExtendedData(data, len);

	m_inWindowSize -= len;
}



//
// Cipher
////////////

static SecByteBlock DeriveKey(Integer const& k_, SecByteBlock const& h, SecByteBlock const& sessionId, byte letter, unsigned int size)
{
	ByteQueue bq;
	EncodeInteger(bq, k_);
	SecByteBlock k(bq.CurrentSize());
	bq.Get(k, k.size);

	SHA sha;
	sha.Update(k, k.size);
	sha.Update(h, h.size);
	sha.Update(&letter, 1);
	sha.Update(sessionId, sessionId.size);
	SecByteBlock key(RoundUpToMultipleOf(size, sha.DigestSize()));
	sha.Final(key);

	for (unsigned int i=sha.DigestSize(); i<key.size; i+=sha.DigestSize())
	{
		sha.Update(k, k.size);
		sha.Update(h, h.size);
		sha.Update(key, i);
		sha.Final(key+i);
	}

	return key;
}


Cipher::Cipher(BufferedTransformation* outQueue)
	: ProxyFilter(new TransparentFilter, 0, 0, outQueue)
{
}


void Cipher::SetCipher(string const& alg, Integer const& k, SecByteBlock const& h, SecByteBlock const& sessionId, Direction direction, CipherDir cipherDir)
{
	byte ivLetter = (direction == CLIENT_TO_SERVER) ? 'A' : 'B';
	byte keyLetter = (direction == CLIENT_TO_SERVER) ? 'C' : 'D';

	m_cipher.reset(NULL);
	m_streamCipher.reset(NULL);

	if (alg == "none")
		SetFilter(new TransparentFilter);
	else if (alg == "aes256-cbc")
	{
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, 32);
		SecByteBlock iv = DeriveKey(k, h, sessionId, ivLetter, AESEncryption::BLOCKSIZE);

		if (cipherDir == ENCRYPTION)
		{
			m_cipher.reset(new AESEncryption(key, 32));
			SetFilter(new CBCRawEncryptor(*m_cipher, iv));
		}
		else
		{
			m_cipher.reset(new AESDecryption(key, 32));
			SetFilter(new CBCRawDecryptor(*m_cipher, iv));
		}
	}
	else if (alg == "blowfish-cbc")
	{
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, BlowfishEncryption::DEFAULT_KEYLENGTH);
		SecByteBlock iv = DeriveKey(k, h, sessionId, ivLetter, BlowfishEncryption::BLOCKSIZE);

		if (cipherDir == ENCRYPTION)
		{
			m_cipher.reset(new BlowfishEncryption(key));
			SetFilter(new CBCRawEncryptor(*m_cipher, iv));
		}
		else
		{
			m_cipher.reset(new BlowfishDecryption(key));
			SetFilter(new CBCRawDecryptor(*m_cipher, iv));
		}
	}
	else if (alg == "3des-cbc")
	{
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, DES_EDE3_Encryption::KEYLENGTH);
		SecByteBlock iv = DeriveKey(k, h, sessionId, ivLetter, DES_EDE3_Encryption::BLOCKSIZE);

		if (cipherDir == ENCRYPTION)
		{
			m_cipher.reset(new DES_EDE3_Encryption(key));
			SetFilter(new CBCRawEncryptor(*m_cipher, iv));
		}
		else
		{
			m_cipher.reset(new DES_EDE3_Decryption(key));
			SetFilter(new CBCRawDecryptor(*m_cipher, iv));
		}
	}
	else if (alg == "twofish-cbc")
	{
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, 32);
		SecByteBlock iv = DeriveKey(k, h, sessionId, ivLetter, TwofishEncryption::BLOCKSIZE);

		if (cipherDir == ENCRYPTION)
		{
			m_cipher.reset(new TwofishEncryption(key, 32));
			SetFilter(new CBCRawEncryptor(*m_cipher, iv));
		}
		else
		{
			m_cipher.reset(new TwofishDecryption(key, 32));
			SetFilter(new CBCRawDecryptor(*m_cipher, iv));
		}
	}
	else if (alg == "arcfour")
	{
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, 16);

		m_streamCipher.reset(new ARC4(key, 16));
		SetFilter(new StreamCipherFilter(*m_streamCipher));
	}
	else if (alg == "cast128-cbc")
	{
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, CAST128Encryption::DEFAULT_KEYLENGTH);
		SecByteBlock iv = DeriveKey(k, h, sessionId, ivLetter, CAST128Encryption::BLOCKSIZE);

		if (cipherDir == ENCRYPTION)
		{
			m_cipher.reset(new CAST128Encryption(key));
			SetFilter(new CBCRawEncryptor(*m_cipher, iv));
		}
		else
		{
			m_cipher.reset(new CAST128Decryption(key));
			SetFilter(new CBCRawDecryptor(*m_cipher, iv));
		}
	}
	else
		throw Exception("SSH::Cipher: unsupported cipher type");
}


unsigned int Cipher::RealBlockSize() const
{
	if (m_cipher.get())
		return m_cipher->BlockSize();
	else
		return 1;
}



//
// Mac
/////////

Mac::Mac(BufferedTransformation* outQueue)
	: ProxyFilter(new OpaqueFilter, 0, 0, outQueue)
	, m_mac(new NullHash)
{
}


void Mac::LastPut(byte const* inString, unsigned int length)
{
	if (m_filter.get())
		m_filter->MessageEnd();
}


static bool HasBuggyHMAC(Version const& inVersion)
{
	// 'SSH Secure Shell' v2 up to and including 2.3.0 has an HMAC bug.
	// 'F-SECURE SSH' is an OEM of 'SSH Secure Shell'.
	if ((inVersion.GetComments().substr(0, 16) != "SSH Secure Shell") &&
		(inVersion.GetComments().substr(0, 12) != "F-SECURE SSH"))
		return false;
	else
	{
		string const& v = inVersion.GetSoftwareVersion();
		string v4 = v.substr(0, 4);
		string v5 = v.substr(0, 5);
		return (v5 == "2.3.0" ||
				v4 == "2.2." || v == "2.2" ||
				v4 == "2.1." || v == "2.1" ||
				v4 == "2.0." || v == "2.0");
	}
}


void Mac::SetMac(string const& alg, Integer const& k, SecByteBlock const& h, SecByteBlock const& sessionId, Direction direction, bool generate, Version const& inVersion)
{
	byte keyLetter = (direction == CLIENT_TO_SERVER) ? 'E' : 'F';

	if (alg == "none")
		m_mac.reset(new NullHash);
	else if (alg == "hmac-sha1")
	{
		unsigned int keylen = HasBuggyHMAC(inVersion) ? 16 : 20;
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, keylen);
		m_mac.reset(new HMAC<SHA>(key, keylen));
	}
	else if (alg == "hmac-sha1-96")
	{
		unsigned int keylen = HasBuggyHMAC(inVersion) ? 16 : 20;
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, keylen);
		m_mac.reset(new TruncatedHashTemplate<HMAC<SHA> >(key, keylen, 12));
	}
	else if (alg == "hmac-md5")
	{
		unsigned int keylen = 16;
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, keylen);
		m_mac.reset(new HMAC<MD5>(key, keylen));
	}
	else if (alg == "hmac-md5-96")
	{
		unsigned int keylen = 16;
		SecByteBlock key = DeriveKey(k, h, sessionId, keyLetter, keylen);
		m_mac.reset(new TruncatedHashTemplate<HMAC<MD5> >(key, keylen, 12));
	}
	else
		throw Exception("SSH::Mac: unsupported MAC algorithm");

	if (generate)
		SetFilter(new HashFilter(*m_mac));
	else
		SetFilter(new HashVerifier(*m_mac, NULL, HashVerifier::HASH_AT_BEGIN));
}


unsigned int Mac::MacSize() const
{
	return m_mac->DigestSize();
}


bool Mac::GetLastResult() const
{
	return static_cast<HashVerifier const*>(m_filter.get())->GetLastResult();
}



//
// Compressor
////////////////

Compressor::Compressor(BufferedTransformation* outQueue, int compressionLevel)
	: ProxyFilter(new TransparentFilter, 0, 0, outQueue)
	, m_compressionLevel(compressionLevel)
{
}


void Compressor::SetCompressor(string const& alg, bool compress)
{
	if (alg == "none")
	{
		SetFilter(new TransparentFilter);
	}
	else if (alg == "zlib")
	{
		if (compress)
			SetFilter(new ZlibCompressor(NULL, ((m_compressionLevel < 0) ? 4 : m_compressionLevel), 14));
		else
			SetFilter(new ZlibDecompressor);
	}
	else
		throw Exception("SSH::Compressor: unsupported compression algorithm");
}



//
// PacketEncoder
///////////////////

PacketEncoder::PacketEncoder(TransportState& transportState, BufferedTransformation* outQueue)
	: FilterWithInputQueue(outQueue)
	, m_transportState(&transportState)
{
	m_inQueue.SetNodeSize(1024);
}


void PacketEncoder::MessageEnd(int propagation)
{
	if (m_transportState->GetOutState() == READY_TO_SEND_VERSION)
		m_inQueue.TransferTo(*AttachedTransformation());
	else
	{
#ifdef _DEBUG
		byte b;
		m_inQueue.Peek(b);
		SSH_Trace(("Sending " + DescribeMessageType(b)).c_str());

		string sHexData;
		m_inQueue.CopyTo(HexEncoder(new StringSink(sHexData)));
		SSH_Trace(("Data: " + sHexData.substr(2)).c_str());
#endif

		Compressor& compressor = m_transportState->AccessOutCompressor();
		m_inQueue.TransferTo(compressor);
		compressor.Flush(true);

		word32 payloadLen = compressor.MaxRetrievable();
		assert(payloadLen > 0);
		word32 packetLen = RoundUpToMultipleOf(payloadLen+SSH_MIN_PADDINGLEN+5, m_transportState->AccessOutCipher().BlockSize()) - 4;
		byte paddingLen = packetLen - (payloadLen+1);

		SecByteBlock padding(paddingLen);
		m_transportState->m_rng->GenerateBlock(padding, paddingLen);

		Cipher& cipher = m_transportState->AccessOutCipher();
		Mac& mac = m_transportState->AccessOutMac();

		cipher.PutWord32(packetLen);
		cipher.Put(paddingLen);
		compressor.CopyTo(cipher);
		cipher.Put(padding, padding.size);
		cipher.TransferTo(*AttachedTransformation());

		if (mac.MacSize())
		{
			mac.Detach(new Redirector(*AttachedTransformation(), false));
			mac.PutWord32(m_transportState->GetOutSequenceNumber());
			mac.PutWord32(packetLen);
			mac.Put(paddingLen);
			compressor.TransferTo(mac);
			mac.Put(padding, padding.size);
			mac.MessageEnd(0);
		}
		else
			compressor.Skip();
		m_transportState->IncrementOutSequenceNumber();
	}
}


void PacketEncoder::PutMessageEnd(byte const* inString, unsigned int length, int propagation)
{
	LazyPutter lp(m_inQueue, inString, length);
	MessageEnd(propagation);
}



//
// PacketDecoder
///////////////////

PacketDecoder::PacketDecoder(TransportState& transportState, BufferedTransformation* outQueue)
	: Filter(outQueue)
	, m_transportState(&transportState)
	, m_state(WAITING_FOR_FIRST_BLOCK)
{
}


void PacketDecoder::Put(byte const* inString, unsigned int length)
{
	LazyPutter lp(m_inQueue, inString, length);

	while (true)
	{
		Cipher& cipher = m_transportState->AccessInCipher();
		Mac& mac = m_transportState->AccessInMac();
		Compressor& compressor = m_transportState->AccessInCompressor();

		if (m_transportState->GetInState() == WAITING_FOR_VERSION)
		{
			unsigned int lineBreak = find(inString, inString+length, '\n') - inString;
			unsigned int lineLength = m_inQueue.CurrentSize()-(length-lineBreak)+1;

			if (lineLength > 255)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "version info line too long");

			if (lineBreak == length)
				break;

			string line;
			StringSink ss(line);
			m_inQueue.TransferTo(ss, lineLength);

			inString += lineBreak+1;
			length -= lineBreak+1;

			if (line.substr(0, 4) == "SSH-")
			{
				if (line[line.size()-2] == '\r')
					line = line.substr(0, line.size()-2);
				else
					line = line.substr(0, line.size()-1);

				string protoVersion, softwareVersion, comments, versionString;
				versionString = line;

				line = line.substr(4);
				istringstream lineStream(line);
				getline(lineStream, protoVersion, '-');
				getline(lineStream, softwareVersion, ' ');
				getline(lineStream, comments, '\n');

				if (protoVersion.empty() ||
					((protoVersion[0] == '1') && (protoVersion != "1.99")))
					throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "protocol version not supported");

				m_transportState->SetInVersion(Version(protoVersion, softwareVersion, comments, versionString));
				m_transportState->SetInState(WAITING_FOR_KEXINIT);
			}
		}
		else if (m_state == WAITING_FOR_FIRST_BLOCK)
		{
			if (m_inQueue.CurrentSize() < cipher.BlockSize())
				break;

			cipher.Detach(new Redirector(m_outQueue, false));
			m_inQueue.TransferTo(cipher, cipher.BlockSize());
			assert(m_outQueue.CurrentSize() == cipher.BlockSize());
			m_outQueue.GetWord32(m_packetLen);
			if (m_packetLen > SSHLIB_MAX_IN_PACKETLEN)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "packet length too large, may be decryption problem");
			if ((m_packetLen+4) % cipher.BlockSize() != 0)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "packet size not a multiple of cipher block size");
			m_state = WAITING_FOR_PACKET_END;
		}
		else
		{
			assert(m_state == WAITING_FOR_PACKET_END);

			if (m_outQueue.CurrentSize() + m_inQueue.CurrentSize() < m_packetLen + mac.MacSize())
				break;

			m_inQueue.TransferTo(cipher, m_packetLen-m_outQueue.CurrentSize());
			assert(m_outQueue.CurrentSize() == m_packetLen);
			byte paddingLen;
			m_outQueue.Get(paddingLen);
			if (paddingLen < SSH_MIN_PADDINGLEN)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "padding too short");
			if (m_packetLen < paddingLen+1)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "negative payload length");

			if (mac.MacSize())
			{
				m_inQueue.TransferTo(mac, mac.MacSize());
				mac.PutWord32(m_transportState->GetInSequenceNumber());
				mac.PutWord32(m_packetLen);
				mac.Put(paddingLen);
				m_outQueue.CopyTo(mac);
				mac.MessageEnd(0);
				if (!mac.GetLastResult())
					throw CauseToDisconnect(SSH_DISCONNECT_MAC_ERROR, "MAC check failed");
			}

			m_outQueue.TransferTo(compressor, m_packetLen-1-paddingLen);
			compressor.Flush(true);
			assert(m_outQueue.CurrentSize() == paddingLen);
			m_outQueue.Skip();

#ifdef _DEBUG
			string sDbgBuf;
			sDbgBuf.resize(compressor.MaxRetrievable());
			compressor.CopyTo(StringSink(sDbgBuf));
			SSH_Trace(("Received " + DescribeMessageType(sDbgBuf[0])).c_str());

			string sHexData;
			StringToHex(sDbgBuf.substr(1), sHexData, "0123456789ABCDEF");
			SSH_Trace(("Data: " + sHexData).c_str());
#endif

			compressor.TransferTo(*AttachedTransformation());
			Filter::MessageEnd(GetAutoSignalPropagation());
			m_state = WAITING_FOR_FIRST_BLOCK;
			m_transportState->IncrementInSequenceNumber();
		}
	}
}


void PacketDecoder::MessageEnd(int propagation)
{
	Filter::MessageSeriesEnd(GetAutoSignalPropagation());
}



//
// KEX_DH
////////////

KEX_DH::KEX_DH(TransportState& transportState, BufferedTransformation* outQueue)
	: FilterWithInputQueue(outQueue)
	, m_transportState(&transportState)
	, m_dh("179769313486231590770839156793787453197860296048756011706444"
		"423684197180216158519368947833795864925541502180565485980503"
		"646440548199239100050792877003355816639229553136239076508735"
		"759914822574862575007425302077447712589550957937778424442426"
		"617334727629299387668709205606050270810842907692932019128194"
		"467627007.", 2)
	, m_priv(m_dh.PrivateKeyLength())
{
}


void KEX_DH::PutInitPacket()
{
	m_transportState->AssertInState(DOING_IN_KEX);
	m_transportState->AssertOutState(DOING_OUT_KEX);

	BufferedTransformation& t = *AttachedTransformation();
	RandomNumberGenerator& rng = *m_transportState->m_rng;

	SecByteBlock pub(m_dh.PublicKeyLength());
	m_dh.GenerateKeyPair(rng, m_priv, pub);
	m_e.Decode(pub, pub.size);

	EncodeByte(t, SSH_MSG_KEXDH_INIT);
	EncodeInteger(t, m_e);
	t.MessageEnd();
}


void KEX_DH::ComputeH()
{
	SHA sha;
	HashFilter hf(sha);
	ByteQueue bq;
	EncodeString(hf, m_transportState->GetClientVersion().GetVersionString());
	EncodeString(hf, m_transportState->GetServerVersion().GetVersionString());
	m_transportState->GetClientKexInitPacket().Encode(bq);
	EncodeString(hf, bq); bq.Clear();
	m_transportState->GetServerKexInitPacket().Encode(bq);
	EncodeString(hf, bq); bq.Clear();
	EncodeString(hf, m_transportState->GetServerHostKey());
	EncodeInteger(hf, m_e);
	EncodeInteger(hf, m_f);
	EncodeInteger(hf, m_transportState->GetK());
	hf.MessageEnd();
	SecByteBlock h(sha.DigestSize());
	hf.Get(h, h.size);
	m_transportState->SetH(h, h.size);
	if (!m_transportState->GetSessionIDLen())
		m_transportState->SetSessionID(h, h.size);
}


void KEX_DH::MessageEnd(int propagation)
{
	m_transportState->AssertInState(DOING_IN_KEX);
	m_transportState->AssertOutState(DOING_OUT_KEX);

	BufferedTransformation& s = m_inQueue;
	BufferedTransformation& t = *AttachedTransformation();
	RandomNumberGenerator& rng = *m_transportState->m_rng;

	if (m_transportState->IsClient())
	{
		string hostKey;
		SecByteBlock signature;

		ExpectMessageNumber(s, SSH_MSG_KEXDH_REPLY);
		DecodeString(s, hostKey);
		DecodeInteger(s, m_f);
		DecodeString(s, signature);
		m_transportState->SetServerHostKey(hostKey);

		SecByteBlock otherPub(m_dh.PublicKeyLength());
		m_f.Encode(otherPub, otherPub.size);

		SecByteBlock k(m_dh.AgreedValueLength());
		if (!m_dh.Agree(k, m_priv, otherPub))
			throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "invalid DH value");
		m_transportState->SetK(Integer(k, k.size));
		ComputeH();

		bool verified = VerifySignature((byte const*) hostKey.data(), hostKey.size(), 
			m_transportState->GetH(), m_transportState->GetHLen(),
			signature, signature.size);
		if (!verified)
			throw CauseToDisconnect(SSH_DISCONNECT_HOST_KEY_NOT_VERIFIABLE, "signature check failed");
	}
	else
	{
		ExpectMessageNumber(s, SSH_MSG_KEXDH_INIT);
		DecodeInteger(s, m_e);
		SecByteBlock otherPub(m_dh.PublicKeyLength());
		m_e.Encode(otherPub, otherPub.size);

		SecByteBlock pub(m_dh.PublicKeyLength());
		m_dh.GenerateKeyPair(rng, m_priv, pub);
		m_f.Decode(pub, pub.size);

		SecByteBlock k(m_dh.AgreedValueLength());
		if (!m_dh.Agree(k, m_priv, otherPub))
			throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "invalid DH value");
		m_transportState->SetK(Integer(k, k.size));
		ComputeH();

		EncodeByte(t, SSH_MSG_KEXDH_REPLY);
		EncodeString(t, m_transportState->GetServerHostKey());
		EncodeInteger(t, m_f);
		EncodeString(t, m_transportState->m_signer->SignMessage(*m_transportState->m_rng, m_transportState->GetH(), m_transportState->GetHLen()));
		Filter::MessageEnd(propagation);
	}

	m_transportState->SetInState(WAITING_FOR_NEWKEYS);
	m_transportState->SetOutState(READY_TO_SEND_NEWKEYS);
}



//
// Support for OpenSSH-compatible key files
/////////////////////////////////////////////////

void OpenSSH_DES_EDE3_PwdToKey(byte const* pwd, unsigned int pwdlen, byte const* iv, byte* key)
{
	// 'iv' is expected to be 8 bytes in size.
	// 'key' must point to storage at least 24 bytes in size.
	// Additionally, the function assumes that the size of an MD5 digest is 16 bytes.

	byte digest[16];

	MD5 md5;
	md5.Update(pwd, pwdlen);
	md5.Update(iv, 8);
	md5.Final(digest);

	memcpy(key, digest, 16);

	md5.Update(digest, 16);
	md5.Update(pwd, pwdlen);
	md5.Update(iv, 8);
	md5.Final(digest);

	memcpy(key+16, digest, 8);
}


void EncodePemPrivateKey(
	BufferedTransformation& out,
	string const& alg,
	RandomNumberGenerator& rng,
	byte const* pwd,
	unsigned int pwdlen,
	BufferedTransformation const& privkey)
{
	PutString(out, "-----BEGIN ");
	PutString(out, alg);
	PutString(out, " PRIVATE KEY-----\n");

	if (!pwdlen)
	{
		Base64Encoder base64(new Redirector(out), true, 64);
		privkey.CopyTo(base64);
		base64.MessageEnd(0);
	}
	else
	{
		byte iv[8];
		rng.GenerateBlock(iv, sizeof iv);

		PutString(out, "Proc-Type: 4,ENCRYPTED\n");
		PutString(out, "DEK-Info: DES-EDE3-CBC,");

		HexEncoder hex(new Redirector(out), true);
		hex.Put(iv, 8);
		hex.MessageEnd(0);

		PutString(out, "\n\n");

		SecByteBlock key(24);
		OpenSSH_DES_EDE3_PwdToKey(pwd, pwdlen, iv, key.Begin());

		DES_EDE3_Encryption des(key.Begin());
		CBCPaddedEncryptor encryptor(des, iv, new Base64Encoder(new Redirector(out), true, 64));
		privkey.CopyTo(encryptor);
		encryptor.MessageEnd(1);
	}

	PutString(out, "-----END ");
	PutString(out, alg);
	PutString(out, " PRIVATE KEY-----\n");
}


// Returns the name of the algorithm as stated in the base64 boundary ("RSA", "DSA")
string DecodePemPrivateKey(
	BufferedTransformation& bt,
	byte const* pwd,
	unsigned int pwdlen,
	BufferedTransformation& privkey)
{
	string sLine;

	// Read lines until we have the beginning marker
	do
		sLine = AssertGetLine(bt);
	while (sLine.substr(0, 11) != "-----BEGIN ");

	// Retrieve algorithm name from the beginning marker
	string sAlgorithm = sLine.substr(11);
	string::size_type nPos = sAlgorithm.find(' ');
	if (nPos != string::npos)
		sAlgorithm.erase(nPos);

	// Read header lines, if any, until we encounter a data line
	string sProcType;
	string sDekInfo;

	while (true)
	{
		sLine = RightTrim(AssertGetLine(bt));

		// If it begins with whitespace, it's a continuation of a previous header line
		if (sLine.size() && ((sLine[0] == ' ') || (sLine[0] == '\t')))
			continue;

		// If there is no ": ", it must be a data line
		string::size_type nPos = sLine.find(": ");
		if (nPos == string::npos)
			break;

		// See if it's a known header line
		if (sLine.substr(0, 11) == "Proc-Type: ")
			sProcType = sLine.substr(11);
		else if (sLine.substr(0, 10) == "DEK-Info: ")
			sDekInfo = sLine.substr(10);
	}

	// If we have a Proc-Type, verify that the algorithm is something we support
	string sIv;

	if (sProcType.size())
	{
		if (sProcType != "4,ENCRYPTED")
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "unsupported processing type");
		if (sDekInfo.substr(0, 13) != "DES-EDE3-CBC,")
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "unsupported encryption algorithm");

		sIv = sDekInfo.substr(13);
		if (sIv.size() != 16)
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "invalid initialization vector");
	}

	// Read data lines until we hit the ending marker
	// Note that the existing line in sLine is already a data line
	Base64Decoder base64;
	do
	{
		base64.Put((byte const*) sLine.data(), sLine.size());
		sLine = RightTrim(AssertGetLine(bt));
	}
	while (sLine.substr(0, 8) != "-----END");

	base64.MessageEnd();

	// See if decryption is necessary
	if (!sProcType.size())
	{
		// No decryption required
		base64.TransferTo(privkey);
	}
	else
	{
		// Decrypt
		byte iv[8];

		HexDecoder hex(new ArraySink(iv, sizeof iv));
		hex.Put((byte const*) sIv.data(), sIv.size());
		hex.MessageEnd();

		SecByteBlock key(24);
		OpenSSH_DES_EDE3_PwdToKey(pwd, pwdlen, iv, key.Begin());

		DES_EDE3_Decryption des(key.Begin());
		CBCPaddedDecryptor decryptor(des, iv, new Redirector(privkey));
		base64.TransferTo(decryptor);
		decryptor.MessageEnd(0);
	}

	return sAlgorithm;
}



class RSAPrivateKey : public InvertibleRSAFunction
{
	// This class is only necessary for the three Get...() accessor methods.
	// In the latest Crypto++ distribution at the time of this writing (4.2),
	// these accessors are not yet available. They are expected in the next distribution.
public:
	RSAPrivateKey(Integer const& n, Integer const& e, Integer const& d,
				  Integer const& p, Integer const& q, Integer const& dp,
				  Integer const& dq, Integer const& u)
		: InvertibleRSAFunction(n, e, d, p, q, dp, dq, u) {}

	RSAPrivateKey(RandomNumberGenerator& rng, unsigned int keybits, Integer const& eStart=17)
		: InvertibleRSAFunction(rng, keybits, eStart) {}

	Integer const& GetModPrime1PrivateExponent() const { return dp; }
	Integer const& GetModPrime2PrivateExponent() const { return dq; }
	Integer const& GetMultiplicativeInverseOfPrime2ModPrime1() const { return u; }
};



class OpenSshRsaPrivateKey
{
public:
	OpenSshRsaPrivateKey(RandomNumberGenerator& rng, unsigned int keybits, Integer const& eStart=17)
		: m_spPrivKey(new RSAPrivateKey(rng, keybits, eStart)) {}

	OpenSshRsaPrivateKey(BufferedTransformation& bt);

	void EncodePrivKey(BufferedTransformation& bt) const;
	void EncodePubKey(BufferedTransformation& bt) const;

	RSAPrivateKey& Get() { return *m_spPrivKey.get(); }
	RSAPrivateKey const& Get() const { return *m_spPrivKey.get(); }

protected:
	auto_ptr<RSAPrivateKey> m_spPrivKey;
};


OpenSshRsaPrivateKey::OpenSshRsaPrivateKey(BufferedTransformation& bt)
{
	Integer n, e, d, p, q, dp, dq, u;

	BERSequenceDecoder sequence(bt);
		word32 version;
		BERDecodeUnsigned<word32>(sequence, version, INTEGER, 0, 0);	// check version
		n.BERDecode(sequence);
		e.BERDecode(sequence);
		d.BERDecode(sequence);
		p.BERDecode(sequence);
		q.BERDecode(sequence);
		dp.BERDecode(sequence);
		dq.BERDecode(sequence);
		u.BERDecode(sequence);
	sequence.MessageEnd();

	m_spPrivKey = auto_ptr<RSAPrivateKey>(new RSAPrivateKey(n, e, d, p, q, dp, dq, u));
}


void OpenSshRsaPrivateKey::EncodePrivKey(BufferedTransformation& bt) const
{
	DERSequenceEncoder sequence(bt);
		DEREncodeUnsigned<word32>(sequence, 0);	// version
		Get().GetModulus().DEREncode(sequence);
		Get().GetExponent().DEREncode(sequence);
		Get().GetDecryptionExponent().DEREncode(sequence);
		Get().GetPrime1().DEREncode(sequence);
		Get().GetPrime2().DEREncode(sequence);
		Get().GetModPrime1PrivateExponent().DEREncode(sequence);
		Get().GetModPrime2PrivateExponent().DEREncode(sequence);
		Get().GetMultiplicativeInverseOfPrime2ModPrime1().DEREncode(sequence);
	sequence.MessageEnd();
}


void OpenSshRsaPrivateKey::EncodePubKey(BufferedTransformation& bt) const
{
	EncodeString(bt, "ssh-rsa");
	EncodeInteger(bt, Get().GetExponent());
	EncodeInteger(bt, Get().GetModulus());
}



class OpenSshDsaPrivateKey
{
public:
	OpenSshDsaPrivateKey(RandomNumberGenerator& rng, unsigned int keybits)
		: m_spPrivKey(new DSAPrivateKey(rng, keybits)) {}

	OpenSshDsaPrivateKey(BufferedTransformation& bt);

	void EncodePrivKey(BufferedTransformation& bt) const;
	void EncodePubKey(BufferedTransformation& bt) const;

	DSAPrivateKey& Get() { return *m_spPrivKey.get(); }
	DSAPrivateKey const& Get() const { return *m_spPrivKey.get(); }

protected:
	auto_ptr<DSAPrivateKey> m_spPrivKey;
};


OpenSshDsaPrivateKey::OpenSshDsaPrivateKey(BufferedTransformation& bt)
{
	Integer p, q, g, y, x;

	BERSequenceDecoder sequence(bt);
		word32 version;
		BERDecodeUnsigned<word32>(sequence, version, INTEGER, 0, 0);	// check version
		p.BERDecode(sequence);
		q.BERDecode(sequence);
		g.BERDecode(sequence);
		y.BERDecode(sequence);
		x.BERDecode(sequence);
	sequence.MessageEnd();

	m_spPrivKey = auto_ptr<DSAPrivateKey>(new DSAPrivateKey(p, q, g, y, x));
}


void OpenSshDsaPrivateKey::EncodePrivKey(BufferedTransformation& bt) const
{
	DERSequenceEncoder sequence(bt);
		DEREncodeUnsigned<word32>(sequence, 0);
		Get().GetModulus().DEREncode(sequence);
		Get().GetSubgroupSize().DEREncode(sequence);
		Get().GetGenerator().DEREncode(sequence);
		Get().GetPublicResidue().DEREncode(sequence);
		Get().GetPrivateExponent().DEREncode(sequence);
	sequence.MessageEnd();
}


void OpenSshDsaPrivateKey::EncodePubKey(BufferedTransformation& bt) const
{
	EncodeString(bt, "ssh-dss");
	EncodeInteger(bt, Get().GetModulus());
	EncodeInteger(bt, Get().GetSubgroupSize());
	EncodeInteger(bt, Get().GetGenerator());
	EncodeInteger(bt, Get().GetPublicResidue());
}



string RemoveControlChars(string const& s)
{
	string ret = s;

	string::iterator it = ret.begin();
	while (it != ret.end())
		if (*it < 32)
			it = ret.erase(it);
		else
			++it;

	return ret;
}


void EncodeSsh2PublicKey(
	BufferedTransformation& out,
	string const& comment,
	BufferedTransformation const& pubkey)
{
	PutString(out, "---- BEGIN SSH2 PUBLIC KEY ----\n");

	if (comment.size())
	{
		string fullcomment = "Comment: \"" + RemoveControlChars(comment) + "\"";
		while (fullcomment.size() > 72)
		{
			PutString(out, fullcomment, 71);
			PutString(out, "\\\n");
			fullcomment.erase(0, 71);
		}
		PutString(out, fullcomment);
		PutChar(out, '\n');
	}

	Base64Encoder base64(new Redirector(out), true, 72);
	pubkey.CopyTo(base64);
	base64.MessageEnd();

	PutString(out, "---- END SSH2 PUBLIC KEY ----\n");
}


void EncodeOpenSshPublicKey(
	BufferedTransformation& out,
	string const& alg,
	string const& comment,
	BufferedTransformation const& pubkey)
{
	PutString(out, alg);
	PutChar(out, ' ');

	Base64Encoder base64(new Redirector(out), false);
	pubkey.CopyTo(base64);
	base64.MessageEnd();

	PutChar(out, ' ');
	PutString(out, RemoveControlChars(comment));
	PutChar(out, '\n');
}


void GenerateOpenSshKeypair(
	string const& algorithm,
	RandomNumberGenerator& rng,
	unsigned int keyBits,
	string const& comment,
	BufferedTransformation& btPubSsh2,
	BufferedTransformation& btPubOpen,
	byte const* pwd,
	unsigned int pwdlen,
	BufferedTransformation& btPriv)
{
	if (algorithm == "ssh-rsa")
	{
		OpenSshRsaPrivateKey privkey(rng, keyBits);

		// Encode private key
		ByteQueue bqPrivKey;
		privkey.EncodePrivKey(bqPrivKey);
		EncodePemPrivateKey(btPriv, "RSA", rng, pwd, pwdlen, bqPrivKey);

		// Encode public key
		ByteQueue bqPubKey;
		privkey.EncodePubKey(bqPubKey);
		EncodeSsh2PublicKey(btPubSsh2, comment, bqPubKey);
		EncodeOpenSshPublicKey(btPubOpen, "ssh-rsa", comment, bqPubKey);
	}
	else if (algorithm == "ssh-dss")
	{
		OpenSshDsaPrivateKey privkey(rng, keyBits);

		// Encode private key
		ByteQueue bqPrivKey;
		privkey.EncodePrivKey(bqPrivKey);
		EncodePemPrivateKey(btPriv, "DSA", rng, pwd, pwdlen, bqPrivKey);

		// Encode public key
		ByteQueue bqPubKey;
		privkey.EncodePubKey(bqPubKey);
		EncodeSsh2PublicKey(btPubSsh2, comment, bqPubKey);
		EncodeOpenSshPublicKey(btPubOpen, "ssh-dss", comment, bqPubKey);
	}
	else
		throw Err(BufferedTransformation::OTHER_ERROR, "unrecognized algorithm id");
}


bool IsOpenSshPrivKeyEncrypted(BufferedTransformation const& btPriv)
{
	char acPeek[60];
	unsigned int nLen = btPriv.Peek((byte*) acPeek, sizeof(acPeek)-1);
	acPeek[nLen] = 0;

	char const* p = strchr(acPeek, '\n');
	return p ? (memcmp(p+1, "Proc-Type: ", 11) == 0) : false;
}


bool IsValidOpenSshPrivKeyPassword(
	BufferedTransformation const& btPriv, byte const* pwd, unsigned int pwdlen)
{
	ByteQueue btCopied;
	btPriv.CopyTo(btCopied);

	ByteQueue bqPrivKey;
	string sPemAlg = DecodePemPrivateKey(btCopied, pwd, pwdlen, bqPrivKey);

	// DecodePemPrivateKey() won't throw if decryption fails - it will simply return
	// an invalid blob. We have to try to decode it to see if decryption succeeded.

	if (sPemAlg == "RSA")
	{
		try { SSH::OpenSshRsaPrivateKey privkey(bqPrivKey); }
		catch (...) { return false; }
	}
	else if (sPemAlg == "DSA")
	{
		try { SSH::OpenSshDsaPrivateKey privkey(bqPrivKey); }
		catch (...) { return false; }
	}
	else
		throw Err(BufferedTransformation::OTHER_ERROR, "unrecognized private key algorithm");

	return true;
}


Ssh2PublicKeyBlob::Ssh2PublicKeyBlob(BufferedTransformation& bt)
{
	// How is the key blob encoded?
	if (bt.MaxRetrievable() < 11)
		throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "insufficient data for key blob");

	byte abPeekBuf[11];
	if (bt.Peek(abPeekBuf, sizeof abPeekBuf) != sizeof abPeekBuf)
		throw Err(BufferedTransformation::INPUT_ERROR, "cannot read key blob");

	// Raw binary blob?
	if ((memcmp(abPeekBuf, "\x0\x0\x0\x7ssh-rsa", 11) == 0) ||
		(memcmp(abPeekBuf, "\x0\x0\x0\x7ssh-dss", 11) == 0))
	{
		bt.TransferTo(m_bq);
	}

	// OpenSSH format?
	else if ((memcmp(abPeekBuf, "ssh-rsa", 7) == 0) ||
			 (memcmp(abPeekBuf, "ssh-dss", 7) == 0))
	{
		string sLine = GetLine(bt);
		if (sLine.size() < 8)
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "invalid key blob encoding");

		// Find the start of Base-64 encoding
		string::size_type nStartPos = sLine.find_first_not_of(" \t\r\n", 7);
		if (nStartPos == string::npos)
			throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "invalid key blob encoding");

		// Find the end of Base-64 encoding
		string::size_type nEndPos = sLine.find_first_of(" \t\r\n", nStartPos);
		if (nEndPos == string::npos)
			nEndPos = sLine.size();

		// Decode
		Base64Decoder base64(new Redirector(m_bq));
		base64.Put((byte const*) (sLine.data() + nStartPos), nEndPos - nStartPos);
		base64.MessageEnd();
	}

	// Else, the blob must be encoded in SSH2 format
	else
	{
		string sLine = GetLine(bt);

		// Find the beginning marker
		while ((sLine.size() < 31) || (sLine.substr(0, 31) != "---- BEGIN SSH2 PUBLIC KEY ----"))
			sLine = AssertGetLine(bt);

		// Decode any header lines
		bool bContinued = false;
		while (true)
		{
			sLine = AssertGetLine(bt);

			if ((sLine.find(':') == string::npos) && !bContinued)
				break;

			sLine = RightTrim(sLine);
			if (sLine.size() && (sLine[sLine.size()-1] == '\\'))
				bContinued = true;
			else
				bContinued = false;
		}

		// Decode data until we hit the ending marker
		Base64Decoder base64(new Redirector(m_bq));

		while ((sLine.size() < 29) || (sLine.substr(0, 29) != "---- END SSH2 PUBLIC KEY ----"))
		{
			base64.Put((byte const*) sLine.data(), sLine.size());
			sLine = AssertGetLine(bt);
		}

		base64.MessageEnd();
	}


	// Blob decoded, now check it out
	ByteQueue bqBlob;
	m_bq.CopyTo(bqBlob);

	// Get algorithm name
	DecodeString(bqBlob, m_sAlgorithm);

	// Get bit count
	Integer iModulus;

	if (m_sAlgorithm == "ssh-rsa")
	{
		Integer iDummy;
		DecodeInteger(bqBlob, iDummy);
		DecodeInteger(bqBlob, iModulus);
	}
	else if (m_sAlgorithm == "ssh-dss")
	{
		DecodeInteger(bqBlob, iModulus);
	}
	else
		throw Err(BufferedTransformation::INVALID_DATA_FORMAT, "unsupported public key algorithm");

	m_nBitCount = iModulus.BitCount();

	// Calculate MD5 fingerprint
	MD5 md5;
	string sMD5;
	HashFilter md5filter(md5, new StringSink(sMD5));
	m_bq.CopyTo(md5filter);
	md5filter.MessageEnd();

	StringToHex(sMD5, m_sMD5Fingerprint, "0123456789abcdef:");

	// Calculate Bubble Babble
	SHA sha;
	HashFilter shafilter(sha, new BubbleBabbleEncoder(new StringSink(m_sBubbleBabble)));
	m_bq.CopyTo(shafilter);
	shafilter.MessageEnd();
}



//
// Manager
/////////////

void ConstructPublicKeyAuthBlob(
	SecByteBlock& tosign,
	SecByteBlock const& sessionId,
	string const& userName, 
	string const& algorithm,
	string const& publicKey)
{
	string part2;
	StringSink part2sink(part2);
	EncodeByte(part2sink, SSH_MSG_USERAUTH_REQUEST);
	EncodeString(part2sink, userName);
	EncodeString(part2sink, "ssh-connection");
	EncodeString(part2sink, "publickey");
	EncodeBoolean(part2sink, true);
	EncodeString(part2sink, algorithm);
	EncodeString(part2sink, publicKey);

	tosign.Resize(4 + sessionId.Size() + part2.size());
	ArraySink tosignsink(tosign, tosign.Size());
	EncodeString(tosignsink, sessionId);
	StringSource(part2, true, new Redirector(tosignsink));

	assert(tosignsink.TotalPutLength() == tosign.Size());
}


Manager::Manager(TransportState& transportState, Authenticator& authenticator, BufferedTransformation* outQueue)
	: Filter(outQueue)
	, m_transportState(&transportState)
	, m_authenticator(&authenticator)
	, m_messageNumberReceived(false)
	, m_messageBytesReceived(0)
	, m_ignoreFirstKexPacket(false)
	, m_disconnectExchangeState(DISCONNECT_NOT_EXCHANGED)
{
}


void Manager::SetInState(InTransportState state)
{
	bool stateChanged = (state != m_transportState->GetInState());
	m_transportState->SetInState(state);
	if (stateChanged)
		OnInStateChange(state);
}


void Manager::SetOutState(OutTransportState state)
{
	bool stateChanged = (state != m_transportState->GetOutState());
	m_transportState->SetOutState(state);
	if (stateChanged)
		OnOutStateChange(state);
}


void Manager::PutProtocolVersion(Version const& version)
{
	m_transportState->AssertOutState(READY_TO_SEND_VERSION);
	string versionString = version.GetVersionString() + "\r\n";
	AttachedTransformation()->Put((byte const*) versionString.data(), versionString.size());
	AttachedTransformation()->MessageEnd();
	m_transportState->SetOutVersion(version);
	SetOutState(READY_TO_SEND_KEXINIT);
}


unsigned int Manager::TotalWaitObjects() const
{
	unsigned int total = CountConnectionWaitObjects();
	word32 bound = ChannelNumberBound();
	for (word32 i=0; i<bound; i++)
	{
		Channel const* channel = GetChannel(i);
		if (channel)
			total += channel->MaxWaitObjects();
	}
	return total;
}


void Manager::GetWaitObjects(WaitObjectContainer& container)
{
	GetConnectionWaitObjects(container);

	word32 bound = ChannelNumberBound();
	for (word32 i=0; i!=bound; ++i)
	{
		Channel* channel = GetChannel(i);
		if (channel)
			channel->GetWaitObjects(container);
	}
}


void Manager::ProcessWaitObjects()
{
	ProcessConnectionWaitObjects();

	word32 bound = ChannelNumberBound();
	for (word32 i=0; i!=bound; ++i)
	{
		Channel* channel = GetChannel(i);
		if (channel)
			channel->ProcessWaitObjects();
	}
}


void Manager::HandlePacketNotCompletelyProcessed(BufferedTransformation& data)
{
	data.Skip();
#ifdef _DEBUG
	SSH_Trace("Packet not completely processed");
#endif
}


void Manager::Disconnect(CauseToDisconnect const& cause)
{
	if (m_transportState->GetInState() != IN_DISCONNECTED)
	{
		BufferedTransformation& t = *AttachedTransformation();
		EncodeByte(t, SSH_MSG_DISCONNECT);
		EncodeWord32(t, cause.GetReason());
		EncodeString(t, cause.GetDescription());
		EncodeString(t, cause.GetLanguage());
		t.MessageEnd();
		SetInState(IN_DISCONNECTED);
		SetOutState(OUT_DISCONNECTED);

		m_disconnectExchangeState = DISCONNECT_SENT;
		m_disconnectReason = cause.GetReason();
		m_disconnectDescription = cause.GetDescription();
		m_disconnectLanguage = cause.GetLanguage();
	}
}


void Manager::StartKex(KexInitPacket packet)
{
	m_transportState->m_rng->GenerateBlock(packet.cookie, 16);
	packet.Encode(*AttachedTransformation());
	AttachedTransformation()->MessageEnd();
	m_transportState->SetOutKexInitPacket(packet);
	SetOutState(DOING_OUT_KEX);

	if (m_transportState->KexInitPacketReceived())
		StartKexDh();
}


void Manager::StartKexDh()
{
	m_transportState->ResetKexInitPacketStatus();

	if (m_transportState->GetKexAlgorithm() != "diffie-hellman-group1-sha1")
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate key exchange algorithm");
	if (m_transportState->GetServerHostKeyAlgorithm() != "ssh-dss")
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate host key algorithm");
	if (m_transportState->GetEncryptionAlgorithm(CLIENT_TO_SERVER).empty())
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate client to server encryption algorithm");
	if (m_transportState->GetEncryptionAlgorithm(SERVER_TO_CLIENT).empty())
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate server to client encryption algorithm");
	if (m_transportState->GetMacAlgorithm(CLIENT_TO_SERVER).empty())
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate client to server MAC algorithm");
	if (m_transportState->GetMacAlgorithm(SERVER_TO_CLIENT).empty())
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate server to client MAC algorithm");
	if (m_transportState->GetCompressionAlgorithm(CLIENT_TO_SERVER).empty())
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate client to server compression algorithm");
	if (m_transportState->GetCompressionAlgorithm(SERVER_TO_CLIENT).empty())
		throw CauseToDisconnect(SSH_DISCONNECT_KEY_EXCHANGE_FAILED, "failed to negotiate server to client compression algorithm");

	m_kexdh.reset(new KEX_DH(*m_transportState, new OutputProxy(*this, true)));
	if (m_transportState->IsClient())
		m_kexdh->PutInitPacket();
}


void Manager::StartService()
{
	m_transportState->SetServiceStarted(true);
	SetInState(IN_SERVICE_STARTED);
	SetOutState(OUT_SERVICE_STARTED);
}


void Manager::ChannelData(word32 channel, byte const* inString, unsigned int length, bool extendedData, word32 dataType)
{
	// Should never be called with more data than there is output window space remaining.
	Channel* pc = m_channelTable.GetChannel(channel);
	assert(pc->m_state == Channel::OPEN && pc->m_eofSent == false);
	assert(length <= pc->OutWindowSpaceRemaining());

	if (extendedData ? (!pc->m_queuedExtendedData || dataType != pc->m_queuedDataType) : pc->m_queuedExtendedData)
		ChannelDataFlush(channel, true);
	pc->m_queuedExtendedData = extendedData;
	pc->m_queuedDataType = dataType;
	LazyPutter lp(pc->m_inQueue, inString, length);
	if (pc->m_inQueue.CurrentSize() >= pc->m_outMaxPacketSize)
		ChannelDataFlush(channel, false);
}


void Manager::ChannelDataFlush(word32 channel, bool completeFlush)
{
	Channel* pc = m_channelTable.GetChannel(channel);

	if (pc->m_eofSent || pc->m_closeSent || pc->m_closeReceived)
		return;

	while (1)
	{
		// pc->m_outWindowSize should never become 0 unless m_inQueue is empty,
		// in which case the loop terminates. There is an assert to that effect
		// in Manager::ChannelData().

		if (completeFlush)
		{
			if (pc->m_inQueue.CurrentSize() == 0)
				break;
		}
		else
		{
			if (pc->m_inQueue.CurrentSize() < pc->m_outMaxPacketSize)
				break;
		}

		BufferedTransformation& t = *AttachedTransformation();
		if (pc->m_queuedExtendedData)
		{
			EncodeByte(t, SSH_MSG_CHANNEL_EXTENDED_DATA);
			EncodeWord32(t, pc->m_yourNumber);
			EncodeWord32(t, pc->m_queuedDataType);
		}
		else
		{
			EncodeByte(t, SSH_MSG_CHANNEL_DATA);
			EncodeWord32(t, pc->m_yourNumber);
		}
		unsigned int packetSize = EncodeString(t, pc->m_inQueue, STDMIN(pc->m_outWindowSize, pc->m_outMaxPacketSize));
		t.MessageEnd();
		pc->m_inQueue.Skip(packetSize);
		pc->m_outWindowSize -= packetSize;
	}
}


void Manager::ChannelClose(word32 channel, bool graceful)
{
	if (graceful)
		ChannelSendEof(channel);
	else
		ChannelSendClose(channel);
}


void Manager::ChannelSendEof(word32 channel)
{
	Channel* pc = m_channelTable.GetChannel(channel);
	if (pc->m_eofSent || pc->m_closeSent)
		return;
	ChannelDataFlush(channel, true);

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_CHANNEL_EOF);
	EncodeWord32(t, pc->m_yourNumber);
	t.MessageEnd();

	pc->m_eofSent = true;
}


void Manager::ChannelSendClose(word32 channel)
{
	Channel* pc = m_channelTable.GetChannel(channel);
	if (pc->m_closeSent)
		return;
	ChannelDataFlush(channel, true);

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_CHANNEL_CLOSE);
	EncodeWord32(t, pc->m_yourNumber);
	t.MessageEnd();

	pc->m_closeSent = true;
}


void Manager::ChannelAdjustWindowSize(word32 channel, word32 incrementSize)
{
	if (incrementSize > 0)
	{
		Channel* pc = m_channelTable.GetChannel(channel);
		assert(pc);

		BufferedTransformation& t = *AttachedTransformation();
		EncodeByte(t, SSH_MSG_CHANNEL_WINDOW_ADJUST);
		EncodeWord32(t, pc->m_yourNumber);
		EncodeWord32(t, incrementSize);
		t.MessageEnd();

		pc->m_inWindowSize += incrementSize;
	}
}


void Manager::ChannelPut(string const& channel, byte const* inString, unsigned int length)
{
	switch (channel.size())
	{
	case 0:
		Put(inString, length);
		break;
	case 4:
		ChannelData(StringToWord32(channel), inString, length);
		break;
	case 8:
		ChannelData(StringToWord32(channel), inString, length, true, StringToWord32(channel.substr(4)));
		break;
	default:
		assert(false);
	}
}


void Manager::ChannelFlush(string const& channel, bool completeFlush, int propagation)
{
	if (channel.empty())
		Flush(completeFlush, propagation);
	else
		ChannelDataFlush(StringToWord32(channel));
}


void Manager::ChannelMessageEnd(string const& channel, int propagation)
{
	if (channel.empty())
		MessageEnd(propagation);
	else
		ChannelClose(StringToWord32(channel), true);
}


void Manager::ChannelMessageSeriesEnd(string const& channel, int propagation)
{
	if (channel.empty())
		MessageSeriesEnd(propagation);
	else
		ChannelClose(StringToWord32(channel), false);
}


void Manager::Put(byte const* inString, unsigned int length)
{
	if (length == 0)
		return;

	if (!m_messageNumberReceived)
	{
		m_messageNumber = inString[0];
		m_messageNumberReceived = true;
	}

	if (m_messageBytesReceived + length > SSHLIB_MAX_IN_DECOMPRESSED_PAYLOADLEN)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "decompressed packet length too large");

	switch (m_messageNumber)
	{
	case SSH_MSG_KEXDH_INIT:
		if (m_ignoreFirstKexPacket)
		{
			m_transportState->AssertInState(DOING_IN_KEX);
			break;
		}
		// fall through intentional

	case SSH_MSG_KEXDH_REPLY:
		m_transportState->AssertInState(DOING_IN_KEX);
		m_kexdh->Put(inString, length);
		break;

	default:
		m_inQueue.Put(inString, length);
		break;
	}

	m_messageBytesReceived += length;
}


void Manager::MessageEnd(int)
{
	if (!m_messageNumberReceived)
		throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "empty packet");

	ByteQueue& s = m_inQueue;
	BufferedTransformation& t = *AttachedTransformation();
	RandomNumberGenerator& rng = *m_transportState->m_rng;

	switch (m_messageNumber)
	{
	case SSH_MSG_IGNORE:
		HandleIgnore(s.MaxRetrievable());
		s.Skip();
		break;

	case SSH_MSG_UNIMPLEMENTED:
	{
		word32 packetSequenceNumber;

		ExpectMessageNumber(s, SSH_MSG_UNIMPLEMENTED);
		DecodeWord32(s, packetSequenceNumber);

		HandleUnimplemented(packetSequenceNumber);
		break;
	}

	case SSH_MSG_DEBUG:
	{
		bool alwaysDisplay;
		string message, languageTag;

		ExpectMessageNumber(s, SSH_MSG_DEBUG);
		DecodeBoolean(s, alwaysDisplay);
		DecodeString(s, message);
		DecodeString(s, languageTag);

		HandleDebug(alwaysDisplay, message, languageTag);
		break;
	}

	case SSH_MSG_USERAUTH_BANNER:
	{
		m_transportState->AssertIsClient();

		if (m_authenticator->m_state == Authenticator::SUCCESS)
		{
			s.Skip();	// ignore request if already authenticated
			break;
		}

		string message, languageTag;

		ExpectMessageNumber(s, SSH_MSG_USERAUTH_BANNER);
		DecodeString(s, message);
		DecodeString(s, languageTag);

		HandleUserAuthBanner(message, languageTag);
		break;
	}

	case SSH_MSG_DISCONNECT:
	{
		SetInState(IN_DISCONNECTED);
		SetOutState(OUT_DISCONNECTED);

		m_disconnectExchangeState = DISCONNECT_RECEIVED;

		ExpectMessageNumber(s, SSH_MSG_DISCONNECT);

		word32 nDisconnectReason;
		DecodeWord32(s, nDisconnectReason);
		m_disconnectReason = (DisconnectReason) nDisconnectReason;

		DecodeString(s, m_disconnectDescription);
		DecodeString(s, m_disconnectLanguage);

		break;
	}

	case SSH_MSG_KEXINIT:
	{
		SetInState(DOING_IN_KEX);

		KexInitPacket inPacket;
		inPacket.Decode(m_inQueue);
		m_transportState->SetInKexInitPacket(inPacket);

		KexInitPacket const& outPacket = m_transportState->GetOutKexInitPacket();

		if (m_transportState->GetOutState() > DOING_OUT_KEX)
			StartKex(outPacket);			// re-exchange

		if (inPacket.first_kex_packet_follows &&
			((m_transportState->GetKexAlgorithm() != GetFirstAlgorithm(inPacket.kex_algorithms)) ||
			 (m_transportState->GetServerHostKeyAlgorithm() != GetFirstAlgorithm(inPacket.server_host_key_algorithms)) ||
			 (m_transportState->GetEncryptionAlgorithm(CLIENT_TO_SERVER) == "") ||
			 (m_transportState->GetEncryptionAlgorithm(SERVER_TO_CLIENT) == "") ||
			 (m_transportState->GetMacAlgorithm(CLIENT_TO_SERVER) == "") ||
			 (m_transportState->GetMacAlgorithm(SERVER_TO_CLIENT) == "") ||
			 (m_transportState->GetCompressionAlgorithm(CLIENT_TO_SERVER) == "") ||
			 (m_transportState->GetCompressionAlgorithm(SERVER_TO_CLIENT) == "")))
			m_ignoreFirstKexPacket = true;

		if (m_transportState->KexInitPacketSent())
			StartKexDh();

		break;
	}

	case SSH_MSG_NEWKEYS:
	{
		m_transportState->AssertInState(WAITING_FOR_NEWKEYS);
		ExpectMessageNumber(s, SSH_MSG_NEWKEYS);

		m_transportState->AccessInCipher().SetCipher(
			m_transportState->GetEncryptionAlgorithm(m_transportState->InDirection()),
			m_transportState->GetK(),
			m_transportState->GetH(),
			m_transportState->GetSessionID(),
			m_transportState->InDirection(),
			DECRYPTION);

		m_transportState->AccessInMac().SetMac(
			m_transportState->GetMacAlgorithm(m_transportState->InDirection()),
			m_transportState->GetK(),
			m_transportState->GetH(),
			m_transportState->GetSessionID(),
			m_transportState->InDirection(),
			false,
			m_transportState->GetInVersion());

		m_transportState->AccessInCompressor().SetCompressor(
			m_transportState->GetCompressionAlgorithm(m_transportState->InDirection()),
			false);

		if (m_transportState->GetServiceStarted())
			SetInState(IN_SERVICE_STARTED);			// key re-exchange
		else
		{
			if (m_transportState->IsClient())
				SetInState(WAITING_FOR_SERVICE_ACCEPT);
			else
				SetInState(WAITING_FOR_SERVICE_REQUEST);
		}
		break;
	}

	case SSH_MSG_KEXDH_INIT:
		if (m_ignoreFirstKexPacket)
		{
			m_transportState->AssertInState(DOING_IN_KEX);
			m_ignoreFirstKexPacket = false;
			break;
		}
		// fall through intentional

	case SSH_MSG_KEXDH_REPLY:
		m_transportState->AssertInState(DOING_IN_KEX);

		m_kexdh->MessageEnd();

		if (m_transportState->GetOutState() == READY_TO_SEND_NEWKEYS)
		{
			m_kexdh.reset(NULL);

			EncodeByte(t, SSH_MSG_NEWKEYS);
			t.MessageEnd();

			m_transportState->AccessOutCipher().SetCipher(
				m_transportState->GetEncryptionAlgorithm(m_transportState->OutDirection()),
				m_transportState->GetK(),
				m_transportState->GetH(),
				m_transportState->GetSessionID(),
				m_transportState->OutDirection(),
				ENCRYPTION);

			m_transportState->AccessOutMac().SetMac(
				m_transportState->GetMacAlgorithm(m_transportState->OutDirection()),
				m_transportState->GetK(),
				m_transportState->GetH(),
				m_transportState->GetSessionID(),
				m_transportState->OutDirection(),
				true,
				m_transportState->GetInVersion());

			m_transportState->AccessOutCompressor().SetCompressor(
				m_transportState->GetCompressionAlgorithm(m_transportState->OutDirection()),
				true);

			if (m_transportState->GetServiceStarted())
				SetOutState(OUT_SERVICE_STARTED);			// key re-exchange
			else
				SetOutState(READY_TO_SEND_SERVICE_REQUEST);
		}
		break;

	case SSH_MSG_SERVICE_REQUEST:
	{
		m_transportState->AssertIsServer();
		m_transportState->AssertInState(WAITING_FOR_SERVICE_REQUEST);

		ExpectMessageNumber(s, SSH_MSG_SERVICE_REQUEST);
		string serviceName;
		DecodeString(s, serviceName);

		if (serviceName != "ssh-userauth")
			throw CauseToDisconnect(SSH_DISCONNECT_SERVICE_NOT_AVAILABLE);
		else
		{
			EncodeByte(t, SSH_MSG_SERVICE_ACCEPT);
			EncodeString(t, serviceName);
			t.MessageEnd();

			StartService();
		}
		break;
	}

	case SSH_MSG_SERVICE_ACCEPT:
		m_transportState->AssertIsClient();
		m_transportState->AssertInState(WAITING_FOR_SERVICE_ACCEPT);
		ExpectMessageNumber(s, SSH_MSG_SERVICE_ACCEPT);
		StartService();
		break;

	case SSH_MSG_USERAUTH_REQUEST:
	{
		m_transportState->AssertIsServer();
		m_transportState->AssertInState(IN_SERVICE_STARTED);

		if (m_authenticator->m_state == Authenticator::SUCCESS)
		{
			m_authenticator->HandleSuperfluousUserAuthRequest(s);
			s.Skip();	// ignore request if already authenticated
			break;
		}
		
		string name, service, method;

		ExpectMessageNumber(s, SSH_MSG_USERAUTH_REQUEST);
		DecodeString(s, name);
		DecodeString(s, service);
		DecodeString(s, method);

		m_authenticator->m_lastMethod = method;

		if (!m_transportState->IsServer() || service != "ssh-connection")
			throw CauseToDisconnect(SSH_DISCONNECT_SERVICE_NOT_AVAILABLE);

		if (m_authenticator->m_anyRequestsReceived && (name != m_authenticator->m_userName))
			m_authenticator->HandleUserAuthUserNameChange(name);

		m_authenticator->m_userName = name;
		m_authenticator->m_anyRequestsReceived = true;

		if (MatchCommaDelimitedList(m_authenticator->m_methodsRemaining, method))
			m_authenticator->HandleUserAuthRequest(*this, method, s);
		else
			m_authenticator->HandleUserAuthMethodMismatch(method);

		switch (m_authenticator->m_state)
		{
		case Authenticator::PK_OK:
			EncodeByte(t, SSH_MSG_USERAUTH_PK_OK);
			EncodeString(t, m_authenticator->m_pkAlgorithm);
			EncodeString(t, m_authenticator->m_pkPublicKey);
			t.MessageEnd();
			break;
		case Authenticator::PASSWD_CHANGEREQ:
			EncodeByte(t, SSH_MSG_USERAUTH_PASSWD_CHANGEREQ);
			EncodeString(t, m_authenticator->m_pwPrompt);
			EncodeString(t, m_authenticator->m_pwLanguage);
			t.MessageEnd();
			break;
		case Authenticator::SUCCESS:
			EncodeByte(t, SSH_MSG_USERAUTH_SUCCESS);
			t.MessageEnd();
			break;
		case Authenticator::PARTIAL_SUCCESS:
		case Authenticator::FAILURE:
			EncodeByte(t, SSH_MSG_USERAUTH_FAILURE);
			EncodeString(t, m_authenticator->m_methodsRemaining);
			EncodeBoolean(t, m_authenticator->m_state == Authenticator::PARTIAL_SUCCESS);
			t.MessageEnd();
			break;
		default:
			assert(false);
		}
		break;
	}

	case SSH_MSG_USERAUTH_FAILURE:
	{
		m_transportState->AssertIsClient();
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		ExpectMessageNumber(s, SSH_MSG_USERAUTH_FAILURE);
		DecodeString(s, m_authenticator->m_methodsRemaining);
		bool partial;
		DecodeBoolean(s, partial);
		m_authenticator->m_state = partial ? Authenticator::PARTIAL_SUCCESS : Authenticator::FAILURE;
		m_authenticator->HandleUserAuthResponse(*this);
		break;
	}

	case SSH_MSG_USERAUTH_SUCCESS:
		m_transportState->AssertIsClient();
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		ExpectMessageNumber(s, SSH_MSG_USERAUTH_SUCCESS);
		m_authenticator->m_state = Authenticator::SUCCESS;
		m_authenticator->HandleUserAuthResponse(*this);
		break;

	case SSH_MSG_USERAUTH_METHODSPECIFIC_1:
		// represents SSH_MSG_USERAUTH_PK_OK for "publickey",
		//			  SSH_MSG_USERAUTH_PASSWD_CHANGEREQ for "password"
		//			  (packets share the same message number)
	{
		m_transportState->AssertIsClient();
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		ExpectMessageNumber(s, SSH_MSG_USERAUTH_METHODSPECIFIC_1);

		if (m_authenticator->m_state == Authenticator::SUCCESS)
		{
			s.Skip();	// ignore this message if already authenticated
			break;
		}

		if (m_authenticator->m_lastMethod == "publickey")
		{
			DecodeString(s, m_authenticator->m_pkAlgorithm);
			DecodeString(s, m_authenticator->m_pkPublicKey);

			m_authenticator->m_state = Authenticator::PK_OK;
			m_authenticator->HandleUserAuthResponse(*this);
		}
		else if (m_authenticator->m_lastMethod == "password")
		{
			DecodeString(s, m_authenticator->m_pwPrompt);
			DecodeString(s, m_authenticator->m_pwLanguage);

			m_authenticator->m_state = Authenticator::PASSWD_CHANGEREQ;
			m_authenticator->HandleUserAuthResponse(*this);
		}
		else
			goto Unimplemented;

		break;
	}

	case SSH_MSG_GLOBAL_REQUEST:
	{
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		m_authenticator->AssertState(Authenticator::SUCCESS);
		ExpectMessageNumber(s, SSH_MSG_GLOBAL_REQUEST);

		string type;
		bool wantReply;

		DecodeString(s, type);
		DecodeBoolean(s, wantReply);

		if (!HandleRequest(type, wantReply, s, t) && wantReply)
		{
			EncodeByte(t, SSH_MSG_REQUEST_FAILURE);
			t.MessageEnd();
		}
		break;
	}

	case SSH_MSG_REQUEST_SUCCESS:
	case SSH_MSG_REQUEST_FAILURE:
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		m_authenticator->AssertState(Authenticator::SUCCESS);
		ExpectMessageNumber(s, m_messageNumber);
		HandleRequestResponse(m_messageNumber == SSH_MSG_REQUEST_SUCCESS, s);
		break;

	case SSH_MSG_CHANNEL_OPEN:
	{
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		m_authenticator->AssertState(Authenticator::SUCCESS);

		string type;
		word32 yourNumber;

		ExpectMessageNumber(s, SSH_MSG_CHANNEL_OPEN);
		DecodeString(s, type);
		DecodeWord32(s, yourNumber);

		Channel* addedChannel = NULL;
		try
		{
			auto_ptr<Channel> pc(NewResponderChannel(type));

			if (!pc.get())
				throw ChannelOpenFailure(SSH_OPEN_UNKNOWN_CHANNEL_TYPE, "channel type not supported");
			if (TotalWaitObjects() + pc->MaxWaitObjects() > MaxWaitObjects())
				throw ChannelOpenFailure(SSH_OPEN_RESOURCE_SHORTAGE, "number of synchronization objects exceeds system limit");

			Channel& channel = m_channelTable.NewChannel(pc.release());
			addedChannel = &channel;
			channel.m_type = type;
			channel.m_yourNumber = yourNumber;
			DecodeWord32(s, channel.m_outWindowSize);
			DecodeWord32(s, channel.m_outMaxPacketSize);
			channel.m_state = Channel::OPEN_REQUEST_RECEIVED;

			if (!channel.CheckRemoteMaxPacketSize(channel.m_outMaxPacketSize))
				throw ChannelOpenFailure(SSH_OPEN_ADMINISTRATIVELY_PROHIBITED, "invalid max packet size");

			ByteQueue channelTypeSpecificData;
			if (channel.HandleOpen(*this, s, channelTypeSpecificData))
			{
				channel.m_state = Channel::OPEN;

				EncodeByte(t, SSH_MSG_CHANNEL_OPEN_CONFIRMATION);
				EncodeWord32(t, channel.m_yourNumber);
				EncodeWord32(t, channel.m_myNumber);
				EncodeWord32(t, channel.m_inWindowSize);
				EncodeWord32(t, channel.m_inMaxPacketSize);
				channelTypeSpecificData.TransferTo(t);
				t.MessageEnd();
			}
		}
		catch (ChannelOpenFailure& failure)
		{
			if (addedChannel)
				m_channelTable.RemoveChannel(addedChannel->m_myNumber);

			EncodeByte(t, SSH_MSG_CHANNEL_OPEN_FAILURE);
			EncodeWord32(t, yourNumber);
			EncodeWord32(t, failure.GetReason());
			EncodeString(t, failure.GetDescription());
			EncodeString(t, failure.GetLanguage());
			t.MessageEnd();
		}

		break;
	}

	case SSH_MSG_CHANNEL_OPEN_CONFIRMATION:
	case SSH_MSG_CHANNEL_OPEN_FAILURE:
	case SSH_MSG_CHANNEL_REQUEST:
	case SSH_MSG_CHANNEL_SUCCESS:
	case SSH_MSG_CHANNEL_FAILURE:
	case SSH_MSG_CHANNEL_DATA:
	case SSH_MSG_CHANNEL_EXTENDED_DATA:
	case SSH_MSG_CHANNEL_WINDOW_ADJUST:
	case SSH_MSG_CHANNEL_EOF:
	case SSH_MSG_CHANNEL_CLOSE:
	{
		m_transportState->AssertInState(IN_SERVICE_STARTED);
		m_authenticator->AssertState(Authenticator::SUCCESS);

		word32 myNumber;
		ExpectMessageNumber(s, m_messageNumber);
		DecodeWord32(s, myNumber);

		Channel* channel = m_channelTable.GetChannel(myNumber);
		if (!channel)
			throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "no such channel number");

		switch (m_messageNumber)
		{
		case SSH_MSG_CHANNEL_OPEN_CONFIRMATION:
			if (channel->m_state != Channel::WAITING_FOR_OPEN_RESPONSE)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			channel->m_state = Channel::OPEN;
			DecodeWord32(s, channel->m_yourNumber);
			DecodeWord32(s, channel->m_outWindowSize);
			DecodeWord32(s, channel->m_outMaxPacketSize);

			if (!channel->CheckRemoteMaxPacketSize(channel->m_outMaxPacketSize))
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "open channel confirmation: invalid max packet size");

			channel->HandleOpenConfirmation(*this, s, t);
			break;

		case SSH_MSG_CHANNEL_OPEN_FAILURE:
		{
			if (channel->m_state != Channel::WAITING_FOR_OPEN_RESPONSE)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			word32 reason;
			string description, language;
			DecodeWord32(s, reason);
			DecodeString(s, description);
			DecodeString(s, language);

			channel->m_state = Channel::OPEN_FAILED;
			channel->HandleOpenFailure(*this, reason, description, language);

			m_channelTable.RemoveChannel(channel->m_myNumber);
			break;
		}

		case SSH_MSG_CHANNEL_REQUEST:
		{
			if (channel->m_state != Channel::OPEN || channel->m_closeReceived)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			string type;
			bool wantReply;

			DecodeString(s, type);
			DecodeBoolean(s, wantReply);

			if (!channel->HandleRequest(*this, type, wantReply, s, t) && wantReply && !channel->m_closeSent)
			{
				EncodeByte(t, SSH_MSG_CHANNEL_FAILURE);
				EncodeWord32(t, channel->m_yourNumber);
				t.MessageEnd();
			}
			break;
		}

		case SSH_MSG_CHANNEL_SUCCESS:
		case SSH_MSG_CHANNEL_FAILURE:
			if (channel->m_state != Channel::OPEN || channel->m_closeReceived)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			channel->HandleRequestResponse(*this, m_messageNumber == SSH_MSG_CHANNEL_SUCCESS, s);
			break;

		case SSH_MSG_CHANNEL_DATA:
			if (channel->m_state != Channel::OPEN || channel->m_eofReceived || channel->m_closeReceived)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			channel->HandleData(*this, s, t);
			break;

		case SSH_MSG_CHANNEL_EXTENDED_DATA:
		{
			if (channel->m_state != Channel::OPEN || channel->m_eofReceived || channel->m_closeReceived)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			word32 dataType;
			DecodeWord32(s, dataType);

			channel->HandleExtendedData(*this, dataType, s, t);
			break;
		}

		case SSH_MSG_CHANNEL_WINDOW_ADJUST:
		{
			if (channel->m_state != Channel::OPEN)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			word32 incrementSize;
			DecodeWord32(s, incrementSize);
			channel->m_outWindowSize += incrementSize;
			break;
		}

		case SSH_MSG_CHANNEL_EOF:
			if (channel->m_eofReceived || channel->m_closeReceived)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			channel->m_eofReceived = true;
			channel->OnEofReceived();
			break;

		case SSH_MSG_CHANNEL_CLOSE:
			if (channel->m_closeReceived)
				throw CauseToDisconnect(SSH_DISCONNECT_PROTOCOL_ERROR, "unexpected channel message");

			channel->m_closeReceived = true;
			channel->OnCloseReceived();
			break;
		}

		break;
	}

	default:
	   Unimplemented:
		EncodeByte(t, SSH_MSG_UNIMPLEMENTED);
		EncodeWord32(t, m_transportState->GetInSequenceNumber());
		t.MessageEnd();
		break;
	}

	m_messageNumberReceived = false;
	m_messageBytesReceived = 0;

	if (!m_inQueue.IsEmpty())
		HandlePacketNotCompletelyProcessed(m_inQueue);
}


void Manager::SendServiceRequest()
{
	assert(m_transportState->GetOutState() == READY_TO_SEND_SERVICE_REQUEST);

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_SERVICE_REQUEST);
	EncodeString(t, "ssh-userauth");
	t.MessageEnd();
}


void Manager::SendUserAuthBanner(string const& message, string const& languageTag)
{
	assert(m_transportState->GetOutState() >= READY_TO_SEND_SERVICE_REQUEST);

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_USERAUTH_BANNER);
	EncodeString(t, message);
	EncodeString(t, languageTag);
	t.MessageEnd();
}


void Manager::SendNoneAuthRequest(
	string const& userName)
{
	assert(m_transportState->GetOutState() >= READY_TO_SEND_SERVICE_REQUEST);
	m_authenticator->m_lastMethod = "none";

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_USERAUTH_REQUEST);
	EncodeString(t, userName);
	EncodeString(t, "ssh-connection");
	EncodeString(t, "none");
	t.MessageEnd();
}


void Manager::SendPasswordAuthRequest(
	string const& userName,
	byte const* pwd, unsigned int pwdlen)
{
	assert(m_transportState->GetOutState() >= READY_TO_SEND_SERVICE_REQUEST);
	m_authenticator->m_lastMethod = "password";

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_USERAUTH_REQUEST);
	EncodeString(t, userName);
	EncodeString(t, "ssh-connection");
	EncodeString(t, "password");
	EncodeBoolean(t, false);
	EncodeString(t, pwd, pwdlen);
	t.MessageEnd();
}


void Manager::SendPasswordChangeAuthRequest(
	string const& userName,
	byte const* oldpwd, unsigned int oldpwdlen,
	byte const* newpwd, unsigned int newpwdlen)
{
	assert(m_transportState->GetOutState() >= READY_TO_SEND_SERVICE_REQUEST);
	m_authenticator->m_lastMethod = "password";

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_USERAUTH_REQUEST);
	EncodeString(t, userName);
	EncodeString(t, "ssh-connection");
	EncodeString(t, "password");
	EncodeBoolean(t, true);
	EncodeString(t, oldpwd, oldpwdlen);
	EncodeString(t, newpwd, newpwdlen);
	t.MessageEnd();
}


void Manager::SendPublicKeyAuthRequest(
	string const& userName,
	string const& algorithm,
	string const& publicKey)
{
	assert(m_transportState->GetOutState() >= READY_TO_SEND_SERVICE_REQUEST);
	m_authenticator->m_lastMethod = "publickey";

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_USERAUTH_REQUEST);
	EncodeString(t, userName);
	EncodeString(t, "ssh-connection");
	EncodeString(t, "publickey");
	EncodeBoolean(t, false);
	EncodeString(t, algorithm);
	EncodeString(t, publicKey);
	t.MessageEnd();
}


void Manager::SendPublicKeyAuthRequest(	
	string const& userName,
	BufferedTransformation& btOpenSshPrivateKey,
	byte const* pwd, unsigned int pwdlen,
	RandomNumberGenerator* rng)
{
	ByteQueue bqPrivKey;
	string sPemAlg = DecodePemPrivateKey(btOpenSshPrivateKey, pwd, pwdlen, bqPrivKey);

	string sAlgorithm;
	string sPubKey;
	string sDecoratedSignature;

	if (sPemAlg == "RSA")
	{
		sAlgorithm = "ssh-rsa";

		OpenSshRsaPrivateKey privkey(bqPrivKey);
		privkey.EncodePubKey(StringSink(sPubKey));

		if (rng)
		{
			SecByteBlock sbbToSign;
			ConstructPublicKeyAuthBlob(sbbToSign, m_transportState->GetSessionID(), userName, sAlgorithm, sPubKey);

			RSASSA_PKCS1v15_SHA_Signer signer(privkey.Get());

			SecByteBlock sbbSignature(signer.SignatureLength());
			signer.SignMessage(*rng, sbbToSign.Begin(), sbbToSign.Size(), sbbSignature.Begin());

			StringSink decorated(sDecoratedSignature);
			EncodeString(decorated, "ssh-rsa");
			EncodeString(decorated, sbbSignature);
		}
	}
	else if (sPemAlg == "DSA")
	{
		sAlgorithm = "ssh-dss";

		OpenSshDsaPrivateKey privkey(bqPrivKey);
		privkey.EncodePubKey(StringSink(sPubKey));

		if (rng)
		{
			SecByteBlock sbbToSign;
			ConstructPublicKeyAuthBlob(sbbToSign, m_transportState->GetSessionID(), userName, sAlgorithm, sPubKey);

			SecByteBlock sbbSignature(privkey.Get().SignatureLength());
			privkey.Get().SignMessage(*rng, sbbToSign.Begin(), sbbToSign.Size(), sbbSignature.Begin());

			StringSink decorated(sDecoratedSignature);
			EncodeString(decorated, "ssh-dss");
			EncodeString(decorated, sbbSignature);
		}
	}
	else
		throw Err(BufferedTransformation::OTHER_ERROR, "unrecognized private key algorithm");

	if (rng)
		SendFullPublicKeyAuthRequest(userName, sAlgorithm, sPubKey, sDecoratedSignature);
	else
		SendPublicKeyAuthRequest(userName, sAlgorithm, sPubKey);
}


void Manager::SendFullPublicKeyAuthRequest(
	string const& userName,
	string const& algorithm,
	string const& publicKey,
	string const& signature)
{
	assert(m_transportState->GetOutState() >= READY_TO_SEND_SERVICE_REQUEST);
	m_authenticator->m_lastMethod = "publickey";

	BufferedTransformation& t = *AttachedTransformation();
	EncodeByte(t, SSH_MSG_USERAUTH_REQUEST);
	EncodeString(t, userName);
	EncodeString(t, "ssh-connection");
	EncodeString(t, "publickey");
	EncodeBoolean(t, true);
	EncodeString(t, algorithm);
	EncodeString(t, publicKey);
	EncodeString(t, signature);
	t.MessageEnd();
}


bool Manager::VerifyPublicKeyAuthAlgorithm(
	string const& algorithm, string const& publicKey)
{
	string publicKeyAlg;

	StringSource publicKeySrc(publicKey, true);
	DecodeString(publicKeySrc, publicKeyAlg);

	if (publicKeyAlg != algorithm)
		return false;

	if ((algorithm != "ssh-rsa") &&
		(algorithm != "ssh-dss"))
		return false;

	return true;
}


bool Manager::VerifyPublicKeyAuthSignature(
	string const& userName, string const& algorithm,
	string const& publicKey, string const& signature)
{
	SecByteBlock sbbToSign;
	ConstructPublicKeyAuthBlob(
		sbbToSign, m_transportState->GetSessionID(), userName, algorithm, publicKey);

	try
	{
		StringSource publicKeySrc(publicKey, true);
		StringSource signatureSrc(signature, true);

		string publicKeyAlg;
		string signatureAlg;
		string signatureData;

		DecodeString(publicKeySrc, publicKeyAlg);
		DecodeString(signatureSrc, signatureAlg);
		DecodeString(signatureSrc, signatureData);

		if ((publicKeyAlg != algorithm) || (signatureAlg != algorithm))
			return false;

		if (algorithm == "ssh-rsa")
		{
			Integer exponent;
			Integer modulus;

			DecodeInteger(publicKeySrc, exponent);
			DecodeInteger(publicKeySrc, modulus);

			RSASSA_PKCS1v15_SHA_Verifier verifier(modulus, exponent);

			if (signatureData.size() != verifier.SignatureLength())
				return false;

			return verifier.VerifyMessage(sbbToSign.Begin(), sbbToSign.Size(), (byte const*) signatureData.data());
		}
		else if (algorithm == "ssh-dss")
		{
			Integer modulus;
			Integer subgroupSize;
			Integer generator;
			Integer publicResidue;

			DecodeInteger(publicKeySrc, modulus);
			DecodeInteger(publicKeySrc, subgroupSize);
			DecodeInteger(publicKeySrc, generator);
			DecodeInteger(publicKeySrc, publicResidue);

			DSAPublicKey verifier(modulus, subgroupSize, generator, publicResidue);

			if (signatureData.size() != verifier.SignatureLength())
				return false;

			return verifier.VerifyMessage(sbbToSign.Begin(), sbbToSign.Size(), (byte const*) signatureData.data());
		}
		else
			return false;
	}
	catch (...)
	{
		return false;
	}
}


Channel& Manager::CreateChannel(string const& type)
{
	auto_ptr<Channel> pc(NewInitiatorChannel(type));
	if (!pc.get())
		throw UnknownChannelType();
	if (TotalWaitObjects() + pc->MaxWaitObjects() > MaxWaitObjects())
		throw TooManyWaitObjects();
	if (pc->m_type.empty())
		pc->m_type = type;
	return m_channelTable.NewChannel(pc.release());
}


void Manager::OpenChannel(Channel& channel)
{
	channel.Open(*this, *AttachedTransformation());
}


bool Manager::AnyChannelsOpen() const
{
	word32 bound = ChannelNumberBound();
	for (word32 channel=0; channel<bound; channel++)
		if (GetChannel(channel))
			return true;

	return false;
}


void Manager::ProcessChannels()
{
	// Halt channel processing on key re-exchange
	if (m_transportState->GetOutState() != OUT_SERVICE_STARTED)
		return;

	// All clear, do channel processing
	OpenNewChannels();

	word32 bound = ChannelNumberBound();
	for (word32 channel=0; channel<bound; channel++)
	{
		Channel* pc = GetChannel(channel);
		if (!pc)
			continue;

		if (pc->m_state == Channel::OPEN_REQUEST_RECEIVED)
		{
			BufferedTransformation& t = *AttachedTransformation();

			try
			{
				ByteQueue channelTypeSpecificData;
				if (pc->ContinueHandleOpen(*this, channelTypeSpecificData))
				{
					pc->m_state = Channel::OPEN;

					EncodeByte(t, SSH_MSG_CHANNEL_OPEN_CONFIRMATION);
					EncodeWord32(t, pc->m_yourNumber);
					EncodeWord32(t, pc->m_myNumber);
					EncodeWord32(t, pc->m_inWindowSize);
					EncodeWord32(t, pc->m_inMaxPacketSize);
					channelTypeSpecificData.TransferTo(t);
					t.MessageEnd();
				}
			}
			catch (ChannelOpenFailure& failure)
			{
				word32 yourNumber = pc->m_yourNumber;
				m_channelTable.RemoveChannel(pc->m_myNumber);

				EncodeByte(t, SSH_MSG_CHANNEL_OPEN_FAILURE);
				EncodeWord32(t, yourNumber);
				EncodeWord32(t, failure.GetReason());
				EncodeString(t, failure.GetDescription());
				EncodeString(t, failure.GetLanguage());
				t.MessageEnd();
			}
		}
		else if (pc->m_state == Channel::OPEN)
		{
			// Note by db: I have completely revamped this method to implement behavior that
			// is (according to my opinion) more consistent with the SSH2 specification.
			// Channels are now not closed immediately as soon as EOF is received OR sent,
			// but only after EOF is received AND sent. Also, I took the liberty of changing
			// the IO interface significantly so as to make it more practical and intuitive.

			// Send a close if the channel is still open and there's nothing more to do:
			// [ case a) ] close has been received from remote
			// If we receive a close from remote, it makes sense to close the channel
			// on our side even if we haven't completed sending all our data. If the remote
			// wanted us to complete sending data, it would have waited for us to send EOF.
			if (!pc->m_closeSent && pc->m_closeReceived)
				ChannelSendClose(channel);

			// Send an EOF if it has been scheduled. This check is done twice, once here
			// and once after processing input/output data. The reason it is also done here
			// is to make sure that if m_eofScheduled is true at this point, the EOF is
			// sent immediately and m_eofSent becomes true as well.
			if (!pc->m_closeSent && !pc->m_eofSent && pc->m_eofScheduled)
				ChannelSendEof(channel);

			// Send a close if the channel is still open and there's nothing more to do:
			// [ case b) ] we sent eof, remote sent eof
			// [ case c) ] we sent eof, we can't receive data from remote any more
			if (!pc->m_closeSent && pc->m_eofSent && (pc->m_eofReceived || pc->m_receiveStopped))
				ChannelSendClose(channel);

			// Are we still in a state where we can receive data from remote?
			// Note that we keep on handling received data even after we have already sent
			// 'close' to remote. This isn't a vulnerability because, after we have sent
			// close, the remote can't send more data than allowed by our output window size.
			if (!pc->m_eofReceived && !pc->m_closeReceived && !pc->m_receiveStopped)
			{
				pc->ProcessReceivedData();

				// Should we send a window adjustment so the remote can send more data?
				if (!pc->m_closeSent)
				{
					// The point of this assert() is to ensure that the channel implementation
					// always has enough room to store as much data as it has promised it would
					// store. Keep in mind that pc->m_inWindowSize is a dynamic variable; it is
					// decremented every time new data is received from remote. This assertion
					// verifies that the implementation can accomodate any amount of data that
					// the remote is allowed to sent.
					assert(pc->MaxRecvBufferSize() >= pc->CurrentRecvBufferSize() + pc->m_inWindowSize);
					word32 windowIncrement = pc->MaxRecvBufferSize() - pc->CurrentRecvBufferSize() - pc->m_inWindowSize;

					// Send the adjustment packet if the increment would be large enough
					// (more than half of maximum receive buffer size)
					if (2*windowIncrement > pc->MaxRecvBufferSize())
						ChannelAdjustWindowSize(channel, windowIncrement);
				}
			}

			// Is it still possible to send data to remote?
			if (!pc->m_closeSent && !pc->m_eofSent)
				pc->ProcessDataToBeSent();

			// Send an EOF if it has been scheduled. This check MUST be done immediately
			// after data has been handled; if it is only done before, EOF will not be sent
			// until the next event occurs, which might be much later.
			if (!pc->m_closeSent && !pc->m_eofSent && pc->m_eofScheduled)
				ChannelSendEof(channel);

			// Send a close if the channel is still open and there's nothing more to do:
			// [ case b) ] we sent eof, remote sent eof
			// [ case c) ] we sent eof, we can't receive data from remote any more
			// We repeat this check here although we've already done it above. In fact,
			// doing it here might be considered more important than doing it above.
			if (!pc->m_closeSent && pc->m_eofSent && (pc->m_eofReceived || pc->m_receiveStopped))
				ChannelSendClose(channel);

			// Is there any lingering data to process? Lingering data is data that stays
			// in our receive buffer after we already know that the remote will not
			// (or should not) be sending anything anymore.
			if ((pc->m_eofReceived || pc->m_closeReceived) &&
				(pc->CurrentRecvBufferSize() && !pc->m_receiveStopped))
				pc->ProcessLingeringData();

			// Can we remove the channel from the channel table?
			// We can remove a channel object once close has been both sent and received,
			// AND all received data has been processed.
			if (pc->m_closeSent && pc->m_closeReceived &&
				(!pc->CurrentRecvBufferSize() || pc->m_receiveStopped))
				m_channelTable.RemoveChannel(channel);
		}
	}
}


NAMESPACE_END
