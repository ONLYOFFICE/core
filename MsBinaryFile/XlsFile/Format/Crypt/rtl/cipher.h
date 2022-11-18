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


