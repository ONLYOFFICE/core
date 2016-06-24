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
#pragma once

#include "datatypes/common_attlists.h"

#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>


#include "office_elements.h"
#include "office_elements_create.h"
#include "anim_elements.h"

#include "datatypes/presentationclass.h"

#include "datatypes/drawfill.h"

namespace cpdoccore { 
namespace odf_reader {


// presentation:placeholder 
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);
 
public:
	_CP_OPT(odf_types::length) svg_x_;
    _CP_OPT(odf_types::length) svg_y_;
    _CP_OPT(odf_types::length) svg_width_;
    _CP_OPT(odf_types::length) svg_height_;

	_CP_OPT(odf_types::presentation_class) presentation_object_;

};

CP_REGISTER_OFFICE_ELEMENT2(presentation_placeholder);

//////////////////////////////////////////////
class drawing_page_properties
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void apply_from(const drawing_page_properties & Other);

	odf_types::common_draw_fill_attlist		common_draw_fill_attlist_;
	anim_transition_filter_attlist			anim_transition_filter_attlist_;

	_CP_OPT(odf_types::length_or_percent)	draw_fill_image_height_;
	_CP_OPT(odf_types::length_or_percent)	draw_fill_image_width_;
	
	_CP_OPT(std::wstring)					draw_background_size_;//"border" or "full"

	_CP_OPT(std::wstring)					presentation_transition_type_;//manual, automatic, semi-automatic (переход отделен от эффектов кликом)
	_CP_OPT(std::wstring)					presentation_transition_style_;//none, fade, move, uncover,clockwise, .... игнор если smil
	_CP_OPT(std::wstring)					presentation_transition_speed_;//slow, medium, fast
	
	_CP_OPT(bool)							presentation_display_footer_;
	_CP_OPT(bool)							presentation_display_page_number_;
	_CP_OPT(bool)							presentation_display_date_time_;
	_CP_OPT(bool)							presentation_display_header_;

	//presentation:background-objects-visible
	//presentation:background-visible
	//style:repeat
	//presentation:page-duration
	//presentation:visibility.
	//presentation:sound.
	//draw:background-size

};
//style:drawing-page-properties
class style_drawing_page_properties : public office_element_impl<style_drawing_page_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleDrawingPageProperties;

    CPDOCCORE_DEFINE_VISITABLE();
	
	const drawing_page_properties & content() const { return drawing_page_properties_; }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    //virtual void pptx_convert(oox::pptx_conversion_context & Context);

	drawing_page_properties drawing_page_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_drawing_page_properties);

}
}
