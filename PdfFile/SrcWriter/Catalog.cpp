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
#include "Catalog.h"
#include "Destination.h"
#include "Pages.h"
#include "Utils.h"
#include "Metadata.h"
#include "Streams.h"
#include "ICCProfile.h"

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
	//----------------------------------------------------------------------------------------
	// CCatalog
	//----------------------------------------------------------------------------------------
	CCatalog::CCatalog(CXref* pXref)
	{
		pXref->Add(this);

		Add("Type", "Catalog");
		Add("Pages", new CPageTree(pXref));

		if (pXref->IsPDFA())
		{
			CDictObject* pMarkInfo = new CDictObject();
			pMarkInfo->Add("Marked", true);
			Add("MarkInfo", pMarkInfo);
			Add("StructTreeRoot", new CStructureTreeRoot(pXref));


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
	CCatalog::CCatalog(CXref* pXref, bool bEmpty)
	{
		pXref->Add(this);
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
	CMetadata*   CCatalog::AddMetadata(CXref* pXref, CInfoDict* pInfo)
	{
		CMetadata* pMetadata = new CMetadata(pXref, pInfo);
		Add("Metadata", pMetadata);
		return pMetadata;
	}
	//----------------------------------------------------------------------------------------
	// CStructureTreeRoot
	//----------------------------------------------------------------------------------------
	CStructureTreeRoot::CStructureTreeRoot(CXref* pXref)
	{
		pXref->Add(this);
		Add("Type", "StructTreeRoot");
	}
}
