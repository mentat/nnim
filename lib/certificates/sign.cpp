/*
    $Id: sign.cpp,v 1.3 2002/06/24 12:07:40 thementat Exp $

    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2001  Henrik Abelsson <henrik@abelsson.com>

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

#include <crypto/cryptlib.h>
#include <crypto/rsa.h>
#include <crypto/hex.h>
#include <crypto/base64.h>
#include <crypto/osrng.h> 
#include <crypto/gzip.h>
#include <crypto/sha.h>
#include <crypto/files.h>
#include <crypto/rng.h>

#include "sign.h"

using namespace CryptoPP;
int dumphex (byte *data,int length);

Certificate::Certificate():
  m_cert()
{

}
Certificate::Certificate(byte *cert,int len):
  m_cert()
{
  m_cert.setData(len,cert);
}

Certificate::Certificate(dstring cert):
  m_cert()
{
  m_cert.setData(cert);
}
Certificate::Certificate(const string &cert,int encoding):
  m_cert()
{
  byte *tmp;
  int len;
  try {
  switch (encoding)
  {
  case Hex:
  {
    StringSource s(cert,true,new HexDecoder(new Gunzip));
    len=s.MaxRetrievable();

    tmp=new byte[len];
    s.Get(tmp,len);
    m_cert.setData(len,tmp);
    delete[] tmp;
  }
  break;
  case Base64:
  {
    StringSource s(cert,true,new Base64Decoder(new Gunzip));
    len=s.MaxRetrievable();

    tmp=new byte[len];
    s.Get(tmp,len);
    m_cert.setData(len,tmp);
    delete[] tmp;
  }
  break;

  default:
    break;
  }
  }
  catch (Exception ex)
  {
    cout << "Caught excption: " << ex.what() << endl;
  }
}

Certificate::~Certificate()
{

}

void Certificate::generateKey(int length, string &privkey, string &pubkey, const string &password)
{
  // get some entropy and generate the keys
  AutoSeededRandomPool randPool;

  RSAES_OAEP_SHA_Decryptor priv(randPool, length);
  ByteQueue privOut;
  priv.DEREncode(privOut);
  privOut.MessageEnd();
  int len= privOut.MaxRetrievable();
  byte *buf = new byte[len];
  privOut.Get(buf,len);

  // store private key
  StringSource(buf,len,true,new HexEncoder(new StringSink(privkey)));

  memset(buf,0,len);
  delete[] buf;

  RSAES_OAEP_SHA_Encryptor pub(priv);
  ByteQueue pubOut;
  pub.DEREncode(pubOut);
  pubOut.MessageEnd();

  len= pubOut.MaxRetrievable();
  buf = new byte[len];
  pubOut.Get(buf,len);

  // store public key
  StringSource(buf,len,true,new HexEncoder(new StringSink(pubkey)));

  memset(buf,0,len);
  delete[] buf;

  return;
}

dstring Certificate::create(const string &_subject,
							const string &_pubkey,
							const string &_issuer,
							const string &_privkey,
							const string &_password,
							const int _serial)
{
  m_cert.addSection(version,CERT_VERSION);
  m_cert.addSection(serial,(unsigned int)123);
  m_cert.addSection(hashAlgorithm,HASH_ALGORITHM_SHA);
  m_cert.addSection(hashAlgorithmParam,(unsigned int)0);
  m_cert.addSection(issuer,_issuer);
  m_cert.addSection(startTime,(unsigned int)0);
  m_cert.addSection(endTime,(unsigned int)31137);
  m_cert.addSection(subject,_subject);
  m_cert.addSection(pubkeyAlgorithm,PUBKEY_ALGORITHM_RSA);
  m_cert.addSection(pubkeyParam,(unsigned int)0);
  m_cert.addSection(pubkey,_pubkey);

  StringSource privKey(_privkey, true, new HexDecoder);
  RSASSA_PKCS1v15_SHA_Signer priv(privKey);
  NullRNG rng;
//  cout << "signing " << m_cert.getData().length() << " bytes: \n";
//  dumphex((byte*)m_cert.getData().data(),m_cert.getData().length());

  StringSource f(m_cert.getData().data(),m_cert.getData().length(), true,
                new SignerFilter(rng, priv));

  int size = f.MaxRetrievable();
  byte *sig = new byte[size];
  f.Get(sig,size);
  m_cert.addSection(signature,size,sig);
//  cout << "SIG: "; dumphex(sig,size);

//  cout << "signing " << m_cert.getData().length() << " bytes: \n";
//  dumphex((byte*)m_cert.getData().data(),m_cert.getData().length());

  return m_cert.getData();

}

bool Certificate::verify(const string &pubkey)
{
  Storage::sectionType section;
  byte *certificate;
  int len=0;
  int n=0;
  try {
    StringSource signerPubkey(pubkey, true, new HexDecoder);
    RSASSA_PKCS1v15_SHA_Verifier pub(signerPubkey);

    //  cout << "signing " << m_cert.getData().length() << " bytes: \n";
    //  dumphex((byte*)m_cert.getData().data(),m_cert.getData().length());

    section=m_cert.readSection(n++);

    do
      {
	len+=5+section.second.second; /* type + len + data */
	section=m_cert.readSection(n++);
      }while (section.first != signature);

    //  certificate = (byte*)m_cert.getData().data(); //

    //  cout << "verifying " << len << " bytes \n";

    certificate=new byte[len];
    memcpy(certificate,m_cert.getData().data(),len);

    //  dumphex(certificate,len);

    assert(section.first == Certificate::signature);

    if ((unsigned)section.second.second != pub.SignatureLength())
      {
	cout << "mismatching sig length\n";
	return false;
      }
    //  cout << "SIG: "; dumphex(section.second.first,section.second.second);

    StringSource signatureStr(section.second.first,section.second.second, true);

    SecByteBlock signature(pub.SignatureLength());
    signatureStr.Get(signature, signature.size);

    VerifierFilter *verifierFilter = new VerifierFilter(pub);
    verifierFilter->PutSignature(signature);

    StringSource f(certificate,len, true, verifierFilter);

    byte result = 0;
    f.Get(result);
    // cout << "result: "<< (int)result << endl;
    return result == 1;
  }
  catch(Exception& ex)
    {
		ex.what();
		return false;
    }
}

