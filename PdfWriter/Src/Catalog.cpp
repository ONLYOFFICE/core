#include "Catalog.h"
#include "Destination.h"
#include "Pages.h"
#include "Utils.h"

namespace PdfWriter
{
	static const char* c_sPageLayoutNames[] =
	{
		"SinglePage",
		"OneColumn",
		"TwoColumnLeft",
		"TwoColumnRight",
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
}