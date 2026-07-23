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

#include "oox_rels.h"

#include <boost/foreach.hpp>
#include "namespaces.h"
#include "mediaitems_utils.h"

#include "../../Common/Utils/simple_xml_writer.h"

namespace oox {

const wchar_t * relationship::ns = L"";
const wchar_t * relationship::name = L"Relationship";

relationship::relationship(const std::wstring & Id, int Type, const std::wstring & Target, bool bExternal)        
{
	id_			= Id;
	type_		= utils::media::get_rel_type((external_items::Type)Type);
	target_		= Target;
	target_mode_= bExternal ? L"External" : L"" ;

}
::std::wostream & relationship::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Relationship")
        {
            CP_XML_ATTR(L"Id", id());
            CP_XML_ATTR(L"Type", type());
            CP_XML_ATTR(L"Target", target());

            if (!target_mode().empty())
                CP_XML_ATTR(L"TargetMode", target_mode());
        }    
    }

    //_Wostream << L"<Relationship ";
    //CP_XML_SERIALIZE_ATTR(L"Id", id());
    //CP_XML_SERIALIZE_ATTR(L"Type", type());
    //CP_XML_SERIALIZE_ATTR(L"Target", target());
    //
    //if (!target_mode().empty())
    //{
    //    CP_XML_SERIALIZE_ATTR(L"TargetMode", target_mode());
    //}

    //_Wostream << L" />";
    return _Wostream;
}

const wchar_t * rels::ns = L"";
const wchar_t * rels::name = L"Relationships";

::std::wostream & rels::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Relationships")
        {
            CP_XML_ATTR(L"xmlns", xmlns::rels.value);

            BOOST_FOREACH(const relationship & r, relationship_)
            {
                r.xml_to_stream(CP_XML_STREAM());
            }
        } // "Relationships"
    }
    return _Wostream;
}

void rels::add(relationship const & r)
{
    relationships().push_back(r);
}

}

