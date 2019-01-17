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
#include "GlobalParams.h"
#include "PSLexer.h"
#include "CharCodeToUnicode.h"

namespace PdfReader
{
	#define MaxUnicodeString 8

	struct CharCodeToUnicodeString
	{
		CharCode nCode;
		Unicode  pUnicodeString[MaxUnicodeString];
		int      nLen;
	};

	//-------------------------------------------------------------------------------------------------------------------------------

	static int GetCharFromString(void *pData)
	{
		int nChar = 0;

		char *sString = *(char **)pData;

		if (*sString)
		{
			nChar = *sString++;
			*(char **)pData = sString;
		}
		else
		{
			nChar = EOF;
		}
		return nChar;
	}

	static int GetCharFromFile(void *pData)
	{
		return fgetc((FILE *)pData);
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	CharCodeToUnicode *CharCodeToUnicode::ParseCIDToUnicode(StringExt *seFileName, StringExt *seCollection)
	{
		char sBuffer[64];
		Unicode nUnicode = 0;
		
		FILE* pFile = NSFile::CFileBinary::OpenFileNative(seFileName->GetWString(), L"rb");
		if (!pFile)
		{
			// TO DO: Error "Couldn't open cidToUnicode file"
			return NULL;
		}

		unsigned int nSize = 32768;
		Unicode *pMap = (Unicode *)MemUtilsMallocArray(nSize, sizeof(Unicode));
		unsigned int nMapLen = 0;

		while (GetLine(sBuffer, sizeof(sBuffer), pFile))
		{
			if (nMapLen == nSize)
			{
				nSize *= 2;
				pMap = (Unicode *)MemUtilsReallocArray(pMap, nSize, sizeof(Unicode));
			}
			if (sscanf(sBuffer, "%x", &nUnicode) == 1)
			{
				pMap[nMapLen] = nUnicode;
			}
			else
			{
				// TO DO: Error "Bad line in cidToUnicode file"
				pMap[nMapLen] = 0;
			}
			++nMapLen;
		}
		fclose(pFile);

		CharCodeToUnicode *pCharCodeToUnicode = new CharCodeToUnicode(seCollection->Copy(), pMap, nMapLen, true, NULL, 0, 0);
		MemUtilsFree(pMap);
		return pCharCodeToUnicode;
	}

	CharCodeToUnicode *CharCodeToUnicode::ParseUnicodeToUnicode(StringExt *seFileName)
	{
		char sBuffer[256];
		char *sToken;
		Unicode nUnicode0;
		Unicode arrUnicodeBuffer[MaxUnicodeString];

		FILE* pFile = NSFile::CFileBinary::OpenFileNative(seFileName->GetWString(), L"rb");
		if (!pFile)
		{
			// TO DO: Error ""Couldn't open unicodeToUnicode file"
			return NULL;
		}

		unsigned int nSize = 4096;
		Unicode *pMap = (Unicode *)MemUtilsMallocArray(nSize, sizeof(Unicode));
		memset(pMap, 0, nSize * sizeof(Unicode));
		unsigned int nLen = 0;
		CharCodeToUnicodeString *pSMap = NULL;
		unsigned int nSMapSize = 0, nSMapLen = 0;

		int nLine = 0;
		while (GetLine(sBuffer, sizeof(sBuffer), pFile))
		{
			++nLine;
			if (!(sToken = strtok(sBuffer, " \t\r\n")) || sscanf(sToken, "%x", &nUnicode0) != 1)
			{
				// TO DO: Error "Bad line in unicodeToUnicode file"
				continue;
			}
			int nCount = 0;
			while (nCount < MaxUnicodeString)
			{
				if (!(sToken = strtok(NULL, " \t\r\n")))
				{
					break;
				}
				if (sscanf(sToken, "%x", &arrUnicodeBuffer[nCount]) != 1)
				{
					// TO DO: Error "Bad line in unicodeToUnicode file"
					break;
				}
				++nCount;
			}
			if (nCount < 1)
			{
				// TO DO: Error "Bad line in unicodeToUnicode file"
				continue;
			}
			if (nUnicode0 >= nSize)
			{
				unsigned int nOldSize = nSize;
				while (nUnicode0 >= nSize)
				{
					nSize *= 2;
				}
				pMap = (Unicode *)MemUtilsReallocArray(pMap, nSize, sizeof(Unicode));
				memset(pMap + nOldSize, 0, (nSize - nOldSize) * sizeof(Unicode));
			}
			if (nCount == 1)
			{
				pMap[nUnicode0] = arrUnicodeBuffer[0];
			}
			else
			{
				pMap[nUnicode0] = 0;
				if (nSMapLen == nSMapSize)
				{
					nSMapSize += 16;
					pSMap = (CharCodeToUnicodeString *)MemUtilsReallocArray(pSMap, nSMapSize, sizeof(CharCodeToUnicodeString));
				}
				pSMap[nSMapLen].nCode = nUnicode0;
				for (int nIndex = 0; nIndex < nCount; ++nIndex)
				{
					pSMap[nSMapLen].pUnicodeString[nIndex] = arrUnicodeBuffer[nIndex];
				}
				pSMap[nSMapLen].nLen = nCount;
				++nSMapLen;
			}
			if (nUnicode0 >= nLen)
			{
				nLen = nUnicode0 + 1;
			}
		}
		fclose(pFile);

		CharCodeToUnicode *pCharCodeToUnicode = new CharCodeToUnicode(seFileName->Copy(), pMap, nLen, true, pSMap, nSMapLen, nSMapSize);
		MemUtilsFree(pMap);
		return pCharCodeToUnicode;
	}

	CharCodeToUnicode *CharCodeToUnicode::Make8BitToUnicode(Unicode *pToUnicode)
	{
		return new CharCodeToUnicode(NULL, pToUnicode, 256, true, NULL, 0, 0);
	}

	CharCodeToUnicode *CharCodeToUnicode::ParseCMap(StringExt *seBuffer, int nBitCount, GlobalParams *pGlobalParams)
	{
		CharCodeToUnicode *pCharCodeToUnicode = new CharCodeToUnicode(NULL);
		char *pData = seBuffer->GetBuffer();
		pCharCodeToUnicode->ParseCMap1(&GetCharFromString, &pData, nBitCount, pGlobalParams);
		return pCharCodeToUnicode;
	}

	void CharCodeToUnicode::MergeCMap(StringExt *seBuffer, int nBitCount, GlobalParams *pGlobalParams)
	{
		char *pData = seBuffer->GetBuffer();
		ParseCMap1(&GetCharFromString, &pData, nBitCount, pGlobalParams);
	}

	void CharCodeToUnicode::ParseCMap1(int(*GetCharFunc)(void *), void *pData, int nBitCount, GlobalParams *pGlobalParams)
	{
		char sToken1[256], sToken2[256], sToken3[256];
		int nLen1, nLen2, nLen3;
		CharCode nCode1, nCode2;


		int nDigitCount = nBitCount / 4;
		PSLexer *pLexer = new PSLexer(GetCharFunc, pData);
		pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1);

		while (pLexer->GetToken(sToken2, sizeof(sToken2), &nLen2))
		{
			if (!strcmp(sToken2, "usecmap"))
			{
				if (sToken1[0] == '/')
				{
					StringExt *seName = new StringExt(sToken1 + 1);
					FILE *pFile = NULL;
					if (pGlobalParams && (pFile = pGlobalParams->FindToUnicodeFile(seName)))
					{
						ParseCMap1(&GetCharFromFile, pFile, nBitCount, pGlobalParams);
						fclose(pFile);
					}
					else
					{
						// TO DO: Error "Couldn't find ToUnicode CMap file"
					}
					delete seName;
				}
				pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else if (!strcmp(sToken2, "beginbfchar"))
			{
				while (pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1))
				{
					if (!strcmp(sToken1, "endbfchar"))
					{
						break;
					}
					if (!pLexer->GetToken(sToken2, sizeof(sToken2), &nLen2) || !strcmp(sToken2, "endbfchar"))
					{
						// TO DO: Error "Illegal entry in bfchar block in ToUnicode CMap"
						break;
					}
					if (!(nLen1 == 2 + nDigitCount && sToken1[0] == '<' && sToken1[nLen1 - 1] == '>' && sToken2[0] == '<' && sToken2[nLen2 - 1] == '>'))
					{
						// TO DO: Error "Illegal entry in bfchar block in ToUnicode CMap"
						continue;
					}
					sToken1[nLen1 - 1] = sToken2[nLen2 - 1] = '\0';
					if (sscanf(sToken1 + 1, "%x", &nCode1) != 1)
					{
						// TO DO: Error "Illegal entry in bfchar block in ToUnicode CMap"
						continue;
					}
					AddMapping(nCode1, sToken2 + 1, nLen2 - 2, 0);
				}
				pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else if (!strcmp(sToken2, "beginbfrange"))
			{
				while (pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1))
				{
					if (!strcmp(sToken1, "endbfrange"))
					{
						break;
					}
					if (!pLexer->GetToken(sToken2, sizeof(sToken2), &nLen2) || !strcmp(sToken2, "endbfrange") || !pLexer->GetToken(sToken3, sizeof(sToken3), &nLen3) || !strcmp(sToken3, "endbfrange"))
					{
						// TO DO: Error "Illegal entry in bfrange block in ToUnicode CMap"
						break;
					}
					if (!(nLen1 == 2 + nDigitCount && sToken1[0] == '<' && sToken1[nLen1 - 1] == '>' && nLen2 == 2 + nDigitCount && sToken2[0] == '<' && sToken2[nLen2 - 1] == '>'))
					{
						// TO DO: Error "Illegal entry in bfrange block in ToUnicode CMap"
						continue;
					}
					sToken1[nLen1 - 1] = sToken2[nLen2 - 1] = '\0';
					if (sscanf(sToken1 + 1, "%x", &nCode1) != 1 || sscanf(sToken2 + 1, "%x", &nCode2) != 1)
					{
						// TO DO: Error "Illegal entry in bfrange block in ToUnicode CMap"
						continue;
					}
					if (!strcmp(sToken3, "["))
					{
						int nIndex = 0;
						while (pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1) && nCode1 + nIndex <= nCode2)
						{
							if (!strcmp(sToken1, "]"))
							{
								break;
							}
							if (sToken1[0] == '<' && sToken1[nLen1 - 1] == '>')
							{
								sToken1[nLen1 - 1] = '\0';
								AddMapping(nCode1 + nIndex, sToken1 + 1, nLen1 - 2, 0);
							}
							else
							{
								// TO DO: Error "Illegal entry in bfrange block in ToUnicode CMap"
							}
							++nIndex;
						}
					}
					else if (sToken3[0] == '<' && sToken3[nLen3 - 1] == '>')
					{
						sToken3[nLen3 - 1] = '\0';
						for (int nIndex = 0; nCode1 <= nCode2; ++nCode1, ++nIndex)
						{
							AddMapping(nCode1, sToken3 + 1, nLen3 - 2, nIndex);
						}
					}
					else
					{
						// TO DO: Error "Illegal entry in bfrange block in ToUnicode CMap"
					}
				}
				pLexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else
			{
				strcpy(sToken1, sToken2);
			}
		}
		delete pLexer;
	}

