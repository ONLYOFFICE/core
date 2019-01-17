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

#include <string>
#include <iosfwd>
#include <sstream>

#include "nodetype.h"

namespace cpdoccore {
namespace xml {

class attributes;

template <class CharT>
class element
{
public:
    virtual const CharT * get_ns() const = 0;
    virtual const CharT * get_name() const = 0;
    virtual NodeType get_xml_type() const = 0;

    virtual std::basic_ostream<CharT> & text_to_stream(std::basic_ostream<CharT> & _Wostream) const
    {
        return _Wostream;
    }

    virtual std::basic_ostream<CharT> & xml_to_stream(std::basic_ostream<CharT> & _Wostream) const = 0;
    
    std::basic_string<CharT> text_to_string() const;
    std::basic_string<CharT> xml_to_string() const;

    virtual ~element() = 0;

//    virtual const attributes< std::basic_string<CharT> > * get_attributes() const = 0;
};

template <class CharT>
inline element<CharT>::~element()
{}

typedef element<char> element_c;
typedef element<wchar_t> element_wc;

template <class Element, class CharT = wchar_t>
class element_impl : public element<CharT>
{
public:
    virtual const CharT * get_ns() const { return Element::ns; }
    virtual const CharT * get_name() const { return Element::name; }
    virtual NodeType get_xml_type() const { return Element::xml_type; };
//    virtual const attributes< std::basic_string<CharT> > * get_attributes() const = 0;
};

template <class CharT>
inline std::basic_string<CharT> element<CharT>::text_to_string() const
{
    std::basic_stringstream<CharT> strm;
    text_to_stream(strm);
    return strm.str();
}

template <class CharT>
inline std::basic_string<CharT> element<CharT>::xml_to_string() const
{
    std::basic_stringstream<CharT> strm;
    xml_to_stream(strm);
    return strm.str();
}

}
}