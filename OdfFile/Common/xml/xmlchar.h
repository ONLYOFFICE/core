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

namespace cpdoccore {
namespace xml {

template <class ElemT>
class xml_char
{
public:
    //static const ElemT eq;
    //static const ElemT quote;
    //static const ElemT space;
    //static const ElemT left_brocket;
    //static const ElemT right_brocket;
    //static const ElemT slash;
    //static const ElemT colon;
    //static const ElemT * openTag;
    //static const ElemT * closeTag;
    //static const ElemT * endTag;
    //static const ElemT * trueVal;
    //static const ElemT * falseVal;
};

template <>
class xml_char<char>
{
public:
    static const char eq;
    static const char quote;
    static const char space;
    static const char left_brocket;
    static const char right_brocket;
    static const char slash;
    static const char colon;
    static const char * openTag;
    static const char * closeTag;
    static const char * endTag;
    static const char * trueVal;
    static const char * falseVal;

};



template <>
class xml_char<wchar_t>
{
public:
    static const wchar_t eq;
    static const wchar_t quote;
    static const wchar_t space;
    static const wchar_t left_brocket;
    static const wchar_t right_brocket;
    static const wchar_t slash;
    static const wchar_t colon;
    static const wchar_t * openTag;
    static const wchar_t * closeTag;
    static const wchar_t * endTag;
    static const wchar_t * trueVal;
    static const wchar_t * falseVal;
};



typedef xml_char<char> xml_char_c;
typedef xml_char<wchar_t> xml_char_wc;

}
}
