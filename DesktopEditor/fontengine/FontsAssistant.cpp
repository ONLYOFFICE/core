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

#include "FontsAssistant.h"
#include "ApplicationFontsWorker.h"

namespace ASC
{
	CFontsAssistant::CFontsAssistant(const std::vector<std::wstring>& arFontsPaths, const std::wstring& sDataFontsPath)
	{
		m_arFontsPaths = arFontsPaths;
		m_sDataFontsPath = sDataFontsPath;
	}

	void CFontsAssistant::AddExcludeFont(const std::wstring& sFontName) {
		m_arExcludeFonts.push_back(sFontName);
	}

	std::wstring CFontsAssistant::GetSystemFontPath()
	{
#if defined(_IOS) || defined(_MAC)
		return std::wstring(L"/System/Library/Fonts");
#elif defined(__ANDROID__)
		return std::wstring(L"/system/fonts");
#endif
		return std::wstring(L"");
	}

	NSFonts::IApplicationFonts* CFontsAssistant::Load()
	{
		CApplicationFontsWorker oFontsWorker;

		oFontsWorker.m_bIsUseOpenType = true;
		oFontsWorker.m_bIsUseSystemFonts = true;
		oFontsWorker.m_bIsNeedThumbnails = false;
		oFontsWorker.m_sDirectory = m_sDataFontsPath;

		for (size_t i = 0; i < m_arFontsPaths.size(); ++i)
		{
			oFontsWorker.m_arAdditionalFolders.push_back(m_arFontsPaths[i]);
		}

		if (!GetSystemFontPath().empty())
		{
			oFontsWorker.m_arAdditionalFolders.push_back(GetSystemFontPath());
		}

		NSFonts::IApplicationFonts* pAppFonts = oFontsWorker.Check();

		m_arAvailableFonts = oFontsWorker.GetFontNamesWithExcludes(pAppFonts, m_arExcludeFonts);
		m_sScriptData = oFontsWorker.GetAllFonts();

		return pAppFonts;
	}

	void CFontsAssistant::Check()
	{
		CApplicationFontsWorker worker;

		worker.m_bIsUseOpenType = true;
		worker.m_bIsUseSystemFonts = true;
		worker.m_bIsNeedThumbnails = false;
		worker.m_sDirectory = m_sDataFontsPath;

		for (size_t i = 0; i < m_arFontsPaths.size(); ++i)
		{
			worker.m_arAdditionalFolders.push_back(m_arFontsPaths[i]);
		}

		if (!GetSystemFontPath().empty())
		{
			worker.m_arAdditionalFolders.push_back(GetSystemFontPath());
		}

		NSFonts::IApplicationFonts* appFonts = worker.Check();

		RELEASEINTERFACE(appFonts);
	}

	std::vector<std::wstring> CFontsAssistant::GetAvailableFonts()
	{
		return m_arAvailableFonts;
	}

	std::string CFontsAssistant::GetScriptData()
	{
		return m_sScriptData;
	}
}