	void CharCodeToUnicode::AddMapping(CharCode nCode, char *sUnicodeString, int nLen, int nOffset)
	{

		if (nCode >= m_nMapLen)
		{
			unsigned int unOldLen = m_nMapLen;
			m_nMapLen = (nCode + 256) & ~255;
			m_pMap = (Unicode *)MemUtilsReallocArray(m_pMap, m_nMapLen, sizeof(Unicode));
			for (unsigned int unIndex = unOldLen; unIndex < m_nMapLen; ++unIndex)
			{
				m_pMap[unIndex] = 0;
			}
		}
		if (nLen <= 4)
		{
			Unicode nUnicode = 0;
			if (sscanf(sUnicodeString, "%x", &nUnicode) != 1)
			{
				// TO DO: Error Illegal entry in ToUnicode CMap"
				return;
			}
			m_pMap[nCode] = nUnicode + nOffset;
		}
		else
		{
			if (m_nSMapLen >= m_nSMapSize)
			{
				m_nSMapSize = m_nSMapSize + 16;
				m_pSMap = (CharCodeToUnicodeString *)MemUtilsReallocArray(m_pSMap, m_nSMapSize, sizeof(CharCodeToUnicodeString));
			}
			m_pMap[nCode] = 0;
			m_pSMap[m_nSMapLen].nCode = nCode;
			m_pSMap[m_nSMapLen].nLen  = nLen / 4;
			for (int nIndex = 0; nIndex < m_pSMap[m_nSMapLen].nLen && nIndex < MaxUnicodeString; ++nIndex)
			{
				char pUnicodeHex[5];
				strncpy(pUnicodeHex, sUnicodeString + nIndex * 4, 4);
				pUnicodeHex[4] = '\0';
				if (sscanf(pUnicodeHex, "%x", &m_pSMap[m_nSMapLen].pUnicodeString[nIndex]) != 1)
				{
					// TO DO: Error "Illegal entry in ToUnicode CMap"
				}
			}
			m_pSMap[m_nSMapLen].pUnicodeString[m_pSMap[m_nSMapLen].nLen - 1] += nOffset;
			++m_nSMapLen;
		}
	}

