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
#include "digest.h"
#include <memory>
#include <string.h>

#define RTL_DIGEST_ROTL(a,n) (((a) << (n)) | ((a) >> (32 - (n))))

#define RTL_DIGEST_LTOC(l,c) \
	(*((c)++) = (unsigned char)(((l)       ) & 0xff), \
	 *((c)++) = (unsigned char)(((l) >>  8L) & 0xff), \
	 *((c)++) = (unsigned char)(((l) >> 16L) & 0xff), \
	 *((c)++) = (unsigned char)(((l) >> 24L) & 0xff))


#define F(x,y,z) ((((y) ^ (z)) & (x)) ^ (z))
#define G(x,y,z) ((((x) ^ (y)) & (z)) ^ (y))
#define H(x,y,z) ((x) ^ (y) ^ (z))
#define I(x,y,z) (((x) | (~(z))) ^ (y))

#define R0(a,b,c,d,k,s,t) { \
	a += ((k) + (t) + F((b), (c), (d))); \
	a  = RTL_DIGEST_ROTL(a, s); \
	a += b; }

#define R1(a,b,c,d,k,s,t) { \
	a += ((k) + (t) + G((b), (c), (d))); \
	a  = RTL_DIGEST_ROTL(a, s); \
	a += b; }

#define R2(a,b,c,d,k,s,t) { \
	a += ((k) + (t) + H((b), (c), (d))); \
	a  = RTL_DIGEST_ROTL(a, s); \
	a += b; }

#define R3(a,b,c,d,k,s,t) { \
	a += ((k) + (t) + I((b), (c), (d))); \
	a  = RTL_DIGEST_ROTL(a, s); \
	a += b; }


DigestMD5::DigestMD5()
{
	InitContext();
}


void DigestMD5::InitContext()
{
	memset(&m_context, 0, sizeof (DigestContextMD5));

	m_context.m_nA = (_UINT32)0x67452301L;
	m_context.m_nB = (_UINT32)0xefcdab89L;
	m_context.m_nC = (_UINT32)0x98badcfeL;
	m_context.m_nD = (_UINT32)0x10325476L;
}

