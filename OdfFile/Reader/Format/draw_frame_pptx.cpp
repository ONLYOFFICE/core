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

#include "draw_common.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/regex.h>

#include <xml/xmlchar.h>
#include "odf_document.h"

#include "serialize_elements.h"

#include "style_graphic_properties.h"

#include "odfcontext.h"
#include "office_document.h"
#include "odfcontext.h"
#include "odf_document_impl.h"

#include "calcs_styles.h"
#include "chart_build_oox.h"

#include "../../Reader/Converter/pptx_drawing.h"
#include "../../Reader/Converter/xlsx_package.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

void draw_g::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
        content_[i]->pptx_convert(Context);
    }
}
void draw_frame::pptx_convert_placeHolder(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().set_property(_property(L"no_rect",true));
	pptx_convert(Context);
}
void draw_frame::pptx_convert(oox::pptx_conversion_context & Context)
{
	bool bInnerFrame = Context.get_slide_context().start_frame();

	if (false == bInnerFrame)
	{
		common_shape_draw_attlist	&common_draw_attlist_ = common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_;
		common_presentation_attlist	&common_presentation_attlist_ = common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_;

		const unsigned int z_index = common_draw_attlist_.draw_z_index_.get_value_or(0);
		const std::wstring name = common_draw_attlist_.draw_name_.get_value_or(L"");
		const std::wstring textStyleName = common_draw_attlist_.draw_text_style_name_.get_value_or(L"");

		Context.get_slide_context().set_name(name);
		//////////////////////////////////////////////////////////////////////////
		const _CP_OPT(length) svg_widthVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_;
		const _CP_OPT(length) svg_heightVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

		double width_pt = 0, height_pt = 0;
		if (svg_widthVal && svg_heightVal)
		{
			const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
			const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

			double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
			double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

			if (x_pt < 0) x_pt = 0;
			if (y_pt < 0) y_pt = 0;

			Context.get_slide_context().set_rect(width_pt, height_pt, x_pt, y_pt);
		}
		///////////////////////////////////////////////////////////////////////////////////////
		if (common_draw_attlist_.draw_transform_)
		{
			std::wstring transformStr = common_draw_attlist_.draw_transform_.get();
			pptx_convert_transforms(transformStr, Context);
		}
		////////////////////////////////////////
		std::wstring Anchor;
		if (common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_cell_address_)
		{
			Anchor = common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_cell_address_.get();
			const double a_x_pt = common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
			const double a_y_pt = common_draw_attlists_.shape_with_text_and_styles_.common_shape_table_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

			Context.get_slide_context().set_anchor(Anchor, a_x_pt, a_y_pt);
		}
		//////////////////////////////////////////////
		std::vector<const odf_reader::style_instance *> instances;

		const std::wstring grStyleName = common_draw_attlist_.draw_style_name_.get_value_or(L"");
		const std::wstring baseStyleName = common_presentation_attlist_.presentation_style_name_.get_value_or(L"");

		odf_reader::style_instance* grStyleInst =
			Context.root()->odf_context().styleContainer().style_by_name(grStyleName, odf_types::style_family::Graphic, Context.process_masters_);

		odf_reader::style_instance* baseStyleInst =
			Context.root()->odf_context().styleContainer().style_by_name(baseStyleName, odf_types::style_family::Presentation, Context.process_masters_);

		if (baseStyleInst && ((!common_presentation_attlist_.presentation_user_transformed_) ||
			((common_presentation_attlist_.presentation_user_transformed_) &&
			(common_presentation_attlist_.presentation_user_transformed_->get() == false))))//векторная фигура презентаций 
		{
			style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Presentation);
			if (defaultStyle)instances.push_back(defaultStyle);
			instances.push_back(baseStyleInst);
		}
		else if (common_presentation_attlist_.presentation_class_)
		{
			instances.push_back(baseStyleInst);
		}
		if (grStyleInst)//обычная векторная фигура
		{
			style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
			if (defaultStyle)instances.push_back(defaultStyle);

			instances.push_back(grStyleInst);
		}
		graphic_format_properties properties = calc_graphic_properties_content(instances);

		////////////////////////////////////////////////////////////////////
		properties.apply_to(Context.get_slide_context().get_properties());

		oox::_oox_fill fill;
		Compute_GraphicFill(properties.common_draw_fill_attlist_, properties.style_background_image_,
			Context.root()->odf_context().drawStyles(), fill);
		if (properties.fo_clip_)
		{
			std::wstring strRectClip = properties.fo_clip_.get();
			Context.get_slide_context().set_clipping(strRectClip.substr(5, strRectClip.length() - 6));
		}

		Context.get_slide_context().set_fill(fill);

		Context.get_slide_context().set_property(odf_reader::_property(L"border_width_left", Compute_BorderWidth(properties, sideLeft)));
		Context.get_slide_context().set_property(odf_reader::_property(L"border_width_top", Compute_BorderWidth(properties, sideTop)));
		Context.get_slide_context().set_property(odf_reader::_property(L"border_width_right", Compute_BorderWidth(properties, sideRight)));
		Context.get_slide_context().set_property(odf_reader::_property(L"border_width_bottom", Compute_BorderWidth(properties, sideBottom)));

		if (common_presentation_attlist_.presentation_class_)
		{
			Context.get_slide_context().set_placeHolder_type(common_presentation_attlist_.presentation_class_->get_type_ms());

			if (idx_in_owner >= 0)
				Context.get_slide_context().set_placeHolder_idx(idx_in_owner);
		}

		if (!textStyleName.empty())
		{
			odf_reader::style_instance* textStyleInst =
				Context.root()->odf_context().styleContainer().style_by_name(textStyleName, odf_types::style_family::Paragraph, Context.process_masters_);

			paragraph_format_properties paragraph_properties = calc_paragraph_properties_content(textStyleInst);
		}

		if (office_event_listeners_) office_event_listeners_->pptx_convert(Context);

		Context.get_text_context().start_base_style(baseStyleName, odf_types::style_family::Presentation);

		oox_drawing_ = oox_drawing_ptr(new oox::_pptx_drawing());
	}

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }

	Context.get_text_context().end_base_style();

	Context.get_slide_context().end_frame();
}

