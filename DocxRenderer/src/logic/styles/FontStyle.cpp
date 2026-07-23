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

#include "FontStyle.h"

#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CFontStyle::CFontStyle()
	{
		static LONG lId = 0;
		lId++;
		wsFontStyleId = m_wsIdStart;

		if(lId < 10)
			wsFontStyleId += L"0" + std::to_wstring(lId);
		else
			wsFontStyleId += std::to_wstring(lId);
	}
	CFontStyle::CFontStyle(const CFontStyle& oFontStyle) : CFontStyle()
	{
		*this = oFontStyle;
	}
	CFontStyle::~CFontStyle()
	{
	}

	CFontStyle& CFontStyle::operator=(const CFontStyle& oSrc)
	{
		if (this == &oSrc)
			return *this;

		dFontSize = oSrc.dFontSize;
		oBrush = oSrc.oBrush;

		wsFontName = oSrc.wsFontName;
		bBold = oSrc.bBold;
		bItalic = oSrc.bItalic;
		return *this;
	}
	bool CFontStyle::operator==(const CFontStyle& oSrc)
	{
		bool bIf1 = oBrush.Type == oSrc.oBrush.Type;
		bool bIf2 = oBrush.Color1 == oSrc.oBrush.Color1;
		bool bIf3 = oBrush.Color2 == oSrc.oBrush.Color2;
		bool bIf4 = oBrush.Alpha1 == oSrc.oBrush.Alpha1;
		bool bIf5 = oBrush.Alpha2 == oSrc.oBrush.Alpha2;
		bool bIf6 = oBrush.LinearAngle == oSrc.oBrush.LinearAngle;

		bool bIf7 = dFontSize == oSrc.dFontSize;
		bool bIf8 = wsFontName == oSrc.wsFontName;
		bool bIf9 = (bItalic == oSrc.bItalic) && (bBold == oSrc.bBold);

		//todo
		//        (TexturePath == pBrush->TexturePath) && (TextureAlpha == pBrush->TextureAlpha) && (TextureMode == pBrush->TextureMode) &&
		//        (Rectable == pBrush->Rectable) && (Rect.Equals(pBrush->Rect)));
		//bool bIf7 = m_oBrush.IsEqual(&oSrc->m_oBrush);

		return (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 &&
				bIf7 && bIf8 && bIf9);
	}

	void CFontStyle::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:style");
		oWriter.WriteString(L" w:type=\"character\"");
		oWriter.WriteString(L" w:customStyle=\"1\"");
		oWriter.WriteString(L" w:styleId=\"");
		oWriter.WriteString(wsFontStyleId);
		oWriter.WriteString(L"\">");

		oWriter.WriteString(L"<w:name w:val=\"");
		oWriter.WriteString(wsFontStyleId);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<w:basedOn w:val=\"");
		oWriter.WriteString(L"DefaultParagraphFont"); //todo create dependency tree
		oWriter.WriteString(L"\"/>");

		//oWriter.WriteString(L"<w:rsid w:val=\"00BB76B0\"/>");

		oWriter.WriteString(L"<w:rPr>");

		oWriter.WriteString(L"<w:rFonts w:ascii=\"");
		oWriter.WriteEncodeXmlString(wsFontName);
		oWriter.WriteString(L"\" w:hAnsi=\"");
		oWriter.WriteEncodeXmlString(wsFontName);
		oWriter.WriteString(L"\" w:cs=\"");
		oWriter.WriteEncodeXmlString(wsFontName);
		oWriter.WriteString(L"\" w:hint=\"default\"/>");

		if (bBold)
		{
			oWriter.WriteString(L"<w:b/>");
			oWriter.WriteString(L"<w:bCs/>");
		}
		if (bItalic)
		{
			oWriter.WriteString(L"<w:i/>");
			oWriter.WriteString(L"<w:iCs/>");
		}

		if (ConvertColorBGRToRGB(oBrush.Color1) != c_iBlackColor2)
		{
			oWriter.WriteString(L"<w:color w:val=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(oBrush.Color1));
			oWriter.WriteString(L"\"/>");
		}

		int lSize = static_cast<int>(2 * dFontSize);
		oWriter.WriteString(L"<w:sz w:val=\"");
		oWriter.AddInt(lSize);
		oWriter.WriteString(L"\"/><w:szCs w:val=\"");
		oWriter.AddInt(lSize);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"</w:rPr>");
		oWriter.WriteString(L"</w:style>");
	}
	void CFontStyle::UpdateAvgSpaceWidth(double dWidth)
	{
		dAvgSpaceWidth = (dAvgSpaceWidth / (m_nN + 1)) * m_nN + (dWidth / (m_nN + 1));
		m_nN++;
	}
	double CFontStyle::GetAvgSpaceWidth() const
	{
		return dAvgSpaceWidth;
	}
}
