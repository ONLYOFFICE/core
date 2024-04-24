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