	CharCodeToUnicode::CharCodeToUnicode(StringExt *seTag)
	{
		m_seTag = seTag;
		m_nMapLen = 256;
		m_pMap = (Unicode *)MemUtilsMallocArray(m_nMapLen, sizeof(Unicode));
		for (unsigned int unIndex = 0; unIndex < m_nMapLen; ++unIndex)
		{
			m_pMap[unIndex] = 0;
		}
		m_pSMap = NULL;
		m_nSMapLen = m_nSMapSize = 0;
		m_nRef = 1;

		m_oCS.InitializeCriticalSection();
	}

	CharCodeToUnicode::CharCodeToUnicode(StringExt *seTag, Unicode *pMap, CharCode unMapLen, bool bCopyMap, CharCodeToUnicodeString *pSMap, int nSMapLen, int nSMapSize)
	{
		m_seTag   = seTag;
		m_nMapLen = unMapLen;
		if (bCopyMap)
		{
			m_pMap = (Unicode *)MemUtilsMallocArray(m_nMapLen, sizeof(Unicode));
			memcpy(m_pMap, pMap, m_nMapLen * sizeof(Unicode));
		}
		else
		{
			m_pMap = pMap;
		}
		m_pSMap     = pSMap;
		m_nSMapLen  = nSMapLen;
		m_nSMapSize = nSMapSize;

		m_nRef = 1;

		m_oCS.InitializeCriticalSection();
	}

