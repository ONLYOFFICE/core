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
#include <stdio.h>
#include <string.h>
#include "MemoryUtils.h"
#include "File.h"
#include "StringExt.h"
#include "List.h"
#include "GlobalParams.h"
#include "UnicodeMap.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "CharTypes.h"

//-------------------------------------------------------------------------------------------------------------------------------

#define maxExtCode 16

namespace PdfReader
{
	struct UnicodeMapExt
	{
		Unicode      nUnicode;
		char         arrCode[maxExtCode];
		unsigned int unBytesCount;
	};

	//-------------------------------------------------------------------------------------------------------------------------------

	UnicodeMap *UnicodeMap::Parse(StringExt *seEncodingName, GlobalParams *pGlobalParams)
	{
		FILE *pFile = NULL;
		if (pGlobalParams && !(pFile = pGlobalParams->GetUnicodeMapFile(seEncodingName)))
		{
			// TO DO: Error "Couldn't find UnicodeMap file for the encoding"
			return NULL;
		}

		UnicodeMap *pMap = new UnicodeMap(seEncodingName->Copy());

		int nSize = 8;
		pMap->m_pRanges = (UnicodeMapRange *)MemUtilsMallocArray(nSize, sizeof(UnicodeMapRange));
		int nEMapsSize = 0;

		int nLineNum = 1;
		char sBuffer[256];
		while (GetLine(sBuffer, sizeof(sBuffer), pFile))
		{
			char *sToken1, *sToken2, *sToken3;
			if ((sToken1 = strtok(sBuffer, " \t\r\n")) && (sToken2 = strtok(NULL, " \t\r\n")))
			{
				if (!(sToken3 = strtok(NULL, " \t\r\n")))
				{
					sToken3 = sToken2;
					sToken2 = sToken1;
				}
				int nBytesCount = strlen(sToken3) / 2;

				if (nBytesCount <= 4)
				{
					if (pMap->m_nLen == nSize)
					{
						nSize *= 2;
						pMap->m_pRanges = (UnicodeMapRange *)MemUtilsReallocArray(pMap->m_pRanges, nSize, sizeof(UnicodeMapRange));
					}
					UnicodeMapRange *pRange = &pMap->m_pRanges[pMap->m_nLen];
					sscanf(sToken1, "%x", &pRange->nStart);
					sscanf(sToken2, "%x", &pRange->nEnd);
					sscanf(sToken3, "%x", &pRange->unCode);
					pRange->nBytesCount = nBytesCount;
					++pMap->m_nLen;
				}
				else if (sToken2 == sToken1)
				{
					if (pMap->m_nEMapsLen == nEMapsSize)
					{
						nEMapsSize += 16;
						pMap->m_pEMaps = (UnicodeMapExt *)MemUtilsReallocArray(pMap->m_pEMaps, nEMapsSize, sizeof(UnicodeMapExt));
					}
					UnicodeMapExt *pEMap = &pMap->m_pEMaps[pMap->m_nEMapsLen];
					sscanf(sToken1, "%x", &pEMap->nUnicode);
					for (int nIndex = 0; nIndex < nBytesCount; ++nIndex)
					{
						int nValue = 0;
						sscanf(sToken3 + nIndex * 2, "%2x", &nValue);
						pEMap->arrCode[nIndex] = (char)nValue;
					}
					pEMap->unBytesCount = nBytesCount;
					++pMap->m_nEMapsLen;
				}
				else
				{
					// TO DO: Error "Bad line in unicodeMap file for encoding"
				}
			}
			else
			{
				// TO DO: Error "Bad line in unicodeMap file for encoding"
			}
			++nLineNum;
		}

		fclose(pFile);

		return pMap;
	}

	UnicodeMap::UnicodeMap(StringExt *seEncodingName)
	{
		m_seEncodingName = seEncodingName;
		m_bUnicodeOut    = false;
		m_eType          = unicodeMapUser;

		m_pRanges = NULL;
		m_nLen    = 0;

		m_pEMaps    = NULL;
		m_nEMapsLen = 0;

		m_nRefCount = 1;

		m_oCS.InitializeCriticalSection();
	}

