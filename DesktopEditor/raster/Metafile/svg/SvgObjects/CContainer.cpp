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

#include "CContainer.h"

#include "../CSvgFile.h"
#include "../SvgUtils.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(CSvgReader& oReader, CRenderedObject *pParent)
		: CRenderedObject(oReader, pParent)
	{}

	void CGraphicsContainer::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("x" == sName)
			m_oWindow.m_oX.SetValue(oReader.GetText());
		else if ("y" == sName)
			m_oWindow.m_oY.SetValue(oReader.GetText());
		else if ("width" == sName)
			m_oWindow.m_oWidth.SetValue(oReader.GetText());
		else if ("height" == sName)
			m_oWindow.m_oHeight.SetValue(oReader.GetText());
		else if ("viewBox" == sName)
		{
			const std::vector<double> arValues{StrUtils::ReadDoubleValues(oReader.GetText())};
			if (4 == arValues.size())
			{
				m_oViewBox.m_oX      = arValues[0];
				m_oViewBox.m_oY      = arValues[1];
				m_oViewBox.m_oWidth  = arValues[2];
				m_oViewBox.m_oHeight = arValues[3];
			}
		}
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, CommandeModeDraw))
			return false;

		for (const CRenderedObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);

		EndPath(pRenderer, pFile, oOldTransform, CommandeModeDraw, pOtherStyles, pContexObject);

		return true;
	}

	TRect CGraphicsContainer::GetWindow() const
	{
		return m_oWindow;
	}

	TRect CGraphicsContainer::GetViewBox() const
	{
		return m_oViewBox;
	}

	TBounds CGraphicsContainer::GetBounds(SvgMatrix* pTransform) const
	{
		TBounds oBounds, oTempBounds;

		oBounds.m_dLeft    = oBounds.m_dRight  = m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop     = oBounds.m_dBottom = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  += m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom += m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		if (nullptr != pTransform)
			*pTransform += m_oTransformation.m_oTransform.GetMatrix();

		for (const CRenderedObject* pObject : m_arObjects)
		{
			oTempBounds = pObject->GetBounds(pTransform);
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		if (nullptr != pTransform)
			*pTransform -= m_oTransformation.m_oTransform.GetMatrix();

		return oBounds;
	}

}
