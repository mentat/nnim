#ifndef GM_BUFFER_H
#define GM_BUFFER_H

#include <string>
#include <vector>
#include "boost/smart_ptr.hpp"


using namespace std;
using namespace boost;

template <class T>
class Buffer: public vector<T>
{
public:

	Buffer():vector<T>() {}

	Buffer(const string& str):vector<T>() {
		resize(str.length());
		for_each(str.begin(), str.end(), (T)push_back);
	}

	Buffer(const char * str, unsigned long length):vector<T>()
	{
		resize(length);
		for ( unsigned int i = 0; i < length; i++)
			push_back(str[i]);
		
	}

	Buffer(const Buffer& buf):vector<T>()
	{
		resize(buf.size());
		insert(end(), buf.begin(), buf.end());
	}

	Buffer(const unsigned char *):vector<T>();

	const T * data() { 
		m_data.reset( new T[size()] );
		for (unsigned long i = 0; i < Size(); i++)
			m_data[i] = [i];
		return m_data.get(); }

	Buffer operator +( const Buffer& buf)
	{
		Buffer tmp;
		tmp.resize(buf.size());
		tmp.insert(tmp.end(), buf.begin(), buf.end());
		return tmp;
	}

	Buffer& operator =( const Buffer& buf)
	{
		resize(buf.size());
		insert(end(), buf.begin(), buf.end());
	}




private:

	shared_array<T> m_data;

};





#endif
