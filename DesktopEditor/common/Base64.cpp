﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "Base64.h"

namespace NSBase64
{
	int Base64EncodeGetRequiredLength(int nSrcLen, DWORD dwFlags)
	{
		T_LONG64 nSrcLen4 = static_cast<T_LONG64>(nSrcLen)*4;
		if (nSrcLen4 > INT_MAX)
			return -1;

		int nRet = static_cast<int>(nSrcLen4/3);

		if ((dwFlags & B64_BASE64_FLAG_NOPAD) == 0)
			nRet += nSrcLen % 3;

		int nCRLFs = nRet / 76 + 1;
		int nOnLastLine = nRet % 76;

		if (nOnLastLine)
		{
			if (nOnLastLine % 4)
				nRet += 4-(nOnLastLine % 4);
		}

		nCRLFs *= 2;

		if ((dwFlags & B64_BASE64_FLAG_NOCRLF) == 0)
			nRet += nCRLFs;

		return nRet;
	}

	int Base64DecodeGetRequiredLength(int nSrcLen)
	{
		return nSrcLen;
	}

	int Base64Encode(const BYTE *pbSrcData, int nSrcLen, BYTE* szDest, int *pnDestLen, DWORD dwFlags)
	{
		static const char s_chBase64EncodingTable[64] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
			'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',	'h',
			'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
			'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

		if (!pbSrcData || !szDest || !pnDestLen)
			return FALSE;

		if (*pnDestLen < Base64EncodeGetRequiredLength(nSrcLen, dwFlags))
			return FALSE;

		int nWritten( 0 );
		int nLen1( (nSrcLen/3)*4 );
		int nLen2( nLen1/76 );
		int nLen3( 19 );

		for (int i=0; i<=nLen2; i++)
		{
			if (i==nLen2)
				nLen3 = (nLen1%76)/4;

			for (int j=0; j<nLen3; j++)
			{
				UINT dwCurr(0);
				for (int n=0; n<3; n++)
				{
					dwCurr |= *pbSrcData++;
					dwCurr <<= 8;
				}
				for (int k=0; k<4; k++)
				{
					BYTE b = (BYTE)(dwCurr>>26);
					*szDest++ = s_chBase64EncodingTable[b];
					dwCurr <<= 6;
				}
			}
			nWritten+= nLen3*4;

			if ((dwFlags & B64_BASE64_FLAG_NOCRLF)==0)
			{
				*szDest++ = '\r';
				*szDest++ = '\n';
				nWritten+= 2;
			}
		}

		if (nWritten && (dwFlags & B64_BASE64_FLAG_NOCRLF)==0)
		{
			szDest-= 2;
			nWritten -= 2;
		}

		nLen2 = (nSrcLen%3) ? (nSrcLen%3 + 1) : 0;
		if (nLen2)
		{
			UINT dwCurr(0);
			for (int n=0; n<3; n++)
			{
				if (n<(nSrcLen%3))
					dwCurr |= *pbSrcData++;
				dwCurr <<= 8;
			}
			for (int k=0; k<nLen2; k++)
			{
				BYTE b = (BYTE)(dwCurr>>26);
				*szDest++ = s_chBase64EncodingTable[b];
				dwCurr <<= 6;
			}
			nWritten+= nLen2;
			if ((dwFlags & B64_BASE64_FLAG_NOPAD)==0)
			{
				nLen3 = nLen2 ? 4-nLen2 : 0;
				for (int j=0; j<nLen3; j++)
				{
					*szDest++ = '=';
				}
				nWritten+= nLen3;
			}
		}

		*pnDestLen = nWritten;
		return TRUE;
	}

	int DecodeBase64Char(unsigned int ch)
	{
		// returns -1 if the character is invalid
		// or should be skipped
		// otherwise, returns the 6-bit code for the character
		// from the encoding table
		if (ch >= 'A' && ch <= 'Z')
			return ch - 'A' + 0;	// 0 range starts at 'A'
		if (ch >= 'a' && ch <= 'z')
			return ch - 'a' + 26;	// 26 range starts at 'a'
		if (ch >= '0' && ch <= '9')
			return ch - '0' + 52;	// 52 range starts at '0'
		if (ch == '+')
			return 62;
		if (ch == '/')
			return 63;
		return -1;
	}

