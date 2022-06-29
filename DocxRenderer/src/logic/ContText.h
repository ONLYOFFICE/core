#pragma once
#include "../DesktopEditor/common/StringBuilder.h"
#include "FontManager.h"

namespace NSDocxRenderer
{
    class CShape;

    class CContText
    {
        public:
            enum UnderlineType
            {
                utUnknown,
                utThinLine,
                utThickLine,
                utDoubleThinLine,
                //todo добавить другие типы
            };

        public:
            NSStructures::CFont		m_oFont;
            NSStructures::CBrush	m_oBrush;

            std::wstring m_strPickFontName;
            LONG	m_lPickFontStyle;

            NSStringUtils::CStringUTF32 m_oText;

            double m_dX;
            double m_dY;
            double m_dWidth;
            double m_dHeight;
            double m_dLastX;

            double m_dSpaceWidthMM;
            double m_dBaselineOffset;

            bool   m_bIsNeedSpaceAtTheEnd;
            bool   m_bIsHighlightPresent;
            LONG   m_lHighlightColor;

            UnderlineType m_eUnderlineType;

            const CShape* m_pShape; //Если не 0, то есть фоновая графика - можно анализировать.

        public:
            CContText();
            ~CContText(){}

            void Clear();

            CContText(const CContText& oSrc);

            CContText& operator=(const CContText& oSrc);
            void CopyFormat(const CContText& oSrc);

            bool IsBigger(const CContText* oSrc);
            bool IsBiggerOrEqual(const CContText* oSrc);

            double GetIntersect(const CContText* oSrc) const;

            void Write(NSStringUtils::CStringBuilder& oWriter,
                       CFontManagerLight* pManagerLight,
                       bool bIsAddSpace = false);

            void WriteTo(double dSpacingMM,
                         NSStringUtils::CStringBuilder& oWriter,
                         CFontManagerLight* pManagerLight);

            void AddSpaceToEnd();
    };
}