void draw_image::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (draw_frame_ptr)
	{
		draw_frame *frame = dynamic_cast<draw_frame *>(draw_frame_ptr.get());
		if (frame)
		{
			office_element_ptr elm = office_element_ptr(new draw_image(*this));
			draw_image *image = dynamic_cast<draw_image *>(elm.get());
			image->draw_frame_ptr = office_element_ptr();

			frame->content_.push_back(elm);
			frame->pptx_convert(Context);
		}
		return;
	}
//-----------------------------------------------------------------------------------------------
	std::wstring href = xlink_attlist_.href_.get_value_or(L"");

	if (true == href.empty())
	{

	}
	else
	{
		if (href[0] == L'#') href = href.substr(1);
	}
	Context.get_slide_context().set_image(href);
////////////////////////////////////в принципе достаточно общая часть ...	
	Context.get_text_context().start_object();

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (!text_content_.empty())
	{
		Context.get_slide_context().set_property(_property(L"text-content", text_content_));
	}
}
void draw_chart::pptx_convert(oox::pptx_conversion_context & Context)
{
    const std::wstring href = xlink_attlist_.href_.get_value_or(L"");
	Context.get_slide_context().set_chart(href);

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
}

void draw_text_box::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (draw_frame_ptr)
	{
		draw_frame *frame = dynamic_cast<draw_frame *>(draw_frame_ptr.get());
		if (frame)
		{
			office_element_ptr elm = office_element_ptr(new draw_text_box(*this));
			draw_text_box *text_box = dynamic_cast<draw_text_box *>(elm.get());
			text_box->draw_frame_ptr = office_element_ptr();

			frame->content_.push_back(elm);
			frame->pptx_convert(Context);

		}
		return;
	}
//---------------------------------------------------------------------------------------------------------------
	Context.get_slide_context().set_text_box();	//rect с наваротами
	Context.get_text_context().start_object();

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
	
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (!text_content_.empty())
	{
		Context.get_slide_context().set_property(_property(L"text-content", text_content_));
	}
}
void draw_object::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (draw_frame_ptr)
	{
		draw_frame *frame = dynamic_cast<draw_frame *>(draw_frame_ptr.get());
		if (frame)
		{
			office_element_ptr elm = office_element_ptr(new draw_object(*this));
			draw_object *object = dynamic_cast<draw_object *>(elm.get());
			object->draw_frame_ptr = office_element_ptr();

			frame->content_.push_back(elm);
			frame->pptx_convert(Context);
		}
		return;
	}
