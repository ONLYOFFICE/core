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

#include "CContainer.h"

#include "../CSvgFile.h"
#include "../SvgUtils.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(const std::wstring &wsName)
		: CRenderedObject(NSCSS::CNode(wsName, L"", L""))
	{}

	CGraphicsContainer::~CGraphicsContainer()
	{
		for (CRenderedObject* pObject : m_arObjects)
			pObject->m_pParent = NULL;
	}

	void CGraphicsContainer::SetData(XmlUtils::CXmlNode &oNode)
	{
		SetNodeData(oNode);

		m_oWindow.m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oWindow.m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oWindow.m_oWidth .SetValue(oNode.GetAttribute(L"width"), 0, true);
		m_oWindow.m_oHeight.SetValue(oNode.GetAttribute(L"height"), 0, true);

		const std::wstring wsViewBox = oNode.GetAttribute(L"viewBox");

		if (!wsViewBox.empty())
		{
			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsViewBox);
			if (4 == arValues.size())
			{
				m_oViewBox.m_oX      = arValues[0];
				m_oViewBox.m_oY      = arValues[1];
				m_oViewBox.m_oWidth  = arValues[2];
				m_oViewBox.m_oHeight = arValues[3];
			}
		}
		else
			m_oViewBox = m_oWindow;
	}

	CGraphicsContainer::CGraphicsContainer(XmlUtils::CXmlNode& oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
	{
		SetData(oNode);
	}

	CGraphicsContainer::CGraphicsContainer(double dWidth, double dHeight, XmlUtils::CXmlNode& oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent), m_oWindow{0, 0, dWidth, dHeight}
	{}

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		for (const CRenderedObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pFile, oMode, pOtherStyles);

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles);

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

	TBounds CGraphicsContainer::GetBounds() const
	{
		TBounds oBounds, oTempBounds;

		oBounds.m_dLeft    = oBounds.m_dRight  = m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop     = oBounds.m_dBottom = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  += m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom += m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		for (const CRenderedObject* pObject : m_arObjects)
		{
			oTempBounds = pObject->GetBounds();
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}

}
