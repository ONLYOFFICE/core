/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../include/cpdoccore/xml/xmlchar.h"

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