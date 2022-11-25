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

#include "office_annotation.h"

#include <xml/xmlchar.h>
#include <xml/utils.h>

#include "serialize_elements.h"

#include "odf_document.h"

#include "odfcontext.h"
#include "draw_common.h"
#include "calcs_styles.h"
#include "../../Reader/Converter/xlsx_utils.h"
#include "office_meta.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//-------------------------------------------------------------------------
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

//-------------------------------------------------------------------------
const wchar_t * office_annotation_end::ns = L"office";
const wchar_t * office_annotation_end::name = L"annotation-end";

void office_annotation_end::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"office:name", office_name_, std::wstring(L""));
}
void office_annotation_end::docx_convert(oox::docx_conversion_context & Context)
{
	Context.get_comments_context().end_comment(office_name_);
}
//-------------------------------------------------------------------------
const wchar_t * office_annotation::ns = L"office";
const wchar_t * office_annotation::name = L"annotation";

void office_annotation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"office:name", office_name_);
	attr_.add_attributes(Attributes);
}
void office_annotation::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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
void office_annotation::docx_convert(oox::docx_conversion_context & Context)
{
	std::wstring date;
 	std::wstring author;
	if (dc_date_)
	{
		date = XmlUtils::EncodeXmlString(dynamic_cast<dc_date * >(dc_date_.get())->content_);
	}
	if (dc_creator_)
	{
		author = XmlUtils::EncodeXmlString(dynamic_cast<dc_creator * >(dc_creator_.get())->content_);
	}
	////////////////////////////////////////

 	oox::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));
  
	bool runState = Context.get_run_state();
	Context.set_run_state(false);
	
	bool pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);		

 	Context.start_comment_content();
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
	Context.end_comment_content();

	std::wstring content = temp_stream.str();

	Context.set_run_state(runState);
	Context.set_paragraph_state(pState);

	Context.set_stream_man(prev);	
	
	Context.dump_hyperlinks(Context.get_comments_context().get_rels(), oox::comment_place);
	Context.get_mediaitems()->dump_rels(Context.get_comments_context().get_rels(), oox::comment_place);
	
	Context.get_comments_context().start_comment(content, author, date, office_name_);//content, date, author	
}

void office_annotation::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    const _CP_OPT(length) svg_widthVal = attr_.svg_width_;
    
    const double width_cm = svg_widthVal.get_value_or(length(0)).get_value_unit(length::cm);
    const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
    
    const _CP_OPT(length) svg_heightVal =attr_.svg_height_;

    const double height_cm = svg_heightVal.get_value_or(length(0)).get_value_unit(length::cm);
    const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

    const double x_pt = attr_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
    const double y_pt = attr_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

//-----------------------------------------------
	std::wstring date;
 	std::wstring author;
	if (dc_date_)
	{
		date = XmlUtils::EncodeXmlString(dynamic_cast<dc_date * >(dc_date_.get())->content_);
	}
	if (dc_creator_)
	{
		author = XmlUtils::EncodeXmlString(dynamic_cast<dc_creator * >(dc_creator_.get())->content_);
	}
	int col = Context.current_table_column();		if (col < 0) col = 0;
	int row = Context.current_table_row();			if (row < 0) row = 0;

	std::wstring  ref = oox::getCellAddress(col, row); 
//-----------------------------------------------
	Context.get_comments_context().start_comment(ref);
	if (attr_.display_)
	{
	}  

	Context.get_text_context().start_comment_content();
	for (size_t i = 0; i < content_.size(); i++)//текст + текстовый стиль
    {
        content_[i]->xlsx_convert(Context);
    }
	Context.get_comments_context().add_author(author);
	Context.get_comments_context().add_content(Context.get_text_context().end_comment_content());
//----------- drawing part ---------------	
	Context.get_drawing_context().start_comment(col, row);
	Context.get_drawing_context().start_drawing(L"");
	
	Context.get_drawing_context().set_rect(width_pt, height_pt, x_pt, y_pt);
	if (attr_.display_)
	{
		Context.get_drawing_context().set_property(_property(L"visibly", attr_.display_.get()));
	}

	if (attr_.draw_style_name_)
	{
		std::vector<const odf_reader::style_instance *> instances;

		odf_reader::style_instance* styleInst = 
			Context.root()->odf_context().styleContainer().style_by_name(*attr_.draw_style_name_, odf_types::style_family::Graphic, false/*Context.process_headers_footers_*/);
		if (styleInst)
		{
			style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
			if (defaultStyle)instances.push_back(defaultStyle);

			instances.push_back(styleInst);
		}
		graphic_format_properties properties = calc_graphic_properties_content(instances);

//-----------------------------------------------
		properties.apply_to(Context.get_drawing_context().get_properties());
		
		oox::_oox_fill fill;
		Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_,
																		Context.root()->odf_context().drawStyles(), fill);	
		Context.get_drawing_context().set_fill(fill);
	}
//-----------------------------------------------
	std::vector<const odf_reader::style_instance *> instances;
	style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(
				attr_.draw_style_name_.get_value_or(L""), odf_types::style_family::Graphic, false/*Context.process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);	

	const std::wstring textStyleName = attr_.draw_text_style_name_.get_value_or(L"");
	
    Context.get_drawing_context().end_drawing();    
	Context.get_drawing_context().end_comment();
	Context.get_drawing_context().clear();
//-----------------------------------------------
	Context.get_comments_context().end_comment();
}
// officeooo:annotation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * officeooo_annotation::ns = L"officeooo";
const wchar_t * officeooo_annotation::name = L"annotation";

void officeooo_annotation::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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
    attr_.add_attributes(Attributes);
}

void officeooo_annotation::pptx_convert(oox::pptx_conversion_context & Context)
{
    const double x = 8 * attr_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
    const double y = 8 * attr_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);
 /////////////////////////////////
	std::wstring date;
	std::wstring author;
 	std::pair<int,int> id_idx;
	
	if (dc_date_)
	{
		date = XmlUtils::EncodeXmlString(dynamic_cast<dc_date * >(dc_date_.get())->content_);
	}
	if (dc_creator_)
	{
		author = XmlUtils::EncodeXmlString(dynamic_cast<dc_creator * >(dc_creator_.get())->content_);
	}
	id_idx = Context.add_author_comments(author);

	Context.get_comments_context().start_comment(x, y, id_idx.first, id_idx.second);//author & idx (uniq number for author
	
	Context.get_text_context().start_comment_content();
	for (size_t i = 0; i < content_.size(); i++)//текст + текстовый стиль
    {
        content_[i]->pptx_convert(Context);
    }

	Context.get_comments_context().add_date(date);
	Context.get_comments_context().add_content(Context.get_text_context().end_comment_content());
	
//////////////////////////////////////////////////////////////////
    /// Обрабатываем стиль draw
	std::vector<const odf_reader::style_instance *> instances;
	style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(
				attr_.draw_style_name_.get_value_or(L""), odf_types::style_family::Graphic,false/*Context.process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);	

	graphicProperties.apply_to(Context.get_comments_context().get_draw_properties());

	const std::wstring textStyleName = attr_.draw_text_style_name_.get_value_or(L"");

	Context.get_comments_context().end_comment();
}
}
}
