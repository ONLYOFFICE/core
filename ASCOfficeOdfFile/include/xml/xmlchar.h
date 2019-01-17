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
