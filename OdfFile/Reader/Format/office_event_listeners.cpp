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

#include "office_event_listeners.h"
#include "serialize_elements.h"

#include "boost/algorithm/string.hpp"

#include <xml/xmlchar.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// office:event_listeners
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_event_listeners::ns		= L"office";
const wchar_t * office_event_listeners::name	= L"event-listeners";

void office_event_listeners::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_event_listeners::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if	CP_CHECK_NAME (L"presentation", L"event-listener")
        CP_CREATE_ELEMENT (presentation_event_listeners_);
	else if CP_CHECK_NAME (L"script", L"event-listener")
        CP_CREATE_ELEMENT (script_event_listeners_);
	else
        CP_NOT_APPLICABLE_ELM();
}
void office_event_listeners::pptx_convert(oox::pptx_conversion_context & Context)
{
    for (size_t i = 0; i < presentation_event_listeners_.size(); i++)
    {
		presentation_event_listeners_[i]->pptx_convert(Context);
	}
}
// presentation:event-listener-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

void presentation_event_listener_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	xlink_attlist_.add_attributes(Attributes);

	CP_APPLY_ATTR(L"script:event-name", script_event_name_);
	CP_APPLY_ATTR(L"presentation:action", presentation_action_);
	
	//...
}
// presentation:event-listener
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_event_listener::ns = L"presentation";
const wchar_t * presentation_event_listener::name = L"event-listener";

void presentation_event_listener::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	attlist_.add_attributes(Attributes);
}

void presentation_event_listener::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"presentation", L"sound")
        CP_CREATE_ELEMENT(presentation_sound_);
    else
        CP_NOT_APPLICABLE_ELM();
}
void presentation_event_listener::pptx_convert(oox::pptx_conversion_context & Context)
{	
	if (attlist_.xlink_attlist_.href_)
	{
		std::wstring href = *attlist_.xlink_attlist_.href_;
		if (boost::algorithm::starts_with(href, L"#"))
			href = href.substr(1); // Remove '#' character

		const std::vector<std::wstring>& page_names = Context.get_page_names();
		
		bool found = false;
		for (size_t i = 0; i < page_names.size(); i++)
		{
			if (href == page_names[i])
			{
				std::wstring pptx_slide_name = L"slides/slide" + std::to_wstring(i + 1) + L".xml";

				Context.get_slide_context().set_link(pptx_slide_name, oox::_rels_type::typeSlide);
				found = true;
				break;
			}
		}

		if (!found)
		{
			Context.get_slide_context().start_action(attlist_.presentation_action_.get_value_or(L""));

			if (boost::algorithm::starts_with(href, L"../"))
				href = href.substr(std::wstring(L"../").size());
			Context.get_slide_context().set_link(href, oox::_rels_type::typeHyperlink);

			if (presentation_sound_)
				presentation_sound_->pptx_convert(Context);

			Context.get_slide_context().end_action();
		}			
	}
}

// script:event-listener
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * script_event_listener::ns = L"script";
const wchar_t * script_event_listener::name = L"event-listener";

void script_event_listener::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void script_event_listener::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

}
}
