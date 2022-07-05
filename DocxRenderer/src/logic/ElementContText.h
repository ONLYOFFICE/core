#pragma once
#include "BaseItem.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "FontManager.h"
#include "../resources/LinesTable.h"

namespace NSDocxRenderer
{
    class CShape;

    class CContText : public CBaseItem
    {
        public:
            NSStructures::CFont		m_oFont;
            NSStructures::CBrush	m_oBrush;

            std::wstring m_strPickFontName;
            LONG	m_lPickFontStyle;

            NSStringUtils::CStringUTF32 m_oText;

            double m_dBaselinePos;
            double m_dBaselineOffset;
            double m_dLastX;

            double m_dSpaceWidthMM;

            bool   m_bIsNeedSpaceAtTheEnd;
            bool   m_bIsDoubleStrikeout;
            bool   m_bIsHighlightPresent;
            LONG   m_lHighlightColor;

            eLineType m_eUnderlineType;
            LONG   m_lUnderlineColor;

            const CShape* m_pShape; //Если не 0, то есть фоновая графика - можно анализировать.

        public:
            CContText();
            ~CContText(){}

            void Clear();

            CContText(const CContText& oSrc);

            CContText& operator=(const CContText& oSrc);
            void CopyFormat(const CContText& oSrc);

            double GetIntersect(const CContText* oSrc) const;

            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter){}

            void ToXml(NSStringUtils::CStringBuilder& oWriter,
                       CFontManagerLight* pManagerLight,
                       bool bIsAddSpace = false);

            void AddWideSpaceToXml(double dSpacingMM,
                                   NSStringUtils::CStringBuilder& oWriter,
                                   CFontManagerLight* pManagerLight,
                                   bool bIsNeedSaveFormat = false);

            void AddSpaceToEnd();
            bool IsEqual(const CContText* oSrc);
    };
}
