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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "MemoryUtils.h"
#include "File.h"
#include "StringExt.h"
#include "GlobalParams.h"
#include "PSLexer.h"
#include "CMap.h"

namespace PdfReader
{
	struct CMapVectorEntry
	{
		bool             bIsVector;
		CID              nCID;
		CMapVectorEntry *pVector;
	};

	//-------------------------------------------------------------------------------------------------------------------------------

	static int GetCharFromFile(void *pFile)
	{
		return fgetc((FILE *)pFile);
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	CMap *CMap::Parse(CMapCache *pCache, StringExt *seCollection, StringExt *seCMapName, GlobalParams *pGlobalParams, wchar_t *wsFilePath)
	{
		FILE *pFile = NULL;
		char sToken1[256], sToken2[256], sToken3[256];
		int nLen1, nLen2, nLen3;

		if (NULL != wsFilePath)
		{
			pFile = NSFile::CFileBinary::OpenFileNative(wsFilePath, L"rb");
			if (!pFile)
				return NULL;
		}
		else
		{
			if (pGlobalParams && !(pFile = pGlobalParams->FindCMapFile(seCollection, seCMapName)))
			{
				// Проверяем на Identity CMap.
				if (!seCMapName->Compare("Identity") || !seCMapName->Compare("Identity-H"))
				{
					return new CMap(pGlobalParams, seCollection->Copy(), seCMapName->Copy(), 0);
				}
				if (!seCMapName->Compare("Identity-V"))
				{
					return new CMap(pGlobalParams, seCollection->Copy(), seCMapName->Copy(), 1);
				}

				// TO DO: Error "Couldn't find CMap file for collection"
				return NULL;
			}
		}

		CMap *pCMap = new CMap(pGlobalParams, seCollection->Copy(), seCMapName->Copy());

		PSLexer *Lexer = new PSLexer(&GetCharFromFile, pFile);
		Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1);

		while (Lexer->GetToken(sToken2, sizeof(sToken2), &nLen2))
		{
			if (!strcmp(sToken2, "usecmap"))
			{
				if (sToken1[0] == '/')
				{
					pCMap->UseCMap(pCache, sToken1 + 1);
				}
				Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else if (!strcmp(sToken1, "/WMode"))
			{
				pCMap->m_nWMode = atoi(sToken2);
				Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else if (!strcmp(sToken2, "begincodespacerange"))
			{
				while (Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1))
				{
					if (!strcmp(sToken1, "endcodespacerange"))
					{
						break;
					}
					if (!Lexer->GetToken(sToken2, sizeof(sToken2), &nLen2) || !strcmp(sToken2, "endcodespacerange"))
					{
						// TO DO: Error "Illegal entry in codespacerange block in CMap"
						break;
					}
					if (sToken1[0] == '<' && sToken2[0] == '<' && nLen1 == nLen2 && nLen1 >= 4 && (nLen1 & 1) == 0)
					{
						unsigned int unStart = 0, unEnd = 0;
						sToken1[nLen1 - 1] = sToken2[nLen1 - 1] = '\0';
						sscanf(sToken1 + 1, "%x", &unStart);
						sscanf(sToken2 + 1, "%x", &unEnd);
						nLen1 = (nLen1 - 2) / 2;
						pCMap->AddCodeSpace(pCMap->m_pVector, unStart, unEnd, nLen1);
					}
				}
				Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else if (!strcmp(sToken2, "begincidchar"))
			{
				while (Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1))
				{
					if (!strcmp(sToken1, "endcidchar"))
					{
						break;
					}
					if (!Lexer->GetToken(sToken2, sizeof(sToken2), &nLen2) || !strcmp(sToken2, "endcidchar"))
					{
						// TO DO: Error "Illegal entry in cidchar block in CMap"
						break;
					}
					if (!(sToken1[0] == '<' && sToken1[nLen1 - 1] == '>' && nLen1 >= 4 && (nLen1 & 1) == 0))
					{
						// TO DO: Error "Illegal entry in cidchar block in CMap"
						continue;
					}
					sToken1[nLen1 - 1] = '\0';
					unsigned int unCode = 0;
					if (sscanf(sToken1 + 1, "%x", &unCode) != 1)
					{
						// TO DO: Error "Illegal entry in cidchar block in CMap"
						continue;
					}
					nLen1 = (nLen1 - 2) / 2;
					pCMap->AddCIDs(unCode, unCode, nLen1, (CID)atoi(sToken2));
				}
				Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else if (!strcmp(sToken2, "begincidrange"))
			{
				while (Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1))
				{
					if (!strcmp(sToken1, "endcidrange"))
					{
						break;
					}
					if (!Lexer->GetToken(sToken2, sizeof(sToken2), &nLen2) || !strcmp(sToken2, "endcidrange") || !Lexer->GetToken(sToken3, sizeof(sToken3), &nLen3) || !strcmp(sToken3, "endcidrange"))
					{
						// TO DO: Error "Illegal entry in cidrange block in CMap"
						break;
					}
					if (sToken1[0] == '<' && sToken2[0] == '<' && nLen1 == nLen2 && nLen1 >= 4 && (nLen1 & 1) == 0)
					{
						unsigned int unStart = 0, unEnd = 0;
						sToken1[nLen1 - 1] = sToken2[nLen1 - 1] = '\0';
						sscanf(sToken1 + 1, "%x", &unStart);
						sscanf(sToken2 + 1, "%x", &unEnd);
						nLen1 = (nLen1 - 2) / 2;
						pCMap->AddCIDs(unStart, unEnd, nLen1, (CID)atoi(sToken3));
					}
				}
				Lexer->GetToken(sToken1, sizeof(sToken1), &nLen1);
			}
			else
			{
				strcpy(sToken1, sToken2);
			}
		}
		delete Lexer;

		fclose(pFile);

		return pCMap;
	}

	CMap::CMap(GlobalParams *pGlobalParams, StringExt *seCollection, StringExt *seCMapName)
	{
		m_pGlobalParams = pGlobalParams;

		m_seCollection = seCollection;
		m_seCMapName   = seCMapName;
		m_nWMode = 0;
		m_pVector = (CMapVectorEntry *)MemUtilsMallocArray(256, sizeof(CMapVectorEntry));

		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			m_pVector[nIndex].bIsVector = false;
			m_pVector[nIndex].nCID = 0;
		}
		m_nRef = 1;

		m_oCS.InitializeCriticalSection();
	}

