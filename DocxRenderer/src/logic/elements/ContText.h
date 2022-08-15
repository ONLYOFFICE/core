#pragma once
#include "BaseItem.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../managers/FontManager.h"
#include "../managers/StyleManager.h"
#include "../styles/FontStyle.h"
#include "../../resources/Constants.h"
#include "../../resources/LinesTable.h"


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
            CFontStyle* m_pFontStyle {nullptr};

            bool   m_bIsStrikeoutPresent {false};
            bool   m_bIsDoubleStrikeout {false};

            bool   m_bIsHighlightPresent {false};
            LONG   m_lHighlightColor {c_iBlackColor};

            bool   m_bIsUnderlinePresent {false};
            eLineType m_eUnderlineType {eLineType::ltUnknown};
            LONG   m_lUnderlineColor {c_iBlackColor};

            bool   m_bIsShadowPresent {false};
            bool   m_bIsOutlinePresent {false};
            bool   m_bIsEmbossPresent {false};
            bool   m_bIsEngravePresent {false};

            NSStringUtils::CStringUTF32 m_oText;

            double m_dBaselineOffset {0};
            double m_dLastX {0};
            double m_dSpaceWidthMM {0};
            bool   m_bSpaceIsNotNeeded {false};

            eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

            const CShape* m_pShape {nullptr}; //Если не nullptr, то есть фоновая графика - можно анализировать.
            CFontManagerLight* m_pManagerLight {nullptr};
            CStyleManager*     m_pStyleManager {nullptr};
            const CContText* m_pCont {nullptr}; //Если не nullptr, то есть привязка к vatSubscript или vatSuperscript;

#if USING_DELETE_DUPLICATING_CONTS == 0
            CContText* m_pDuplicateCont {nullptr};
#endif

        public:
            CContText(CFontManagerLight* pManagerLight, CStyleManager* pStyleManager);
            ~CContText(){}

            void Clear() override final;

            CContText(const CContText& oSrc);

            CContText& operator=(const CContText& oSrc);

            double GetIntersect(const CContText* oSrc) const;

            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            void AddWideSpaceToXml(double dSpacingMM,
                                   NSStringUtils::CStringBuilder& oWriter,
                                   bool bIsNeedSaveFormat = false);

            bool IsEqual(const CContText* oSrc);

            UINT GetNumberOfFeatures();

            bool IsDuplicate(CContText* pCont, const eVerticalCrossingType& eVType);
            bool IsThereAreFontEffects(CContText* pCont, const eVerticalCrossingType& eVType, const eHorizontalCrossingType& eHType);
            bool IsVertAlignTypeBetweenConts(CContText* pCont, const eVerticalCrossingType& eVType, const eHorizontalCrossingType& eHType);

            double CalculateWideSpace();
            double CalculateThinSpace();
    };
}
