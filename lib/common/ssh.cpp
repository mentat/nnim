// --*-c++-*--
/*
    $Id: ssh.cpp,v 1.2 2002/06/13 16:38:50 thementat Exp $
 
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

#include "ssh.h"
#include "crypto/misc.h"
#include "crypto/hmac.h"
#include "crypto/sha.h"
#include "crypto/md5.h"
#include "crypto/zlib.h"
#include "crypto/mars.h"
#include "crypto/cbc.h"
#include "crypto/aes.h"
#include "crypto/serpent.h"
#include "crypto/blowfish.h"
#include "crypto/twofish.h"
#include "crypto/des.h"
#include "crypto/arc4.h"
#include "crypto/cast.h"
#include "crypto/idea.h"

using namespace std;
using namespace CryptoPP;

// to-from network byte order
template<class T>
T NBO(T num){

    if (CheckEndianess(false)) // false little endian first
        return byteReverse(num);
    else
        return num;
}

template<class T>
SecByteBlock GetMAC( const SecByteBlock& key, int keyLength, const SecByteBlock& packet, unsigned int trun)
{
    SecByteBlock ret(trun);

    HMAC<T> themac(key, keyLength);
    themac.Update(packet, packet.Size());
    themac.TruncatedFinal(ret, trun);
 
    return ret;
}

template<class T>
bool CheckMAC( const SecByteBlock& key, int keyLength, const SecByteBlock& packet, const SecByteBlock& hmac)
{
    return HMAC<T>(key, keyLength).VerifyDigest(hmac, packet, packet.Size());
}

template<class T>
SecByteBlock DoEncrypt( const SecByteBlock& key, int keyLength, const SecByteBlock& packet, 
                       SecByteBlock& IV, unsigned int blockLength)
{
    // create templated cipher
    T cipher(key, keyLength);

    // cbc encryptor
    CBCRawEncryptor enc(cipher, IV);
    enc.Put(packet, packet.Size());
    enc.MessageEnd();

    // return ciphertext
    SecByteBlock ret(enc.MaxRetrievable());
    
    enc.Get(ret, ret.Size());

    // update IV

    if (IV.Size() < blockLength)
        IV.CleanNew(blockLength);

    for (unsigned int i = 0; i < blockLength; i++)
        IV[i] = ret[ret.Size() - blockLength + i]; // gets the last block

    return ret;

}

template<class T>
SecByteBlock DoDecrypt( const SecByteBlock& key, int keyLength, const SecByteBlock& packet, 
                       SecByteBlock& IV, unsigned int blockLength)
{
  
    // update remote IV, opposite of encrypt

    if (IV.Size() < blockLength)
        IV.CleanNew(blockLength);

    for (unsigned int i = 0; i < blockLength; i++)
        IV[i] = packet[packet.Size() - blockLength + i]; // gets the last block

    // create templated cipher
    T cipher(key, keyLength);

    // cbc encryptor
    CBCRawDecryptor dec(cipher, IV);
    dec.Put(packet, packet.Size());
    dec.MessageEnd();

    // return ciphertext
    SecByteBlock ret(dec.MaxRetrievable());
    
    dec.Get(ret, ret.Size());

    return ret;

}

// transform a real string to a ssh2 (ietf draft pg 6/7) string
SecByteBlock sshString(const string& str)
{
    SecByteBlock out(str.length() + 4);

    uint32 size = NBO((uint32)str.length());

    out[0] = (size && 0xff);
    out[1] = ((size >> 8) && 0xff);
    out[2] = ((size >> 16) && 0xff);
    out[3] = ((size >> 24) && 0xff);

    for (unsigned int i = 0; i < str.length(); i++)
        out[i + 4] = str[i];

    return out;
}

SSH2::SSH2()
{
    m_sequenceNumber = 0;
}

SecByteBlock SSH2::MakeBinaryPacket(const SecByteBlock& payload,
            const SecByteBlock& random_padding,
            const SecByteBlock& mac)
{
    if ((4 + payload.Size() + random_padding.Size() + mac.Size()) > MAX_PACKET_LENGTH)
        throw("To large of a packet");

    SecByteBlock return_packet(4 + payload.Size() + random_padding.Size() + mac.Size());
    
    // total packet size in NBO
    uint32 size(NBO((uint32)return_packet.Size()));

    // add total packet length
    return_packet[0] = (byte)(size & 0xff);
    return_packet[1] = (byte)((size >> 8) & 0xff);
    return_packet[2] = (byte)((size >> 16) & 0xff);
    return_packet[3] = (byte)((size >> 24) & 0xff);
    
    // add padding length
    if (random_padding.Size() > 255)
        throw ("Random padding is to large");
    return_packet[4] = (byte)random_padding.Size();

    unsigned int i;
    // i know these loops are very slow, but i'm trying to avoid memcpy/move functions

    // add encrypted payload
    for ( i = 0; i < payload.Size(); i++)
        return_packet[i+5] = payload[i];

    // add padding
    for ( i = 0; i < random_padding.Size(); i++)
        return_packet[i + 5 + payload.Size() ] = random_padding[i];

    // add mac
    for ( i = 0; i < mac.Size(); i++)
        return_packet[i + 5 + payload.Size() + random_padding.Size() ] = mac[i];

    // return packet
    return return_packet;

}

SecByteBlock SSH2::MakeBinaryPacketNoMAC(const SecByteBlock& payload,
    const SecByteBlock& random_padding)
{
    return SecByteBlock();
}

SecByteBlock SSH2::EncryptPacket(const SecByteBlock& packet_minus_mac)
{

    switch (m_cipherType)
    {
    case (THREE_DES_CBC): return DoEncrypt<DES_EDE3_Encryption>(m_sessionKey, 24, packet_minus_mac, m_IV, 8); break;
    case (BLOWFISH_CBC): return DoEncrypt<BlowfishEncryption>(m_sessionKey, 16, packet_minus_mac, m_IV, 8); break;
    case (TWOFISH256_CBC): return DoEncrypt<TwofishEncryption>(m_sessionKey, 32, packet_minus_mac, m_IV, 16); break;
    case (TWOFISH192_CBC): return DoEncrypt<TwofishEncryption>(m_sessionKey, 24, packet_minus_mac, m_IV, 16); break;
    case (TWOFISH128_CBC): return DoEncrypt<TwofishEncryption>(m_sessionKey, 16, packet_minus_mac, m_IV, 16); break;
    case (AES256_CBC): return DoEncrypt<AESEncryption>(m_sessionKey, 32, packet_minus_mac, m_IV, 16); break;
    case (AES192_CBC): return DoEncrypt<AESEncryption>(m_sessionKey, 24, packet_minus_mac, m_IV, 16); break;
    case (AES128_CBC): return DoEncrypt<AESEncryption>(m_sessionKey, 16, packet_minus_mac, m_IV, 16); break;
    case (SERPENT256_CBC): return DoEncrypt<SerpentEncryption>(m_sessionKey, 32, packet_minus_mac, m_IV, 16); break;
    case (SERPENT192_CBC): return DoEncrypt<SerpentEncryption>(m_sessionKey, 24, packet_minus_mac, m_IV, 16); break;
    case (SERPENT128_CBC): return DoEncrypt<SerpentEncryption>(m_sessionKey, 16, packet_minus_mac, m_IV, 16); break;
 //   case (ARCFOUR): return DoEncrypt<ARC4>(m_sessionKey, 16, packet_minus_mac, m_IV, 8); break;
    case (IDEA_CBC): return DoEncrypt<IDEAEncryption>(m_sessionKey, 16, packet_minus_mac, m_IV, 8); break;
    case (CAST128_CBC): return DoEncrypt<CAST128Encryption>(m_sessionKey, 16, packet_minus_mac, m_IV, 8);  break;
    case (NO_CIPHER): return packet_minus_mac; break;
    case (MARS256_CBC): return DoEncrypt<MARSEncryption>(m_sessionKey, 32, packet_minus_mac, m_IV,16); break;
    default: throw("No such cipher available");
    }

}

SecByteBlock SSH2::DecryptPacket(const SecByteBlock& cipher_text)
{
    switch (m_remoteCipherType)
    {
    case (THREE_DES_CBC): return DoDecrypt<DES_EDE3_Encryption>(m_sessionKey, 24, cipher_text, m_remoteIV, 8); break;
    case (BLOWFISH_CBC): return DoDecrypt<BlowfishEncryption>(m_sessionKey, 16, cipher_text, m_remoteIV, 8); break;
    case (TWOFISH256_CBC): return DoDecrypt<TwofishEncryption>(m_sessionKey, 32, cipher_text, m_remoteIV, 16); break;
    case (TWOFISH192_CBC): return DoDecrypt<TwofishEncryption>(m_sessionKey, 24, cipher_text, m_remoteIV, 16); break;
    case (TWOFISH128_CBC): return DoDecrypt<TwofishEncryption>(m_sessionKey, 16, cipher_text, m_remoteIV, 16); break;
    case (AES256_CBC): return DoDecrypt<AESEncryption>(m_sessionKey, 32, cipher_text, m_remoteIV, 16); break;
    case (AES192_CBC): return DoDecrypt<AESEncryption>(m_sessionKey, 24, cipher_text, m_remoteIV, 16); break;
    case (AES128_CBC): return DoDecrypt<AESEncryption>(m_sessionKey, 16, cipher_text, m_remoteIV, 16); break;
    case (SERPENT256_CBC): return DoDecrypt<SerpentEncryption>(m_sessionKey, 32, cipher_text, m_remoteIV, 16); break;
    case (SERPENT192_CBC): return DoDecrypt<SerpentEncryption>(m_sessionKey, 24, cipher_text, m_remoteIV, 16); break;
    case (SERPENT128_CBC): return DoDecrypt<SerpentEncryption>(m_sessionKey, 16, cipher_text, m_remoteIV, 16); break;
 //   case (ARCFOUR): return DoDecrypt<ARC4>(m_sessionKey, 16, cipher_text, m_remoteIV, 8); break;
    case (IDEA_CBC): return DoDecrypt<IDEAEncryption>(m_sessionKey, 16, cipher_text, m_remoteIV, 8); break;
    case (CAST128_CBC): return DoDecrypt<CAST128Encryption>(m_sessionKey, 16, cipher_text, m_remoteIV, 8);  break;
    case (NO_CIPHER): return cipher_text; break;
    case (MARS256_CBC): return DoDecrypt<MARSEncryption>(m_sessionKey, 32, cipher_text, m_remoteIV,16); break;
    default: throw("No such cipher available");
    }

}

SecByteBlock SSH2::MakeNegotiationPacket()
{
    /*
      byte      SSH_MSG_KEXINIT
  byte[16]  cookie (random bytes)
  string    kex_algorithms
  string    server_host_key_algorithms
  string    encryption_algorithms_client_to_server
  string    encryption_algorithms_server_to_client
  string    mac_algorithms_client_to_server
  string    mac_algorithms_server_to_client
  string    compression_algorithms_client_to_server
  string    compression_algorithms_server_to_client
  string    languages_client_to_server
  string    languages_server_to_client
  boolean   first_kex_packet_follows
  uint32    0 (reserved for future extension)*/

   //string largeString(


    return SecByteBlock();

}

