#ifndef __WSTRING_H__
#define __WSTRING_H__

#include <string>

#ifdef HAVE_CRYPTO
#include "crypto/misc.h"
using namespace CryptoPP;
#endif

using namespace std;
class WString
{
public:

	WString();
	WString(const char * str);
	WString(const wchar_t * wstr);

#ifdef HAVE_CRYPTO
	WString(const SecByteBlock & secstr);
#endif

private:

#ifdef HAVE_CRYPTO 
	basic_string<wchar_t> m_data;
#else
	SecByteBlock m_data;
#endif
};

#endif
