#include "Outline.h"
#include "Destination.h"

#define OUTLINE_CLOSED     0
#define OUTLINE_OPENED     1

namespace PdfWriter
{ 
	COutline::COutline(CXref* pXref)
	{
		pXref->Add(this);

		CNumberObject* pOpenFlag = new CNumberObject(OUTLINE_OPENED);
		if (!pOpenFlag)
			return;

		pOpenFlag->SetHidden();

		Add("_OPENED", pOpenFlag);
		Add("Type", "Outline");
	}
	COutline::COutline(COutline* pParent, const char* sTitle, CXref* pXref)
	{
		if (!pParent || !pXref)
			return;

		pXref->Add(this);

		CStringObject* pString = new CStringObject(sTitle);
		if (!pString)
			return;

		Add("Title", pString);

		CNumberObject* pOpenFlag = new CNumberObject(OUTLINE_OPENED);
		if (!pOpenFlag)
			return;

		pOpenFlag->SetHidden();

		Add("_OPENED", pOpenFlag);
		Add("Type", "Outline");

		pParent->AddChild(this);
	}
	void         COutline::BeforeWrite()
	{
		CNumberObject* pNumber = (CNumberObject*)Get("Count");
		unsigned int unCount = CountChild();

		if (0 == unCount && pNumber)
			return Remove("Count");

		if (!GetOpened())
			unCount *= -1;

		if (pNumber)
			pNumber->Set(unCount);
		else if (unCount)
			Add("Count", unCount);
	}
	void         COutline::AddChild(COutline* pItem)
	{
		COutline* pFirst = (COutline*)Get("First");
		COutline* pLast  = (COutline*)Get("Last");

		if (!pFirst)
			Add("First", pItem);

		if (pLast)
		{
			pLast->Add("Next", pItem);
			pItem->Add("Prev", pLast);
		}

		Add("Last", pItem);
		pItem->Add("Parent", this);
	}
	unsigned int COutline::CountChild()
	{
		COutline* pOutline = GetFirst();
		unsigned int unCount = 0;

		while (pOutline)
		{
			unCount++;

			if (pOutline->GetOpened())
				unCount += pOutline->CountChild();

			pOutline = pOutline->GetNext();
		}

		return unCount;
	}
	COutline*    COutline::GetFirst()
	{
		return (COutline*)Get("First");
	}
	COutline*    COutline::GetLast  ()
	{
		return (COutline*)Get("Last");
	}
	COutline*    COutline::GetPrev  ()
	{
		return (COutline*)Get("Prev");
	}
	COutline*    COutline::GetNext  ()
	{
		return (COutline*)Get("Next");
	}
	COutline*    COutline::GetParent()
	{
		return (COutline*)Get("Parent");
	}
	bool         COutline::GetOpened()
	{
		CNumberObject* pNumber = (CNumberObject*)Get("_OPENED");

		if (!pNumber)
			return false;

		return (pNumber->Get() == OUTLINE_OPENED ? true : false);
	}
	void         COutline::SetDestination(CDestination* pDestination)
	{
		if (NULL == pDestination)
			Remove("Dest");
		else
			Add("Dest", pDestination);
	}
	void         COutline::SetOpened(bool bOpened)
	{
		CNumberObject* pNumber = (CNumberObject*)Get("_OPENED");
		if (!pNumber)
		{
			pNumber = new CNumberObject(bOpened ? OUTLINE_OPENED : OUTLINE_CLOSED);
			if (pNumber)
				Add("_OPENED", pNumber);
		}
		else
			pNumber->Set(bOpened ? OUTLINE_OPENED : OUTLINE_CLOSED);
	}
}