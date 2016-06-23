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

#include "office_event_listeners.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// office:event_listeners
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_event_listeners::ns = L"office";
const wchar_t * office_event_listeners::name = L"event-listeners";

void office_event_listeners::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_event_listeners::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if	CP_CHECK_NAME(L"presentation", L"event-listener")
        CP_CREATE_ELEMENT(presentation_event_listeners_);
	else if CP_CHECK_NAME(L"script", L"event-listener")
        CP_CREATE_ELEMENT(script_event_listeners_);
    else
        CP_NOT_APPLICABLE_ELM();
}
void office_event_listeners::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, presentation_event_listeners_)
    {
		elm->pptx_convert(Context);
	}
}
// presentation:event-listener-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void presentation_event_listener_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_xlink_attlist_.add_attributes(Attributes);

	CP_APPLY_ATTR(L"script:event_name", script_event_name_);
	CP_APPLY_ATTR(L"presentation:action", presentation_action_);
	
	//...
}
// presentation:event-listener
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_event_listener::ns = L"presentation";
const wchar_t * presentation_event_listener::name = L"event-listener";

void presentation_event_listener::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	presentation_event_listener_attlist_.add_attributes(Attributes);
}

void presentation_event_listener::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if CP_CHECK_NAME(L"presentation", L"sound")
        CP_CREATE_ELEMENT(presentation_sound_);
    else
        CP_NOT_APPLICABLE_ELM();
}
void presentation_event_listener::pptx_convert(oox::pptx_conversion_context & Context)
{
	common_xlink_attlist & xlink = presentation_event_listener_attlist_.common_xlink_attlist_;

	if (xlink.href_)
	{
		Context.get_slide_context().add_hyperlink(*xlink.href_,true);
	}
}

// script:event-listener
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * script_event_listener::ns = L"script";
const wchar_t * script_event_listener::name = L"event-listener";

void script_event_listener::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void script_event_listener::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

}
}
