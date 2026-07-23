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

#include "xmlchar.h"

namespace cpdoccore {
namespace xml {

const char xml_char<char>::eq = '=';
const char xml_char<char>::quote = '\"';
const char xml_char<char>::space = ' ';
const char xml_char<char>::left_brocket = '<';
const char xml_char<char>::right_brocket = '>';
const char xml_char<char>::slash = '/';
const char xml_char<char>::colon = ':';
const char * xml_char<char>::openTag = "<";
const char * xml_char<char>::closeTag = " />";
const char * xml_char<char>::endTag = "</";
const char * xml_char<char>::trueVal = "true";
const char * xml_char<char>::falseVal = "false";

const wchar_t xml_char<wchar_t>::eq = L'=';
const wchar_t xml_char<wchar_t>::quote = L'\"';
const wchar_t xml_char<wchar_t>::space = L' ';
const wchar_t xml_char<wchar_t>::left_brocket = L'<';
const wchar_t xml_char<wchar_t>::right_brocket = L'>';
const wchar_t xml_char<wchar_t>::slash = L'/';
const wchar_t xml_char<wchar_t>::colon = L':';
const wchar_t * xml_char<wchar_t>::openTag = L"<";
const wchar_t * xml_char<wchar_t>::closeTag = L" />";
const wchar_t * xml_char<wchar_t>::endTag = L"</";
const wchar_t * xml_char<wchar_t>::trueVal = L"true";
const wchar_t * xml_char<wchar_t>::falseVal = L"false";

}
}
