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

#ifdef __linux__
    #include <inttypes.h>
#endif

#include "../../../../../OOXML/Base/Base.h"

class DigestMD5
{
public:
	typedef enum 
	{
		rtl_Digest_E_None,
		rtl_Digest_E_Argument,
		rtl_Digest_E_BufferSize,
	} rtlDigestError;

public:
	DigestMD5();

	rtlDigestError Update(const void *pData, const unsigned int nDatLen);
	rtlDigestError Get(unsigned char *pBuffer, const unsigned int nBufLen);
	rtlDigestError Raw (unsigned char *pBuffer, const unsigned int nBufLen);

	static const unsigned int RTL_DIGEST_LENGTH_MD5 = 16;

private:
	void InitContext();
	void UpdateContext();
	void EndContext();

private:
	static const _UINT32 DIGEST_CBLOCK_MD5 = 64;
	static const _UINT32 DIGEST_LBLOCK_MD5 = 16;

	typedef struct digestMD5_context_st
	{
		_UINT32 m_nDatLen;
		_UINT32 m_pData[DIGEST_LBLOCK_MD5];
		_UINT32 m_nA, m_nB, m_nC, m_nD;
		_UINT32 m_nL, m_nH;
	} DigestContextMD5;

	DigestContextMD5 m_context;
};

