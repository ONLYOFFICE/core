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

#include "FontStyle.h"
#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
    CFontStyle::CFontStyle() : CBaseStyle(CBaseStyle::eStyleType::stCharacter)
    {
        static UINT iId = 0;
        iId++;
        if (iId < 10)
        {
            m_strStyleId = L"fontstyle0" + std::to_wstring(iId);
        }
        else
        {
            m_strStyleId = L"fontstyle" + std::to_wstring(iId);
        }
    }

    CFontStyle& CFontStyle::operator=(const CFontStyle& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        CBaseStyle::operator=(oSrc);

        m_strStyleId = oSrc.m_strStyleId;

        m_oFont		= oSrc.m_oFont;
        m_oBrush	= oSrc.m_oBrush;

        m_strPickFontName	= oSrc.m_strPickFontName;
        m_lPickFontStyle	= oSrc.m_lPickFontStyle;

        return *this;
    }

    void CFontStyle::CopyFormat(const CFontStyle& oSrc)
    {
        if (this == &oSrc)
        {
            return;
        }

        CBaseStyle::operator=(oSrc);

        m_oFont		= oSrc.m_oFont;
        m_oBrush	= oSrc.m_oBrush;

        m_strPickFontName	= oSrc.m_strPickFontName;
        m_lPickFontStyle	= oSrc.m_lPickFontStyle;
    }

    bool CFontStyle::IsEqual(std::shared_ptr<CFontStyle> oSrc)
    {
        //note Бывают fonts только с разными path => новый стиль => m_oFont.IsEqual не берем
        //todo проверить FaceIndex StringGID
        bool bIf1 = m_oFont.Name == oSrc->m_oFont.Name;
        bool bIf2 = m_oFont.Size == oSrc->m_oFont.Size;
        bool bIf3 = m_oFont.Bold == oSrc->m_oFont.Bold;
        bool bIf4 = m_oFont.Italic == oSrc->m_oFont.Italic;
        bool bIf5 = m_oFont.Underline == oSrc->m_oFont.Underline;
        bool bIf6 = m_oFont.Strikeout == oSrc->m_oFont.Strikeout;

        bool bIf7 = m_oBrush.Type == oSrc->m_oBrush.Type;
        bool bIf8 = m_oBrush.Color1 == oSrc->m_oBrush.Color1;
        bool bIf9 = m_oBrush.Color2 == oSrc->m_oBrush.Color2;
        bool bIf10 = m_oBrush.Alpha1 == oSrc->m_oBrush.Alpha1;
        bool bIf11 = m_oBrush.Alpha2 == oSrc->m_oBrush.Alpha2;
        bool bIf12 = m_oBrush.LinearAngle == oSrc->m_oBrush.LinearAngle;

        //todo
        //        (TexturePath == pBrush->TexturePath) && (TextureAlpha == pBrush->TextureAlpha) && (TextureMode == pBrush->TextureMode) &&
        //        (Rectable == pBrush->Rectable) && (Rect.Equals(pBrush->Rect)));
        //bool bIf7 = m_oBrush.IsEqual(&oSrc->m_oBrush);

        bool bIf13 = m_strPickFontName == oSrc->m_strPickFontName;
        bool bIf14 = m_lPickFontStyle == oSrc->m_lPickFontStyle;

        if (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 &&
            bIf7 && bIf8 && bIf9 && bIf10 && bIf11 && bIf12 &&
            bIf13 && bIf14)
        {
            return true;
        }
        return false;
    }
    \
    void CFontStyle::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
        if (m_bIsNotNecessaryToUse)
        {
            return;
        }

        oWriter.WriteString(L"<w:style");
        oWriter.WriteString(L" w:type=\"character\"");
        oWriter.WriteString(L" w:customStyle=\"1\"");
        oWriter.WriteString(L" w:styleId=\"");
        oWriter.WriteString(m_strStyleId);
        oWriter.WriteString(L"\">");

        oWriter.WriteString(L"<w:name w:val=\"");
        oWriter.WriteString(m_strStyleId);
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<w:basedOn w:val=\"");
        oWriter.WriteString(L"DefaultParagraphFont"); //todo сделать дерево зависимостей
        oWriter.WriteString(L"\"/>");

        //oWriter.WriteString(L"<w:rsid w:val=\"00BB76B0\"/>");

        oWriter.WriteString(L"<w:rPr>");

        std::wstring& strFontName = m_strPickFontName.empty() ? m_oFont.Name : m_strPickFontName;
        oWriter.WriteString(L"<w:rFonts w:ascii=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:hAnsi=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:cs=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:hint=\"default\"/>");

        if (m_strPickFontName.empty())
        {
            if (m_oFont.Bold)
            {
                oWriter.WriteString(L"<w:b/>");
                oWriter.WriteString(L"<w:bCs/>");
            }
            if (m_oFont.Italic)
            {
                oWriter.WriteString(L"<w:i/>");
                oWriter.WriteString(L"<w:iCs/>");
            }
        }
        else
        {
            if (0x01 == (0x01 & m_lPickFontStyle))
            {
                oWriter.WriteString(L"<w:b/>");
                oWriter.WriteString(L"<w:bCs/>");
            }
            if (0x02 == (0x02 & m_lPickFontStyle))
            {
                oWriter.WriteString(L"<w:i/>");
                oWriter.WriteString(L"<w:iCs/>");
            }
        }

        if (ConvertColorBGRToRGB(m_oBrush.Color1) != c_iBlackColor2)
        {
            oWriter.WriteString(L"<w:color w:val=\"");
            oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
            oWriter.WriteString(L"\"/>");
        }

        int lSize = static_cast<int>(2 * m_oFont.Size);
        oWriter.WriteString(L"<w:sz w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/><w:szCs w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"</w:rPr>");

        oWriter.WriteString(L"</w:style>");
    }
}
