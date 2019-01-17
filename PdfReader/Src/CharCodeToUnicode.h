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
#ifndef _PDF_READER_CHARCODE_TO_UNICODE_H
#define _PDF_READER_CHARCODE_TO_UNICODE_H

#include "CharTypes.h"
#include "GlobalParams.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
	struct CharCodeToUnicodeString;

	//-------------------------------------------------------------------------------------------------------------------------------

	class CharCodeToUnicode
	{
	public:

		static CharCodeToUnicode *ParseCIDToUnicode(StringExt *sFileName, StringExt *seCollection);

		static CharCodeToUnicode *ParseUnicodeToUnicode(StringExt *sFileName);

		static CharCodeToUnicode *Make8BitToUnicode(Unicode *pToUnicode);

		static CharCodeToUnicode *ParseCMap(StringExt *sBuffer, int nBitCount, GlobalParams *pGlobalParams);

		void MergeCMap(StringExt *sBuffer, int nBitCount, GlobalParams *pGlobalParams);

		~CharCodeToUnicode();

		// Считаем ссылки
		void AddRef();
		void Release();

		// Сравниваем по данному тэгу
		bool Match(StringExt *seTag);

		void SetMapping(CharCode nCode, Unicode *pUnicode, int nLen);

		int  MapToUnicode(CharCode nCode, Unicode *pUnicode, int nSize);

		CharCode GetLength()
		{
			return m_nMapLen;
		}

	private:

		void ParseCMap1(int(*GetCharFunc)(void *), void *pData, int nBitCount, GlobalParams *pGlobalParams);
		void AddMapping(CharCode nCode, char *sUnicodeString, int nLen, int nOffset);
		CharCodeToUnicode(StringExt *sTag);
		CharCodeToUnicode(StringExt *sTag, Unicode *pMap, CharCode nMapLen, bool bCopyMap, CharCodeToUnicodeString *pSMap, int nSMapLen, int nSMapSize);

	private:

		StringExt              *m_seTag;
		Unicode                *m_pMap;
		CharCode                m_nMapLen;
		CharCodeToUnicodeString*m_pSMap;
		int                     m_nSMapLen;
		int                     m_nSMapSize;
								
		int                     m_nRef;

		NSCriticalSection::CRITICAL_SECTION m_oCS;
	};

	//-------------------------------------------------------------------------------------------------------------------------------

	class CharCodeToUnicodeCache
	{
	public:

		CharCodeToUnicodeCache(int nSize);
		~CharCodeToUnicodeCache();

		CharCodeToUnicode *GetCharCodeToUnicode(StringExt *seTag);

		void Add(CharCodeToUnicode *pCharCodeToUnicode);

	private:

		CharCodeToUnicode **m_ppCache;
		int                 m_nSize;
	};
}

#endif // _PDF_READER_CHARCODE_TO_UNICODE_H
