// BubbleBabble implementation, written and
// placed in the public domain by denis bider


#ifndef BABBLE_H_INCLUDED
#define BABBLE_H_INCLUDED


#include "filters.h"


NAMESPACE_BEGIN(CryptoPP)


class BubbleBabbleEncoder : public Filter
{
public:
	BubbleBabbleEncoder(BufferedTransformation* pQ = 0);

	void Put(byte b) { Put(&b, 1); }
	void Put(byte const* p, unsigned int n);

	void MessageEnd(int nPropagation = -1);

protected:
	bool m_started;
	bool m_haveOddByte;
	byte m_oddByte;
	unsigned int m_checksum;
};


NAMESPACE_END


#endif
