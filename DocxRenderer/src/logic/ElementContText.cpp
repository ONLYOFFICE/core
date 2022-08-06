#include "ElementContText.h"
#include "../resources/ColorTable.h"
#include "../resources/SingletonTemplate.h"
#include "../resources/utils.h"

namespace NSDocxRenderer
{
    CContText::CContText(CFontManagerLight& oManagerLight): CBaseItem(ElemType::etContText),
        m_pManagerLight(&oManagerLight)
    {
    }

    void CContText::Clear()
    {
    }

    CContText::CContText(const CContText& oSrc): CBaseItem(ElemType::etContText)
    {
        *this = oSrc;
    }

    CContText& CContText::operator=(const CContText& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        CBaseItem::operator=(oSrc);

        m_oFont		= oSrc.m_oFont;
        m_oBrush	= oSrc.m_oBrush;

        m_strPickFontName	= oSrc.m_strPickFontName;
        m_lPickFontStyle	= oSrc.m_lPickFontStyle;

        m_oText	 = oSrc.m_oText;

        m_dBaselineOffset = oSrc.m_dBaselineOffset;
        m_dLastX    = oSrc.m_dLastX;
        m_dSpaceWidthMM = oSrc.m_dSpaceWidthMM;

        m_bIsNeedSpace = oSrc.m_bIsNeedSpace;
        m_bIsDoubleStrikeout = oSrc.m_bIsDoubleStrikeout;
        m_bIsHighlightPresent = oSrc.m_bIsHighlightPresent;
        m_lHighlightColor = oSrc.m_lHighlightColor;

        m_eUnderlineType = oSrc.m_eUnderlineType;
        m_lUnderlineColor = oSrc.m_lUnderlineColor;

        m_eVertAlignType = oSrc.m_eVertAlignType;

        m_bIsShadowPresent = oSrc.m_bIsShadowPresent;
        m_bIsOutlinePresent = oSrc.m_bIsOutlinePresent;
        m_bIsEmbossPresent = oSrc.m_bIsEmbossPresent;
        m_bIsEngravePresent = oSrc.m_bIsEngravePresent;

        m_pShape    = oSrc.m_pShape;
        m_pManagerLight = oSrc.m_pManagerLight;
        m_pCont = oSrc.m_pCont;
#if USING_DELETE_DUPLICATING_CONTS == 0
        m_pDuplicateCont = oSrc.m_pDuplicateCont;
#endif

        return *this;
    }

    double CContText::GetIntersect(const CContText* oSrc) const
    {
        double d1 = std::max(m_dLeft, oSrc->m_dLeft);
        double d2 = std::min(m_dLeft + m_dWidth, oSrc->m_dLeft + oSrc->m_dWidth);

        if (d2 > d1)
            return d2 - d1;
        return 0;
    }

    void CContText::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
        if (m_bIsNotNecessaryToUse)
        {
            return;
        }

        oWriter.WriteString(L"<w:r>");
        oWriter.WriteString(L"<w:rPr>");

        oWriter.WriteString(L"<w:noProof/>"); //отключение проверки орфографии

