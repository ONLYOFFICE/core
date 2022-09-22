#pragma once
#include "BaseItem.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "FontManager.h"
#include "../resources/Constants.h"
#include "../resources/LinesTable.h"


namespace NSDocxRenderer
{
    class CShape;

    enum class eVertAlignType
    {
        vatUnknown,
        vatBase,
        vatSubscript,
        vatSuperscript
    };

    class CContText : public CBaseItem
    {
        public:
            NSStructures::CFont		m_oFont;
            NSStructures::CBrush	m_oBrush;

            std::wstring m_strPickFontName {L""};
            LONG	m_lPickFontStyle {0};

            NSStringUtils::CStringUTF32 m_oText;

            double m_dBaselineOffset {0};
            double m_dLastX {0};

            double m_dSpaceWidthMM {0};

            bool   m_bIsNeedSpace {false};
            bool   m_bIsDoubleStrikeout {false};
            bool   m_bIsHighlightPresent {false};
            LONG   m_lHighlightColor {c_iBlackColor};

            eLineType m_eUnderlineType {eLineType::ltUnknown};
            LONG   m_lUnderlineColor {c_iBlackColor};

            eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

            bool   m_bIsShadowPresent {false};
            bool   m_bIsOutlinePresent {false};
            bool   m_bIsEmbossPresent {false};
            bool   m_bIsEngravePresent {false};

            const CShape* m_pShape {nullptr}; //Если не nullptr, то есть фоновая графика - можно анализировать.
            CFontManagerLight* m_pManagerLight {nullptr};
            const CContText* m_pCont {nullptr}; //Если не nullptr, то есть привязка к vatSubscript или vatSuperscript;

#if USING_DELETE_DUPLICATING_CONTS == 0
            CContText* m_pDuplicateCont {nullptr};
#endif

        public:
            CContText(CFontManagerLight& oManagerLight);
            ~CContText(){}

            void Clear() override final;

            CContText(const CContText& oSrc);

            CContText& operator=(const CContText& oSrc);

            double GetIntersect(const CContText* oSrc) const;

            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            void AddWideSpaceToXml(double dSpacingMM,
                                   NSStringUtils::CStringBuilder& oWriter,
                                   bool bIsNeedSaveFormat = false);

            void AddSpaceToEnd();
            bool IsEqual(const CContText* oSrc);

            bool IsDuplicate(CContText* pCont, const eVerticalCrossingType& eVType);
            bool IsThereAreFontEffects(CContText* pCont, const eVerticalCrossingType& eVType, const eHorizontalCrossingType& eHType);
            bool IsVertAlignTypeBetweenConts(CContText* pCont, const eVerticalCrossingType& eVType, const eHorizontalCrossingType& eHType);
    };
}