SecByteBlock SSH2::MakeDisconnectPacket(unsigned long reason_code, const char * description, 
                                              const char * lang_code)
{
    return SecByteBlock();
}

SecByteBlock SSH2::MakeServiceRequestPacket(const char * service_name)
{
    return SecByteBlock();
}

SecByteBlock SSH2::MakeMAC(const SecByteBlock& packet)
{
    SecByteBlock mac;

    SecByteBlock ToMac(packet.Size() + 4);

    uint32 seq(NBO(m_sequenceNumber));

    ToMac[0] = (byte)(seq && 0xff);
    ToMac[1] = (byte)((seq >> 8) && 0xff);
    ToMac[2] = (byte)((seq >> 16) && 0xff);
    ToMac[3] = (byte)((seq >> 24) && 0xff);

    for (uint32 i = 0; i < packet.Size(); i++)
        ToMac[i+4] = packet[i];

    switch (m_macType)
    {
    case(NO_HMAC): 
        return SecByteBlock(); 
        break;
    case (HMAC_SHA1_96): 
        mac = GetMAC<SHA>(m_sessionKey, 20, ToMac, 12); break;
    case (HMAC_SHA1): 
        mac = GetMAC<SHA>(m_sessionKey, 20, ToMac, 20); break;
    case (HMAC_MD5):
        mac = GetMAC<MD5>(m_sessionKey, 16, ToMac, 16); break;
    case (HMAC_MD5_96):
        mac = GetMAC<MD5>(m_sessionKey, 16, ToMac, 12); break;
    default: throw ("No such MAC available");
    }

    return mac;
}

