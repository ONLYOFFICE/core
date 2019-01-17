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
#include <stddef.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "XRef.h"
#include "Array.h"
#include "Dict.h"
#include "Page.h"
#include "Link.h"
#include "Catalog.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Catalog
	//------------------------------------------------------------------------

	Catalog::Catalog(GlobalParams *pGlobalParams, XRef *pXref)
	{
		m_pGlobalParams = pGlobalParams;

		m_bValid = true;
		m_pXref = pXref;
		m_ppPages = NULL;
		m_pPageRefs = NULL;
		m_nPagesCount = m_nPagesSize = 0;
		m_seBaseURI = NULL;

		Object oCatalogDict;
		m_pXref->GetCatalog(&oCatalogDict);

		if (!oCatalogDict.IsDict())
		{
			// TO DO: Error "Catalog object is wrong type "
			oCatalogDict.Free();
			m_oDests.InitNull();
			m_oNameTree.InitNull();
			m_bValid = false;

			return;
		}

		// Считываем Pages
		Object oPagesDict;
		oCatalogDict.DictLookup("Pages", &oPagesDict);

		if (!oPagesDict.IsDict())
		{
			// TO DO: Error "Top-level pages object is wrong type "
			oPagesDict.Free();
			oCatalogDict.Free();
			m_oDests.InitNull();
			m_oNameTree.InitNull();
			m_bValid = false;

			return;
		}

		// Считываем количество страниц
		Object oTemp;
		oPagesDict.DictLookup("Count", &oTemp);
		if (!oTemp.IsNum()) // почему-то иногда количество страниц пишется как вещественное число(поэтому IsNum, а не IsInt)
		{
			// TO DO: Error "Page count in top-level pages object is wrong type"
			oTemp.Free();
			oPagesDict.Free();
			oCatalogDict.Free();
			m_oDests.InitNull();
			m_oNameTree.InitNull();
			m_bValid = false;

			return;
		}

		int nPagesCount = 0;
		m_nPagesSize = nPagesCount = (int)oTemp.GetNum();
		oTemp.Free();

		// выделяем место под соответствующее количество страниц
		m_ppPages   = (Page**)MemUtilsMallocArray(m_nPagesSize, sizeof(Page*));
		m_pPageRefs =   (Ref*)MemUtilsMallocArray(m_nPagesSize, sizeof(Ref));

		for (int nIndex = 0; nIndex < m_nPagesSize; ++nIndex)
		{
			m_ppPages[nIndex] = NULL;
			m_pPageRefs[nIndex].nNum = -1;
			m_pPageRefs[nIndex].nGen = -1;
		}

		char* sAlreadyRead = (char *)MemUtilsMalloc(m_pXref->GetObjectsCount());
		memset(sAlreadyRead, 0, m_pXref->GetObjectsCount());

		Object oPagesDictRef;
		if (oCatalogDict.DictLookupAndCopy("Pages", &oPagesDictRef)->IsRef() && oPagesDictRef.GetRefNum() >= 0 && oPagesDictRef.GetRefNum() < m_pXref->GetObjectsCount())
		{
			sAlreadyRead[oPagesDictRef.GetRefNum()] = 1;
		}
		oPagesDictRef.Free();

		m_nPagesCount = ReadPageTree(oPagesDict.GetDict(), NULL, 0, sAlreadyRead);
		MemUtilsFree(sAlreadyRead);

		if (m_nPagesCount != nPagesCount)
		{
			// TO DO: Error "Page count in top-level pages object is incorrect"
		}
		oPagesDict.Free();

		// Считываем Named destination
		oCatalogDict.DictLookup("Dests", &m_oDests);

		if (oCatalogDict.DictLookup("Names", &oTemp)->IsDict())
			oTemp.DictLookup("Dests", &m_oNameTree);
		else
			m_oNameTree.InitNull();
		oTemp.Free();

		// URI
		if (oCatalogDict.DictLookup("URI", &oTemp)->IsDict())
		{
			Object oBase;
			if (oTemp.DictLookup("Base", &oBase)->IsString())
			{
				m_seBaseURI = oBase.GetString()->Copy();
			}
			oBase.Free();
		}
		oTemp.Free();

		// Metadata
		oCatalogDict.DictLookup("Metadata", &m_oMetadata);

		// StructTreeRoot
		oCatalogDict.DictLookup("StructTreeRoot", &m_oStructTreeRoot);

		// Outlines
		oCatalogDict.DictLookup("Outlines", &m_oOutline);

		// AcroForm
		oCatalogDict.DictLookup("AcroForm", &m_oAcroForm);

		oCatalogDict.Free();
		return;
	}

	Catalog::~Catalog()
	{
		if (m_ppPages)
		{
			for (int nIndex = 0; nIndex < m_nPagesSize; ++nIndex)
			{
				if (m_ppPages[nIndex])
				{
					delete m_ppPages[nIndex];
				}
			}
			MemUtilsFree(m_ppPages);
			MemUtilsFree(m_pPageRefs);
		}
		m_oDests.Free();
		m_oNameTree.Free();

		if (m_seBaseURI)
		{
			delete m_seBaseURI;
		}

		m_oMetadata.Free();
		m_oStructTreeRoot.Free();
		m_oOutline.Free();
		m_oAcroForm.Free();
	}

	StringExt *Catalog::ReadMetadata()
	{
		if (!m_oMetadata.IsStream())
		{
			return NULL;
		}
		Dict *pDict = m_oMetadata.StreamGetDict();
		Object oTemp;
		if (!pDict->Search("Subtype", &oTemp)->IsName("XML"))
		{
			// TO DO: Error "Unknown Metadata type"
		}
		oTemp.Free();
		StringExt *seResult = new StringExt();

		m_oMetadata.StreamReset();
		int nChar = 0;
		// TO DO: Вообще здесь выводится вся xml как она есть, надо бы ее более детально разобрать
		while ((nChar = m_oMetadata.StreamGetChar()) != EOF)
		{
			seResult->Append(nChar);
		}
		m_oMetadata.StreamClose();
		return seResult;
	}

	int Catalog::ReadPageTree(Dict *pPagesDict, PageAttrs *pAttrs, int nStart, char *sAlreadyRead)
	{
		PageAttrs *pPageAttrs = new PageAttrs(pAttrs, pPagesDict);
		Object oKids;
		pPagesDict->Search("Kids", &oKids);

		if (!oKids.IsArray())
		{
			// TO DO: Error "Kids object is wrong type"
			oKids.Free();
			delete pPageAttrs;
			m_bValid = false;
			return -1;
		}

		for (int nIndex = 0; nIndex < oKids.ArrayGetLength(); ++nIndex)
		{
			Object oKidRef;
			oKids.ArrayGetCopy(nIndex, &oKidRef);
			if (oKidRef.IsRef() && oKidRef.GetRefNum() >= 0 && oKidRef.GetRefNum() < m_pXref->GetObjectsCount())
			{
				if (sAlreadyRead[oKidRef.GetRefNum()])
				{
					// TO DO: Error "Loop in Pages tree"
					oKidRef.Free();
					continue;
				}
				sAlreadyRead[oKidRef.GetRefNum()] = 1;
			}
			Object oKid;
			oKids.ArrayGet(nIndex, &oKid);
			if (oKid.IsDict("Page"))
			{
				PageAttrs *pCurPageAttrs = new PageAttrs(pPageAttrs, oKid.GetDict());
				Page *pCurPage = new Page(m_pGlobalParams, m_pXref, nStart + 1, oKid.GetDict(), pCurPageAttrs);
				if (!pCurPage->IsValid())
				{
					++nStart;
					MemUtilsFree(pCurPage);
					oKid.Free();
					oKids.Free();
					MemUtilsFree(pPageAttrs);
					m_bValid = false;
					return -1;
				}
				if (nStart >= m_nPagesSize)
				{
					m_nPagesSize += 32;
					// выделяем дополнительную память
					m_ppPages   = (Page**)MemUtilsReallocArray(m_ppPages, m_nPagesSize, sizeof(Page *));
					m_pPageRefs =   (Ref*)MemUtilsReallocArray(m_pPageRefs, m_nPagesSize, sizeof(Ref));

					for (int nJ = m_nPagesSize - 32; nJ < m_nPagesSize; ++nJ)
					{
						m_ppPages[nJ] = NULL;
						m_pPageRefs[nJ].nNum = -1;
						m_pPageRefs[nJ].nGen = -1;
					}
				}
				m_ppPages[nStart] = pCurPage;
				if (oKidRef.IsRef())
				{
					m_pPageRefs[nStart].nNum = oKidRef.GetRefNum();
					m_pPageRefs[nStart].nGen = oKidRef.GetRefGen();
				}
				++nStart;
			}
			else if (oKid.IsDict()) // иногда почему-то пропускают ключ /Type
			{
				if ((nStart = ReadPageTree(oKid.GetDict(), pPageAttrs, nStart, sAlreadyRead)) < 0)
				{
					oKid.Free();
					oKids.Free();
					MemUtilsFree(pPageAttrs);
					m_bValid = false;
					return -1;
				}
			}
			else
			{
				// TO DO: Error "Kid object is wrong type"
			}
			oKid.Free();
			oKidRef.Free();
		}

		delete pPageAttrs;
		oKids.Free();
		return nStart;
	}

	int Catalog::FindPage(int nNum, int nGen)
	{
		for (int nIndex = 0; nIndex < m_nPagesCount; ++nIndex)
		{
			if (m_pPageRefs[nIndex].nNum == nNum && m_pPageRefs[nIndex].nGen == nGen)
				return nIndex + 1;
		}
		return 0;
	}

	LinkDestination *Catalog::FindDest(StringExt *seName)
	{
		bool bFound = false;

		Object oDestDict;
		if (m_oDests.IsDict())
		{
			if (!m_oDests.DictLookup(seName->GetBuffer(), &oDestDict)->IsNull())
				bFound = true;
			else
				oDestDict.Free();
		}

		if (!bFound && m_oNameTree.IsDict())
		{
			if (!FindDestInTree(&m_oNameTree, seName, &oDestDict)->IsNull())
				bFound = true;
			else
				oDestDict.Free();
		}

		if (!bFound)
			return NULL;

		// construct LinkDest
		LinkDestination *pLinkDest = NULL;

		if (oDestDict.IsArray())
		{
			pLinkDest = new LinkDestination(oDestDict.GetArray());
		}
		else if (oDestDict.IsDict())
		{
			Object oTemp;
			if (oDestDict.DictLookup("D", &oTemp)->IsArray())
				pLinkDest = new LinkDestination(oTemp.GetArray());
			else
			{
				// TO DO: Error "Bad named destination value"
			}
			oTemp.Free();
		}
		else
		{
			// TO DO: Error "Bad named destination value"
		}
		oDestDict.Free();
		if (pLinkDest && !pLinkDest->CheckValidate())
		{
			delete pLinkDest;
			pLinkDest = NULL;
		}

		return pLinkDest;
	}

	Object *Catalog::FindDestInTree(Object *pTree, StringExt *seName, Object *pObject)
	{
		bool bDone = false, bFound = false;

		Object oNames;
		if (pTree->DictLookup("Names", &oNames)->IsArray())
		{
			bDone = bFound = false;

			for (int nIndex = 0; !bDone && nIndex < oNames.ArrayGetLength(); nIndex += 2)
			{
				Object oCurName;
				if (oNames.ArrayGet(nIndex, &oCurName)->IsString())
				{
					int nCompareRes = seName->Compare(oCurName.GetString());
					if (nCompareRes == 0)
					{
						oNames.ArrayGet(nIndex + 1, pObject);
						bFound = true;
						bDone = true;
					}
					else if (nCompareRes < 0)
					{
						bDone = true;
					}
				}
				oCurName.Free();
			}
			oNames.Free();
			if (!bFound)
				pObject->InitNull();
			return pObject;
		}
		oNames.Free();

		bDone = false;

		Object oKids;
		if (pTree->DictLookup("Kids", &oKids)->IsArray())
		{
			for (int nIndex = 0; !bDone && nIndex < oKids.ArrayGetLength(); ++nIndex)
			{
				Object oKid;
				if (oKids.ArrayGet(nIndex, &oKid)->IsDict())
				{
					Object oLimits;
					if (oKid.DictLookup("Limits", &oLimits)->IsArray())
					{
						Object oLow;
						if (oLimits.ArrayGet(0, &oLow)->IsString() && seName->Compare(oLow.GetString()) >= 0)
						{
							Object oHigh;
							if (oLimits.ArrayGet(1, &oHigh)->IsString() && seName->Compare(oHigh.GetString()) <= 0)
							{
								FindDestInTree(&oKid, seName, pObject);
								bDone = true;
							}
							oHigh.Free();
						}
						oLow.Free();
					}
					oLimits.Free();
				}
				oKid.Free();
			}
		}
		oKids.Free();

		if (!bDone)
			pObject->InitNull();

		return pObject;
	}
}
