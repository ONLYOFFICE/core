#ifndef _PDF_WRITER_SRC_UTILS_H
#define _PDF_WRITER_SRC_UTILS_H

#include "Types.h"

#define NEEDS_ESCAPE(c)    (c < 0x21 || \
                            c > 0x7e || \
                            c == '\\' || \
                            c == '%' || \
                            c == '#' || \
                            c == '/' || \
                            c == '(' || \
                            c == ')' || \
                            c == '<' || \
                            c == '>' || \
                            c == '[' || \
                            c == ']' || \
                            c == '{' || \
                            c == '}' )  \

#define IS_WHITE_SPACE(c)   (c == 0x00 || \
                             c == 0x09 || \
                             c == 0x0A || \
                             c == 0x0C || \
                             c == 0x0D || \
                             c == 0x20 ) \


//-----------------------------------------------------------------------------------------------------
//  CRC 32
//-----------------------------------------------------------------------------------------------------
class CRC32
{

public:   

	CRC32()
	{
		const unsigned CRC_POLY = 0xEDB88320;

		for ( unsigned int i = 0; i < 256; i++ )
		{
			unsigned int r, j;
			for ( r = i, j = 8; j; j--)
				r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;

			m_pTable[i] = r;
		}

		m_nCRC32 = 0;
	}
	void Init(unsigned int nCRC = 0)
	{
		m_nCRC32 = nCRC;
	}
	void ProcessCRC(void* pData, int nLen)
	{
		const unsigned CRC_MASK = 0xD202EF8D;
		register unsigned char *sData = reinterpret_cast<unsigned char*>(pData);
		register unsigned int nCRC = m_nCRC32;

		while ( nLen-- )
		{
			nCRC = m_pTable[static_cast<unsigned char>(nCRC) ^ *sData++] ^ nCRC >> 8;
			nCRC ^= CRC_MASK;
		}

		m_nCRC32 = nCRC;
	}

protected:

	unsigned int m_pTable[256];

public:

	unsigned int m_nCRC32;   

};

namespace PdfWriter
{
	BYTE* MemCpy(BYTE* pDst, const BYTE *pSrc, unsigned int unLen);
	void  MemSet(void* pBuf, BYTE nChar, unsigned int unLen);

	BYTE* StrCpy(char* sDst, const char* sSrc, char* pEnd);
	int   StrLen(const char* sString, int nMaxLen);
	char* ItoA  (char* sDst, int nVal, char* pEnd);
	char* ItoA2 (char* sDst, unsigned int unVal, unsigned int unLen);
	int   StrCmp(const char* s1, const char* s2);
	char* FtoA  (char* sDst, double dVal, char* pEnd);

	void  UIntChangeBit(unsigned int& nValue, short nBit);
	void  UIntChangeBit2(unsigned int& nValue, short nBit);
}

#endif // _PDF_WRITER_SRC_UTILS_H

