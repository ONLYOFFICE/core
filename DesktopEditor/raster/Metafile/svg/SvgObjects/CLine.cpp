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

#include "CLine.h"

namespace SVG
{
	CLine::CLine(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent)
		: CPath(oNode, pParent, false)
	{	
		SvgDigit oX1;
		SvgDigit oY1;
		SvgDigit oX2;
		SvgDigit oY2;

		oX1.SetValue(oNode.GetAttribute(L"x1"));
		oY1.SetValue(oNode.GetAttribute(L"y1"));
		oX2.SetValue(oNode.GetAttribute(L"x2"));
		oY2.SetValue(oNode.GetAttribute(L"y2"));

		AddElement(new CMoveElement(Point{oX1.ToDouble(NSCSS::Pixel), oY1.ToDouble(NSCSS::Pixel)}));
		AddElement(new CLineElement(Point{oX2.ToDouble(NSCSS::Pixel), oY2.ToDouble(NSCSS::Pixel)}));
	}

	void CLine::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetMarker(mAttributes, ushLevel, bHardMode);
	}

	void CLine::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath) const
	{
            if (ApplyStroke(pRenderer, &pStyles->m_oStroke, true))
			nTypePath += c_nStroke;
	}
}
