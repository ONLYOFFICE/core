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

#include "./BaseFonts.h"

#include "../Resources/Fontn022003l.h"
#include "../Resources/Fontn022004l.h"
#include "../Resources/Fontn022024l.h"
#include "../Resources/Fontn022023l.h"
#include "../Resources/Fontn019003l.h"
#include "../Resources/Fontn019004l.h"
#include "../Resources/Fontn019024l.h"
#include "../Resources/Fontn019023l.h"
#include "../Resources/Fonts050000l.h"
#include "../Resources/Fontn021004l.h"
#include "../Resources/Fontn021024l.h"
#include "../Resources/Fontn021023l.h"
#include "../Resources/Fontn021003l.h"
#include "../Resources/Fontd050000l.h"
#include "CMapMemory/cmap_memory.h"

#include <map>

struct TFontData
{
    const unsigned char* Data;
    unsigned int Size;
};

std::map<std::wstring, TFontData> g_base_fonts;

bool PdfReader::GetBaseFont(const std::wstring& sName, const unsigned char*& pData, unsigned int& nSize)
{
    if (g_base_fonts.empty())
    {
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Courier", { c_arrn022003l, c_nSizen022003l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Courier-Bold", { c_arrn022004l, c_nSizen022004l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Courier-BoldOblique", { c_arrn022024l, c_nSizen022024l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Courier-Oblique", { c_arrn022023l, c_nSizen022023l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Helvetica", { c_arrn019003l, c_nSizen019003l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Helvetica-Bold", { c_arrn019004l, c_nSizen019004l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Helvetica-BoldOblique", { c_arrn019024l, c_nSizen019024l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Helvetica-Oblique", { c_arrn019023l, c_nSizen019023l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Symbol", { c_arrs050000l, c_nSizes050000l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Times-Bold", { c_arrn021004l, c_nSizen021004l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Times-BoldItalic", { c_arrn021024l, c_nSizen021024l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Times-Italic", { c_arrn021023l, c_nSizen021023l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"Times-Roman", { c_arrn021003l, c_nSizen021003l }));
        g_base_fonts.insert(std::pair<std::wstring, TFontData>(L"ZapfDingbats", { c_arrd050000l, c_nSized050000l }));
    }

    std::map<std::wstring, TFontData>::const_iterator find = g_base_fonts.find(sName);
    if (find != g_base_fonts.end())
    {
        pData = find->second.Data;
        nSize = find->second.Size;
        return true;
    }
    return false;
}

bool PdfReader::GetBaseCidToUnicode(const char* sName, const unsigned int*& pData, unsigned int& nSize)
{
	return GetCidToUnicodeMemoryMap(sName, pData, nSize);
}

bool PdfReader::ParseFontNameHash(const std::wstring& wsFontName, std::wstring* outBaseName, std::wstring* outHash)
{
	size_t lastSpace = wsFontName.find_last_of(L' ');
	if (lastSpace == std::wstring::npos)
		return false;

	std::wstring sHash = wsFontName.substr(lastSpace + 1);
	if (sHash.size() != 32)
		return false;

	for (wchar_t c : sHash)
	{
		if (!((c >= L'A' && c <= L'F') || (c >= L'0' && c <= L'9')))
			return false;
	}

	if (outBaseName) *outBaseName = wsFontName.substr(0, lastSpace);
	if (outHash)     *outHash     = std::move(sHash);
	return true;
}
