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

#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {

class office_event_listeners : public office_element_impl<office_event_listeners>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeEventListeners;
    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

private:
    office_element_ptr_array presentation_event_listeners_;
    office_element_ptr_array script_event_listeners_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(office_event_listeners);

//-------------------------------------------------------------------------------------
class presentation_event_listener_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

	odf_types::common_xlink_attlist xlink_attlist_;

	_CP_OPT(std::wstring)	script_event_name_;
	_CP_OPT(std::wstring)	presentation_action_;
	//presentation:verb
	//presentation:start-scale
	//presentation:speed
	//presentation:direction
	//presentation:effect
};

class presentation_event_listener : public office_element_impl<presentation_event_listener>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationEventListener;
    CPDOCCORE_DEFINE_VISITABLE();
    
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

private:
    //office_element_ptr_array				content_;
	office_element_ptr						presentation_sound_;
	presentation_event_listener_attlist		attlist_;

    
};

CP_REGISTER_OFFICE_ELEMENT2(presentation_event_listener);


//  script:event-listeners_
class script_event_listener : public office_element_impl<presentation_event_listener>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeScriptEventListener;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

private:
    office_element_ptr_array content_;

    
};

CP_REGISTER_OFFICE_ELEMENT2(script_event_listener);


}
}