bool SSH2::VerifyMAC(const SecByteBlock& packet, const SecByteBlock& hmac)
{
    switch (m_remoteMacType)
    {
    case(NO_HMAC): 
        return true; 
        break;
    case (HMAC_SHA1_96): 
        return CheckMAC<SHA>(m_sessionKey, 20, packet, hmac); break;
    case (HMAC_SHA1): 
        return CheckMAC<SHA>(m_sessionKey, 20, packet, hmac); break;
    case (HMAC_MD5):
        return CheckMAC<MD5>(m_sessionKey, 16, packet, hmac); break;
    case (HMAC_MD5_96):
        return CheckMAC<MD5>(m_sessionKey, 16, packet, hmac); break;
    default: throw ("No such MAC available");
    }


}


SecByteBlock SSH2::CompressPayload(const SecByteBlock& packet)
{
    static ZlibCompressor comp; // ietf talks of "partial flush" to the
                                // compressor so is static (check with crypto++)

    comp.Put(packet, packet.Size());
    comp.MessageEnd();

    SecByteBlock compressed(comp.MaxRetrievable());
    comp.Get(compressed, compressed.Size());

    return compressed;
}

SecByteBlock SSH2::DecompressPayload(const SecByteBlock& packet)
{
    static ZlibDecompressor decomp;

    decomp.Put(packet, packet.Size());
    decomp.MessageEnd();

    SecByteBlock decompressed(decomp.MaxRetrievable());
    decomp.Get(decompressed, decompressed.Size());

    return decompressed;
}