	CMap::CMap(GlobalParams *pGlobalParams, StringExt *seCollection, StringExt *seCMapName, int nWMode)
	{
		m_pGlobalParams = pGlobalParams;

		m_seCollection = seCollection;
		m_seCMapName   = seCMapName;
		m_nWMode  = nWMode;
		m_pVector = NULL;
		m_nRef = 1;

		m_oCS.InitializeCriticalSection();
	}

	void CMap::UseCMap(CMapCache *pCache, char *sUseName)
	{
		StringExt *pUseNameStr = new StringExt(sUseName);
		if (!pUseNameStr)
			return;
		CMap *pSubCMap = pCache->GetCMap(m_seCollection, pUseNameStr, m_pGlobalParams);
		delete pUseNameStr;
		if (!pSubCMap)
			return;
		CopyVector(m_pVector, pSubCMap->m_pVector);
		pSubCMap->Release();
	}

	void CMap::CopyVector(CMapVectorEntry *pDest, CMapVectorEntry *pSrc)
	{
		for (int nSrcIndex = 0; nSrcIndex < 256; ++nSrcIndex)
		{
			if (pSrc[nSrcIndex].bIsVector)
			{
				if (!pDest[nSrcIndex].bIsVector)
				{
					pDest[nSrcIndex].bIsVector = true;
					pDest[nSrcIndex].pVector   = (CMapVectorEntry *)MemUtilsMallocArray(256, sizeof(CMapVectorEntry));
					for (int pDstIndex = 0; pDstIndex < 256; ++pDstIndex)
					{
						pDest[nSrcIndex].pVector[pDstIndex].bIsVector = false;
						pDest[nSrcIndex].pVector[pDstIndex].nCID = 0;
					}
				}
				CopyVector(pDest[nSrcIndex].pVector, pSrc[nSrcIndex].pVector);
			}
			else
			{
				if (pDest[nSrcIndex].bIsVector)
				{
					// TO DO: Error "Collision in usecmap"
				}
				else
				{
					pDest[nSrcIndex].nCID = pSrc[nSrcIndex].nCID;
				}
			}
		}
	}

