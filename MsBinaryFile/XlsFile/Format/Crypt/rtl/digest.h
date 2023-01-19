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

