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

#include <xml/xmlchar.h>
#include <xml/attributes.h>

#include "office_elements_create.h"

#include <xml/simple_xml_writer.h>


namespace cpdoccore { 

namespace odf_writer {

//---------------------------------------------------------------------------------------------------------------
const wchar_t * office_event_listeners::ns		= L"office";
const wchar_t * office_event_listeners::name	= L"event-listeners";

void office_event_listeners::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if	CP_CHECK_NAME (L"presentation", L"event-listener")
        CP_CREATE_ELEMENT (presentation_event_listeners_);
	else if CP_CHECK_NAME (L"script", L"event-listener")
        CP_CREATE_ELEMENT (script_event_listeners_);
}
void office_event_listeners::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typePresentationEventListener)
	{
		presentation_event_listeners_.push_back(child_element);
	}
	else if (type_ == typeScriptEventListener)
	{
		script_event_listeners_.push_back(child_element);
	}
}

void office_event_listeners::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < presentation_event_listeners_.size(); i++)
			{
				presentation_event_listeners_[i]->serialize(CP_XML_STREAM());
			}
			for (size_t i = 0; i < script_event_listeners_.size(); i++)
			{
				script_event_listeners_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
void presentation_event_listener_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT(L"script:event-name", script_event_name_);
	CP_XML_ATTR_OPT(L"presentation:action", presentation_action_);

	common_xlink_attlist_.serialize(CP_GET_XML_NODE());
}
//---------------------------------------------------------------------------------------------------------------
const wchar_t * presentation_event_listener::ns = L"presentation";
const wchar_t * presentation_event_listener::name = L"event-listener";

void presentation_event_listener::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"presentation", L"sound")
        CP_CREATE_ELEMENT(presentation_sound_);
    else
        CP_NOT_APPLICABLE_ELM();
}
void presentation_event_listener::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeStylePresentationSound)
	{
		presentation_sound_ = child_element;
	}
}
void presentation_event_listener::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
			if (presentation_sound_)
				presentation_sound_->serialize(CP_XML_STREAM());
		}
	}
}

// script:event-listener
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * script_event_listener::ns	= L"script";
const wchar_t * script_event_listener::name = L"event-listener";

void script_event_listener::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void script_event_listener::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void script_event_listener::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
		}
	}
}
}
}