	void CMap::AddCodeSpace(CMapVectorEntry *pVector, unsigned int unStart, unsigned int unEnd, unsigned int unBytesCount)
	{
		if (unBytesCount > 1)
		{
			int nStartByte = (unStart >> (8 * (unBytesCount - 1))) & 0xff;
			int nEndByte   = (unEnd >> (8 * (unBytesCount - 1))) & 0xff;
			unsigned int unStart2 = unStart & ((1 << (8 * (unBytesCount - 1))) - 1);
			unsigned int unEnd2   = unEnd   & ((1 << (8 * (unBytesCount - 1))) - 1);
			for (int nI = nStartByte; nI <= nEndByte; ++nI)
			{
				if (!pVector[nI].bIsVector)
				{
					pVector[nI].bIsVector = true;
					pVector[nI].pVector = (CMapVectorEntry *)MemUtilsMallocArray(256, sizeof(CMapVectorEntry));
					for (int nJ = 0; nJ < 256; ++nJ)
					{
						pVector[nI].pVector[nJ].bIsVector = false;
						pVector[nI].pVector[nJ].nCID = 0;
					}
				}
				AddCodeSpace(pVector[nI].pVector, unStart2, unEnd2, unBytesCount - 1);
			}
		}
	}

	void CMap::AddCIDs(unsigned int unStart, unsigned int unEnd, unsigned int unBytesCount, CID nFirstCID)
	{
		CMapVectorEntry *pVector = m_pVector;
		for (unsigned int nIndex = unBytesCount - 1; nIndex >= 1; --nIndex)
		{
			int nByte = (unStart >> (8 * nIndex)) & 0xff;
			if (!pVector[nByte].bIsVector)
			{
				// TO DO: Error "Invalid CID in CMap"
				return;
			}
			pVector = pVector[nByte].pVector;
		}
		CID nCID = nFirstCID;
		for (int nByte = (int)(unStart & 0xff); nByte <= (int)(unEnd & 0xff); ++nByte)
		{
			if (pVector[nByte].bIsVector)
			{
				// TO DO: Error "Invalid CID in CMap"
			}
			else
			{
				pVector[nByte].nCID = nCID;
			}
			++nCID;
		}
	}

	CMap::~CMap()
	{
		if (m_seCollection)
			delete m_seCollection;

		if (m_seCMapName)
			delete m_seCMapName;
		if (m_pVector)
		{
			FreeCMapVector(m_pVector);
		}

		m_oCS.DeleteCriticalSection();
	}

