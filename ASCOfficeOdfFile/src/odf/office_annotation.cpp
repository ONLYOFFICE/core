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

#include "office_annotation.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

#include "serialize_elements.h"

#include <cpdoccore/odf/odf_document.h>
#include "../odf/odfcontext.h"

#include "../odf/calcs_styles.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// dc:date
/////////////////////////////////////////////
const wchar_t * dc_date::ns = L"dc";
const wchar_t * dc_date::name = L"date";

void dc_date::add_text(const std::wstring & Text)
{
	content_ = Text;
}

// dc:creator
///////////////////////////////////////////
const wchar_t * dc_creator::ns = L"dc";
const wchar_t * dc_creator::name = L"creator";

void dc_creator::add_text(const std::wstring & Text)
{
	content_ = Text;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void office_annotation_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"draw:style-name",		draw_style_name_); 
	CP_APPLY_ATTR(L"draw:text-style-name",	draw_text_style_name_); 
	
	CP_APPLY_ATTR(L"draw:caption-point-x",	caption_point_x_); 
	CP_APPLY_ATTR(L"draw:caption-point-x",	caption_point_y_); 
	
	CP_APPLY_ATTR(L"svg:y", svg_y_);
	CP_APPLY_ATTR(L"svg:x", svg_x_);
	CP_APPLY_ATTR(L"svg:width",		svg_width_); 
	CP_APPLY_ATTR(L"svg:height",	svg_height_); 
	
	CP_APPLY_ATTR(L"office:display",display_);
}

// office:annotation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_annotation::ns = L"office";
const wchar_t * office_annotation::name = L"annotation";

void office_annotation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if (Ns==L"dc" && Name == L"date")
	{
		CP_CREATE_ELEMENT(dc_date_);
	}
	else if (Ns==L"dc" && Name == L"creator")
	{
		CP_CREATE_ELEMENT(dc_creator_);
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}

void office_annotation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    office_annotation_attr_.add_attributes(Attributes);
}

void office_annotation::docx_convert(oox::docx_conversion_context & Context)
{
	std::wstring date;
 	std::wstring author;
	if (dc_date_)
	{
		date = xml::utils::replace_text_to_xml(dynamic_cast<dc_date * >(dc_date_.get())->content_);
	}
	if (dc_creator_)
	{
		author = xml::utils::replace_text_to_xml(dynamic_cast<dc_creator * >(dc_creator_.get())->content_);
	}
	////////////////////////////////////////
	Context.start_comment();

 	oox::docx_conversion_context::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));
  
	bool runState = Context.get_run_state();
	Context.set_run_state(false);
	
	bool pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);		

	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

	Context.set_run_state(runState);
	Context.set_paragraph_state(pState);
	
	Context.get_comments_context().start_comment(temp_stream.str(), author,date);//content, date, author
	
	Context.dump_hyperlinks(Context.get_comments_context().get_rels(), oox::hyperlinks::comment_place);
		
	Context.set_stream_man(prev);
	
	Context.end_comment();
}

void office_annotation::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    const _CP_OPT(length) svg_widthVal = office_annotation_attr_.svg_width_;
    
    const double width_cm = svg_widthVal.get_value_or(length(0)).get_value_unit(length::cm);
    const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
    
    const _CP_OPT(length) svg_heightVal =office_annotation_attr_.svg_height_;

    const double height_cm = svg_heightVal.get_value_or(length(0)).get_value_unit(length::cm);
    const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

    const double x_pt = office_annotation_attr_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
    const double y_pt = office_annotation_attr_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);
 /////////////////////////////////
	std::wstring date;
 	std::wstring author;
	if (dc_date_)
	{
		date = xml::utils::replace_text_to_xml(dynamic_cast<dc_date * >(dc_date_.get())->content_);
	}
	if (dc_creator_)
	{
		author = xml::utils::replace_text_to_xml(dynamic_cast<dc_creator * >(dc_creator_.get())->content_);
	}
////////////////////////////////////////
	Context.get_comments_context().start_comment(width_pt, height_pt, x_pt, y_pt);
	if (office_annotation_attr_.display_)
	{
		Context.get_comments_context().set_visibly(office_annotation_attr_.display_.get());
	}  

	Context.get_text_context().start_comment_content();
	BOOST_FOREACH(office_element_ptr const & elm, content_)//текст + текстовый стиль
    {
        elm->xlsx_convert(Context);
    }
	Context.get_comments_context().add_author(author);
	Context.get_comments_context().add_content(Context.get_text_context().end_comment_content());
	

//////////////////////////////////////////////////////////////////
    /// Обрабатываем стиль draw
	std::vector<const odf_reader::style_instance *> instances;
	style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(
				office_annotation_attr_.draw_style_name_.get_value_or(style_ref(L"")).style_name(), odf_types::style_family::Graphic,false/*Context.process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);	

	graphicProperties.apply_to(Context.get_comments_context().get_draw_properties());

	const std::wstring textStyleName = office_annotation_attr_.draw_text_style_name_.get_value_or(style_ref(L"")).style_name();

	std::wstring  ref = Context.current_cell_address();  
	Context.get_comments_context().end_comment(ref,Context.current_table_column(), Context.current_table_row());
}
// officeooo:annotation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * officeooo_annotation::ns = L"officeooo";
const wchar_t * officeooo_annotation::name = L"annotation";

void officeooo_annotation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if (Ns==L"dc" && Name == L"date")
	{
		CP_CREATE_ELEMENT(dc_date_);
	}
	else if (Ns==L"dc" && Name == L"creator")
	{
		CP_CREATE_ELEMENT(dc_creator_);
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}

void officeooo_annotation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    office_annotation_attr_.add_attributes(Attributes);
}

void officeooo_annotation::pptx_convert(oox::pptx_conversion_context & Context)
{
    const double x = 8 * office_annotation_attr_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
    const double y = 8 * office_annotation_attr_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);
 /////////////////////////////////
	std::wstring date;
	std::wstring author;
 	std::pair<int,int> id_idx;
	
	if (dc_date_)
	{
		date = xml::utils::replace_text_to_xml(dynamic_cast<dc_date * >(dc_date_.get())->content_);
	}
	if (dc_creator_)
	{
		author = xml::utils::replace_text_to_xml(dynamic_cast<dc_creator * >(dc_creator_.get())->content_);
	}
	id_idx = Context.add_author_comments(author);

	Context.get_comments_context().start_comment(x, y,id_idx.first,id_idx.second);//author & idx (uniq number for author
	
	Context.get_text_context().start_comment_content();
	BOOST_FOREACH(office_element_ptr const & elm, content_)//текст + текстовый стиль
    {
        elm->pptx_convert(Context);
    }

	Context.get_comments_context().add_date(date);
	Context.get_comments_context().add_content(Context.get_text_context().end_comment_content());
	
//////////////////////////////////////////////////////////////////
    /// Обрабатываем стиль draw
	std::vector<const odf_reader::style_instance *> instances;
	style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(
				office_annotation_attr_.draw_style_name_.get_value_or(style_ref(L"")).style_name(), odf_types::style_family::Graphic,false/*Context.process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);	

	graphicProperties.apply_to(Context.get_comments_context().get_draw_properties());

	const std::wstring textStyleName = office_annotation_attr_.draw_text_style_name_.get_value_or(style_ref(L"")).style_name();

	Context.get_comments_context().end_comment();
}
}
}
