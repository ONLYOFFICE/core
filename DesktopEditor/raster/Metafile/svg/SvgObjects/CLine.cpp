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

#include "CLine.h"

namespace SVG
{
	CLine::CLine(CSvgReader& oReader, CRenderedObject* pParent)
		: CPath(oReader, pParent)
	{
		AddElement(new CMoveElement(Point{0, 0}));
		AddElement(new CLineElement(Point{0, 0}));
	}

	void CLine::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		//TODO:: not ideal, think of how to do this differently
		if ("x1" == sName)
		{
			CMoveElement* pMoveElement{dynamic_cast<CMoveElement*>(operator[](0))};

			if (NULL != pMoveElement)
				pMoveElement->m_oPoint.dX = oReader.GetDouble();
		}
		else if ("y1" == sName)
		{
			CMoveElement* pMoveElement{dynamic_cast<CMoveElement*>(operator[](0))};

			if (NULL != pMoveElement)
				pMoveElement->m_oPoint.dY = oReader.GetDouble();
		}
		else if ("x2" == sName)
		{
			CLineElement* pLineElement{dynamic_cast<CLineElement*>(operator[](1))};

			if (NULL != pLineElement)
				pLineElement->m_oPoint.dX = oReader.GetDouble();
		}
		else if ("y2" == sName)
		{
			CLineElement* pLineElement{dynamic_cast<CLineElement*>(operator[](1))};

			if (NULL != pLineElement)
				pLineElement->m_oPoint.dY = oReader.GetDouble();
		}
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	void CLine::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetMarker(mAttributes, ushLevel, bHardMode);
	}

	void CLine::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, const CRenderedObject* pContexObject) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke, true, pContexObject))
			nTypePath += c_nStroke;
	}
}