	UnicodeMap::UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapRange *pRanges, int nLen)
	{
		m_seEncodingName = new StringExt(sEncodingName);
		m_bUnicodeOut    = bUnicodeOut;
		m_eType          = unicodeMapResident;

		m_pRanges = pRanges;
		m_nLen    = nLen;

		m_pEMaps    = NULL;
		m_nEMapsLen = 0;

		m_nRefCount = 1;

		m_oCS.InitializeCriticalSection();
	}

	UnicodeMap::UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapFunc pFunction)
	{
		m_seEncodingName = new StringExt(sEncodingName);
		m_bUnicodeOut    = bUnicodeOut;
		m_eType          = unicodeMapFunc;

		m_pFunction = pFunction;

		m_pEMaps    = NULL;
		m_nEMapsLen = 0;

		m_nRefCount = 1;

		m_oCS.InitializeCriticalSection();
	}

	UnicodeMap::~UnicodeMap()
	{
		if (m_seEncodingName)
			delete m_seEncodingName;

		if (m_eType == unicodeMapUser && m_pRanges)
		{
			MemUtilsFree(m_pRanges);
		}

		if (m_pEMaps)
		{
			MemUtilsFree(m_pEMaps);
		}

		m_oCS.DeleteCriticalSection();
	}

	void UnicodeMap::AddRef()
	{
		CTemporaryCS *pCS = new CTemporaryCS(&m_oCS);

		++m_nRefCount;

		RELEASEOBJECT(pCS);
	}

	void UnicodeMap::Release()
	{
		CTemporaryCS *pCS = new CTemporaryCS(&m_oCS);

		bool bDone = (--m_nRefCount == 0);

		RELEASEOBJECT(pCS);

		if (bDone)
		{
			delete this;
		}
	}

	bool UnicodeMap::Match(StringExt *seEncodingName)
	{
		return (!m_seEncodingName->Compare(seEncodingName));
	}

	int UnicodeMap::MapUnicode(Unicode nUnicode, char *sBuffer, int nBufferSize)
	{
		if (m_eType == unicodeMapFunc)
		{
			return (*m_pFunction)(nUnicode, sBuffer, nBufferSize);
		}

		int nFirst = 0;
		int nLast = m_nLen;
		if (nUnicode >= m_pRanges[nFirst].nStart)
		{
			// m_pRanges[nFirst].nStart <= nUnicode < m_pRanges[nLast].nStart
			while (nLast - nFirst > 1)
			{
				int nMiddle = (nFirst + nLast) / 2;
				if (nUnicode >= m_pRanges[nMiddle].nStart)
				{
					nFirst = nMiddle;
				}
				else if (nUnicode < m_pRanges[nMiddle].nStart)
				{
					nLast = nMiddle;
				}
			}

			if (nUnicode <= m_pRanges[nFirst].nEnd)
			{
				int nLen = m_pRanges[nFirst].nBytesCount;
				if (nLen > nBufferSize)
				{
					return 0;
				}
				unsigned int unCode = m_pRanges[nFirst].unCode + (nUnicode - m_pRanges[nFirst].nStart);
				for (int nIndex = nLen - 1; nIndex >= 0; --nIndex)
				{
					sBuffer[nIndex] = (char)(unCode & 0xff);
					unCode >>= 8;
				}
				return nLen;
			}
		}

		for (int nIndex = 0; nIndex < m_nEMapsLen; ++nIndex)
		{
			if (m_pEMaps[nIndex].nUnicode == nUnicode)
			{
				int nLen = m_pEMaps[nIndex].unBytesCount;
				for (int nJ = 0; nJ < nLen; ++nJ)
				{
					sBuffer[nJ] = m_pEMaps[nIndex].arrCode[nJ];
				}
				return nLen;
			}
		}

		return 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	UnicodeMapCache::UnicodeMapCache()
	{
		for (int nIndex = 0; nIndex < unicodeMapCacheSize; ++nIndex)
		{
			m_ppCache[nIndex] = NULL;
		}
	}

	UnicodeMapCache::~UnicodeMapCache()
	{
		for (int nIndex = 0; nIndex < unicodeMapCacheSize; ++nIndex)
		{
			if (m_ppCache[nIndex])
			{
				m_ppCache[nIndex]->Release();
			}
		}
	}

	UnicodeMap *UnicodeMapCache::GetUnicodeMap(StringExt *seEncodingName, GlobalParams *pGlobalParams)
	{
		if (m_ppCache[0] && m_ppCache[0]->Match(seEncodingName))
		{
			m_ppCache[0]->AddRef();
			return m_ppCache[0];
		}
		UnicodeMap *pMap = NULL;
		for (int nIndex = 1; nIndex < unicodeMapCacheSize; ++nIndex)
		{
			if (m_ppCache[nIndex] && m_ppCache[nIndex]->Match(seEncodingName))
			{
				pMap = m_ppCache[nIndex];
				for (int nJ = nIndex; nJ >= 1; --nJ)
				{
					m_ppCache[nJ] = m_ppCache[nJ - 1];
				}
				m_ppCache[0] = pMap;
				pMap->AddRef();
				return pMap;
			}
		}
		if ((pMap = UnicodeMap::Parse(seEncodingName, pGlobalParams)))
		{
			if (m_ppCache[unicodeMapCacheSize - 1])
			{
				m_ppCache[unicodeMapCacheSize - 1]->Release();
			}
			for (int nJ = unicodeMapCacheSize - 1; nJ >= 1; --nJ)
			{
				m_ppCache[nJ] = m_ppCache[nJ - 1];
			}
			m_ppCache[0] = pMap;
			pMap->AddRef();
			return pMap;
		}
		return NULL;
	}
}
