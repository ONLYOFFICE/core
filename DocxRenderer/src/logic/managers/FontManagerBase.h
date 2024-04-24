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

﻿#pragma once
#include "../DesktopEditor/graphics/structures.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/common/StringUTF32.h"
#include <list>

namespace NSFontManager
{
    static NSFonts::IFontManager* CreateFontManager(NSFonts::IApplicationFonts* pApplication)
    {
        NSFonts::IFontManager* pManager = pApplication->GenerateFontManager();
        pManager->CreateOwnerCache(8);
        return pManager;
    }

    class CUnicodeRange
    {
        public:
            BYTE RangeNum {0};
            BYTE Range {0};

            int Start {0};
            int End {0};

            CUnicodeRange(const int& _start = 0,
                          const int& _end = 0,
                          const BYTE& _range = 0,
                          const BYTE& _rangenum = 0);
    };

    // класс для проставления Ranges для подбора шрифта по символу
    class CUnicodeRanges
    {
        public:
            std::list<CUnicodeRange> m_arRanges;

        public:
            CUnicodeRanges();

            void CheckRange(const int& symbol, BYTE& Range, BYTE& RangeNum);

            void CheckRange(const int& symbol, int& Range1, int& Range2, int& Range3, int& Range4);
    };

    class CFontAdvanced
    {
        public:
            NSStructures::CFont m_oFont;

            // font metrics
            double							m_dAscent {0.0};
            double							m_dDescent {0.0};
            double							m_dLineSpacing {0.0};
            double							m_dEmHeight {0.0};

            double							m_dBaselineOffset {0.0};

            double							m_dSpaceWidthMM {0.0};

            // font params
            std::wstring					m_strFamilyName {L""};
            std::wstring					m_strPANOSE {L""};
            LONG							m_lStyle {0};
            std::vector<UINT>   			m_arSignature;
            bool							m_bIsFixedWidth {false};
            SHORT							m_lAvgWidth {-1};

        public:
            CFontAdvanced();

            CFontAdvanced(const CFontAdvanced& oSrc);

            CFontAdvanced& operator=(const CFontAdvanced& oSrc);
    };

    class CFontPickUp
    {
        public:
            CFontAdvanced	m_oFont;
            BYTE			m_lRangeNum {0xFF};
            BYTE			m_lRange {0xFF};
            std::wstring	m_strPickFont {L""};
            LONG			m_lPickStyle  {0};

        public:
            CFontPickUp();
            CFontPickUp(const CFontPickUp& oSrc);
            CFontPickUp& operator=(const CFontPickUp& oSrc);
    };

    class CFontManagerBase
    {
        public:
            enum MeasureType
            {
                mtGlyph	= 0,
                mtPosition	= 1
            };

        protected:
            NSFonts::IFontManager*          m_pManager;
            std::wstring					m_strDefaultFont;

        public:

            CFontAdvanced					m_oFont;

            //для подбора шрифтов
            CUnicodeRanges                  m_oRanges;

            std::list<CFontPickUp>			m_arListPicUps;
            std::wstring					m_strCurrentPickFont;
            LONG							m_lCurrentPictFontStyle;

        public:
            CFontManagerBase(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManagerBase();

            void ClearPickUps();

        public:
            void SetDefaultFont(const std::wstring& strName);
            std::wstring GetDefaultFont();

            virtual void LoadFont(long lFaceIndex = 0, bool bIsNeedAddToMap = true);

            void LoadFontByName(const std::wstring& strName, const double& dSize, const LONG& lStyle, const double& dDpiX, const double& dDpiY);

            void LoadFontByFile(const std::wstring& strPath, const double& dSize, const double& dDpiX, const double& dDpiY, const LONG& lFaceIndex);

        public:
            virtual void MeasureString(const std::wstring& sText, double x, double y, double& dBoxX, double& dBoxY,
                                       double& dBoxWidth, double& dBoxHeight, MeasureType measureType) = 0;
            virtual void CalculateBaselineOffset();

        public:
            void LoadFontMetrics();

            std::wstring ToHexString( BYTE uc );

            BYTE FromHexString( wchar_t c1, wchar_t c2 );

            void LoadFontParams(bool bIsPath = true);

        private:
            void CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange);

        public:
            bool GenerateFontName(NSStringUtils::CStringUTF32& oText);

            bool CheckFontNameStyle(std::wstring& sName, const std::wstring& sStyle);

            void CheckFontNamePDF(std::wstring& sName, bool& bBold, bool& bItalic);
    };
}
