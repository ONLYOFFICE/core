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

#include "../../../../Common/3dParty/hunspell/hunspell/src/hunspell/hunspell.h"
#include "../../../../DesktopEditor/common/StringExt.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include <iostream>

bool CheckCaret(std::vector<std::wstring>& words)
{
	bool bIsCaret = false;
	for (int i = 0, len = (int)words.size(); i < len; ++i)
	{
		if (words[i].find('\r') == (words[i].length() - 1))
		{
			words[i] = words[i].substr(0, words[i].length() - 1);
			bIsCaret = true;
		}
	}
	return bIsCaret;
}

std::wstring CheckWord(Hunhandle* pDic, const std::wstring& sWord, const bool& bIsCaret)
{
	std::wstring sResult = sWord;

	std::string sWordA = U_TO_UTF8(sWord);
	int nSpellResult = Hunspell_spell(pDic, sWordA.c_str());

	if (0 == nSpellResult)
	{
		char** pSuggest;
		int nSuggestCount = Hunspell_suggest(pDic, &pSuggest, sWordA.c_str());

		sResult += L" [";

		for (int i = 0; i < nSuggestCount; ++i)
		{
			std::string sSuggestA(pSuggest[i], strlen(pSuggest[i]));
			std::wstring sSuggest = UTF8_TO_U(sSuggestA);

			sResult += sSuggest;
			if (i != (nSuggestCount - 1))
				sResult += (L", ");
		}

		if (0 < nSuggestCount)
			Hunspell_free_list(pDic, &pSuggest, nSuggestCount);

		sResult += L"]";
	}

	if (bIsCaret)
		sResult += L"\r";
	sResult += L"\n";

	return sResult;
}

#if defined(_WIN32) || defined(_WIN64)
#define USE_WCHAR_ARGC
#endif

#ifdef USE_WCHAR_ARGC
std::wstring GetParam(wchar_t* arg)
{
	return std::wstring(arg);
}
#else
std::wstring GetParam(char* arg)
{
	return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE *)arg, (LONG)strlen(arg));
}
#endif

#ifdef USE_WCHAR_ARGC
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	std::wstring sSrcDir = NSFile::GetProcessDirectory() + L"/../src";
	std::wstring sDstDir = NSFile::GetProcessDirectory() + L"/../dst";
	std::wstring sDictionariesDir = NSFile::GetProcessDirectory() + L"/../../../../../../dictionaries";

	if (argc > 1) sSrcDir = GetParam(argv[1]);
	if (argc > 2) sDstDir = GetParam(argv[2]);
	if (argc > 3) sDictionariesDir = GetParam(argv[3]);

	std::vector<std::wstring> arSrcFiles = NSDirectory::GetFiles(sSrcDir);

	for (int i = 0, len = (int)arSrcFiles.size(); i < len; ++i)
	{
		std::wstring sFileWords = arSrcFiles[i];
		std::wstring sName = NSFile::GetFileName(sFileWords);
		std::wstring::size_type sNamePos = sName.find(L".");
		if (std::wstring::npos != sNamePos)
			sName = sName.substr(0, sNamePos);

		std::wstring sFileWordsContent = L"";
		NSFile::CFileBinary::ReadAllTextUtf8(sFileWords, sFileWordsContent);

		std::vector<std::wstring> arWords = NSStringExt::Split(sFileWordsContent, '\n');
		bool bIsCaret = CheckCaret(arWords);

		std::wstring sAff = sDictionariesDir + L"/" + sName + L"/" + sName + L".aff";
		std::wstring sDic = sDictionariesDir + L"/" + sName + L"/" + sName + L".dic";

		// skip check diffs if dictionary is not exists
		if (!NSFile::CFileBinary::Exists(sAff) || !NSFile::CFileBinary::Exists(sDic))
			continue;

		std::string sAffA = U_TO_UTF8(sAff);
		std::string sDicA = U_TO_UTF8(sDic);

		Hunhandle* pDictionary = Hunspell_create(sAffA.c_str(), sDicA.c_str());

		std::wstring sFileDst = sDstDir + L"/" + sName + L".txt";

		std::wstring sResult = L"";
		for (const std::wstring& word : arWords)
		{
			sResult += CheckWord(pDictionary, word, bIsCaret);
		}

		Hunspell_destroy(pDictionary);

		NSFile::CFileBinary::SaveToFile(sFileDst, sResult, true);

		std::cout << "[" << (i + 1) << " of " << (int)arSrcFiles.size() << "] " << U_TO_UTF8(sName) << std::endl;
	}

	return 0;
}
