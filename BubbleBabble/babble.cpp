// BubbleBabble implementation, written and
// placed in the public domain by denis bider


#include "babble.h"


NAMESPACE_BEGIN(CryptoPP)


static char const* const c_zVowels = "aeiouy";
static char const* const c_zConsonants = "bcdfghklmnprstvzx";


BubbleBabbleEncoder::BubbleBabbleEncoder(BufferedTransformation* pQ)
:
	Filter(pQ),
	m_started(false),
	m_haveOddByte(false),
	m_checksum(1)
{
}


void BubbleBabbleEncoder::Put(byte const* p, unsigned int n)
{
	while (n--)
	{
		if (!m_haveOddByte)
		{
			m_oddByte = *p++;
			m_haveOddByte = true;
		}
		else
		{
			BufferedTransformation& out = *AttachedTransformation();

			unsigned int oddByte = m_oddByte;
			unsigned int evenByte = *p++;

			m_haveOddByte = false;

			if (!m_started)
			{
				out.Put('x');
				m_started = true;
			}

			out.Put(c_zVowels[(((oddByte >> 6) & 3) + m_checksum) % 6]);
			out.Put(c_zConsonants[(oddByte >> 2) & 15]);
			out.Put(c_zVowels[((oddByte & 3) + (m_checksum / 6)) % 6]);
			out.Put(c_zConsonants[(evenByte >> 4) & 15]);
			out.Put('-');
			out.Put(c_zConsonants[evenByte & 15]);

			m_checksum = ((5*m_checksum) + (7*oddByte) + evenByte) % 36;
		}
	}
}


void BubbleBabbleEncoder::MessageEnd(int nPropagation)
{
	BufferedTransformation& out = *AttachedTransformation();

	if (!m_started)
		out.Put('x');

	if (m_haveOddByte)
	{
		unsigned int oddByte = m_oddByte;
		out.Put(c_zVowels[(((oddByte >> 6) & 3) + m_checksum) % 6]);
		out.Put(c_zConsonants[(oddByte >> 2) & 15]);
		out.Put(c_zVowels[((oddByte & 3) + (m_checksum / 6)) % 6]);
	}
	else
	{
		out.Put(c_zVowels[m_checksum % 6]);
		out.Put('x');
		out.Put(c_zVowels[m_checksum / 6]);
	}

	out.Put('x');

	m_started = false;
	m_haveOddByte = false;
	m_checksum = 1;

	if (nPropagation)
		out.MessageEnd(nPropagation - 1);
}


NAMESPACE_END

