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
#include "FontManagerBase.h"
#include "../DesktopEditor/graphics/Matrix.h"

namespace NSDocxRenderer
{
    using namespace NSFontManager;

    class CFontTableEntry
    {
        public:
            std::wstring    	m_strFamilyName {L""};
            std::wstring		m_strPANOSE {L""};
            LONG				m_lStyle {0};
            std::vector<UINT>	m_arSignature;
            bool				m_bIsFixedWidth {false};

        public:
            CFontTableEntry(){}
            virtual ~CFontTableEntry(){}

            CFontTableEntry(const CFontTableEntry& oSrc);

            CFontTableEntry& operator =(const CFontTableEntry& oSrc);
    };

    class CFontTable
    {
        public:
            std::map<std::wstring, CFontTableEntry> m_mapTable;

        public:
            CFontTable() : m_mapTable(){}
    };

    class CFontManager : public CFontManagerBase
    {
        public:
            NSStructures::CFont*	m_pFont {nullptr};
            Aggplus::CMatrix*		m_pTransform {nullptr};
            double					m_dSpaceWidthMM {0.0};

        public:
            CFontTable				m_oFontTable;

        public:
            CFontManager(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManager(){}

        public:
            void Init();

            void AddFontToMap();

        public:
            virtual void LoadFont(long lFaceIndex = 0, bool bNeedAddToMap = true);

            virtual void MeasureString(const std::wstring& sText, double x, double y, double& dBoxX, double& dBoxY,
                               double& dBoxWidth, double& dBoxHeight, MeasureType measureType);

            void MeasureStringGids(unsigned int* pGids, unsigned int count, double x, double y,
                                   double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType);

            double GetBaseLineOffset();

            double GetFontHeight();

            void SetStringGid(const LONG& lGid);

            void GenerateFontName2(NSStringUtils::CStringUTF32& oText);
    };

    class CFontManagerLight
    {
        private:
            std::wstring m_strFontName {L""};
            LONG	m_lFontStyle {0};
            double	m_dSize {0.0};

            double	m_dSpaceWidth {0.0};

            NSFonts::IFontManager*	m_pManager {nullptr};

        public:
            CFontManagerLight(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManagerLight();

            double GetSpaceWidth();

        public:
            void LoadFont(std::wstring& strFontName, LONG& lStyle, const double &dSize, const bool& bIsGID);

            double MeasureStringWidth(const std::wstring& sText);
    };

}
