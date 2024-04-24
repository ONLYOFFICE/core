/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "CSymbol.h"

namespace SVG
{
	CSymbol::CSymbol(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager)
		: CGraphicsContainer(oNode)
	{}

	bool CSymbol::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
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

		CGraphicsContainer::Draw(pRenderer, pFile, oMode, pOtherStyles);

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);

		return true;
	}
}

