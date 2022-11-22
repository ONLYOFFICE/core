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