void DigestMD5::UpdateContext()
{
	_UINT32  A, B, C, D;
	_UINT32 *X;

	A = m_context.m_nA;
	B = m_context.m_nB;
	C = m_context.m_nC;
	D = m_context.m_nD;
	X = m_context.m_pData;

	R0 (A, B, C, D, X[ 0],  7, 0xd76aa478L);
	R0 (D, A, B, C, X[ 1], 12, 0xe8c7b756L);
	R0 (C, D, A, B, X[ 2], 17, 0x242070dbL);
	R0 (B, C, D, A, X[ 3], 22, 0xc1bdceeeL);
	R0 (A, B, C, D, X[ 4],  7, 0xf57c0fafL);
	R0 (D, A, B, C, X[ 5], 12, 0x4787c62aL);
	R0 (C, D, A, B, X[ 6], 17, 0xa8304613L);
	R0 (B, C, D, A, X[ 7], 22, 0xfd469501L);
	R0 (A, B, C, D, X[ 8],  7, 0x698098d8L);
	R0 (D, A, B, C, X[ 9], 12, 0x8b44f7afL);
	R0 (C, D, A, B, X[10], 17, 0xffff5bb1L);
	R0 (B, C, D, A, X[11], 22, 0x895cd7beL);
	R0 (A, B, C, D, X[12],  7, 0x6b901122L);
	R0 (D, A, B, C, X[13], 12, 0xfd987193L);
	R0 (C, D, A, B, X[14], 17, 0xa679438eL);
	R0 (B, C, D, A, X[15], 22, 0x49b40821L);

	R1 (A, B, C, D, X[ 1],  5, 0xf61e2562L);
	R1 (D, A, B, C, X[ 6],  9, 0xc040b340L);
	R1 (C, D, A, B, X[11], 14, 0x265e5a51L);
	R1 (B, C, D, A, X[ 0], 20, 0xe9b6c7aaL);
	R1 (A, B, C, D, X[ 5],  5, 0xd62f105dL);
	R1 (D, A, B, C, X[10],  9, 0x02441453L);
	R1 (C, D, A, B, X[15], 14, 0xd8a1e681L);
	R1 (B, C, D, A, X[ 4], 20, 0xe7d3fbc8L);
	R1 (A, B, C, D, X[ 9],  5, 0x21e1cde6L);
	R1 (D, A, B, C, X[14],  9, 0xc33707d6L);
	R1 (C, D, A, B, X[ 3], 14, 0xf4d50d87L);
	R1 (B, C, D, A, X[ 8], 20, 0x455a14edL);
	R1 (A, B, C, D, X[13],  5, 0xa9e3e905L);
	R1 (D, A, B, C, X[ 2],  9, 0xfcefa3f8L);
	R1 (C, D, A, B, X[ 7], 14, 0x676f02d9L);
	R1 (B, C, D, A, X[12], 20, 0x8d2a4c8aL);

	R2 (A, B, C, D, X[ 5],  4, 0xfffa3942L);
	R2 (D, A, B, C, X[ 8], 11, 0x8771f681L);
	R2 (C, D, A, B, X[11], 16, 0x6d9d6122L);
	R2 (B, C, D, A, X[14], 23, 0xfde5380cL);
	R2 (A, B, C, D, X[ 1],  4, 0xa4beea44L);
	R2 (D, A, B, C, X[ 4], 11, 0x4bdecfa9L);
	R2 (C, D, A, B, X[ 7], 16, 0xf6bb4b60L);
	R2 (B, C, D, A, X[10], 23, 0xbebfbc70L);
	R2 (A, B, C, D, X[13],  4, 0x289b7ec6L);
	R2 (D, A, B, C, X[ 0], 11, 0xeaa127faL);
	R2 (C, D, A, B, X[ 3], 16, 0xd4ef3085L);
	R2 (B, C, D, A, X[ 6], 23, 0x04881d05L);
	R2 (A, B, C, D, X[ 9],  4, 0xd9d4d039L);
	R2 (D, A, B, C, X[12], 11, 0xe6db99e5L);
	R2 (C, D, A, B, X[15], 16, 0x1fa27cf8L);
	R2 (B, C, D, A, X[ 2], 23, 0xc4ac5665L);

	R3 (A, B, C, D, X[ 0],  6, 0xf4292244L);
	R3 (D, A, B, C, X[ 7], 10, 0x432aff97L);
	R3 (C, D, A, B, X[14], 15, 0xab9423a7L);
	R3 (B, C, D, A, X[ 5], 21, 0xfc93a039L);
	R3 (A, B, C, D, X[12],  6, 0x655b59c3L);
	R3 (D, A, B, C, X[ 3], 10, 0x8f0ccc92L);
	R3 (C, D, A, B, X[10], 15, 0xffeff47dL);
	R3 (B, C, D, A, X[ 1], 21, 0x85845dd1L);
	R3 (A, B, C, D, X[ 8],  6, 0x6fa87e4fL);
	R3 (D, A, B, C, X[15], 10, 0xfe2ce6e0L);
	R3 (C, D, A, B, X[ 6], 15, 0xa3014314L);
	R3 (B, C, D, A, X[13], 21, 0x4e0811a1L);
	R3 (A, B, C, D, X[ 4],  6, 0xf7537e82L);
	R3 (D, A, B, C, X[11], 10, 0xbd3af235L);
	R3 (C, D, A, B, X[ 2], 15, 0x2ad7d2bbL);
	R3 (B, C, D, A, X[ 9], 21, 0xeb86d391L);

	m_context.m_nA += A;
	m_context.m_nB += B;
	m_context.m_nC += C;
	m_context.m_nD += D;
}

