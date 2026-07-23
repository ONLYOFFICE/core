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
#include "../../Common/Utils/simple_xml_writer.h"

#include "oox_content_type.h"
#include "namespaces.h"

#include <boost/foreach.hpp>

namespace oox {

const wchar_t * default_content_type::ns = L"";
const wchar_t * default_content_type::name = L"Default";

::std::wostream & default_content_type::xml_to_stream(std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Default")
        {
            CP_XML_ATTR(L"Extension", extension());
            CP_XML_ATTR(L"ContentType", content_type());        
        }    
    }

    //_Wostream << L"<Default ";
    //CP_XML_SERIALIZE_ATTR(L"Extension", extension());
    //CP_XML_SERIALIZE_ATTR(L"ContentType", content_type());
    //_Wostream << L" />";
    return _Wostream;
}

const wchar_t * override_content_type::ns = L"";
const wchar_t * override_content_type::name = L"Override";

::std::wostream & override_content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Override")
        {
            CP_XML_ATTR(L"PartName", part_name());
            CP_XML_ATTR(L"ContentType", content_type());
        }
    }

    //_Wostream << L"<Override ";
    //CP_XML_SERIALIZE_ATTR(L"PartName", part_name());
    //CP_XML_SERIALIZE_ATTR(L"ContentType", content_type());
    //_Wostream << L" />";
    return _Wostream;
}

const wchar_t * content_type::ns = L"";
const wchar_t * content_type::name = L"Types";

::std::wostream & content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Types")
        {
            CP_XML_ATTR(L"xmlns", xmlns::types.value);

            
            BOOST_FOREACH(const default_content_type & elm, default_)
            {
                elm.xml_to_stream(CP_XML_STREAM());
            }

            BOOST_FOREACH(const override_content_type & elm, override_)
            {
                elm.xml_to_stream(CP_XML_STREAM());
            }            
        }
	}
    return _Wostream;
}

}