        if (m_strPickFontName.empty())
        {
            if (m_oFont.Bold)
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (m_oFont.Italic)
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            if (m_bIsNeedSpace)
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

            if (m_bIsNeedSpace)
            {
                m_dWidth  += m_pManagerLight->GetSpaceWidth();
                m_oText += L" ";
            }

            if (m_eVertAlignType != eVertAlignType::vatSubscript &&
                m_eVertAlignType != eVertAlignType::vatSuperscript)
            {
                // нужно перемерять...
                double ___dSize = (double)(static_cast<LONG>(m_oFont.Size * 2)) / 2;
                m_pManagerLight->LoadFont(m_strPickFontName, m_lPickFontStyle, ___dSize, false);
                double dWidth = m_pManagerLight->MeasureStringWidth(m_oText.ToStdWString());

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
        }

        if (m_bIsEmbossPresent)
        {
            oWriter.WriteString(L"<w:emboss/>");
        }
        else if (m_bIsEngravePresent)
        {
            oWriter.WriteString(L"<w:imprint/>");
        }
        else
        {
            if (m_bIsOutlinePresent)
            {
                oWriter.WriteString(L"<w:outline/>");
            }
            if (m_bIsShadowPresent)
            {
                oWriter.WriteString(L"<w:shadow/>");
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

        if (m_eVertAlignType == eVertAlignType::vatSubscript)
        {
            oWriter.WriteString(L"<w:vertAlign w:val=\"subscript\"/>");
        }
        else if (m_eVertAlignType == eVertAlignType::vatSuperscript)
        {
            oWriter.WriteString(L"<w:vertAlign w:val=\"superscript\"/>");
        }

        if (ConvertColorBGRToRGB(m_oBrush.Color1) != c_iBlackColor)
        {
            oWriter.WriteString(L"<w:color w:val=\"");
            oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
            oWriter.WriteString(L"\"/>");
        }

        if (m_oFont.Strikeout == TRUE)
        {
            if (m_bIsDoubleStrikeout)
            {
                oWriter.WriteString(L"<w:dstrike/>");
            }
            else
            {
                oWriter.WriteString(L"<w:strike/>");
            }
        }

        if (m_oFont.Underline == TRUE)
        {
            oWriter.WriteString(L"<w:u w:val=");
            oWriter.WriteString(SingletonInstance<LinesTable>().ConverLineToString(m_eUnderlineType));

            if (m_lUnderlineColor != m_oBrush.Color1)
            {
                oWriter.WriteString(L" w:color=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
                oWriter.WriteString(L"\"");
            }
            oWriter.WriteString(L"/>");
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

            dSpaceMMSize = m_pManagerLight->GetSpaceWidth();
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
            oWriter.WriteString(L"<w:u w:val=");
            oWriter.WriteString(SingletonInstance<LinesTable>().ConverLineToString(m_eUnderlineType));

            if (m_lUnderlineColor != m_oBrush.Color1)
            {
                oWriter.WriteString(L" w:color=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
                oWriter.WriteString(L"\"");
            }
            oWriter.WriteString(L"/>");
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
        m_bIsNeedSpace = true;
        m_dWidth += m_dSpaceWidthMM;
    }

    bool CContText::IsEqual(const CContText* oSrc)
    {
        //todo Скорее всего это временное решение
        bool bIf1 = true; //m_strPickFontName == oSrc->m_strPickFontName;
        bool bIf2 = m_eUnderlineType == oSrc->m_eUnderlineType;
        bool bIf3 = m_lUnderlineColor == oSrc->m_lUnderlineColor;
        bool bIf4 = m_bIsHighlightPresent == oSrc->m_bIsHighlightPresent;
        bool bIf5 = m_lHighlightColor == oSrc->m_lHighlightColor;
        bool bIf6 = m_bIsDoubleStrikeout == oSrc->m_bIsDoubleStrikeout;
        bool bIf7 = m_bIsShadowPresent == oSrc->m_bIsShadowPresent;
        bool bIf8 = m_pShape == oSrc->m_pShape;
        bool bIf9 = m_oFont.Name == L"" || oSrc->m_oFont.Name == L"" ? true : m_oFont.IsEqual(&oSrc->m_oFont);
        bool bIf10 = m_oBrush.IsEqual(&oSrc->m_oBrush);

        if (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 && bIf7 && bIf8 && bIf9 && bIf10)
        /*if( m_strPickFontName == oSrc->m_strPickFontName &&
            m_eUnderlineType == oSrc->m_eUnderlineType &&
            m_lUnderlineColor == oSrc->m_lUnderlineColor &&
            m_bIsHighlightPresent == oSrc->m_bIsHighlightPresent &&
            m_lHighlightColor == oSrc->m_lHighlightColor &&
            m_bIsDoubleStrikeout == oSrc->m_bIsDoubleStrikeout &&
            m_bIsShadowPresent == oSrc->m_bIsShadowPresent &&
            //m_eVertAlignType == oSrc->m_eVertAlignType &&
            m_pShape == oSrc->m_pShape &&
            m_oFont.IsEqual(&oSrc->m_oFont) &&
            m_oBrush.IsEqual(&oSrc->m_oBrush))*/
        {
            return true;
        }
        return false;
    }

    bool CContText::IsDuplicate(CContText* pCont, const eVerticalCrossingType& eVType)
    {
        if (eVType == eVerticalCrossingType::vctDublicate &&
            m_oText == pCont->m_oText)
        {
#if USING_DELETE_DUPLICATING_CONTS
            pCont->m_bIsNotNecessaryToUse = true;
#else
            //В итоге собираем список дубликатов
            if (!m_pDuplicateCont)
            {
                m_pDuplicateCont = pCont;
            }
            return true;
#endif
        }
        return false;
    }

    bool CContText::IsThereAreFontEffects(CContText* pCont, const eVerticalCrossingType& eVType, const eHorizontalCrossingType& eHType)
    {
        //Условие пересечения по вертикали
        bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext; //текущий cont выше
        bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext; //текущий cont ниже
        //Условие пересечения по горизонтали
        bool bIf3 = eHType == eHorizontalCrossingType::hctCurrentLeftOfNext; //текущий cont левее
        bool bIf4 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext; //текущий cont правее
        //Размеры шрифта и текст должны бать одинаковыми
        bool bIf5 = m_oFont.Size == pCont->m_oFont.Size;
        bool bIf6 = m_oText == pCont->m_oText;
        //Цвет тени должен быть серым
        bool bIf7 = m_oBrush.Color1 == c_iGreyColor;
        bool bIf8 = pCont->m_oBrush.Color1 == c_iGreyColor;
        bool bIf9 = m_oBrush.Color1 == c_iBlackColor;
        bool bIf10 = pCont->m_oBrush.Color1 == c_iBlackColor;
        bool bIf11 = m_oBrush.Color1 == c_iGreyColor2;
        bool bIf12 = pCont->m_oBrush.Color1 == c_iGreyColor2;

        //note Каждый символ с Emboss или Engrave разбиваются на 3 символа с разными цветами
        //note Логика подобрана для конкретного примера - возможно нужно будет ее обобщить.
        if (bIf5 && bIf6)
        {
            if (m_bIsEmbossPresent && bIf11)
            {
                if (bIf2 && bIf4)
                {
                    pCont->m_bIsEmbossPresent = true;
                    m_bIsNotNecessaryToUse = true;
                    return true;
                }
            }

            if (m_bIsEngravePresent && bIf9)
            {
                if (bIf2 && bIf4)
                {
                    pCont->m_bIsEngravePresent = true;
                    m_bIsNotNecessaryToUse = true;
                    return true;
                }
            }

            //Shadow
            if (bIf1 && bIf3 && bIf8)
            {
                m_bIsShadowPresent = true;
                pCont->m_bIsNotNecessaryToUse = true;
                return true;
            }
            else if (bIf2 && bIf4 && bIf7)
            {
                pCont->m_bIsShadowPresent = true;
                m_bIsNotNecessaryToUse = true;
                return true;
            }

            //Emboss
            else if (bIf2 && bIf4 && bIf10)
            {
                m_bIsEmbossPresent = true;
                pCont->m_bIsNotNecessaryToUse = true;
                return true;
            }
            //Engrave
            else if (bIf2 && bIf4 && bIf12)
            {
                m_bIsEngravePresent = true;
                pCont->m_bIsNotNecessaryToUse = true;
                return true;
            }
        }
        return false;
    }

    bool CContText::IsVertAlignTypeBetweenConts(CContText* pCont, const eVerticalCrossingType& eVType, const eHorizontalCrossingType& eHType)
    {
        //Условие пересечения по вертикали
        bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext ||
                    eVType == eVerticalCrossingType::vctCurrentInsideNext;
        bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext;
        //Условие пересечения по горизонтали
        bool bIf3 = (eHType == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
                    eHType == eHorizontalCrossingType::hctCurrentLeftOfNext) &&
                fabs(m_dRight - pCont->m_dLeft) < c_dTHE_STRING_X_PRECISION_MM * 3;
        bool bIf4 = (eHType == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext ||
                    eHType == eHorizontalCrossingType::hctCurrentRightOfNext) &&
                fabs(m_dLeft - pCont->m_dRight) < c_dTHE_STRING_X_PRECISION_MM * 3;
        //Размеры шрифта должны бать разными
        bool bIf5 = m_oFont.Size * 0.7 > pCont->m_oFont.Size;
        bool bIf6 = m_oFont.Size < pCont->m_oFont.Size * 0.7;

        if (bIf3 || bIf4)
        {
            if (bIf1 && bIf5)
            {
                pCont->m_eVertAlignType = eVertAlignType::vatSubscript;
                pCont->m_pCont = this;
                m_eVertAlignType = eVertAlignType::vatBase;
                m_pCont = pCont;
                return true;
            }
            else if (bIf2 && bIf5)
            {
                pCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
                pCont->m_pCont = this;
                m_eVertAlignType = eVertAlignType::vatBase;
                m_pCont = pCont;
                return true;
            }
            else if (bIf1 && bIf6)
            {
                m_eVertAlignType = eVertAlignType::vatSuperscript;
                m_pCont = pCont;
                pCont->m_eVertAlignType = eVertAlignType::vatBase;
                pCont->m_pCont = this;
                return true;
            }
            else if (bIf2 && bIf6)
            {
                m_eVertAlignType = eVertAlignType::vatSubscript;
                m_pCont = pCont;
                pCont->m_eVertAlignType = eVertAlignType::vatBase;
                pCont->m_pCont = this;
                return true;
            }
        }
        return false;
    }
}
