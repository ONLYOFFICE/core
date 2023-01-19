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