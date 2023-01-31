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
#include "Destination.h"
#include "Pages.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CDestination
	//----------------------------------------------------------------------------------------
	CDestination::CDestination(CPage* pPage, CXref* pXref)
	{
		pXref->Add(this);

		// Первый элемент массива должен быть страницей, которой принадлежит объект
		Add((CObjectBase*)pPage);
		Add("Fit"); // Значение по умолчанию Fit
	}
	bool CDestination::IsValid() const
	{
		if (m_arrList.size() < 2)
			return false;

		CObjectBase* pObject = Get(0, false);
		if ((object_type_DICT != pObject->GetType() || dict_type_PAGE != ((CDictObject*)pObject)->GetDictType()) &&
				(object_type_PROXY != pObject->GetType() || object_type_DICT != ((CProxyObject*)pObject)->Get()->GetType() || dict_type_PAGE != ((CDictObject*)((CProxyObject*)pObject)->Get())->GetDictType()))
			return false;

		return true;
	}
	void CDestination::PrepareArray()
	{
		CPage* pPage = (CPage*)Get(0);

		if (m_arrList.size() > 1)
		{
			Clear();
			Add((CObjectBase*)pPage);
		}
	}
	void CDestination::SetXYZ(float fLeft, float fTop, float fZoom)
	{
		if (!IsValid())
			return;

		// Если параметр приближения задан некорректно, тогда оставляем его нетронутым(что соответствует значению 0)
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

