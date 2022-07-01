#include "ContText.h"
#include "../resources/ColorTable.h"
#include "../resources/Constants.h"
#include "../resources/SingletonTemplate.h"
#include "../resources/utils.h"

namespace NSDocxRenderer
{
    CContText::CContText()
    {
        m_strPickFontName	= L"";
        m_lPickFontStyle	= 0;

        m_dX			= 0;
        m_dY			= 0;
        m_dWidth		= 0;
        m_dHeight		= 0;
        m_dLastX        = 0;

        m_dSpaceWidthMM	= 0;
        m_dBaselineOffset = 0;

        m_bIsNeedSpaceAtTheEnd = false;
        m_bIsHighlightPresent = false;
        m_lHighlightColor = c_iBlackColor;

        m_eUnderlineType = utUnknown;

        m_pShape         = nullptr;
    }

    void CContText::Clear()
    {
    }

    CContText::CContText(const CContText& oSrc)
    {
        *this = oSrc;
    }

    CContText& CContText::operator=(const CContText& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        m_oFont		= oSrc.m_oFont;
        m_oBrush	= oSrc.m_oBrush;

        m_strPickFontName	= oSrc.m_strPickFontName;
        m_lPickFontStyle	= oSrc.m_lPickFontStyle;

        m_oText	 = oSrc.m_oText;

        m_dX		= oSrc.m_dX;
        m_dY		= oSrc.m_dY;
        m_dWidth	= oSrc.m_dWidth;
        m_dHeight	= oSrc.m_dHeight;
        m_dLastX    = oSrc.m_dLastX;

        m_dSpaceWidthMM = oSrc.m_dSpaceWidthMM;
        m_dBaselineOffset = oSrc.m_dBaselineOffset;

        m_bIsNeedSpaceAtTheEnd = oSrc.m_bIsNeedSpaceAtTheEnd;
        m_bIsHighlightPresent = oSrc.m_bIsHighlightPresent;
        m_lHighlightColor = oSrc.m_lHighlightColor;

        m_eUnderlineType = oSrc.m_eUnderlineType;

        m_pShape    = oSrc.m_pShape;

        return *this;
    }

    bool CContText::IsBigger(const CContText* oSrc)
    {
        return (m_dX > oSrc->m_dX) ? true : false;
    }

    bool CContText::IsBiggerOrEqual(const CContText* oSrc)
    {
        return (m_dX >= oSrc->m_dX) ? true : false;
    }

    double CContText::GetIntersect(const CContText* oSrc) const
    {
        double d1 = std::max(m_dX, oSrc->m_dX);
        double d2 = std::min(m_dX + m_dWidth, oSrc->m_dX + oSrc->m_dWidth);

        if (d2 > d1)
            return d2 - d1;
        return 0;
    }

