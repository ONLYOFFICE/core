/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
