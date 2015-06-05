
#include "draw_common.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/regex.h>
#include <boost/algorithm/string.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include <cpdoccore/odf/odf_document.h>

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
namespace odf {

void draw_g::pptx_convert(oox::pptx_conversion_context & Context)
{
	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
		office_element_ptr const & elm = content_[i];
        elm->pptx_convert(Context);
		i++;
    }
}
void draw_frame::pptx_convert_placeHolder(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().set_property(_property(L"no_rect",true));
	pptx_convert(Context);
}
void draw_frame::pptx_convert(oox::pptx_conversion_context & Context)
{
	common_draw_shape_with_styles_attlist common_draw_attlist_ = common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_;

    const int z_index = common_draw_attlist_.common_draw_z_index_attlist_.draw_z_index_.get_value_or(0);

    const std::wstring name = common_draw_attlist_.common_draw_name_attlist_.draw_name_.get_value_or(L"");

    const std::wstring textStyleName = common_draw_attlists_.shape_with_text_and_styles_.
        common_draw_text_style_name_attlist_.
        draw_text_style_name_.get_value_or(style_ref(L"")).style_name();

//////////////////////////////////////////////////////////////////////////
	const _CP_OPT(length) svg_widthVal =  common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_;    
    const _CP_OPT(length) svg_heightVal = common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

	double width_pt=0, height_pt =0; 
	if (svg_widthVal && svg_heightVal)
	{
		const double width_pt = svg_widthVal.get_value_or(length(0)).get_value_unit(length::pt);
		const double height_pt = svg_heightVal.get_value_or(length(0)).get_value_unit(length::pt);

		double x_pt = common_draw_attlists_.position_.svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
		double y_pt = common_draw_attlists_.position_.svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

		if (x_pt <0) x_pt =0;
		if (y_pt <0) y_pt =0;

		Context.get_slide_context().set_rect(width_pt, height_pt, x_pt, y_pt);
	}
///////////////////////////////////////////////////////////////////////////////////////
	if (common_draw_attlist_.common_draw_transform_attlist_.draw_transform_)
	{
		std::wstring transformStr = common_draw_attlist_.common_draw_transform_attlist_.draw_transform_.get();
		pptx_convert_transforms(transformStr,Context);
	}
////////////////////////////////////////
	std::wstring Anchor;
	if (common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_)
	{
		Anchor = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_cell_address_.get();
		const double a_x_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_x_.get_value_or(length(0)).get_value_unit(length::pt);
		const double a_y_pt = common_draw_attlist_.common_text_spreadsheet_shape_attlist_.table_end_y_.get_value_or(length(0)).get_value_unit(length::pt);

		Context.get_slide_context().set_anchor(Anchor,a_x_pt,a_y_pt);
	}
//////////////////////////////////////////////
	std::vector<const odf::style_instance *> instances;

	const std::wstring grStyleName = common_draw_attlist_.common_draw_style_name_attlist_.draw_style_name_.get_value_or(style_ref(L"")).style_name();
	const std::wstring baseStyleName = common_draw_attlist_.common_draw_style_name_attlist_.presentation_style_name_.get_value_or(style_ref(L"")).style_name();

	odf::style_instance* grStyleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(grStyleName, odf::style_family::Graphic,Context.process_masters_);
	
	odf::style_instance* baseStyleInst = 
		Context.root()->odf_context().styleContainer().style_by_name(baseStyleName, odf::style_family::Presentation,Context.process_masters_);

	if (baseStyleInst)//векторная фигура презентаций
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf::style_family::Presentation);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(baseStyleInst);
	}
	if (grStyleInst)//обычная векторная фигура
	{		
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);

		instances.push_back(grStyleInst);
	}
	graphic_format_properties properties = calc_graphic_properties_content(instances);
	
////////////////////////////////////////////////////////////////////
	properties.apply_to(Context.get_slide_context().get_properties());
	
	oox::_oox_fill fill;
	Compute_GraphicFill(properties.common_draw_fill_attlist_, Context.root()->odf_context().drawStyles() ,fill);	
	Context.get_slide_context().set_fill(fill);

	Context.get_slide_context().set_property(odf::_property(L"border_width_left",	Compute_BorderWidth(properties, sideLeft)));
	Context.get_slide_context().set_property(odf::_property(L"border_width_top",	Compute_BorderWidth(properties, sideTop)));
	Context.get_slide_context().set_property(odf::_property(L"border_width_right",	Compute_BorderWidth(properties, sideRight)));
	Context.get_slide_context().set_property(odf::_property(L"border_width_bottom", Compute_BorderWidth(properties, sideBottom))); 
	
	if (properties.fo_clip_)
	{
		std::wstring strRectClip = properties.fo_clip_.get();
		Context.get_slide_context().set_clipping(strRectClip.substr(5,strRectClip.length()-6));
	}
	if (common_presentation_attlist_.presentation_class_)
	{
		Context.get_slide_context().set_placeHolder_type(common_presentation_attlist_.presentation_class_->get_type_ms());
		
		if (idx_in_owner >=0)
			Context.get_slide_context().set_placeHolder_idx(idx_in_owner);
	}

	if (office_event_listeners_)office_event_listeners_->pptx_convert(Context);

	Context.get_text_context().start_base_style(baseStyleName ,odf::style_family::Presentation);
