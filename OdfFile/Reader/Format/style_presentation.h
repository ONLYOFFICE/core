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

#include "../../DataTypes/common_attlists.h"

#include "office_elements.h"
#include "office_elements_create.h"
#include "anim_elements.h"

#include "../../DataTypes/presentationclass.h"
#include "../../DataTypes/presentationvisibility.h"

#include "../../DataTypes/drawfill.h"

namespace cpdoccore { 
namespace odf_reader {


class presentation_placeholder : public office_element_impl<presentation_placeholder>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePresentationPlaceholder;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);
 
public:
	_CP_OPT(odf_types::length) svg_x_;
    _CP_OPT(odf_types::length) svg_y_;
    _CP_OPT(odf_types::length) svg_width_;
    _CP_OPT(odf_types::length) svg_height_;

	_CP_OPT(odf_types::presentation_class) presentation_object_;

    _CP_OPT(std::wstring) text_style_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(presentation_placeholder);
//-------------------------------------------------------------------------------------------

class presentation_sound : public office_element_impl<presentation_sound>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePresentationSound;

    CPDOCCORE_DEFINE_VISITABLE();

    odf_types::common_xlink_attlist xlink_attlist_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);
};

CP_REGISTER_OFFICE_ELEMENT2(presentation_sound);

//-------------------------------------------------------------------------------------------
class drawing_page_properties
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
   
	void apply_from(const drawing_page_properties & Other);

	odf_types::common_draw_fill_attlist		        common_draw_fill_attlist_;
    odf_types::anim_transition_filter_attlist		anim_transition_filter_attlist_;

	_CP_OPT(odf_types::length_or_percent)	draw_fill_image_height_;
	_CP_OPT(odf_types::length_or_percent)	draw_fill_image_width_;
	
	_CP_OPT(std::wstring)					draw_background_size_;			//"border" or "full"

	_CP_OPT(std::wstring)					presentation_transition_type_;	//manual, automatic, semi-automatic (transition separated from effects by click)
	_CP_OPT(std::wstring)					presentation_transition_style_;	//none, fade, move, uncover,clockwise, .... ignore if smil
	_CP_OPT(std::wstring)					presentation_transition_speed_;	//slow, medium, fast
	
	_CP_OPT(bool)							presentation_display_footer_;
	_CP_OPT(bool)							presentation_display_page_number_;
	_CP_OPT(bool)							presentation_display_date_time_;
	_CP_OPT(bool)							presentation_display_header_;
	_CP_OPT(std::wstring)					presentation_page_duration_;

    _CP_OPT(odf_types::presentation_visibility)     presentation_visibility_;

	office_element_ptr						presentation_sound_;

	//presentation:background-objects-visible
	//presentation:background-visible
	//style:repeat
	//draw:background-size

};

class style_drawing_page_properties : public office_element_impl<style_drawing_page_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawingPageProperties;

    CPDOCCORE_DEFINE_VISITABLE();
	
	drawing_page_properties & content() { return content_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    //virtual void pptx_convert(oox::pptx_conversion_context & Context);

	drawing_page_properties content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_drawing_page_properties);

}
}
