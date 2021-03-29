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
#ifndef BASE64_H_DEFINE
#define BASE64_H_DEFINE

#pragma once

#include <stdio.h>

#pragma pack(push, 8)

namespace Base64
{
    const int B64_BASE64_FLAG_NONE		= 0;
    const int B64_BASE64_FLAG_NOPAD		= 1;
    const int B64_BASE64_FLAG_NOCRLF	= 2;
    const int B64_BASE64_INT_MAX		= 2147483647;    /* maximum (signed) int value */

    inline int Base64EncodeGetRequiredLength(int nSrcLen, DWORD dwFlags = B64_BASE64_FLAG_NONE)
    {
        __int64 nSrcLen4 = static_cast<__int64>(nSrcLen)*4;
        if (nSrcLen4 > B64_BASE64_INT_MAX)
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

    inline int Base64DecodeGetRequiredLength(int nSrcLen) throw()
    {
        return nSrcLen;
    }

    inline bool Base64Encode(const BYTE *pbSrcData, int nSrcLen, char* szDest, int *pnDestLen, DWORD dwFlags = B64_BASE64_FLAG_NONE) throw()
    {
        static const char s_chBase64EncodingTable[64] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',	'h',
            'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
            'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

        if (!pbSrcData || !szDest || !pnDestLen)
            return false;

        if (*pnDestLen < Base64EncodeGetRequiredLength(nSrcLen, dwFlags))
            return false;

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
                DWORD dwCurr(0);
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
        if (nLen2 != 0)
        {
            DWORD dwCurr(0);
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
                nLen3 = (nLen2 != 0) ? 4 - nLen2 : 0;
                for (int j=0; j<nLen3; j++)
                {
                    *szDest++ = '=';
                }
                nWritten+= nLen3;
            }
        }

        *pnDestLen = nWritten;
        return true;
    }

    inline int DecodeBase64Char(unsigned int ch) throw()
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

    inline bool Base64Decode(const char* szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen) throw()
    {
        // walk the source buffer
        // each four character sequence is converted to 3 bytes
        // CRLFs and =, and any characters not in the encoding table
        // are skiped

        if (szSrc == NULL || pnDestLen == NULL)
            return false;

        const char* szSrcEnd = szSrc + nSrcLen;
        int nWritten = 0;

        bool bOverflow = (pbDest == NULL) ? true : false;

        while (szSrc < szSrcEnd && (*szSrc) != 0)
        {
            DWORD dwCurr = 0;
            int i;
            int nBits = 0;
            for (i=0; i<4; i++)
            {
                if (szSrc >= szSrcEnd)
                    break;
                int nCh = DecodeBase64Char(*szSrc);
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
                bOverflow = true;

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

            return false;
        }

        return true;
    }
}

#pragma pack(pop)

namespace Base64_1
{
#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))
#define BASE64_SIZE(x)  (((x)+2) / 3 * 4 + 1)

static const BYTE map2[] =
{
    0x3e, 0xff, 0xff, 0xff, 0x3f, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
    0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33
};
static bool Base64Decode(const char* szSrc, int nSrcLen, BYTE *& pbDest, int *pnDestLen)
{
	const char *in = szSrc;
	BYTE *out = pbDest;
	int out_size = *pnDestLen;

    int i, v;
    BYTE *dst = out;

    v = 0;
    for (i = 0; in[i] && in[i] != '='; i++)
    {
        unsigned int index= in[i]-43;
        if (index>=FF_ARRAY_ELEMS(map2) || map2[index] == 0xff)
            return false; ///???? было -1
        v = (v << 6) + map2[index];
        if (i & 3) {
            if (dst - out < out_size) {
                *dst++ = v >> (6 - 2 * (i & 3));
            }
        }
    }

    return true;
}

static bool Base64Encode(const BYTE *pbSrcData, int nSrcLen, BYTE *& szDest, int *pnDestLen)
{
	const BYTE *in = pbSrcData;
	int in_size = nSrcLen;
	char *out = (char *)szDest;
	int out_size = *pnDestLen;

    static const char b64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *ret, *dst;
    unsigned i_bits = 0;
    int i_shift = 0;
    int bytes_remaining = in_size;

	int out_size_1 =BASE64_SIZE(in_size);

    //if (/*in_size >= UINT_MAX / 4 ||*/out_size < BASE64_SIZE(in_size))
    //    return false;
   
	ret = dst = out;
    while (bytes_remaining) {
        i_bits = (i_bits << 8) + *in++;
        bytes_remaining--;
        i_shift += 8;

        do {
            *dst++ = b64[(i_bits << 6 >> i_shift) & 0x3f];
            i_shift -= 6;
        } while (i_shift > 6 || (bytes_remaining == 0 && i_shift > 0));
    }
    while ((dst - ret) & 3)
        *dst++ = '=';
    *dst = '\0';

    return true;
}

}
#endif//BASE64_H_DEFINE
