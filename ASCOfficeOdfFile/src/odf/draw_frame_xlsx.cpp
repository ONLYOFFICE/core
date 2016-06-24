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

#include "draw_common.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/regex.h>
#include <boost/algorithm/string.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/odf/odf_document.h>

#include "serialize_elements.h"

#include "style_graphic_properties.h"
#include "odfcontext.h"
#include "office_document.h"
#include "odfcontext.h"
#include "odf_document_impl.h"

#include "calcs_styles.h"
#include "../docx/oox_drawing.h"
#include "chart_build_oox.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void draw_g::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	common_draw_shape_with_styles_attlist common_draw_attlist_ = common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_;

    const int z_index = common_draw_attlist_.common_draw_z_index_attlist_.draw_z_index_.get_value_or(0);

    const std::wstring name = common_draw_attlist_.common_draw_name_attlist_.draw_name_.get_value_or(L"");

 //////////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_group( name);
	
	//const _CP_OPT(length) svg_widthVal =  common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_;    
 //   const _CP_OPT(length) svg_heightVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

	//if (svg_widthVal && svg_heightVal)
	//{
	//	const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
	//	const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

	//	const double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
	//	const double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

	//	Context.get_drawing_context().set_rect(width_pt, height_pt, x_pt, y_pt);
	//}

////////////////////////////////////////
	if (common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_)
	{
		std::wstring Anchor = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_.get();
		
		const double a_x_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double a_y_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_drawing_context().set_anchor(Anchor, a_x_pt, a_y_pt, true);
	}
////////////////////////////////////////////////

	for (int i = 0; i < content_.size(); i++)
    {
		office_element_ptr const & elm = content_[i];
        elm->xlsx_convert(Context);
    }

    Context.get_drawing_context().end_group();    
	Context.get_drawing_context().clear();
}
void draw_frame::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	common_draw_shape_with_styles_attlist common_draw_attlist_ = common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_;

    const int z_index = common_draw_attlist_.common_draw_z_index_attlist_.draw_z_index_.get_value_or(0);

    const std::wstring name = common_draw_attlist_.common_draw_name_attlist_.draw_name_.get_value_or(L"");

    const std::wstring styleName = common_draw_attlist_.common_draw_style_name_attlist_.draw_style_name_.get_value_or(style_ref(L"")).style_name();
    
    const std::wstring textStyleName = common_draw_attlists_.shape_with_text_and_styles_.
        common_draw_text_style_name_attlist_.
        draw_text_style_name_.get_value_or(style_ref(L"")).style_name();

//////////////////////////////////////////////////////////////////////////
	Context.get_drawing_context().start_drawing( name);
	
	const _CP_OPT(length) svg_widthVal =  common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_;    
    const _CP_OPT(length) svg_heightVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

	if (svg_widthVal && svg_heightVal)
	{
		const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
		const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

		const double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_drawing_context().set_rect(width_pt, height_pt, x_pt, y_pt);
	}
///////////////////////////////////////////////////////////////////////////////////////
	if (common_draw_attlist_.common_draw_transform_attlist_.draw_transform_)
	{
		std::wstring transformStr = common_draw_attlist_.common_draw_transform_attlist_.draw_transform_.get();
		xlsx_convert_transforms(transformStr,Context);
	}
////////////////////////////////////////
	std::wstring Anchor;
	if (common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_)
	{
		Anchor = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_.get();
		
		const double a_x_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double a_y_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_drawing_context().set_anchor(Anchor, a_x_pt, a_y_pt);
	}
//////////////////////////////////////////////
	std::vector<const odf_reader::style_instance *> instances;

	odf_reader::style_instance* styleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic,false/*process_headers_footers_*/);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(styleInst);
	}
	graphic_format_properties properties = calc_graphic_properties_content(instances);
	
////////////////////////////////////////////////////////////////////
	properties.apply_to(Context.get_drawing_context().get_properties());

	Context.get_drawing_context().set_property(odf_reader::_property(L"border_width_left",	Compute_BorderWidth(properties, sideLeft)));
	Context.get_drawing_context().set_property(odf_reader::_property(L"border_width_top",	Compute_BorderWidth(properties, sideTop)));
	Context.get_drawing_context().set_property(odf_reader::_property(L"border_width_right",Compute_BorderWidth(properties, sideRight)));
	Context.get_drawing_context().set_property(odf_reader::_property(L"border_width_bottom", Compute_BorderWidth(properties, sideBottom))); 
	
	if (properties.fo_clip_)
	{
		std::wstring strRectClip = properties.fo_clip_.get();
		Context.get_drawing_context().set_clipping(strRectClip.substr(5,strRectClip.length()-6));
	}
	oox::_oox_fill fill;
	Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_,
																	Context.root()->odf_context().drawStyles() ,fill);	
	Context.get_drawing_context().set_fill(fill);
