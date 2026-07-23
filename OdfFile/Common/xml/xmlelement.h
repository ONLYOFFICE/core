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