	void CMap::FreeCMapVector(CMapVectorEntry *pVector)
	{
		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			if (pVector[nIndex].bIsVector)
			{
				FreeCMapVector(pVector[nIndex].pVector);
			}
		}
		MemUtilsFree(pVector);
	}

	void CMap::AddRef()
	{
		CTemporaryCS *pCS = new CTemporaryCS(&m_oCS);
		++m_nRef;

		RELEASEOBJECT(pCS);
	}

	void CMap::Release()
	{
		CTemporaryCS *pCS = new CTemporaryCS(&m_oCS);

		bool bDelete = (--m_nRef == 0);

		RELEASEOBJECT(pCS);

		if (bDelete)
		{
			delete this;
		}
	}

	bool CMap::Match(StringExt *seCollection, StringExt *seCMapName)
	{
		return !m_seCollection->Compare(seCollection) && !m_seCMapName->Compare(seCMapName);
	}

	CID CMap::GetCID(char *sChar, int nLen, int *pnUsed)
	{
		CMapVectorEntry *pVector;

		if (!(pVector = m_pVector))
		{
			// Identity CMap
			*pnUsed = 2;
			if (nLen < 2)
			{
				return 0;
			}
			return ((sChar[0] & 0xff) << 8) + (sChar[1] & 0xff);
		}
		int nUsedCount = 0;
		while (1)
		{
			if (nUsedCount >= nLen)
			{
				*pnUsed = nUsedCount;
				return 0;
			}
			int nIndex = sChar[nUsedCount++] & 0xff;
			if (!pVector[nIndex].bIsVector)
			{
				*pnUsed = nUsedCount;
				return pVector[nIndex].nCID;
			}
			pVector = pVector[nIndex].pVector;
		}
	}

	void CMap::ToXml(std::wstring wsFilePath)
	{
		CXmlWriter oWriter;
		oWriter.WriteNodeBegin(L"PDF-CMap", true);
		oWriter.WriteAttribute(L"collection", m_seCollection->GetWString());
		oWriter.WriteAttribute(L"name", m_seCMapName->GetWString());
		oWriter.WriteNodeEnd(L"PDF-CMap", true, false);

		if (!m_seCMapName->Compare("Identity") || !m_seCMapName->Compare("Identity-H") || !m_seCMapName->Compare("Identity-V"))
			WriteVectorToXml(&oWriter, m_pVector);

		oWriter.WriteNodeEnd(L"PDF-CMap", false, true);
		oWriter.SaveToFile(wsFilePath);
	}

	void CMap::WriteVectorToXml(CXmlWriter *pWriter, CMapVectorEntry *pVector)
	{
		if (NULL == pVector)
			return;

		for (int nIndex = 0; nIndex < 256; nIndex++)
		{
			if (0 != pVector[nIndex].nCID || false != pVector[nIndex].bIsVector)
			{
				if (pVector[nIndex].bIsVector)
				{
					pWriter->WriteNodeBegin(L"Vector", true);
					pWriter->WriteAttribute(L"index", nIndex);
					pWriter->WriteAttribute(L"isvector", 1);
					pWriter->WriteAttribute(L"cid", (int)pVector[nIndex].nCID);
					pWriter->WriteNodeEnd(L"Vector", true, false);

					WriteVectorToXml(pWriter, pVector[nIndex].pVector);

					pWriter->WriteNodeEnd(L"Vector", false, true);
				}
				else
				{
					pWriter->WriteNodeBegin(L"Vector", true);
					pWriter->WriteAttribute(L"index", nIndex);
					pWriter->WriteAttribute(L"isvector", 0);
					pWriter->WriteAttribute(L"cid", (int)pVector[nIndex].nCID);
					pWriter->WriteNodeEnd(L"Vector", true, true);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------

	CMapCache::CMapCache()
	{
		for (int nIndex = 0; nIndex < CMapCacheSize; ++nIndex)
		{
			m_ppCache[nIndex] = NULL;
		}
	}

	CMapCache::~CMapCache()
	{
		for (int nIndex = 0; nIndex < CMapCacheSize; ++nIndex)
		{
			if (m_ppCache[nIndex])
			{
				m_ppCache[nIndex]->Release();
			}
		}
	}

	CMap *CMapCache::GetCMap(StringExt *seCollection, StringExt *seCMapName, GlobalParams* pGlobalParams, wchar_t *wsFilePath)
	{
		CMap *pCMap = NULL;

		if (m_ppCache[0] && m_ppCache[0]->Match(seCollection, seCMapName))
		{
			m_ppCache[0]->AddRef();
			return m_ppCache[0];
		}

		for (int nIndex = 1; nIndex < CMapCacheSize; ++nIndex)
		{
			if (m_ppCache[nIndex] && m_ppCache[nIndex]->Match(seCollection, seCMapName))
			{
				pCMap = m_ppCache[nIndex];
				for (int nJ = nIndex; nJ >= 1; --nJ)
				{
					m_ppCache[nJ] = m_ppCache[nJ - 1];
				}
				m_ppCache[0] = pCMap;
				pCMap->AddRef();
				return pCMap;
			}
		}
		if ((pCMap = CMap::Parse(this, seCollection, seCMapName, pGlobalParams, wsFilePath)))
		{
			if (m_ppCache[CMapCacheSize - 1])
			{
				m_ppCache[CMapCacheSize - 1]->Release();
			}
			for (int nJ = CMapCacheSize - 1; nJ >= 1; --nJ)
			{
				m_ppCache[nJ] = m_ppCache[nJ - 1];
			}
			m_ppCache[0] = pCMap;
			pCMap->AddRef();
			return pCMap;
		}
		return NULL;
	}
}
