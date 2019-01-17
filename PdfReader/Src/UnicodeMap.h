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
#ifndef _PDF_READER_UNICODE_MAP_H
#define _PDF_READER_UNICODE_MAP_H

#include "CharTypes.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
	class StringExt;

	//-------------------------------------------------------------------------------------------------------------------------------

	enum UnicodeMapType
	{
		unicodeMapUser,      // Чтение из файла
		unicodeMapResident,  // Статический список
		unicodeMapFunc       // Указатель на функцию
	};

	typedef int(*UnicodeMapFunc)(Unicode nUnicode, char *sBuffer, int nBufferSize);

	struct UnicodeMapRange
	{
		Unicode      nStart;      // Диапазон юникодных символов
		Unicode      nEnd;        // 
		unsigned int unCode;      // Первый код
		unsigned int nBytesCount; //
	};

	struct UnicodeMapExt;

	//-------------------------------------------------------------------------------------------------------------------------------
	// UnicodeMap
	//-------------------------------------------------------------------------------------------------------------------------------

	class UnicodeMap
	{
	public:

		// Создаем UnicodeMap по имени <seEncodingName>. Устанавливаем счетчик ссылок равный 1.
		static UnicodeMap *Parse(StringExt *seEncodingName, GlobalParams *pGlobalParams);

		// Создаем Resident UnicodeMap.
		UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapRange *pRanges, int nLen);

		// Создаем Resident UnicodeMap, который использует функцию вместо диапазона значений.
		UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapFunc pFunction);

		~UnicodeMap();

		// Счетчик ссылок
		void AddRef();
		void Release();

		StringExt *GetEncodingName()
		{
			return m_seEncodingName;
		}

		bool IsUnicode()
		{
			return m_bUnicodeOut;
		}


		// Совпадают ли названия кодировок?
		bool Match(StringExt *seEncodingName);

		// Находим образ текущего юникодного значения в кодировке. Заполняем <sBuffer> результатом и возвращаем
		// количество используемых байт. Никаких строковых разделителей (типа \0) не пишем. 
		int MapUnicode(Unicode nUnicode, char *sBuffer, int nBufferSize);

	private:

		UnicodeMap(StringExt *seEncodingName);

	private:

		StringExt     *m_seEncodingName;
		UnicodeMapType m_eType;
		bool           m_bUnicodeOut;
		union
		{
			UnicodeMapRange *m_pRanges;	  // Для типов: User, Resident
			UnicodeMapFunc   m_pFunction; // Для типа Func
		};
		int            m_nLen;      // User, Resident
		UnicodeMapExt *m_pEMaps;    // User
		int            m_nEMapsLen; // User

		int            m_nRefCount;

		NSCriticalSection::CRITICAL_SECTION m_oCS;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// UnicodeMapCache
	//-------------------------------------------------------------------------------------------------------------------------------

#define unicodeMapCacheSize 4

	class UnicodeMapCache
	{
	public:

		UnicodeMapCache();
		~UnicodeMapCache();

		UnicodeMap *GetUnicodeMap(StringExt *seEncodingName, GlobalParams *pGlobalParams);

	private:

		UnicodeMap *m_ppCache[unicodeMapCacheSize];
	};
}

#endif // _PDF_READER_UNICODE_MAP_H
