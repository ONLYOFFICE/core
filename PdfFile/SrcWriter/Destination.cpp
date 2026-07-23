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
#include "Destination.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CDestination
	//----------------------------------------------------------------------------------------
	CDestination::CDestination(CObjectBase* pPage, CXref* pXref, bool bInline)
	{
		if (!bInline)
			pXref->Add(this);

		// First element of the array must be the page to which the object belongs
		Add(pPage);
		Add("Fit"); // Default value Fit
	}
	bool CDestination::IsValid() const
	{
		if (m_arrList.size() < 2)
			return false;

		// Check that the object is a page. But it can be a reference to a non-editable page
		// CObjectBase* pObject = Get(0, false);
		// if ((object_type_DICT != pObject->GetType() || dict_type_PAGE != ((CDictObject*)pObject)->GetDictType()) &&
		// 		(object_type_PROXY != pObject->GetType() || object_type_DICT != ((CProxyObject*)pObject)->Get()->GetType() || dict_type_PAGE != ((CDictObject*)((CProxyObject*)pObject)->Get())->GetDictType()))
		// 	return false;

		return true;
	}
	void CDestination::ChangePage(CObjectBase* pPage)
	{
		if (!pPage)
			return;
		Insert(Get(0, false), pPage, true);
	}
	void CDestination::PrepareArray()
	{
		if (m_arrList.size() > 1)
		{
			CObjectBase* pPage = Get(0);
			if (pPage->GetType() != object_type_DICT)
			{
				CObjectBase* pCopy = pPage->Copy();
				pCopy->SetRef(pPage->GetObjId(), pPage->GetGenNo());
				pPage = new CProxyObject(pCopy, true);
			}

			Clear();
			Add(pPage);
		}
	}
	void CDestination::SetXYZ(float fLeft, float fTop, float fZoom)
	{
		if (!IsValid())
			return;

		// If zoom parameter is invalid, leave it unchanged (which corresponds to value 0)
		if (fZoom < 0.08 || fZoom > 32)
			fZoom = 0;

        fLeft = std::max(fLeft, 0.f);
        fTop  = std::max(fTop, 0.f);

		PrepareArray();
		Add("XYZ");
		Add(fLeft);
		Add(fTop);
		Add(fZoom);
	}
	void CDestination::SetFit()
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("Fit");
	}
	void CDestination::SetFitH(float fTop)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitH");
		Add(fTop);
	}
	void CDestination::SetFitV(float fLeft)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitV");
		Add(fLeft);
	}
	void CDestination::SetFitR(float fLeft, float fBottom, float fRight, float fTop)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitR");
		Add(fLeft);
		Add(fBottom);
		Add(fRight);
		Add(fTop);
	}
	void CDestination::SetFitB()
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitB");
	}
	void CDestination::SetFitBH(float fTop)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitBH");
		Add(fTop);
	}
	void CDestination::SetFitBV(float fLeft)
	{
		if (!IsValid())
			return;

		PrepareArray();
		Add("FitBV");
		Add(fLeft);
	}
}