	CharCodeToUnicode::~CharCodeToUnicode()
	{
		if (m_seTag)
		{
			delete m_seTag;
		}
		MemUtilsFree(m_pMap);
		MemUtilsFree(m_pSMap);

		m_oCS.DeleteCriticalSection();
	}

	void CharCodeToUnicode::AddRef()
	{
		CTemporaryCS *pCS = new CTemporaryCS(&m_oCS);

		++m_nRef;

		RELEASEOBJECT(pCS);
	}

	void CharCodeToUnicode::Release()
	{
		CTemporaryCS *pCS = new CTemporaryCS(&m_oCS);

		bool bDelete = (--m_nRef == 0);

		RELEASEOBJECT(pCS);

		if (bDelete)
		{
			delete this;
		}
	}

	bool CharCodeToUnicode::Match(StringExt *seTag)
	{
		return m_seTag && !m_seTag->Compare(seTag);
	}

	void CharCodeToUnicode::SetMapping(CharCode nCode, Unicode *pUnicode, int nLen)
	{
		if (nLen == 1)
		{
			m_pMap[nCode] = pUnicode[0];
		}
		else
		{
			int nIndex = 0;
			for (nIndex = 0; nIndex < m_nSMapLen; ++nIndex)
			{
				if (m_pSMap[nIndex].nCode == nCode)
				{
					break;
				}
			}
			if (nIndex == m_nSMapLen)
			{
				if (m_nSMapLen == m_nSMapSize)
				{
					m_nSMapSize += 8;
					m_pSMap = (CharCodeToUnicodeString *)MemUtilsReallocArray(m_pSMap, m_nSMapSize, sizeof(CharCodeToUnicodeString));
				}
				++m_nSMapLen;
			}
			m_pMap[nCode] = 0;
			m_pSMap[nIndex].nCode = nCode;
			m_pSMap[nIndex].nLen  = nLen;

			for (int nJ = 0; nJ < nLen && nJ < MaxUnicodeString; ++nJ)
			{
				m_pSMap[nIndex].pUnicodeString[nJ] = pUnicode[nJ];
			}
		}
	}

