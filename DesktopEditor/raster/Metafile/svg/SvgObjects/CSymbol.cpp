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

#include "CSymbol.h"

namespace SVG
{
	CSymbol::CSymbol(CSvgReader& oReader, CRenderedObject *pParent)
		: CGraphicsContainer(oReader)
	{}

	bool CSymbol::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer)
			return false;

		double dScaleX = 1, dScaleY = 1;
		double dTraslateX = m_oWindow.m_oX.ToDouble(NSCSS::Pixel), dTranslateY = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);

		if (!m_oWindow.m_oWidth.Empty() && !m_oViewBox.m_oWidth.Empty())
			dScaleX = m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel);

		if (!m_oWindow.m_oHeight.Empty() && !m_oViewBox.m_oHeight.Empty())
			dScaleY = m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

		double dM11, dM12, dM21, dM22, dDx, dDy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oMatrix(dM11, dM12, dM21, dM22, dDx, dDy);

		oMatrix.Translate(dTraslateX, dTranslateY);
		oMatrix.Scale(dScaleX, dScaleY);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());

		CGraphicsContainer::Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);

		return true;
	}
}

