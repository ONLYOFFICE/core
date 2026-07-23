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

#include "DrawParam.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

namespace OFD
{
CDrawParam::CDrawParam(CXmlReader& oXmlReader)
	: IOFDElement(oXmlReader), m_oPenSettings(oXmlReader),
	  m_pStrokeColor(nullptr), m_pFillColor(nullptr)
{
	std::string sName;
	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		sName = oXmlReader.GetNameA();

		if ("ofd:FillColor" == sName)
		{
			if (nullptr != m_pFillColor)
				delete m_pFillColor;

			m_pFillColor = new CColor(oXmlReader);
		}
		else if ("ofd:StrokeColor" == sName)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new CColor(oXmlReader);
		}
	}
}

CDrawParam::~CDrawParam()
{
	if (nullptr != m_pStrokeColor)
		delete m_pStrokeColor;

	if (nullptr != m_pFillColor)
		delete m_pFillColor;
}

bool CDrawParam::ApplyStrokeColor(IRenderer* pRenderer, const CRes* pPublicRes) const
{
	if (nullptr == pRenderer || nullptr == m_pStrokeColor)
		return false;

	m_oPenSettings.Apply(pRenderer);

	pRenderer->put_PenColor(m_pStrokeColor->ToInt(pPublicRes));
	pRenderer->put_PenAlpha(m_pStrokeColor->GetAlpha());

	return true;
}

bool CDrawParam::ApplyFillColor(IRenderer* pRenderer, const CRes* pPublicRes) const
{
	if (nullptr == pRenderer || nullptr == m_pFillColor)
		return false;

	pRenderer->put_BrushColor1(m_pFillColor->ToInt(pPublicRes));
	pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());

	return true;
}
}
