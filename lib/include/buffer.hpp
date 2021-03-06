// -*- C++ -*-
/*
    $Id: buffer.hpp,v 1.4 2002/06/25 01:36:03 thementat Exp $

    vbuf Class - Extention of STL vector
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

#ifndef GM_BUFFER_H
#define GM_BUFFER_H

#include <string>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "crypto/misc.h"

using namespace std;
using namespace boost;
using namespace CryptoPP;

//template <class T>
class vbuf: public vector<byte>
{
public:

	vbuf():vector<byte>() {}

	vbuf(const string& str):vector<byte>() {
		for ( unsigned int i = 0; i < str.length(); i++)
			push_back((byte)str[i]);
	}

	vbuf(const char * str, unsigned long length):vector<byte>()
	{
		for ( unsigned int i = 0; i < length; i++)
			push_back((byte)str[i]);
		
	}

	vbuf(const vbuf& buf):vector<byte>()
	{
		insert(end(), buf.begin(), buf.end());
	}

	vbuf(byte * str, unsigned long length):vector<byte>()
    {
		for ( unsigned int i = 0; i < length; i++)
			push_back(str[i]);
		
	}

	byte * data() const {
		vector<byte>::const_iterator it = begin();
		m_data.reset( new byte[size()] );
		for (unsigned long i = 0; i < size(); i++)
		{
			m_data[i] = *it;
			it++;
		}
		return m_data.get(); }

	vbuf operator +( const vbuf& buf)
	{
		vbuf tmp(*this);
		tmp.insert(tmp.end(), buf.begin(), buf.end());
		return tmp;
	}

	vbuf& operator =( const vbuf& buf)
	{
        clear();
		insert(end(), buf.begin(), buf.end());
        return *this;
	}

    vbuf& operator += ( const vbuf& buf )
    {
        insert(end(), buf.begin(), buf.end());
        return *this;
    }

    vbuf& operator += ( const byte& b )
    {
        push_back(b);
        return *this;
    }

    vbuf sub( unsigned long start, unsigned long length = 0) const
    {
        vbuf temp;
        unsigned long i;

        if ((start >= size()) || (length > size()))
            return temp;

        vector<byte>::const_iterator itStart = begin();
        if (start != 0)
            for (i = 0; i < start; i++)
                itStart++;

        vector<byte>::const_iterator itEnd = itStart;
        if (length == 0)
			itEnd = end();
		else
            for (i = 0; i < length; i++)
                itEnd++;

        temp.insert(temp.begin(), itStart, itEnd);

        return temp;
    }



private:

	mutable scoped_array<byte> m_data;

};

//typedef Buffer<byte> vbuf;



#endif
