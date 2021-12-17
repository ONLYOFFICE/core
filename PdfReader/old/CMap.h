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
#ifndef _PDF_READER_CMAP_H
#define _PDF_READER_CMAP_H

#include "CharTypes.h"
#include "GlobalParams.h"
#include "XmlUtils.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{

	class StringExt;
	struct CMapVectorEntry;
	class CMapCache;

	//-------------------------------------------------------------------------------------------------------------------------------

	class CMap
	{
	public:

		// Создаем карту CMap определенную по <seCollection> и <seCMapName>. 
		// Устанавливаем счетчик ссылок на 1. 
		static CMap *Parse(CMapCache *pCache, StringExt *seCollection, StringExt *seCMapName, GlobalParams *pGlobalParams, wchar_t *wsFilePath = NULL);

		~CMap();

		// Считаем ссылки
		void AddRef();
		void Release();

		// Название коллекции возвращаем в следующем формате: registry-ordering.
		StringExt *GetCollection()
		{
			return m_seCollection;
		}

		// Возвращаем true, если параметры CMap совпадают с заданными <seCollection> и <seCMapName>.
		bool Match(StringExt *seCollection, StringExt *seCMapName);

		// Возвращаем CID соответствующий коду символа, который начинается в <sChar>, 
		// и содержит <nLen> байт. 
		CID GetCID(char *sChar, int nLen, int *pnUsed);

		// Writing mode (0 = horizontal, 1 = vertical).
		int GetWMode()
		{
			return m_nWMode;
		}
		void ToXml(std::wstring wsFilePath);

	private:

		CMap(GlobalParams *pGlobalParams, StringExt *seCollection, StringExt *seCMapName);
		CMap(GlobalParams *pGlobalParams, StringExt *seCollection, StringExt *seCMapName, int nWMode);
		void UseCMap(CMapCache *pCache, char *sUseName);
		void CopyVector(CMapVectorEntry *pDest, CMapVectorEntry *pSrc);
		void AddCodeSpace(CMapVectorEntry *pVector, unsigned int unStart, unsigned int unEnd, unsigned int unBytesCount);
		void AddCIDs(unsigned int unStart, unsigned int unEnd, unsigned int unBytesCount, CID nFirstCID);
		void FreeCMapVector(CMapVectorEntry *pVector);
		void WriteVectorToXml(CXmlWriter *pWriter, CMapVectorEntry *pVector);

	private:

		StringExt       *m_seCollection;  // 
		StringExt       *m_seCMapName;    // 
		int              m_nWMode;        // writing mode (0=horizontal, 1=vertical)
		CMapVectorEntry *m_pVector;       // vector for first byte (NULL for identity CMap)
		int              m_nRef;          // Счетчик ссылок
						
		GlobalParams    *m_pGlobalParams;

		NSCriticalSection::CRITICAL_SECTION m_oCS;
	};

	//-------------------------------------------------------------------------------------------------------------------------------

#define CMapCacheSize 4

	class CMapCache
	{
	public:

		CMapCache();
		~CMapCache();

		CMap *GetCMap(StringExt *seCollection, StringExt *seCMapName, GlobalParams *pGlobalParams, wchar_t *wsFilePath = NULL);

	private:

		CMap *m_ppCache[CMapCacheSize];
	};
}

#endif // _PDF_READER_CMAP_H
