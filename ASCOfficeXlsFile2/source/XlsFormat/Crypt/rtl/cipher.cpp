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

#include "cipher.h"
#include <memory>


CipherARCFOUR::CipherARCFOUR()
:	m_direction(rtl_Cipher_DirectionInvalid)
{	
}

CipherARCFOUR::rtlCipherError CipherARCFOUR::Init(CipherARCFOUR::rtlCipherDirection Direction, const unsigned char *pKeyData, const size_t nKeyLen, const unsigned char *pArgData, const size_t nArgLen)
{
	if (pKeyData == NULL)
	{
		return rtl_Cipher_E_Argument;
	}

	if (Direction != rtl_Cipher_DirectionInvalid)
	{
		m_direction = Direction;
	}
	else
	{
		return rtl_Cipher_E_Direction;
	}

	unsigned int  K[CIPHER_CBLOCK_ARCFOUR];
	unsigned int *L, *S;
	unsigned int  x, y, t;
	size_t      n, k;

	S = &(m_context.m_S[0]);

	/* Initialize S linearly. */
	for (x = 0; x < CIPHER_CBLOCK_ARCFOUR; x++)
		S[x] = x;

	/* Initialize K with key, repeat key as necessary. */
	for (L = K, n = CIPHER_CBLOCK_ARCFOUR; n > nKeyLen; n -= nKeyLen)
	{
		for (k = 0; k < nKeyLen; k++) L[k] = pKeyData[k];
		L += nKeyLen;
	}
	for (k = 0; k < n; k++) L[k] = pKeyData[k];

	/* Initialize S with K. */
	for (x = 0, y = 0; x < CIPHER_CBLOCK_ARCFOUR; x++)
	{
		y = (y + S[x] + K[x]) % CIPHER_CBLOCK_ARCFOUR;
		t = S[x], S[x] = S[y], S[y] = t; /* swap S[x] and S[y] */
	}

	/* Initialize counters X and Y. */
	m_context.m_X = 0;
	m_context.m_Y = 0;

	return rtl_Cipher_E_None;
}


CipherARCFOUR::rtlCipherError CipherARCFOUR::Encode(const void *pvData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen)
{
	if (m_direction == rtl_Cipher_DirectionInvalid)
	{
		return rtl_Cipher_E_Direction;
	}

	return Update((const unsigned char*)pvData, nDatLen, pBuffer, nBufLen);
}


CipherARCFOUR::rtlCipherError CipherARCFOUR::Decode(const void *pvData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen)
{
	if (m_direction == rtl_Cipher_DirectionInvalid)
	{
		return rtl_Cipher_E_Direction;
	}

	return Update((const unsigned char*)pvData, nDatLen, pBuffer, nBufLen);
}


CipherARCFOUR::rtlCipherError CipherARCFOUR::Update(const unsigned char *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen)
{
	_UINT32 *S;
	_UINT32  x, y, t;
	size_t k;

	/* Check arguments. */
	if ((pData == NULL) || (pBuffer == NULL))
		return rtl_Cipher_E_Argument;

	if (!((0 < nDatLen) && (nDatLen <= nBufLen)))
		return rtl_Cipher_E_BufferSize;

	/* Update. */
	S = &(m_context.m_S[0]);
	for (k = 0; k < nDatLen; k++)
	{
		/* Update counters X and Y. */
		x = m_context.m_X;
		y = m_context.m_Y;
		x = (x + 1   ) % CIPHER_CBLOCK_ARCFOUR;
		y = (y + S[x]) % CIPHER_CBLOCK_ARCFOUR;
		m_context.m_X = x;
		m_context.m_Y = y;

		/* Swap S[x] and S[y]. */
		t = S[x], S[x] = S[y], S[y] = t;

		/* Evaluate next key unsigned char S[t]. */
		t = (S[x] + S[y]) % CIPHER_CBLOCK_ARCFOUR;
		pBuffer[k] = pData[k] ^ ((unsigned char)(S[t] & 0xff));
	}

	return rtl_Cipher_E_None;
}


