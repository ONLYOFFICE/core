/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <stddef.h>

#ifdef __linux__
    #include <inttypes.h>
#endif

#include "../../../../../OOXML/Base/Base.h"

class CipherARCFOUR
{
public:
	typedef enum 
	{
		rtl_Cipher_DirectionBoth,
		rtl_Cipher_DirectionDecode,
		rtl_Cipher_DirectionEncode,
		rtl_Cipher_DirectionInvalid,
	} rtlCipherDirection;

	typedef enum
	{
		rtl_Cipher_E_None,
		rtl_Cipher_E_Argument,
		rtl_Cipher_E_Direction,
		rtl_Cipher_E_BufferSize,
	} rtlCipherError;

public:
	CipherARCFOUR();

	rtlCipherError Init(rtlCipherDirection Direction, const unsigned char *pKeyData, const size_t nKeyLen, const unsigned char *pArgData, const size_t nArgLen);
	rtlCipherError Encode(const void *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen);
	rtlCipherError Decode(const void *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen);


private:
	rtlCipherError Update(const unsigned char *pData, const size_t nDatLen, unsigned char *pBuffer, const size_t nBufLen);

private:
	static const _UINT32 CIPHER_CBLOCK_ARCFOUR = 256;
	typedef struct
	{
		_UINT32 m_S[CIPHER_CBLOCK_ARCFOUR];
		_UINT32 m_X, m_Y;
	} CipherContext;

	rtlCipherDirection  m_direction;
	CipherContext m_context;

};


