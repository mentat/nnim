// --*-c++*--
// $Id: sign.h,v 1.1 2002/06/06 17:21:53 thementat Exp $

#ifndef KIM_SIGN_H
#define KIM_SIGN_H

/**
   A GM certificate (Loosely modeled after X.509)
*/

#include "dstring.h"
#include "gmstorage.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#define CERT_VERSION (unsigned int)1
#define HASH_ALGORITHM_SHA (unsigned int)1
#define HASH_PARAM (unsigned int)0
#define PUBKEY_ALGORITHM_RSA (unsigned int)1

class Certificate
{
public:
  enum{Hex,Base64};
  Certificate();
  Certificate(dstring cert);
  Certificate(byte *cert,int len);
  Certificate(const string &cert,int encoding);
  ~Certificate();
  dstring create(const string &_subject,
							const string &_pubkey,
							const string &_issuer,
							const string &_privkey,
							const string &_password,
							const int _serial);

  static void generateKey(int length,string &privkey,string &pubkey,const string &password);
  bool   verify(const string &pubkey);
  string encodeHex();
  string encodeBase64();
  int    getVersion();
  int    getSerial();
  string getIssuer();
  time_t getStartTime();
  time_t getEndTime();
  string getSubject();
  int    getPubkeyAlgorithm();
  string getPubkey();

private:
  enum {
    version,
    serial,
    hashAlgorithm,
    hashAlgorithmParam,
    issuer,
    startTime,
    endTime,
    subject,
    pubkeyAlgorithm,
    pubkeyParam,
    pubkey,
    signature,
  };
  Storage m_cert;
};

#endif
