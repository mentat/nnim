// -*- C++ -*-
/*
    $Id: gmException.h,v 1.2 2002/06/23 18:35:51 thementat Exp $

    GNU Messenger - The secure instant messenger
    Copyright (C) 1999-2002  Jesse Lovelace <jllovela@eos.ncsu.edu>

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
