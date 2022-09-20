#include "ContText.h"
#include "../../resources/ColorTable.h"
#include "../../resources/SingletonTemplate.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
    CContText::CContText(CFontManagerLight* pManagerLight, CStyleManager* pStyleManager):
        CBaseItem(ElemType::etContText), m_pManagerLight(pManagerLight), m_pStyleManager(pStyleManager)
    {
    }

    CContText::~CContText()
    {
        Clear();
    }

    void CContText::Clear()
    {
        m_pFontStyle = nullptr;
    }

    double CContText::GetIntersect(const CContText* pCont) const
    {
        double d1 = std::max(m_dLeft, pCont->m_dLeft);
        double d2 = std::min(m_dLeft + m_dWidth, pCont->m_dLeft + pCont->m_dWidth);

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

        oWriter.WriteString(L"<w:rStyle w:val=\"");
        oWriter.WriteString(m_pFontStyle->GetStyleId());
        oWriter.WriteString(L"\"/>");

        LONG lCalculatedSpacing = 0;

        if (!m_pFontStyle->m_strPickFontName.empty() && !m_oText.empty())
        {
            if (m_eVertAlignType != eVertAlignType::vatSubscript &&
                m_eVertAlignType != eVertAlignType::vatSuperscript)
            {
                // нужно перемерять...
                m_pManagerLight->LoadFont(m_pFontStyle->m_strPickFontName, m_pFontStyle->m_lPickFontStyle, m_pFontStyle->m_oFont.Size, false);
                double dWidth = m_pManagerLight->MeasureStringWidth(m_oText.ToStdWString());

                double dSpacing = (m_dWidth - dWidth) / (m_oText.length());
                dSpacing *= c_dMMToDx;

                lCalculatedSpacing = static_cast<LONG>(dSpacing);
            }
        }

        //note принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
        //note 1 -> 0.5pt
        lCalculatedSpacing -= 1;

        if (lCalculatedSpacing != 0)
        {
            oWriter.WriteString(L"<w:spacing w:val=\"");
            oWriter.AddInt(lCalculatedSpacing);
            oWriter.WriteString(L"\"/>");
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

        if (m_bIsStrikeoutPresent)
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

        if (m_bIsUnderlinePresent)
        {
            oWriter.WriteString(L"<w:u w:val=");
            oWriter.WriteString(SingletonInstance<LinesTable>().ConverLineToString(m_eUnderlineType));

            if (m_lUnderlineColor != m_pFontStyle->m_oBrush.Color1)
            {
                oWriter.WriteString(L" w:color=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
                oWriter.WriteString(L"\"");
            }
            oWriter.WriteString(L"/>");
        }

        if (m_bIsHighlightPresent)
        {
            //note В <w:style это не работает
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

        if (m_eVertAlignType == eVertAlignType::vatSubscript)
        {
            oWriter.WriteString(L"<w:vertAlign w:val=\"subscript\"/>");
        }
        else if (m_eVertAlignType == eVertAlignType::vatSuperscript)
        {
            oWriter.WriteString(L"<w:vertAlign w:val=\"superscript\"/>");
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

        oWriter.WriteString(L"<w:rStyle w:val=\"");
        oWriter.WriteString(m_pFontStyle->GetStyleId());
        oWriter.WriteString(L"\"/>");

        double dSpaceMMSize = m_dSpaceWidthMM;
        if (!m_pFontStyle->m_strPickFontName.empty())
        {
            dSpaceMMSize = m_pManagerLight->GetSpaceWidth();
        }

        LONG lCalculatedSpacing = static_cast<LONG>((dSpacingMM - dSpaceMMSize) * c_dMMToDx);
        //note принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
        lCalculatedSpacing -= 1;
        if (lCalculatedSpacing != 0)
        {
            oWriter.WriteString(L"<w:spacing w:val=\"");
            oWriter.AddInt(lCalculatedSpacing);
            oWriter.WriteString(L"\"/>");
        }

        if (m_bIsEmbossPresent && bIsNeedSaveFormat)
        {
            oWriter.WriteString(L"<w:emboss/>");
        }
        else if (m_bIsEngravePresent && bIsNeedSaveFormat)
        {
            oWriter.WriteString(L"<w:imprint/>");
        }
        else
        {
            if (m_bIsOutlinePresent && bIsNeedSaveFormat)
            {
                oWriter.WriteString(L"<w:outline/>");
            }
            if (m_bIsShadowPresent && bIsNeedSaveFormat)
            {
                oWriter.WriteString(L"<w:shadow/>");
            }
        }

        if (m_bIsStrikeoutPresent && bIsNeedSaveFormat)
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

        if (m_bIsUnderlinePresent && bIsNeedSaveFormat)
        {
            oWriter.WriteString(L"<w:u w:val=");
            oWriter.WriteString(SingletonInstance<LinesTable>().ConverLineToString(m_eUnderlineType));

            if (m_lUnderlineColor != m_pFontStyle->m_oBrush.Color1)
            {
                oWriter.WriteString(L" w:color=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
                oWriter.WriteString(L"\"");
            }
            oWriter.WriteString(L"/>");
        }

        if (m_bIsHighlightPresent && bIsNeedSaveFormat)
        {
            //note В <w:style это не работает
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

    bool CContText::IsEqual(const CContText *pCont)
    {
        bool bIf1 = m_pFontStyle->GetStyleId() == pCont->m_pFontStyle->GetStyleId();
        bool bIf2 = m_bIsStrikeoutPresent == pCont->m_bIsStrikeoutPresent;
        bool bIf3 = m_bIsDoubleStrikeout == pCont->m_bIsDoubleStrikeout;
        bool bIf4 = m_bIsHighlightPresent == pCont->m_bIsHighlightPresent;
        bool bIf5 = m_lHighlightColor == pCont->m_lHighlightColor;
        bool bIf6 = m_bIsUnderlinePresent == pCont->m_bIsUnderlinePresent;
        bool bIf7 = m_eUnderlineType == pCont->m_eUnderlineType;
        bool bIf8 = m_lUnderlineColor == pCont->m_lUnderlineColor;
        bool bIf9 = m_bIsShadowPresent == pCont->m_bIsShadowPresent;
        bool bIf10 = m_bIsOutlinePresent == pCont->m_bIsOutlinePresent;
        bool bIf11 = m_bIsEmbossPresent == pCont->m_bIsEmbossPresent;
        bool bIf12 = m_bIsEngravePresent == pCont->m_bIsEngravePresent;

        if (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 && bIf7 &&
            bIf8 && bIf9 && bIf10 && bIf11 && bIf12)
        {
            return true;
        }
        return false;
    }

    UINT CContText::GetNumberOfFeatures()
    {
        UINT ret = 0;

        if (m_pFontStyle->m_oFont.Bold)
        {
            ret++;
        }
        if (m_pFontStyle->m_oFont.Italic)
        {
            ret++;
        }
        if (m_bIsStrikeoutPresent)
        {
            ret++;
        }
        if (m_bIsDoubleStrikeout)
        {
            ret++;
        }
        if (m_bIsHighlightPresent)
        {
            ret++;
        }
        if (m_bIsUnderlinePresent)
        {
            ret++;
        }
        if (m_eVertAlignType != eVertAlignType::vatUnknown)
        {
            ret++;
        }

        return ret;
    }

    bool CContText::IsDuplicate(CContText* pCont, eVerticalCrossingType eVType)
    {
        if (eVType == eVerticalCrossingType::vctDublicate &&
            m_oText == pCont->m_oText)
        {
            pCont->m_bIsNotNecessaryToUse = true;
            m_iNumDuplicates++;
            return true;
        }
        return false;
    }

    bool CContText::IsThereAreFontEffects(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType)
    {
        //Условие пересечения по вертикали
        bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext; //текущий cont выше
        bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext; //текущий cont ниже
        //Условие пересечения по горизонтали
        bool bIf3 = eHType == eHorizontalCrossingType::hctCurrentLeftOfNext; //текущий cont левее
        bool bIf4 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext; //текущий cont правее
        //Размеры шрифта и текст должны бать одинаковыми
        bool bIf5 = m_pFontStyle->m_oFont.Size == pCont->m_pFontStyle->m_oFont.Size;
        bool bIf6 = m_oText == pCont->m_oText;
        //Цвет тени должен быть серым
        bool bIf7 = m_pFontStyle->m_oBrush.Color1 == c_iGreyColor;
        bool bIf8 = pCont->m_pFontStyle->m_oBrush.Color1 == c_iGreyColor;
        bool bIf9 = m_pFontStyle->m_oBrush.Color1 == c_iBlackColor;
        bool bIf10 = pCont->m_pFontStyle->m_oBrush.Color1 == c_iBlackColor;
        bool bIf11 = m_pFontStyle->m_oBrush.Color1 == c_iGreyColor2;
        bool bIf12 = pCont->m_pFontStyle->m_oBrush.Color1 == c_iGreyColor2;

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

    bool CContText::IsVertAlignTypeBetweenConts(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType)
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
        bool bIf5 = m_pFontStyle->m_oFont.Size * 0.7 > pCont->m_pFontStyle->m_oFont.Size;
        bool bIf6 = m_pFontStyle->m_oFont.Size < pCont->m_pFontStyle->m_oFont.Size * 0.7;

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

    double CContText::CalculateWideSpace()
    {
        return m_dSpaceWidthMM * 4;
    }

    double CContText::CalculateThinSpace()
    {
        return m_dSpaceWidthMM;
    }
}