    void CContText::ToXml(NSStringUtils::CStringBuilder& oWriter,
                          CFontManagerLight* pManagerLight,
                          bool bIsAddSpace)
    {
        oWriter.WriteString(L"<w:r>");
        oWriter.WriteString(L"<w:rPr>");

        if (m_strPickFontName.empty())
        {
            if (m_oFont.Bold)
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (m_oFont.Italic)
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            if (bIsAddSpace || m_bIsNeedSpaceAtTheEnd)
            {
                m_dWidth += m_dSpaceWidthMM;
                m_oText += L" ";
            }
        }
        else
        {
            if (0x01 == (0x01 & m_lPickFontStyle))
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (0x02 == (0x02 & m_lPickFontStyle))
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            if (bIsAddSpace || m_bIsNeedSpaceAtTheEnd)
            {
                m_dWidth  += pManagerLight->GetSpaceWidth();
                m_oText += L" ";
            }

            // нужно перемерять...
            double ___dSize = (double)(static_cast<LONG>(m_oFont.Size * 2)) / 2;
            pManagerLight->LoadFont(m_strPickFontName, m_lPickFontStyle, ___dSize, false);
            double dWidth = pManagerLight->MeasureStringWidth(m_oText.ToStdWString());

            double dSpacing = (m_dWidth - dWidth) / (m_oText.length() + 1);
            dSpacing *= c_dMMToDx;

            LONG lSpacing = static_cast<LONG>(dSpacing);
            //note принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
            lSpacing -= 1;

            if (lSpacing != 0)
            {
                oWriter.WriteString(L"<w:spacing w:val=\"");
                oWriter.AddInt(lSpacing);
                oWriter.WriteString(L"\"/>");
            }
        }

        int lSize = static_cast<int>(2 * m_oFont.Size);
        oWriter.WriteString(L"<w:sz w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/><w:szCs w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/>");

        std::wstring& strFontName = m_strPickFontName.empty() ? m_oFont.Name : m_strPickFontName;
        oWriter.WriteString(L"<w:rFonts w:ascii=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:hAnsi=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:cs=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\"/>");

        if (ConvertColorBGRToRGB(m_oBrush.Color1) != c_iBlackColor)
        {
            oWriter.WriteString(L"<w:color w:val=\"");
            oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
            oWriter.WriteString(L"\"/>");
        }

        if (m_oFont.Strikeout == TRUE)
        {
            oWriter.WriteString(L"<w:strike/>");
        }

        if (m_oFont.Underline == TRUE)
        {
            switch (m_eUnderlineType)
            {
            case utThinLine:
                oWriter.WriteString(L"<w:u w:val=\"single\"/>");
                break;
            case utThickLine:
                oWriter.WriteString(L"<w:u w:val=\"thick\"/>");
                break;
            case utDoubleThinLine:
                oWriter.WriteString(L"<w:u w:val=\"double\"/>");
                break;
            default:
                break;
            }
        }

        if (m_bIsHighlightPresent)
        {
            ColorTable& colorTable = SingletonInstance<ColorTable>();
            if (colorTable.IsStandardColor(m_lHighlightColor))
            {
                oWriter.WriteString(L"<w:highlight w:val=\"");
                oWriter.WriteString(colorTable.ConverColorToString(ConvertColorBGRToRGB(m_lHighlightColor)));
            }
            else
            {
                oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lHighlightColor));
            }
            oWriter.WriteString(L"\"/>");
        }

        oWriter.WriteString(L"</w:rPr>");

        oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
        oWriter.WriteEncodeXmlString(m_oText.ToStdWString());
        oWriter.WriteString(L"</w:t>");

        oWriter.WriteString(L"</w:r>");
    }

    void CContText::AddWideSpaceToXml(double dSpacingMM,
                                      NSStringUtils::CStringBuilder& oWriter,
                                      CFontManagerLight* pManagerLight,
                                      bool bIsNeedSaveFormat)
    {
        oWriter.WriteString(L"<w:r><w:rPr>");

        double dSpaceMMSize = m_dSpaceWidthMM;
        if (m_strPickFontName.empty())
        {
            if (m_oFont.Bold && bIsNeedSaveFormat)
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (m_oFont.Italic && bIsNeedSaveFormat)
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");
        }
        else
        {
            if (0x01 == (0x01 & m_lPickFontStyle) && bIsNeedSaveFormat)
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (0x02 == (0x02 & m_lPickFontStyle) && bIsNeedSaveFormat)
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            dSpaceMMSize = pManagerLight->GetSpaceWidth();
        }

        int lSize = (int)(2 * m_oFont.Size);
        oWriter.WriteString(L"<w:sz w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/><w:szCs w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/>");

        std::wstring& strFontName = m_strPickFontName.empty() ? m_oFont.Name : m_strPickFontName;
        oWriter.WriteString(L"<w:rFonts w:ascii=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:hAnsi=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:cs=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<w:color w:val=\"");
        oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
        oWriter.WriteString(L"\"/>");

        LONG lSpacing = static_cast<LONG>((dSpacingMM - dSpaceMMSize) * c_dMMToDx);
        //note принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
        lSpacing -= 1;
        if (lSpacing != 0)
        {
            oWriter.WriteString(L"<w:spacing w:val=\"");
            oWriter.AddInt(lSpacing);
            oWriter.WriteString(L"\"/>");
        }

        if (ConvertColorBGRToRGB(m_oBrush.Color1) != c_iBlackColor)
        {
            oWriter.WriteString(L"<w:color w:val=\"");
            oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
            oWriter.WriteString(L"\"/>");
        }

        if (m_oFont.Strikeout == TRUE  && bIsNeedSaveFormat)
        {
            oWriter.WriteString(L"<w:strike/>");
        }

        if (m_oFont.Underline == TRUE  && bIsNeedSaveFormat)
        {
            switch (m_eUnderlineType)
            {
            case utThinLine:
                oWriter.WriteString(L"<w:u w:val=\"single\"/>");
                break;
            case utThickLine:
                oWriter.WriteString(L"<w:u w:val=\"thick\"/>");
                break;
            case utDoubleThinLine:
                oWriter.WriteString(L"<w:u w:val=\"double\"/>");
                break;
            default:
                break;
            }
        }

        if (m_bIsHighlightPresent && bIsNeedSaveFormat)
        {
            ColorTable& colorTable = SingletonInstance<ColorTable>();
            if (colorTable.IsStandardColor(m_lHighlightColor))
            {
                oWriter.WriteString(L"<w:highlight w:val=\"");
                oWriter.WriteString(colorTable.ConverColorToString(ConvertColorBGRToRGB(m_lHighlightColor)));
            }
            else
            {
                oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lHighlightColor));
            }
            oWriter.WriteString(L"\"/>");
        }


        oWriter.WriteString(L"</w:rPr>");

        oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
        oWriter.WriteString(L" ");
        oWriter.WriteString(L"</w:t>");

        oWriter.WriteString(L"</w:r>");
    }

    void CContText::AddSpaceToEnd()
    {
        m_bIsNeedSpaceAtTheEnd = true;
        m_dWidth += m_dSpaceWidthMM;
    }

    bool CContText::IsEqual(const CContText* oSrc)
    {
        if( m_strPickFontName == oSrc->m_strPickFontName &&
            m_eUnderlineType == oSrc->m_eUnderlineType &&
            m_bIsHighlightPresent == oSrc->m_bIsHighlightPresent &&
            m_lHighlightColor == oSrc->m_lHighlightColor &&
            m_pShape == oSrc->m_pShape &&
            m_oFont.IsEqual(&oSrc->m_oFont) &&
            m_oBrush.IsEqual(&oSrc->m_oBrush))
        {
            return true;
        }
        return false;
    }
}
