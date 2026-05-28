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
#include "Catalog.h"
#include "Destination.h"
#include "Pages.h"
#include "Utils.h"
#include "Metadata.h"
#include "Streams.h"
#include "ICCProfile.h"
#include "Annotation.h"

namespace PdfWriter
{
	static const char* c_sPageLayoutNames[] =
	{
		"SinglePage",
		"OneColumn",
		"TwoColumnLeft",
		"TwoColumnRight",
		"TwoPageLeft",
		"TwoPageRight",
		NULL
	};
	static const char* c_sPageModeNames[] =
	{
		"UseNone",
		"UseOutlines",
		"UseThumbs",
		"FullScreen",
		"UseOC",
		"UseAttachments",
		NULL
	};
	const static char* c_sStructElemType[] =
	{
		"Link",
		"Form",
		"Annot",
		NULL
	};
	//----------------------------------------------------------------------------------------
	// CCatalog
	//----------------------------------------------------------------------------------------
	CCatalog::CCatalog(CXref* pXref)
	{
		m_pStructTreeRoot = NULL;

		pXref->Add(this);

		Add("Type", "Catalog");
		Add("Pages", new CPageTree(pXref));

		if (pXref->IsPDFA())
		{
			CDictObject* pMarkInfo = new CDictObject();
			pMarkInfo->Add("Marked", true);
			Add("MarkInfo", pMarkInfo);
			m_pStructTreeRoot = new CStructureTreeRoot(pXref);
			Add("StructTreeRoot", m_pStructTreeRoot);

			if (pXref->IsPDFUA())
			{
				Add("Lang", new CStringObject("en-US"));
				CDictObject* pVP = new CDictObject();
				pVP->Add("DisplayDocTitle", true);
				Add("ViewerPreferences", pVP);
			}

			CArrayObject* pArray = new CArrayObject();
			Add("OutputIntents", pArray);

			CDictObject* pRGB = new CDictObject();
			pArray->Add(pRGB);

			pRGB->Add("Type", "OutputIntent");
			pRGB->Add("S", "GTS_PDFA1");
			pRGB->Add("OutputConditionIdentifier", new CStringObject("sRGB IEC61966-2.1"));

			CDictObject* pRGBProfile = new CDictObject(pXref);
			pRGB->Add("DestOutputProfile", pRGBProfile);
			pRGB->Add("RegistryName", new CStringObject("http://www.color.org"));

			pRGBProfile->Add("N", 3);

#ifndef FILTER_FLATE_DECODE_DISABLED
			pRGBProfile->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
            pRGBProfile->GetStream()->Write((unsigned char*)c_arrICCsRGB, c_nSizeICCsRGB, false);
		}
	}
	CCatalog::CCatalog()
	{
	}
	CPageTree*   CCatalog::GetRoot() const
	{
		return (CPageTree*)Get("Pages");
	}
	EPageLayout  CCatalog::GetPageLayout() const
	{
		CNameObject* pLayout = (CNameObject*)Get("PageLayout");
		if (!pLayout)
			return pagelayout_Single;

		unsigned int unIndex = 0;
		while (c_sPageLayoutNames[unIndex])
		{
			if (0 == StrCmp(pLayout->Get(), c_sPageLayoutNames[unIndex]))
				return (EPageLayout)unIndex;

			unIndex++;
		}

		return pagelayout_Single;
	}
	void         CCatalog::SetPageLayout(EPageLayout eLayout)
	{
		return Add("PageLayout", c_sPageLayoutNames[(int)eLayout]);
	}
	EPageMode    CCatalog::GetPageMode() const
	{
		CNameObject* pMode = (CNameObject*)Get("PageMode");
		if (!pMode)
			return pagemode_UseNone;

		unsigned int unIndex = 0;
		while (c_sPageModeNames[unIndex])
		{
			if (0 == StrCmp(pMode->Get(), c_sPageModeNames[unIndex]))
				return (EPageMode)unIndex;

			unIndex++;
		}

		return pagemode_UseNone;
	}
	void         CCatalog::SetPageMode(EPageMode eMode)
	{
		Add("PageMode", c_sPageModeNames[(int)eMode]);
	}
	void         CCatalog::SetOpenAction(CDestination* pOpenAction)
	{
		if (!pOpenAction)
			Remove("OpenAction");
		else
			Add("OpenAction", pOpenAction);
	}
	void         CCatalog::AddPageLabel(unsigned int unPageNum, CDictObject* pPageLabel)
	{
		CDictObject* pLabels = (CDictObject*)Get("PageLabels");
		if (!pLabels)
		{
			pLabels = new CDictObject();
			if (!pLabels)
				return;

			Add("PageLabels", pLabels);
		}

		CArrayObject* pNums = (CArrayObject*)pLabels->Get("Nums");
		if (!pNums)
		{
			pNums = new CArrayObject();
			if (!pNums)
				return;

			pLabels->Add("Nums", pNums);
		}

		pNums->Add(unPageNum);
		pNums->Add(pPageLabel);
	}
	CMetadata*   CCatalog::AddMetadata(CXref* pXref, CInfoDict* pInfo, const std::string& sAltTitle)
	{
		CMetadata* pMetadata = new CMetadata(pXref, pInfo, sAltTitle);
		Add("Metadata", pMetadata);
		return pMetadata;
	}
	CStructureTreeRoot* CCatalog::GetStructTreeRoot()
	{
		return m_pStructTreeRoot;
	}
	//----------------------------------------------------------------------------------------
	// CStructureTreeRoot
	//----------------------------------------------------------------------------------------
	CStructureTreeRoot::CStructureTreeRoot(CXref* pXref)
	{
		m_pXref = pXref;
		m_nParentTreeKey = 0;
		m_pNamespace     = NULL;
		m_pParentTree    = NULL;
		m_pDocumentElem  = NULL;
		m_pDocumentK     = NULL;

		pXref->Add(this);
		Add("Type", "StructTreeRoot");

		if (pXref->IsPDFUA())
		{
			CDictObject* m_pNamespace = new CDictObject();
			pXref->Add(m_pNamespace);
			m_pNamespace->Add("Type", "Namespace");
			m_pNamespace->Add("NS", new CStringObject("http://iso.org/pdf2/ssn"));

			CArrayObject* pNamespaces = new CArrayObject();
			Add("Namespaces", pNamespaces);
			pNamespaces->Add(m_pNamespace);

			m_pDocumentElem = new CDictObject();
			pXref->Add(m_pDocumentElem);
			m_pDocumentElem->Add("Type", "StructElem");
			m_pDocumentElem->Add("S", "Document");
			m_pDocumentElem->Add("NS", m_pNamespace);
			m_pDocumentElem->Add("P", this);

			m_pDocumentK = new CArrayObject();
			m_pDocumentElem->Add("K", m_pDocumentK);

			Add("K", m_pDocumentElem);

			m_pParentTree = new CDictObject();
			pXref->Add(m_pParentTree);
			CArrayObject* pNums = new CArrayObject();
			m_pParentTree->Add("Nums", pNums);
			Add("ParentTree", m_pParentTree);
			Add("ParentTreeNextKey", 0);
		}
	}
	void CStructureTreeRoot::AddAnnot(CAnnotation* pAnnot, CDictObject* pPage, StructElemType eType)
	{
		if (!m_pDocumentElem || !m_pDocumentK)
			return;

		int nKey = m_nParentTreeKey++;

		pAnnot->Add("StructParent", nKey);
		if (pAnnot->GetAnnotationType() == EAnnotType::AnnotWidget)
		{
			if (!pAnnot->Get("TU"))
				pAnnot->Add("TU", new CStringObject(""));
		}
		else
		{
			if (!pAnnot->Get("Contents"))
				pAnnot->Add("Contents", new CStringObject(""));
		}

		CDictObject* pObjR = new CDictObject();
		pObjR->Add("Type", "OBJR");
		pObjR->Add("Obj", pAnnot);
		if (pPage)
			pObjR->Add("Pg", pPage);

		CDictObject* pLinkElem = new CDictObject();
		m_pXref->Add(pLinkElem);
		pLinkElem->Add("Type", "StructElem");
		pLinkElem->Add("S", c_sStructElemType[(int)eType]);
		pLinkElem->Add("NS", m_pNamespace);
		pLinkElem->Add("P", m_pDocumentElem);
		CArrayObject* pK = new CArrayObject();
		pK->Add(pObjR);
		pLinkElem->Add("K", pK);

		m_pDocumentK->Add(pLinkElem);

		CArrayObject* pNums = (CArrayObject*)m_pParentTree->Get("Nums");
		pNums->Add(nKey);
		pNums->Add(pLinkElem);

		Add("ParentTreeNextKey", m_nParentTreeKey);
	}
}