////////////////////////////////////////////////
	for (int i = 0 ; i < content_.size(); i++)
    {
		office_element_ptr const & elm = content_[i];
        elm->xlsx_convert(Context);
    }
    Context.get_drawing_context().end_drawing();    

	Context.get_drawing_context().clear();
}

void draw_image::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    const std::wstring href = common_xlink_attlist_.href_.get_value_or(L"");
    Context.get_drawing_context().start_image(href);
////////////////////////////////////в принципе достаточно общая часть ...
	Context.get_text_context().start_drawing_content();//...  если в объекте есть текст он привяжется к объекту - иначе к ячейке

	for (int i = 0 ; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
    }
	std::wstring text_content_ = Context.get_text_context().end_drawing_content();

	if (text_content_.length()>0)
	{
		Context.get_drawing_context().set_property(_property(L"text-content",text_content_));
	}
////////////////////////////////////////////////////////////////////////////3 раза уже повторилась Content -> Context
    Context.get_drawing_context().end_image();
}
void draw_chart::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    const std::wstring href = common_xlink_attlist_.href_.get_value_or(L"");
    Context.get_drawing_context().start_chart(href);

 	for (int i = 0 ; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
    }
    Context.get_drawing_context().end_chart();
}
void draw_text_box::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	Context.get_drawing_context().start_shape(2);//rect с наваротами
	Context.get_text_context().start_drawing_content();

	for (int i = 0 ; i < content_.size(); i++)
    {
		content_[i]->xlsx_convert(Context);
    }

	std::wstring text_content_ = Context.get_text_context().end_drawing_content();

	if (text_content_.length()>0)
	{
		Context.get_drawing_context().set_property(_property(L"text-content",text_content_));
	}
    Context.get_drawing_context().end_shape();  
}
void draw_object::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    try {
        const std::wstring href		= common_xlink_attlist_.href_.get_value_or(L"");

        odf_reader::odf_document * odf_reader = Context.root();
        
		std::wstring folderPath = odf_reader->get_folder();
        std::wstring objectPath = folderPath + FILE_SEPARATOR_STR + href;

		// normalize path ???? todooo
		boost::algorithm::replace_all(objectPath, FILE_SEPARATOR_STR + std::wstring(L"./"), FILE_SEPARATOR_STR);

        cpdoccore::odf_reader::odf_document objectSubDoc(objectPath,NULL);    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//в отдельных embd объектах чаще всего диаграммы... но МОГУT быть и обычные объекты подтипа frame!!! пример RemanejamentoOrcamentario.ods
///////////////////////////////////////////////////////////////////////////
//функциональная часть
		const office_element *contentSubDoc = objectSubDoc.get_impl()->get_content();
		chart_build objectBuild;
		
		if (contentSubDoc)
		{
			process_build_chart process_build_object_(objectBuild, objectSubDoc.odf_context());
			contentSubDoc->accept(process_build_object_); 
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//отображательная часть

		if (objectBuild.object_type_ == 1)//диаграмма
		{		
			const std::wstring href_draw = common_xlink_attlist_.href_.get_value_or(L"");
			objectBuild.xlsx_convert(Context);
			
			Context.get_drawing_context().start_chart(href_draw); // в рисовательной части только место объекта, рамочки ... и релсы 
			Context.get_drawing_context().end_chart();		
		}
		else if (objectBuild.object_type_ == 2)//текст (odt text)
		{
			Context.get_drawing_context().start_shape(2); 
			Context.get_text_context().start_drawing_content();

			//сменить контекст с главного на другой ... проблема со стилями!!
			Context.get_text_context().set_local_styles_container(&objectSubDoc.odf_context().styleContainer());

			objectBuild.xlsx_convert(Context);
			
			std::wstring text_content_ = Context.get_text_context().end_drawing_content();
			Context.get_text_context().set_local_styles_container(NULL);//вытираем вручную ...

			if (text_content_.length()>0)
			{
				Context.get_drawing_context().set_property(_property(L"text-content",text_content_));
			}
			Context.get_drawing_context().end_shape();		
		}
		else
		{
			//временно - замещающая картинка(если она конечно присутствует)
			Context.get_drawing_context().set_use_image_replacement();
		}
	
	}
    catch(...)
    {
        _CP_LOG << "[error] : convert draw::object error" << std::endl;
    }
}
void draw_object_ole::xlsx_convert(oox::xlsx_conversion_context & Context)
{
	//временно - замещающая картинка(если она конечно присутствует)
	//Context.get_drawing_context().start_object_ole();

	Context.get_drawing_context().set_use_image_replacement();

	//Context.get_drawing_context().end_object_ole();
}
}
}