	int CharCodeToUnicode::MapToUnicode(CharCode nCode, Unicode *pUnicode, int size)
	{
		if (nCode >= m_nMapLen)
		{
			return 0;
		}
		if (m_pMap[nCode])
		{
			pUnicode[0] = m_pMap[nCode];
			return 1;
		}
		for (int nIndex = 0; nIndex < m_nSMapLen; ++nIndex)
		{
			if (m_pSMap[nIndex].nCode == nCode)
			{
				int nJ = 0;
				for (nJ = 0; nJ < m_pSMap[nIndex].nLen && nJ < size; ++nJ)
				{
					pUnicode[nJ] = m_pSMap[nIndex].pUnicodeString[nJ];
				}
				return nJ;
			}
		}
		return 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	CharCodeToUnicodeCache::CharCodeToUnicodeCache(int nSize)
	{
		m_nSize = nSize;
		m_ppCache = (CharCodeToUnicode **)MemUtilsMallocArray(m_nSize, sizeof(CharCodeToUnicode *));
		for (int nIndex = 0; nIndex < m_nSize; ++nIndex)
		{
			m_ppCache[nIndex] = NULL;
		}
	}

	CharCodeToUnicodeCache::~CharCodeToUnicodeCache()
	{
		for (int nIndex = 0; nIndex < m_nSize; ++nIndex)
		{
			if (m_ppCache[nIndex])
			{
				m_ppCache[nIndex]->Release();
			}
		}
		MemUtilsFree(m_ppCache);
	}

	CharCodeToUnicode *CharCodeToUnicodeCache::GetCharCodeToUnicode(StringExt *seTag)
	{
		if (m_ppCache[0] && m_ppCache[0]->Match(seTag))
		{
			m_ppCache[0]->AddRef();
			return m_ppCache[0];
		}

		for (int nIndex = 1; nIndex < m_nSize; ++nIndex)
		{
			if (m_ppCache[nIndex] && m_ppCache[nIndex]->Match(seTag))
			{
				CharCodeToUnicode *pCharCodeToUnicode = m_ppCache[nIndex];
				for (int nJ = nIndex; nJ >= 1; --nJ)
				{
					m_ppCache[nJ] = m_ppCache[nJ - 1];
				}
				m_ppCache[0] = pCharCodeToUnicode;
				pCharCodeToUnicode->AddRef();
				return pCharCodeToUnicode;
			}
		}
		return NULL;
	}

	void CharCodeToUnicodeCache::Add(CharCodeToUnicode *pCharCodeToUnicode)
	{
		if (m_ppCache[m_nSize - 1])
		{
			m_ppCache[m_nSize - 1]->Release();
		}

		for (int nIndex = m_nSize - 1; nIndex >= 1; --nIndex)
		{
			m_ppCache[nIndex] = m_ppCache[nIndex - 1];
		}
		m_ppCache[0] = pCharCodeToUnicode;
		pCharCodeToUnicode->AddRef();
	}
}