	template<class T>
	int Base64DecodeBase(const T* szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)
	{
		// walk the source buffer
		// each four character sequence is converted to 3 bytes
		// CRLFs and =, and any characters not in the encoding table
		// are skiped

		if (szSrc == NULL || pnDestLen == NULL)
			return FALSE;
		
		const T* szSrcEnd = szSrc + nSrcLen;
		int nWritten = 0;
		
		INT bOverflow = (pbDest == NULL) ? TRUE : FALSE;
		
		while (szSrc < szSrcEnd &&(*szSrc) != 0)
		{
			DWORD dwCurr = 0;
			int i;
			int nBits = 0;
			for (i=0; i<4; i++)
			{
				if (szSrc >= szSrcEnd)
					break;
				int nCh = DecodeBase64Char((unsigned int)*szSrc);
				szSrc++;
				if (nCh == -1)
				{
					// skip this char
					i--;
					continue;
				}
				dwCurr <<= 6;
				dwCurr |= nCh;
				nBits += 6;
			}

			if(!bOverflow && nWritten + (nBits/8) > (*pnDestLen))
				bOverflow = TRUE;

			// dwCurr has the 3 bytes to write to the output buffer
			// left to right
			dwCurr <<= 24-nBits;
			for (i=0; i<nBits/8; i++)
			{
				if(!bOverflow)
				{
					*pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
					pbDest++;
				}
				dwCurr <<= 8;
				nWritten++;
			}

		}
		
		*pnDestLen = nWritten;
		
		if(bOverflow)
		{
			// if (pbDest != NULL) ATLASSERT(FALSE);

			return FALSE;
		}
		
		return TRUE;
	}

	int Base64Decode(const char* szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)
	{
		return Base64DecodeBase(szSrc, nSrcLen, pbDest, pnDestLen);
	}
	int Base64Decode(const wchar_t* szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)
	{
		return Base64DecodeBase(szSrc, nSrcLen, pbDest, pnDestLen);
	}
}

#include <cstring>
namespace NSBase32
{
	const unsigned char PADDING_CHAR_32 = '=';

	inline void pad(unsigned char* buf, int len)
	{
		for (int i = 0; i < len; i++)
			buf[i] = PADDING_CHAR_32;
	}
	inline unsigned char shift_right(unsigned char byte, signed char offset)
	{
		if (offset > 0)
			return byte >>  offset;
		else
			return byte << -offset;
	}
	inline unsigned char shift_left(unsigned char byte, signed char offset)
	{
		return shift_right(byte, - offset);
	}

	unsigned char encode_char(unsigned char c)
	{
		static unsigned char base32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		return base32[c & 0x1F];
	}

	int decode_char(unsigned char c)
	{
		char retval = -2;

		if (c >= 'A' && c <= 'Z')
			retval = c - 'A';
		else if (c >= '2' && c <= '7')
			retval = c - '2' + 26;
		else if (c == PADDING_CHAR_32)
			retval = -1;

		return  retval;
	}

	int decode_sequence(const unsigned char* coded, unsigned char* plain)
	{
		plain[0] = 0;
		for (int block = 0; block < 8; block++)
		{
			int offset = (8 - 5 - (5*block) % 8);
			int octet = (block*5) / 8;

			int c = decode_char(coded[block]);
			if (c < 0)
				return c;

			plain[octet] |= shift_left(c, offset);
			if (offset < 0)
			{  // does this block overflows to next octet?
				plain[octet+1] = shift_left(c, 8 + offset);
			}
		}
		return 5;
	}

	bool Decode(unsigned char* in, int inLen, unsigned char* out)
	{
		for (size_t i = 0, j = 0; (i + 8) <= inLen; i += 8, j += 5)
		{
			int n = decode_sequence(&in[i], &out[j]);
			if (n == -2)
				return false;
			if (n < 5)
				break;
		}
		return true;
	}

	void encode_sequence(const unsigned char* plain, int len, unsigned char* coded)
	{
		for (int block = 0; block < 8; block++)
		{
			int octet = (block*5) / 8;
			int junk = (8 - 5 - (5*block) % 8);

			if (octet >= len)
			{
				pad(&coded[block], 8 - block);
				return;
			}

			unsigned char c = shift_right(plain[octet], junk);  // first part

			if (junk < 0  // is there a second part?
				&&  octet < len - 1)  // is there still something to read?
			{
				c |= shift_right(plain[octet+1], 8 + junk);
			}
			coded[block] = encode_char(c);
		}
	}

	bool Encode(unsigned char* src, int len, unsigned char* dst)
	{
		for (int i = 0, j = 0; i < len; i += 5, j += 8)
		{
			int tmpLen = len - i;
			encode_sequence(&src[i], tmpLen > 5 ? 5 : tmpLen, &dst[j]);
		}
		return true;
	}

	int  DecodeGetRequiredLength(int bytes)
	{
		return (((bytes)/8)*5);
	}
	int  EncodeGetRequiredLength(int bytes)
	{
		return (((bytes)/5)*8 + ((bytes) % 5 ? 8 : 0));
	}
}
