/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
                            c == '}')  \

#define NEEDS_ESCAPE_DICTVALUE(c) (c != 0x9 && \
                            c != 0xA && \
                           (c < 0x20 || \
                            c > 0x7e || \
                            c == '\\' || \
                            c == '%' || \
                            c == '#' || \
							c == '/' ))  \

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

