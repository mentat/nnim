#include <cryptlib.h>
#include <rsa.h>
#include <hex.h>
#include <base64.h>
#include <randpool.h>
#include <osrng.h>
#include <gzip.h>
#include <sha.h>
#include <files.h>
#include <rng.h>

#include <log.h>
#include <sign.h>
#include <securecomm.h>

using namespace CryptoPP;


/*
   alice -> bob
   bob -> alice
   alice -> bob
*/


CryptoSession::CryptoSession()
{
  status=None;
  sessionKeyLen=0;
  sessionKey=0;
}
CryptoSession::~CryptoSession()
{

}
// Alice the initiator
string CryptoSession::authenticate()
{

  AutoSeededRandomPool rng(false,128);

  /* Alice validates bobs certificate */
  peer.verify("");

  sessionKeyLen=16;

  /* Alice generates a msg with a random string, bobs identity, and a session key */
  Storage msg;
  byte random[16];
  sessionKey=new byte[sessionKeyLen];
  rng.GenerateBlock(random,16);
  rng.GenerateBlock(sessionKey,16);

  cout << "Random data: ";
  StringSource(random,16,true,new HexEncoder(new FileSink(cout)));
  cout <<"\nSessionkey : ";
  StringSource(sessionKey,sessionKeyLen,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  msg.addSection(IDENTITY,peer.getSubject());
  msg.addSection(SESSIONKEY,sessionKeyLen,sessionKey);
  msg.addSection(RANDOMDATA,16,random);

  /* Alice encrypts the msg with Bobs public key */
  StringSource bobskey(peer.getPubkey(),true,new HexDecoder);
  RSAES_OAEP_SHA_Encryptor spub(bobskey);
  if (msg.getData().size() > spub.MaxPlainTextLength())
  {
    cerr << "message too long for this key\n";
    debug() << "Bleh!!" << endl;
  }
  byte *bobsMsg = new byte[2*spub.CipherTextLength()+1];
  spub.Encrypt(rng,msg.getData().data(),msg.getData().size(),bobsMsg);

  /* Alice sends Bob her certificate and the msg signed with her private key */
  StringSource privKey(privKey,true,new HexDecoder);
  RSASSA_PKCS1v15_SHA_Signer priv(privKey);

  StringSource f(bobsMsg,spub.CipherTextLength(),true,new SignerFilter(rng,priv));
  int len=f.MaxRetrievable();
  byte *request = new byte[len];
  f.Get(request,len);

  string myCertificate = mycert.encodeHex();

  cout << myCertificate << endl;

  StringSource acert(myCertificate,true,new HexDecoder(new Gunzip));
  int len2 = acert.MaxRetrievable();
  byte *b_acert = new byte[len2];
  acert.Get(b_acert,len2);

  cout <<"Data : ";
  StringSource(bobsMsg,spub.CipherTextLength(),true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  Storage r;
  r.addSection(CERTIFICATE,len2,b_acert);
  r.addSection(MSG,spub.CipherTextLength(),bobsMsg);
  r.addSection(SIGNATURE,len,request);

  string req;
  StringSource(r.getData().data(),r.getData().size(),true,new Gzip(new Base64Encoder(new StringSink(req),false)));
  return req;
}

// Bob
void CryptoSession::parseAuthenticateReply(const string &alicePacket)
{

  AutoSeededRandomPool rng(false,128);

  int len;
  Storage::sectionType s;

  StringSource pct(alicePacket,true,new Base64Decoder(new Gunzip));
  len = pct.MaxRetrievable();
  byte *tmp = new byte[len];
  pct.Get(tmp,len);
  Storage r;
  r.setData(len,tmp);

  s=r.readSection(0);
  if (s.first != CERTIFICATE)
  {
    cout << "Corrupt packet\n";
    debug() << "Bleh!!" << endl;
  }
  /* Bob verifies the validity of the certificate */
  Certificate c(s.second.first,s.second.second);
  cout << "Version: "<< c.getVersion() << endl;
  cout << "Serial: "<< c.getSerial() << endl;
  cout << "Issuer: "<<  c.getIssuer() << endl;
  cout << "Subject: "<< c.getSubject() << endl;
  cout << "Pubkey Algorithm: "<< c.getPubkeyAlgorithm() << endl;
  cout << "Pubkey: "<< c.getPubkey() << endl;
  if (c.verify(""))
    cout << "Certificate verified!\n";
  else
    cout << "************ invalid certificate! **************\n";

  peer = c;

  /* Bob verifies her signature */
  s=r.readSection(1);
  Storage::sectionType s2 = r.readSection(2);

  StringSource pubFile(c.getPubkey().c_str(),true,new HexDecoder);
  RSASSA_PKCS1v15_SHA_Verifier pub(pubFile);

  if ((unsigned)s2.second.second!=pub.SignatureLength())
  {
    cout << "Alices signature is invalid\n";
    debug() << "Bleh!!" << endl;
  }
  SecByteBlock signature(pub.SignatureLength());
  signature.Assign(s2.second.first,s2.second.second);

  VerifierFilter *verifierFilter = new VerifierFilter(pub);
  verifierFilter->PutSignature(signature);
  StringSource f(s.second.first,s.second.second, true, verifierFilter);

  byte result = 0;
  f.Get(result);
  if (result != 1)
  {
    cout << "Alices signature is invalid\n";
    debug() << "Bleh!!" << endl;
  }

  /* Bob now decrypts the signed packet */
  cout <<"Data : ";
  StringSource(s.second.first,s.second.second,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  StringSource privFile(privKey, true, new HexDecoder);
  RSAES_OAEP_SHA_Decryptor priv(privFile);
  SecByteBlock buf;
  buf.Assign(s.second.first,s.second.second);
  byte *outstr = new byte[priv.MaxPlainTextLength()+1];
  unsigned messageLength = priv.Decrypt(buf,outstr);

  cout << messageLength << " " << s.second.second<< endl;

  Storage msg;
  msg.setData(messageLength,outstr);

  s=msg.readSection(0);
  cout << s.first << endl;
  if (s.first != IDENTITY)
  {
    cout << "you suck too much!\n";
    debug() << "Bleh!!" << endl;
  }
  cout << s.second.first << endl;

  s=msg.readSection(1);
  if (s.first != SESSIONKEY)
  {
    cout << "you suck too much ass!\n";
    debug() << "Bleh!!" << endl;
  }
  sessionKeyLen=s.second.second;
  sessionKey=new byte[sessionKeyLen];
  memcpy(sessionKey,s.second.first,sessionKeyLen);

  cout <<"\nSessionkey : ";
  StringSource(sessionKey,sessionKeyLen,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  s = msg.readSection(2);
  cout << s.first << endl;
  if (s.first != RANDOMDATA ||s.second.second != 16)
  {
    cout << "you suck too much ass!\n";
    debug() << "Bleh!!" << endl;
  }
  memcpy(random,s.second.first,16);
}

/* Bob generates a packet identical to Alices except with a new random string and a
   new session key. The old random string is included. */
string CryptoSession::authenticate2()
{
  AutoSeededRandomPool rng(false, 128);
  Storage msg;

  sessionKey2Len=16;
  sessionKey2=new byte[sessionKey2Len];
  rng.GenerateBlock(random2,16);
  rng.GenerateBlock(sessionKey2,sessionKey2Len);

  finalKeyLen=16;
  finalKey=new byte[finalKeyLen];
  for (int i=0;i<16;i++)
    finalKey[i] = sessionKey[i] ^ sessionKey2[i];

  cout << "Random data: ";
  StringSource(random2,16,true,new HexEncoder(new FileSink(cout)));
  cout <<"\nSessionkey : ";
  StringSource(sessionKey2,sessionKey2Len,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  msg.addSection(IDENTITY,peer.getSubject());
  msg.addSection(RANDOMDATA,16,random);
  msg.addSection(SESSIONKEY2,sessionKey2Len,sessionKey2);
  msg.addSection(RANDOMDATA2,16,random2);

  /* The msg is encrypted with Alices public key */
  cout << "Alices key:" << peer.getPubkey() << endl;
  StringSource aliceskey(peer.getPubkey(),true,new HexDecoder);
  //FileSource aliceskey("alice.pub", true, new HexDecoder);
  RSAES_OAEP_SHA_Encryptor spub(aliceskey);
  if (msg.getData().size() > spub.MaxPlainTextLength())
  {
    cerr << "message too long for this key\n";
    debug() << "Bleh!!" << endl;
  }
  byte *alicesMsg = new byte[2*spub.CipherTextLength()+1];
  spub.Encrypt(rng,msg.getData().data(),msg.getData().size(),alicesMsg);

  /* The msg is signed with Bobs private key */
  StringSource privKey(privKey,true,new HexDecoder);
  RSASSA_PKCS1v15_SHA_Signer priv(privKey);

  StringSource f(alicesMsg,spub.CipherTextLength(),true,new SignerFilter(rng,priv));
  int len=f.MaxRetrievable();
  byte *request = new byte[len];
  f.Get(request,len);

  cout <<"Data : ";
  StringSource(alicesMsg,spub.CipherTextLength(),true,new HexEncoder(new FileSink(cout)));
  cout << "\n\n";

  Storage r;
  r.addSection(MSG,spub.CipherTextLength(),alicesMsg);
  r.addSection(SIGNATURE,len,request);

  string req;
  StringSource(r.getData().data(),r.getData().size(),true,new Gzip(new Base64Encoder(new StringSink(req),false)));
  return req;

}


void CryptoSession::parseAuthenticate2Reply(const string &bobPacket)
{
  /* Alice verifies that the random string she sent bob is included in bobs msg */
  AutoSeededRandomPool rng(false,128);

  int len;
  Storage::sectionType s;

  StringSource pct(bobPacket,true,new Base64Decoder(new Gunzip));
  len = pct.MaxRetrievable();
  byte *tmp = new byte[len];
  pct.Get(tmp,len);
  Storage r;
  r.setData(len,tmp);

  s=r.readSection(0);
  Storage::sectionType s2 = r.readSection(1);

  /* verify signature */
  StringSource pubFile(peer.getPubkey().c_str(),true,new HexDecoder);
  RSASSA_PKCS1v15_SHA_Verifier pub(pubFile);

  if ((unsigned)s2.second.second!=pub.SignatureLength())
  {
    cout << "you puny earthling!\n";
    debug() << "Bleh!!" << endl;
  }
  SecByteBlock signature(pub.SignatureLength());
  signature.Assign(s2.second.first,s2.second.second);

  VerifierFilter *verifierFilter = new VerifierFilter(pub);
  verifierFilter->PutSignature(signature);
  StringSource f(s.second.first,s.second.second, true, verifierFilter);

  byte result = 0;
  f.Get(result);
  if (result != 1)
  {
    cout << "you deserve a horrible pimple on your nose\n";
    debug() << "Bleh!!" << endl;
  }
  else
    cout << "Signature verified\n";
  /* Alice now decrypts the signed packet */
  cout <<"Data : ";
  StringSource(s.second.first,s.second.second,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  StringSource privFile(privKey, true, new HexDecoder);
  RSAES_OAEP_SHA_Decryptor priv(privFile);
  SecByteBlock buf;
  buf.Assign(s.second.first,s.second.second);
  byte *outstr = new byte[priv.MaxPlainTextLength()+1];
  unsigned messageLength = priv.Decrypt(buf,outstr);

  cout << messageLength << " " << s.second.second<< endl;

  Storage msg;
  msg.setData(messageLength,outstr);

  s=msg.readSection(0);
  cout << s.first << endl;
  if (s.first != IDENTITY)
  {
    cout << "you suck way too much!\n";
    debug() << "Bleh!!" << endl;
  }
  cout << s.second.first << endl;

  //  byte *sessionKey;
  //int sessionKeyLen;
  s=msg.readSection(1);
  if (s.first != RANDOMDATA)
  {
    cout << "you suck too much ass!\n";
    debug() << "Bleh!!" << endl;
  }
  int randomDataLen=s.second.second;
  byte *randomData=new byte[randomDataLen];
  memcpy(randomData,s.second.first,randomDataLen);

  cout <<"\nRandom data we sent: ";
  StringSource(randomData,randomDataLen,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  // assert this str is equal to the random string we sent earlier.

  s = msg.readSection(2);
  if (s.first != SESSIONKEY2)
  {
    cout << "you suck way too much ass!\n";
    debug() << "Bleh!!" << endl;
  }
  sessionKey2Len=s.second.second;
  sessionKey2=new byte[sessionKey2Len];
  memcpy(sessionKey2,s.second.first,sessionKey2Len);

  cout <<"Sessionkey2: ";
  StringSource(sessionKey2,sessionKey2Len,true,new HexEncoder(new FileSink(cout)));
  cout << "\n";

  finalKeyLen=16;
  finalKey=new byte[finalKeyLen];
  for (int i=0;i<16;i++)
    finalKey[i] = sessionKey[i] ^ sessionKey2[i];

  s = msg.readSection(3);
  if (s.first != RANDOMDATA2)
  {
    cout << "you suck way too much donkey ass!\n";
    debug() << "Bleh!!" << endl;
  }
  cout << s.second.second << endl;
  int random2DataLen=s.second.second;
  byte *random2Data=new byte[random2DataLen];
  memcpy(random2Data,s.second.first,random2DataLen);

  cout <<"Random data we got from Bob: " << random2DataLen << endl;
  StringSource(random2Data,random2DataLen,true,new HexEncoder(new FileSink(cout)));
  cout << "\n\n";
  memcpy(random2,random2Data,16);
}

/* Alice signs bobs random number with her private key */

string CryptoSession::authenticate3()
{
  AutoSeededRandomPool rng;
  StringSource privateStr(privKey,true,new HexDecoder);
  RSASSA_PKCS1v15_SHA_Signer priv(privateStr);

  StringSource f(random2,16,true,new SignerFilter(rng,priv));
  int len=f.MaxRetrievable();
  byte *request = new byte[len];
  f.Get(request,len);

  string req;
  StringSource(request,len,true,new Base64Encoder(new StringSink(req),false));
  return req;
}

void CryptoSession::parseAuthenticate3Reply(const string &alicePacket)
{
  StringSource pkt(alicePacket,true,new Base64Decoder);
  int sigRandomlen=pkt.MaxRetrievable();
  byte *sigRandom=new byte[sigRandomlen];
  pkt.Get(sigRandom,sigRandomlen);

  /* Bob verifies Alices signature thereby also verifying that the random string is
     equal to the one he sent in his first packet */

  StringSource pubFile(peer.getPubkey().c_str(),true,new HexDecoder);
  RSASSA_PKCS1v15_SHA_Verifier pub(pubFile);

  if ((unsigned)sigRandomlen!=pub.SignatureLength())
  {
    cout << "you blow!\n";
    debug() << "Bleh!!" << endl;
  }
  SecByteBlock signature(pub.SignatureLength());
  signature.Assign(sigRandom,sigRandomlen);

  VerifierFilter *verifierFilter = new VerifierFilter(pub);
  verifierFilter->PutSignature(signature);
  StringSource f(random2,16, true, verifierFilter);

  byte result = 0;
  f.Get(result);
  if (result != 1)
  {
    cout << "you give blowjobs to the hairy domestic asses while standing in their yellow excrement\n";
    debug() << "Bleh!!" << endl;
  }
  else
    cout << "Woho! we've authenticated a secure connection!\n";
}

string get(const string&filename)
{
  string tmp;
  FileSource(filename.c_str(),true,new StringSink(tmp));
  return tmp;
}

#if 0
int main()
{
  string msg;
  CryptoSession alice,bob;

  alice.setPeer(Certificate(get("bob.cert"),0));
  alice.setCert(Certificate(get("alice.cert"),0));
  alice.setPrivkey(get("alice.priv"));

  bob.setPeer(Certificate(get("alice.cert"),0));
  bob.setCert(Certificate(get("bob.cert"),0));
  bob.setPrivkey(get("bob.priv"));

  cout << "\n------------------------------------- alice (i) -------------------------------\n\n";
  msg=alice.authenticate();
  cout << "\n-------------------------------------  bob  (i) -------------------------------\n\n";
  bob.parseAuthenticateReply(msg);

  cout << "\n---------------------------------------------------------------------------\n";
  cout << "---------------------------------------------------------------------------\n";


  cout << "\n-------------------------------------  bob (ii) -------------------------------\n\n";
  msg=bob.authenticate2();
  cout << "\n------------------------------------- alice (ii) -------------------------------\n\n";
  alice.parseAuthenticate2Reply(msg);

  cout << "\n---------------------------------------------------------------------------\n";
  cout << "---------------------------------------------------------------------------\n";


  cout << "\n------------------------------------- alice (iii) -------------------------------\n\n";
  msg=alice.authenticate3();
  cout << "\n-------------------------------------  bob  (iii) -------------------------------\n\n";
  bob.parseAuthenticate3Reply(msg);
  cout << "Step three complete..\n";
  cout <<"Alice key: ";
  StringSource(alice.sessionKey,alice.sessionKeyLen,true,new HexEncoder(new FileSink(cout))); cout << endl << "Bob key  : ";
  StringSource(bob.sessionKey,bob.sessionKeyLen,true,new HexEncoder(new FileSink(cout)));
  cout << endl;
  cout <<"Alice key2: ";
  StringSource(alice.sessionKey2,alice.sessionKey2Len,true,new HexEncoder(new FileSink(cout))); cout << endl << "Bob key2  : ";
  StringSource(bob.sessionKey2,bob.sessionKey2Len,true,new HexEncoder(new FileSink(cout)));
  cout << endl;
  cout <<"Alice finalkey: ";
  StringSource(alice.finalKey,alice.finalKeyLen,true,new HexEncoder(new FileSink(cout))); cout << endl << "Bob finalkey  : ";
  StringSource(bob.finalKey,bob.finalKeyLen,true,new HexEncoder(new FileSink(cout)));
  cout << endl;
}
/*
int main()
{
  string asig,bsig,cert,cert2;
  cout << "Alices signatue? ";
//  cin >> asig;
  cout << "Bobs sig? ";
  //cin >> bsig;
  FileSource("bob.cert",true,new StringSink(cert));
  Certificate bob(cert,0);
  FileSource("alice.cert",true,new StringSink(cert2));
  Certificate alice(cert2,0);


  try {
    cout << "\n------------------------------------- alice -------------------------------\n\n";
    string msg= alice1(bob);
    cout << msg << endl;
    cout << "\n-------------------------------------  bob  -------------------------------\n\n";
    msg=bob1(alice,msg);
    cout << msg << endl;
    cout << "\n------------------------------------- alice -------------------------------\n\n";
    msg= alice2(bob,msg);
    cout << msg << endl;
    cout << "\n-------------------------------------  bob  -------------------------------\n\n";
    bob2(alice,msg);
  }
  catch (Exception &e)
  {
    cout << e.what() << endl;
  }
}
*/

#endif
