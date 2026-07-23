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

#include "../../SystemUtility/File.h"
#include "BinReaderWriterDefines.h"
#include "FontCutter.h"

namespace NSFontCutter
{
	class CFontDstManager
	{
	public:		
		std::map<std::wstring, std::wstring> m_mapPicks;
		NSFonts::IApplicationFonts* m_pApplicationFonts;
		NSFonts::IFontManager* m_pFontManager;

		std::wstring m_strFontsDir;

        bool					m_bIsEmbeddedFonts;

        CEmbeddedFontsManager	m_oEmbeddedFonts;

	public:
		CFontDstManager();
		~CFontDstManager();

		void Init(const std::wstring& strDir);
		std::wstring GetTypefacePickByName(const std::wstring& strTypeface);

        template<typename TTextFont>
		std::wstring GetTypefacePick(TTextFont& textFont)
		{
            return GetTypefacePickByName(textFont.typeface);
		}
	};
}

class COfficeFontPicker 	
{
private:
	NSFontCutter::CFontDstManager m_oPicker;

public:
	
	HRESULT Init(const std::wstring& bsFontsDirectory);

	NSFonts::IFontManager* get_FontManager();

	HRESULT SetEmbeddedFontsDirectory(const std::wstring& bsFontsDirectory);
	HRESULT SetEmbeddedFontsParam(LONG lParam);

	HRESULT CheckString(const std::wstring& bsText);
	HRESULT CheckFont(const std::wstring& bsFontName);

	HRESULT PickFont(LONG lParamType, const std::wstring& bsParams, std::wstring* pDstName);
	HRESULT GetBinaryData(LONG lType, BYTE** ppyArray, size_t& szCount);

	NSFontCutter::CFontDstManager* GetNativePicker();
	NSFontCutter::CEmbeddedFontsManager* GetNativeCutter();
};
