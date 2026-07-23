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
#include <iostream>
#include <set>

#include "../common/Directory.h"
#include "../fontengine/ApplicationFontsWorker.h"

// #define _GENERATE_FONT_MAP_

#ifdef _GENERATE_FONT_MAP_
#include "../freetype_names/FontMaps/FontDictionary.h"
#endif

std::wstring CorrectDir(const std::wstring& sDir)
{
	if (sDir.empty())
		return L"";

	const wchar_t* data = sDir.c_str();

	std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
	std::wstring::size_type pos2 = sDir.length();

	if (data[pos2 - 1] == '\"')
		--pos2;

	if (pos2 > 0 && ((data[pos2 - 1] == '\\') || (data[pos2 - 1] == '/')))
		--pos2;

	return sDir.substr(pos1, pos2 - pos1);
}

std::wstring CorrectValue(const std::wstring& value)
{
	if (value.empty())
		return L"";

	const wchar_t* data = value.c_str();

	std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
	std::wstring::size_type pos2 = value.length();

	if (data[pos2 - 1] == '\"')
		--pos2;

	return value.substr(pos1, pos2 - pos1);
}

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	std::vector<std::wstring> arFontsDirs;
	bool bIsUseSystemFonts = false;
	bool bIsUseSystemUserFonts = true;
	std::wstring strAllFontsWebPath = L"";
	std::wstring strAllFontsPath = L"";
	std::wstring strThumbnailsFolder = L"";
	std::wstring strFontsSelectionBin = L"";
	std::wstring strOutputDir = L"";
	int nFontFlag = 3;

	for (int i = 0; i < argc; ++i)
	{
#ifdef WIN32
		std::wstring sParam(argv[i]);
#else
		std::string sParamA(argv[i]);
		std::wstring sParam = UTF8_TO_U(sParamA);
#endif

		if (sParam.find(L"--") == 0)
		{
			std::wstring sKey = L"";
			std::wstring sValue = L"";

			std::wstring::size_type _pos = sParam.find('=');
			if (std::wstring::npos == _pos)
			{
				sKey = sParam;
			}
			else
			{
				sKey = sParam.substr(0, _pos);
				sValue = sParam.substr(_pos + 1);
			}

			if (sKey == L"--use-system")
			{
				sValue = CorrectValue(sValue);
				if (sValue == L"1" || sValue == L"true")
					bIsUseSystemFonts = true;
			}
			else if (sKey == L"--use-system-user-fonts")
			{
				sValue = CorrectValue(sValue);
				if (sValue == L"0" || sValue == L"false")
					bIsUseSystemUserFonts = false;
			}
			else if (sKey == L"--allfonts-web")
			{
				strAllFontsWebPath = CorrectDir(sValue);
			}
			else if (sKey == L"--allfonts")
			{
				strAllFontsPath = CorrectDir(sValue);
			}
			else if (sKey == L"--images")
			{
				strThumbnailsFolder = CorrectDir(sValue);
			}
			else if (sKey == L"--selection")
			{
				strFontsSelectionBin = CorrectDir(sValue);
			}
			else if (sKey == L"--input")
			{
				const wchar_t* src = sValue.c_str();
				const wchar_t* limit = src + sValue.length();

				const wchar_t* srcPrev = src;
				while (src < limit)
				{
					if (*src == ';')
					{
						if (srcPrev != src)
						{
							arFontsDirs.push_back(CorrectDir(std::wstring(srcPrev, src - srcPrev)));
						}
						src++;
						srcPrev = src;
					}
					else
						src++;
				}

				if (src > srcPrev)
				{
					arFontsDirs.push_back(CorrectDir(std::wstring(srcPrev, src - srcPrev)));
				}
			}
			else if (sKey == L"--output-web")
			{
				strOutputDir = CorrectDir(sValue);
			}
			else if (sKey == L"--font-format")
			{
				// first byte => isSupportCFF
				// second byte => isUnsupport DFont (mac)

				int nFlag = std::stoi(sValue);
				if (nFlag > 0)
					nFontFlag = nFlag;
			}
		}
	}

	/*
	--input="./fontsInput" --allfonts="./fonts/AllFonts.js" --allfonts-web="./fonts/AllFonts2.js" --images="./fonts" --selection="./fonts/font_selection.bin" --output-web="./fonts"
	--use-system="false"
	*/

	CApplicationFontsWorker oWorker;
	// this is not a working folder with only fonts
	oWorker.m_bIsCleanDirectory = false;
	oWorker.m_bIsRemoveOldThumbnails = true;

	// input fonts
	oWorker.m_bIsUseSystemFonts = bIsUseSystemFonts;
	if (bIsUseSystemFonts)
		oWorker.m_bIsUseSystemUserFonts = bIsUseSystemUserFonts;
	oWorker.m_arAdditionalFolders = arFontsDirs;

	// font_selection.bin
	// fonts.log
	oWorker.m_sDirectory = NSFile::GetDirectoryName(strFontsSelectionBin);

	// thumbnails
	oWorker.m_sThumbnailsDirectory = strThumbnailsFolder;
	oWorker.m_bIsNeedThumbnails = !oWorker.m_sThumbnailsDirectory.empty();
	oWorker.m_bSeparateThumbnails = true;

	// allfonts
	oWorker.m_sAllFontsJSPath = strAllFontsPath;
	oWorker.m_sWebAllFontsJSPath = strAllFontsWebPath;

	// webfonts
	oWorker.m_sWebFontsDirectory = strOutputDir;

	// opentype
	oWorker.m_bIsUseOpenType = (0x01 == (0x01 & nFontFlag));

	NSFonts::IApplicationFonts* pApplicationFonts = oWorker.Check();
	if (oWorker.m_bIsNeedThumbnails)
		oWorker.CheckThumbnails();

	RELEASEINTERFACE(pApplicationFonts);

#ifdef _GENERATE_FONT_MAP_

	NSCommon::DumpFontsDictionary(L"./fonts_dictionary.txt");

#endif

	return 0;
}
