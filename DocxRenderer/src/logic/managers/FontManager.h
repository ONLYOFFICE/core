/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#pragma once
#include <list>
#include <vector>

#include "../../../../DesktopEditor/graphics/structures.h"
#include "../../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../../DesktopEditor/common/StringUTF32.h"

namespace NSDocxRenderer
{
	class CUnicodeRange
	{
	public:
		BYTE RangeNum {0};
		BYTE Range {0};

		int Start {0};
		int End {0};

		CUnicodeRange(const int& _start = 0, const int& _end = 0, const BYTE& _range = 0, const BYTE& _rangenum = 0);
	};

	// class for setting Ranges for font selection by character
	class CUnicodeRanges
	{
	public:
		std::list<CUnicodeRange> m_arRanges;

	public:
		CUnicodeRanges();
		void CheckRange(const int& symbol, BYTE& Range, BYTE& RangeNum);
		void CheckRange(const int& symbol, int& Range1, int& Range2, int& Range3, int& Range4);
	};

	struct CFontMetrics
	{
		double dAscent {0.0};
		double dDescent {0.0};
		double dLineSpacing {0.0};
		double dEmHeight {0.0};
		double dBaselineOffset {0.0};
	};

	struct CFontSelectParams
	{
		// initial parameters that may be known to us
		std::wstring wsDefaultName{L""};
		bool bDefaultBold{false};
		bool bDefaultItalic{false};

		SHORT lAvgWidth{-1};
		bool bIsFixedWidth{false};

		BYTE arPANOSE[10]{};
		std::vector<UINT> arSignature;

		CFontSelectParams() = default;
		CFontSelectParams(const CFontSelectParams& oOther);
		CFontSelectParams& operator=(const CFontSelectParams& oOther);
		bool operator==(const CFontSelectParams& oOther);
	};

	// selects font by parameters
	class CFontSelector
	{
	public:
		// structure for storing already selected fonts
		struct CFontSelectInfo
		{
			CFontSelectParams oFontSelectParams;
			BYTE lRangeNum;
			BYTE lRange;

			std::wstring wsSelectedName;
			bool bIsSelectedBold;
			bool bIsSelectedItalic;
		};

		CFontSelector(NSFonts::IApplicationFonts* pApplication);
		~CFontSelector();

		void SelectFont(const CFontSelectParams& oFontSelectParams,
		                const CFontMetrics& oFontMetrics,
		                const NSStringUtils::CStringUTF32& oText);
		std::wstring GetSelectedName() const noexcept;
		bool IsSelectedBold() const noexcept;
		bool IsSelectedItalic() const noexcept;

		const std::list<CFontSelectInfo>& GetCache() const;
		void ClearCache();

	private:
		std::list<CFontSelectInfo> m_arParamsCache;

		NSFonts::IFontManager* m_pManager;
		std::wstring m_wsSelectedName;
		bool m_bIsSelectedBold;
		bool m_bIsSelectedItalic;

		CUnicodeRanges m_oRanges;
		void CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange);

		void CheckFontNamePDF(std::wstring& wsName, bool& bBold, bool& bItalic);
		bool CheckFontNameStyle(std::wstring& wsName, const std::wstring& sStyle);
	};

	// loads font, its parameters and metrics + measures font
	class CFontManager
	{
	public:
		enum MeasureType
		{
			mtGlyph	= 0,
			mtPosition	= 1
		};

		CFontManager(NSFonts::IApplicationFonts* pFonts);
		~CFontManager();

		bool LoadFontByFile(const NSStructures::CFont& oFont);
		bool LoadFontByName(const NSStructures::CFont& oFont);

		const CFontSelectParams& GetFontSelectParams() const noexcept;
		const CFontMetrics& GetFontMetrics() const noexcept;

		double GetFontHeight() const;
		double GetFontAscent() const;
		double GetFontDescent() const;

		double GetSpaceWidthMM() const;

		void SetStringGid(const LONG& lGid);

		void MeasureString(
		        const std::wstring& wsText,
		        double x,
		        double y,
		        double& dBoxX,
		        double& dBoxY,
		        double& dBoxWidth,
		        double& dBoxHeight,
		        MeasureType measureType) const;

		void MeasureStringGids(
		        unsigned int* pGids,
		        unsigned int count,
		        double x,
		        double y,
		        double& dBoxX,
		        double& dBoxY,
		        double& dBoxWidth,
		        double& dBoxHeight,
		        MeasureType measureType) const;

		void ClearCache();
	private:
		NSFonts::IFontManager* m_pManager;

		NSStructures::CFont m_oFont;
		CFontMetrics m_oFontMetrics;
		CFontSelectParams m_oFontSelectParams;

		void LoadFontMetrics();
		void LoadFontSelectParams();

		void CheckPdfResources();

	};
}