//-----------------------------------------------------------------------------------------------
	try 
	{
		std::wstring tempPath	= Context.root()->get_temp_folder();
		std::wstring odfPath	= Context.root()->get_folder();
        std::wstring href		= xlink_attlist_.href_.get_value_or(L"");
		
		if (!odf_document_ && false == href.empty())
		{			
			if (href[0] == L'#') href = href.substr(1);
			
			std::wstring objectPath = odfPath + FILE_SEPARATOR_STR + href;

			// normalize path ???? todooo
			XmlUtils::replace_all( objectPath, FILE_SEPARATOR_STR + std::wstring(L"./"), FILE_SEPARATOR_STR);

            odf_document_ = odf_document_ptr(new odf_document(objectPath, tempPath, L""));
		}
//---------------------------------------------------------------------------------------------------------------------
		office_element *contentSubDoc = odf_document_ ? odf_document_->get_impl()->get_content() : NULL;
		if (!contentSubDoc)
		{
			//здесь другой формат xml (не Open Office)
			//временно - замещающая картинка(если она конечно присутствует)
			return;
		}
		object_odf_context objectBuild(href);

		process_build_object process_build_object_(objectBuild, odf_document_->odf_context() );
		contentSubDoc->accept(process_build_object_); 
		
		if (objectBuild.table_table_)
		{
			oox::xlsx_conversion_context xlsx_context(odf_document_.get());
			cpdoccore::oox::package::xlsx_document outputXlsx;

			xlsx_context.set_output_document (&outputXlsx);

			xlsx_context.start_document();
				objectBuild.table_table_->xlsx_convert(xlsx_context);
			xlsx_context.end_document();
			
			std::wstring href_folder = tempPath + FILE_SEPARATOR_STR + L"temp_xlsx";
			NSDirectory::CreateDirectory(href_folder);
			outputXlsx.write(href_folder);

			std::wstring href = L"Microsoft_Excel_Worksheet_" + std::to_wstring(Context.get_mediaitems()->count_object + 1) + L".xlsx";
			
			COfficeUtils oCOfficeUtils(NULL);
			if (S_OK == oCOfficeUtils.CompressFileOrDirectory(href_folder, odfPath + FILE_SEPARATOR_STR + href, true))
			{				
				objectBuild.embeddedData = href;
			}
		}
//---------------------------------------------------------------------------------------------------------------------
		if (objectBuild.object_type_ == 1)//диаграмма
		{		
			const std::wstring href_draw = xlink_attlist_.href_.get_value_or(L"Chart");
			objectBuild.pptx_convert(Context);
			
			Context.get_slide_context().set_chart(href_draw); // в рисовательной части только место объекта, рамочки ... и релсы 
		}
		else if (objectBuild.object_type_ == 2)//odt text
		{
			Context.get_slide_context().set_use_image_replacement();

			std::wstring href_new = office_convert( odf_document_, 1);
			
			if (!href_new.empty())
			{
				bool isMediaInternal = true;  
				href += FILE_SEPARATOR_STR + href_new;
				Context.get_slide_context().set_ms_object(href, L"Word.Document");
			}
		}
		else if (objectBuild.object_type_ == 3) //math
		{
			bool bNewObject = false;
			if (bNewObject = Context.get_slide_context().isDefault())
			{
				Context.get_slide_context().set_text_box();
			}

			objectBuild.pptx_convert(Context);
			
			std::wstring math_content = Context.get_math_context().end();
	
			if (!math_content.empty())
			{
				std::wstring text_content = L"<a:p><a14:m xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\">";
				text_content += L"<m:oMathPara xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\">";
				text_content += L"<m:oMathParaPr/>";
				text_content += L"<m:oMath xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\">";
				text_content += math_content;
				text_content += L"</m:oMath></m:oMathPara></a14:m></a:p>";

				if (bNewObject)
				{
					Context.get_slide_context().set_property(_property(L"fit-to-size", true));
					Context.get_slide_context().set_property(_property(L"text-content", text_content));
				}
				else
				{
					Context.get_text_context().add_paragraph(text_content);
				}
			}
		}
		else if (objectBuild.object_type_ == 4) //ods sheet
		{	
			Context.get_slide_context().set_use_image_replacement();

			std::wstring href_new = office_convert( odf_document_, 2);
			
			if (!href_new.empty())
			{
				bool isMediaInternal = true;  
				href += FILE_SEPARATOR_STR + href_new;
				Context.get_slide_context().set_ms_object(href, L"Excel.Sheet");
			}
		}
		else
		{
			//замещающая картинка(если она конечно присутствует)
			Context.get_slide_context().set_use_image_replacement();
		}
	
	}
    catch(...)
    {
        _CP_LOG << "[error] : convert draw::object error" << std::endl;
    }
}

void draw_object_ole::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().set_use_image_replacement();
	
	std::wstring href		= xlink_attlist_.href_.get_value_or(L"");
	std::wstring folderPath = Context.root()->get_folder();
	std::wstring objectPath = folderPath + FILE_SEPARATOR_STR + href;

	if (!href.empty()) 
	{
		std::wstring prog, extension;
		oox::_rels_type relsType;
		detectObject(objectPath, prog, extension, relsType);
		
		NSFile::CFileBinary::Copy(objectPath, objectPath + extension);

		if (relsType == oox::typeMsObject)
			Context.get_slide_context().set_ms_object(href + extension, prog);
		else
			Context.get_slide_context().set_ole_object(href + extension, prog);
	}
}

void draw_param::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (!draw_name_ && !draw_value_) return;

	Context.get_slide_context().set_media_param(*draw_name_, *draw_value_);
}

void draw_plugin::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().set_use_image_replacement();
	
	std::wstring href		= xlink_attlist_.href_.get_value_or(L"");
	std::wstring folderPath = Context.root()->get_folder();
	std::wstring objectPath = folderPath + FILE_SEPARATOR_STR + href;

	Context.get_slide_context().set_media(href);
//params	
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
}
}
}

