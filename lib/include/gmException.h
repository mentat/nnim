#ifndef __GM_EXCEPTION_H__
#define __GM_EXCEPTION_H__

#include <string>

using namespace std;

class gmException
{
public:
	enum { gDEBUG = 0, gFATAL, gDISK, gMEM, gUSER};

	gmException(): m_type(gUSER) {}
	gmException(const gmException& e) { m_what = e.what(); m_type = e.whatType(); }
	gmException(const string &s, int type) : m_what(s), m_type(type) {}
	gmException(int type) : m_type(type) {}

	void setWhat(const char * msg) { m_what = msg; }
	void setType(int type) { m_type = type; }

	const char *what() const { return m_what.c_str(); }
	int whatType() const { return m_type; };

private:
	string m_what;
	int m_type;
};

#endif
