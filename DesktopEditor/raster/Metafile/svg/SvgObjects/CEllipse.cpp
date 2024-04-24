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

#include "CEllipse.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CEllipse::CEllipse(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
	{
		m_oCx.SetValue(oNode.GetAttribute(L"cx"));
		m_oCy.SetValue(oNode.GetAttribute(L"cy"));
		m_oRx.SetValue(oNode.GetAttribute(L"rx"));
		m_oRy.SetValue(oNode.GetAttribute(L"ry"));
	}

	void CEllipse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CEllipse::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX  = m_oCx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY  = m_oCy.ToDouble(NSCSS::Pixel, dParentHeight);
		double dRx = m_oRx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dRy = m_oRy.ToDouble(NSCSS::Pixel, dParentHeight);

		pRenderer->PathCommandMoveTo(dX + dRx, dY);
		pRenderer->PathCommandArcTo(dX - dRx, dY - dRy, dRx * 2.0, dRy * 2.0, 0, 360);

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles);

		return true;
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke))
			nTypePath += c_nStroke;

		if (ApplyFill(pRenderer, &pStyles->m_oFill, pFile, true))
			nTypePath += c_nWindingFillMode;
	}

	TBounds CEllipse::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oCx.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oCy.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oRx.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oRy.ToDouble(NSCSS::Pixel);;

		return oBounds;
	}
}