////////////////////////////////////////////////
	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
		office_element_ptr const & elm = content_[i];
        elm->pptx_convert(Context);
		i++;
    }

	Context.get_text_context().end_base_style();
}

void draw_image::pptx_convert(oox::pptx_conversion_context & Context)
{
    const std::wstring href = common_xlink_attlist_.href_.get_value_or(L"");

    Context.get_slide_context().start_image(href);
////////////////////////////////////в принципе достаточно общая часть ...	
	Context.get_text_context().start_object();

	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
		content_[i]->pptx_convert(Context);
		i++;
    }
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (text_content_.length()>0)
	{
		Context.get_slide_context().set_property(_property(L"text-content",text_content_));
	}
////////////////////////////////////////////////////////////////////////////3 раза уже повторилась Content -> Context
    Context.get_slide_context().end_image();
}
void draw_chart::pptx_convert(oox::pptx_conversion_context & Context)
{
    const std::wstring href = common_xlink_attlist_.href_.get_value_or(L"");
    Context.get_slide_context().start_chart(href);

	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
		content_[i]->pptx_convert(Context);
		i++;
    }
    Context.get_slide_context().end_chart();
}
void draw_text_box::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().start_shape(2);//rect с наваротами
	Context.get_text_context().start_object();

	int i=0;
	int size = content_.size();
	while(true)
    {
		if (i>=size)break;
		content_[i]->pptx_convert(Context);
		i++;
    }
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (text_content_.length()>0)
	{
		Context.get_slide_context().set_property(_property(L"text-content",text_content_));
	}
    Context.get_slide_context().end_shape();    
}
void draw_object::pptx_convert(oox::pptx_conversion_context & Context)
{
    try {
        const std::wstring href		= common_xlink_attlist_.href_.get_value_or(L"");

        odf::odf_document::Impl * odfImpl = Context.root()->get_impl();
		const std::wstring folderPath = odfImpl->get_folder();

        std::wstring objectPath = folderPath + FILE_SEPARATOR_STR +  href;

		//normalize path ??? todooo

        cpdoccore::odf::odf_document objectSubDoc(objectPath, NULL);    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//в отдельных embd объектах чаще всего диаграммы, уравнения... но МОГУТ быть и обычные объекты подтипа frame!!! 
		//пример RemanejamentoOrcamentario.ods
///////////////////////////////////////////////////////////////////////////
//функциональная часть
		const office_element *contentSubDoc = objectSubDoc.get_impl()->get_content();
		if (!contentSubDoc)
		{
			//здесь другой формат xml (не Open Office)
			//временно - замещающая картинка(если она конечно присутствует)
			Context.get_slide_context().start_object_ole();
			return;
		}


		chart_build chartBuild;

		process_build_chart process_build_chart_(chartBuild,objectSubDoc.odf_context().styleContainer(),objectSubDoc.odf_context().drawStyles());
        contentSubDoc->accept(process_build_chart_); 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//отображательная часть	

		if (chartBuild.object_type_ == 1)//диаграмма
		{		
			const std::wstring href_draw = common_xlink_attlist_.href_.get_value_or(L"");
			chartBuild.pptx_convert(Context);
			
			Context.get_slide_context().start_chart(href_draw); // в рисовательной части только место объекта, рамочки ... и релсы 
			Context.get_slide_context().end_chart();		
		}
		if (chartBuild.object_type_ == 2)//текст
		{
			Context.get_slide_context().start_shape(2); 
			Context.get_text_context().start_object();

			//сменить контекст с главного на другой ... проблема со стилями!!
			Context.get_text_context().set_local_styles_container(&objectSubDoc.odf_context().styleContainer());

			chartBuild.office_text_->pptx_convert(Context);
			
			std::wstring text_content_ = Context.get_text_context().end_object();
			Context.get_text_context().set_local_styles_container(NULL);//вытираем вручную ...

			if (text_content_.length()>0)
			{
				Context.get_slide_context().set_property(_property(L"text-content",text_content_));
			}
			Context.get_slide_context().end_shape();		
		}
		else
		{
			//временно - замещающая картинка(если она конечно присутствует)
			Context.get_slide_context().start_object_ole();
		}
	
	}
    catch(...)
    {
        _CP_LOG << "[error] : convert draw::object error" << std::endl;
    }
}

void draw_object_ole::pptx_convert(oox::pptx_conversion_context & Context)
{
	//объект бин в embeddings
	//распознать тип по guid???


	//временно - замещающая картинка(если она конечно присутствует)
	Context.get_slide_context().start_object_ole();
}

}
}

