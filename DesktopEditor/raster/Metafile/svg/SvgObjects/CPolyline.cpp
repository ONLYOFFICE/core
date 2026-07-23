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

#include "CPolyline.h"

#include "../../../../../Common/3dParty/html/css/src/StaticFunctions.h"

namespace SVG
{
	CPolyline::CPolyline(CSvgReader& oReader, CRenderedObject* pParent)
		: CPath(oReader, pParent)
	{}

	void CPolyline::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("points" == sName)
		{
			const std::vector<double> arValues = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(oReader.GetText());

			if (arValues.size() < 4)
				return;

			AddElement(new CMoveElement(Point{arValues[0], arValues[1]}));

			for (unsigned int unIndex = 2; unIndex < arValues.size(); unIndex += 2)
				AddElement(new CLineElement(Point{arValues[unIndex + 0], arValues[unIndex + 1]}));
		}
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	CPolygon::CPolygon(CSvgReader& oReader, CRenderedObject* pParent)
	    : CPolyline(oReader, pParent)
	{}

	void CPolygon::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		CPolyline::SetAttribute(sName, oReader);
		AddElement(new CCloseElement());
	}
}