string Certificate::encodeHex()
{
  byte *data = (byte*)m_cert.getData().data();
  int len=m_cert.getData().length();
  string out;
  StringSource f(data,len, true,new Gzip(new HexEncoder(new StringSink(out))));
//  StringSource f(data,len, true,new HexEncoder(new StringSink(out)));
//  StringSource fs(data,len, true,new Gzip(new FileSink("cert")));

  return out;
}
string Certificate::encodeBase64()
{
  byte *data = (byte*)m_cert.getData().data();
  int len=m_cert.getData().length();
  string out;
  StringSource f(data,len, true,new Gzip(new Base64Encoder(new StringSink(out))));
  return out;
}
int Certificate::getVersion()
{
  Storage::sectionType data = m_cert.readSection(0);

  if (data.first != version)
    return -1;

  if (data.second.second != 4)
    return -1;
  return Storage::char2int(data.second.first);
}
int Certificate::getSerial()
{
  Storage::sectionType data = m_cert.readSection(1);

  if (data.first != serial)
    return -1;

  if (data.second.second != 4)
    return -1;
  return Storage::char2int(data.second.first);

}
string Certificate::getIssuer()
{
  Storage::sectionType data = m_cert.readSection(4);

  if (data.first != issuer)
    return "";

  if (data.second.second < 0)
    return "";

  return string((char*)data.second.first);

}
time_t Certificate::getStartTime()
{
  Storage::sectionType data = m_cert.readSection(5);

  if (data.first != startTime)
    return -1;

  if (data.second.second != 4)
    return -1;
  return Storage::char2int(data.second.first);

}
time_t Certificate::getEndTime()
{
  Storage::sectionType data = m_cert.readSection(6);

  if (data.first != endTime)
    return -1;

  if (data.second.second != 4)
    return -1;
  return Storage::char2int(data.second.first);
}
string Certificate::getSubject()
{
  Storage::sectionType data = m_cert.readSection(7);

  if (data.first != subject)
    return "";

  if (data.second.second < 0)
    return "";

  return string((char*)data.second.first);

}
int Certificate::getPubkeyAlgorithm()
{
  Storage::sectionType data = m_cert.readSection(8);

  if (data.first != pubkeyAlgorithm)
    return -1;

  if (data.second.second != 4)
    return -1;
  return Storage::char2int(data.second.first);

}
string Certificate::getPubkey()
{
  Storage::sectionType data = m_cert.readSection(10);

  if (data.first != pubkey)
    return "";

  if (data.second.second < 0)
    return "";

  return string((char*)data.second.first);
}

#if 0
int main()
{
  Certificate s;
  s.create("henrik abelsson","123","Trent","",123);
  cout << s.encodeBase64() << endl;
  cout << "Version: "<< s.getVersion() << endl;
  cout << "..\n";
  cout << "Serial: "<< s.getSerial() << endl;
  cout << "Issuer: "<<  s.getIssuer() << endl;
  cout << "Subject: "<< s.getSubject() << endl;
  cout << "Pubkey Algorithm: "<< s.getPubkeyAlgorithm() << endl;
  cout << "Pubkey: "<< s.getPubkey() << endl;
  if (s.verify(""))
    cout << "Certificate verified!\n";
  else
    cout << "************ invalid certificate! **************\n";
}
#endif
