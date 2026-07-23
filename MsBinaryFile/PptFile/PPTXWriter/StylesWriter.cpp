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
#include "StylesWriter.h"

void CStylesWriter::ConvertStyles(PPT::CTextStyles &oStyles, PPT::CStringWriter &oWriter, int nCount)
{
    for (int i = 0; i < nCount; ++i)
    {
        if (oStyles.m_pLevels[i].is_init())
            ConvertStyleLevel(oStyles.m_pLevels[i].get(), oWriter, i);
    }
}

std::wstring CStylesWriter::ConvertStyleLevel(PPT::CTextStyleLevel &oLevel, const int &nLevel)
{
    PPT::CStringWriter oWriter;
    ConvertStyleLevel(oLevel, oWriter, nLevel);
    return oWriter.GetData();
}

std::wstring CStylesWriter::ConvertStyles(PPT::CTextStyles &oStyles, int nCount)
{
    PPT::CStringWriter oWriter;
    ConvertStyles(oStyles, oWriter, nCount);
    return oWriter.GetData();
}

std::wstring CStylesWriter::GetTextAnchor(const WORD &value)
{
    if (0 == value)			return L"t";
    if (1 == value)			return L"ctr";
    if (2 == value)			return L"b";
    return L"t";
}

std::wstring CStylesWriter::GetTextAlign(const WORD &value)
{
    if (0 == value)			return L"l";
    if (1 == value)			return L"ctr";
    if (2 == value)			return L"r";
    if (3 == value)			return L"just";
    if (4 == value)			return L"dist";
    if (5 == value)			return L"thaiDist";
    if (6 == value)			return L"justLow";
    return L"l";
}

std::wstring CStylesWriter::GetColorInScheme(const LONG &lIndex)
{
    switch (lIndex)
    {
    case 0:	 return L"phClr";
    case 1:	 return L"bg1";
    case 2:	 return L"tx1";
    case 3:	 return L"bg2";
    case 4:	 return L"tx2";
    case 5:	 return L"accent1";
    case 6:	 return L"accent2";
    case 7:	 return L"accent3";
    case 8:	 return L"accent4";
    case 9:	 return L"accent5";
    case 10: return L"accent6";
    case 11: return L"hlink";
    case 12: return L"folHlink";
    case 13: return L"lt1";
    case 14: return L"dk1";
    case 15: return L"lt2";
    case 16: return L"dk2";
        break;
    };
    return L"none";
}

std::wstring CStylesWriter::GetFontAlign(const WORD &value)
{
    if (0 == value)			return L"base";
    if (1 == value)			return L"t";
    if (2 == value)			return L"ctr";
    if (3 == value)			return L"b";
    return L"auto";
}