void DigestMD5::EndContext()
{
	static const unsigned char end[4] =
	{
		0x80, 0x00, 0x00, 0x00
	};
	register const unsigned char *p = end;
	
	register _UINT32 *X;
	register _INT32 i;

	X = m_context.m_pData;
	i = (m_context.m_nDatLen >> 2);

	switch (m_context.m_nDatLen & 0x03)
	{
		case 1: X[i] &= 0x000000ff; break;
		case 2: X[i] &= 0x0000ffff; break;
		case 3: X[i] &= 0x00ffffff; break;
	}

	switch (m_context.m_nDatLen & 0x03)
	{
		case 0: X[i]  = ((unsigned int)(*(p++))) <<  0L;
		case 1: X[i] |= ((unsigned int)(*(p++))) <<  8L;
		case 2: X[i] |= ((unsigned int)(*(p++))) << 16L;
		case 3: X[i] |= ((unsigned int)(*(p++))) << 24L;
	}

	i += 1;

	if (i >= (DIGEST_LBLOCK_MD5 - 2))
	{
		for (; i < DIGEST_LBLOCK_MD5; i++)
			X[i] = 0;
		UpdateContext();
		i = 0;
	}

	for (; i < (DIGEST_LBLOCK_MD5 - 2); i++)
		X[i] = 0;

	X[DIGEST_LBLOCK_MD5 - 2] = m_context.m_nL;
	X[DIGEST_LBLOCK_MD5 - 1] = m_context.m_nH;

	UpdateContext();
}


DigestMD5::rtlDigestError DigestMD5::Update(const void *pData, const unsigned int nDatLen)
{
	if (nDatLen == 0)
		return rtl_Digest_E_None;

	unsigned int data_len_left = nDatLen;
	unsigned int len_calc_tmp;

	len_calc_tmp = m_context.m_nL + (data_len_left << 3);
	if (len_calc_tmp < m_context.m_nL) m_context.m_nH += 1;
	m_context.m_nH += (data_len_left >> 29);
	m_context.m_nL  = len_calc_tmp;

	const unsigned char  *d     = (const unsigned char *)pData;
	if (m_context.m_nDatLen)
	{
		unsigned char  *p = (unsigned char *)(m_context.m_pData) + m_context.m_nDatLen;
		unsigned int  n = DIGEST_CBLOCK_MD5 - m_context.m_nDatLen;

		if (data_len_left < n)
		{
			memcpy(p, d, data_len_left);
			m_context.m_nDatLen += data_len_left;

			return rtl_Digest_E_None;
		}

		memcpy (p, d, n);
		d       += n;
		data_len_left -= n;

		UpdateContext();
		m_context.m_nDatLen = 0;
	}

	while (data_len_left >= DIGEST_CBLOCK_MD5)
	{
		memcpy (m_context.m_pData, d, DIGEST_CBLOCK_MD5);
		d       += DIGEST_CBLOCK_MD5;
		data_len_left -= DIGEST_CBLOCK_MD5;

		UpdateContext();
	}

	memcpy (m_context.m_pData, d, data_len_left);
	m_context.m_nDatLen = data_len_left;

	return rtl_Digest_E_None;
}

DigestMD5::rtlDigestError DigestMD5::Get(unsigned char *pBuffer, const unsigned int nBufLen)
{
	unsigned char *p = pBuffer;

	if ((pBuffer == NULL))
		return rtl_Digest_E_Argument;

	if (!(RTL_DIGEST_LENGTH_MD5 <= nBufLen))
		return rtl_Digest_E_BufferSize;

	EndContext();
	RTL_DIGEST_LTOC (m_context.m_nA, p);
	RTL_DIGEST_LTOC (m_context.m_nB, p);
	RTL_DIGEST_LTOC (m_context.m_nC, p);
	RTL_DIGEST_LTOC (m_context.m_nD, p);
	InitContext();

	return rtl_Digest_E_None;
}


DigestMD5::rtlDigestError DigestMD5::Raw(unsigned char *pBuffer, const unsigned int nBufLen)
{
	unsigned char *p = pBuffer;

	if (pBuffer == NULL)
		return rtl_Digest_E_Argument;

	if (!(RTL_DIGEST_LENGTH_MD5 <= nBufLen))
		return rtl_Digest_E_BufferSize;

	/* EndContext(); *//* not finalized */
	RTL_DIGEST_LTOC (m_context.m_nA, p);
	RTL_DIGEST_LTOC (m_context.m_nB, p);
	RTL_DIGEST_LTOC (m_context.m_nC, p);
	RTL_DIGEST_LTOC (m_context.m_nD, p);
	
	InitContext();

	return rtl_Digest_E_None;
}




