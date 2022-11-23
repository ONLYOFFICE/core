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

#include "draw_page.h"

#include "serialize_elements.h"
#include "odfcontext.h"
#include "odf_document.h"

#include "calcs_styles.h"
#include "draw_common.h"

#include <xml/xmlchar.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


void draw_page_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:id",			draw_id_);

    CP_APPLY_ATTR(L"draw:name",			draw_name_);
    CP_APPLY_ATTR(L"draw:style-name",	draw_style_name_);
    
	CP_APPLY_ATTR(L"presentation:presentation-page-layout-name", page_layout_name_);
    CP_APPLY_ATTR(L"draw:master-page-name",				master_page_name_);
    
	CP_APPLY_ATTR(L"presentation:use-date-time-name",	use_date_time_name_);
    CP_APPLY_ATTR(L"presentation:use-footer-name",		use_footer_name_);

}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_page::ns = L"draw";
const wchar_t * draw_page::name = L"page";

void draw_page::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if	CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(animation_);
    else if (L"presentation" == Ns && L"notes" == Name)
        CP_CREATE_ELEMENT(presentation_notes_); 
	else
		CP_CREATE_ELEMENT(content_);
}

void draw_page::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void draw_page::pptx_convert_placeHolder(oox::pptx_conversion_context & Context, std::wstring styleName, presentation_class::type PresentationClass)
{
	office_element_ptr elm = Context.root()->odf_context().drawStyles().find_by_style_name(styleName);
	//todooo если это элемент datatime -нужно вытащить формат поля

	if (!elm)return;

	int index=-1;

    const std::wstring masterName = attlist_.master_page_name_.get_value_or(L"");
	style_master_page * master = Context.root()->odf_context().pageLayoutContainer().master_page_by_name(masterName);

	if (master)
		index = master->find_placeHolderIndex(PresentationClass, Context.last_idx_placeHolder);


	Context.get_slide_context().start_shape(1);
	Context.get_slide_context().set_placeHolder_type(presentation_class(PresentationClass).get_type_ms());
	Context.get_slide_context().set_placeHolder_idx(index);
	
	Context.get_text_context().start_object();
	
	if (PresentationClass == presentation_class::date_time)
	{
		Context.get_text_context().start_field(oox::datetime, L"");
	}
	
	elm->pptx_convert(Context);
	
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (text_content_.length()>0)
	{
		Context.get_slide_context().set_property(_property(L"text-content",text_content_));
	}
	Context.get_slide_context().set_property(_property(L"no_rect",true));
	Context.get_slide_context().end_shape();

}

void draw_page::pptx_convert(oox::pptx_conversion_context & Context)
{
	const std::wstring pageStyleName	= attlist_.draw_style_name_.get_value_or(L"");
    const std::wstring pageName			= attlist_.draw_name_.get_value_or(L"");
    const std::wstring layoutName		= attlist_.page_layout_name_.get_value_or(L"");
    const std::wstring masterName		= attlist_.master_page_name_.get_value_or(L"");

    _CP_LOG << L"[info][pptx] process page(slide) \"" << pageName /*L"" */<< L"\"" << std::endl;

    Context.start_page(pageName, pageStyleName, layoutName,masterName);

	if (attlist_.draw_style_name_)
	{
		style_instance * style_inst = Context.root()->odf_context().styleContainer().style_by_name(pageStyleName,style_family::DrawingPage,false);

		if ((style_inst) && (style_inst->content()))
		{
			const style_drawing_page_properties * properties = style_inst->content()->get_style_drawing_page_properties();

			if (properties)
			{				
				oox::_oox_fill fill;
				Compute_GraphicFill(properties->content().common_draw_fill_attlist_, office_element_ptr(), 
																	Context.root()->odf_context().drawStyles() ,fill);
				Context.get_slide_context().add_background(fill);
			
				//часть свойств переходов между слайдами тута
				
				if (properties->content().presentation_transition_type_)
				{
					if (properties->content().presentation_transition_type_.get() == L"automatic" || 
						properties->content().presentation_transition_type_.get() == L"semi-automatic")
					{
						Context.get_slide_context().set_transitionAction(false);
					}else
						Context.get_slide_context().set_transitionAction(true);//manual (default)
				}
				if (properties->content().presentation_transition_speed_)
				{
					Context.get_slide_context().set_transitionSpeed(properties->content().presentation_transition_speed_.get());
				}

	////////////////////////////////////////////////
				if ((properties->content().presentation_display_footer_) && (*properties->content().presentation_display_footer_))
					Context.get_slide_context().set_footer();
					
				if ((properties->content().presentation_display_header_) && (*properties->content().presentation_display_header_))
					Context.get_slide_context().set_header();			
				
				if ((properties->content().presentation_display_page_number_) && (*properties->content().presentation_display_page_number_))
					Context.get_slide_context().set_page_number();
				
				if ((properties->content().presentation_display_date_time_) && (*properties->content().presentation_display_date_time_))
					Context.get_slide_context().set_date_time();			
			}
		}
	}
	//сначала анимашки .. потому что объекты используют анимацию не нанапрямую (как бы ) а с общей кучи
	//animation_context на slide_context завести
	if (animation_)
	{
		animation_->pptx_convert(Context);
	}
/////////////////////////
	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->pptx_convert(Context);
	}

	if (attlist_.use_footer_name_)//from master_page
	{
		std::wstring name = L"footer:" + *attlist_.use_footer_name_;
		pptx_convert_placeHolder(Context, name, presentation_class::footer);
	}
	if (attlist_.use_date_time_name_)//from master_page
	{
		std::wstring name = L"datetime:" + *attlist_.use_date_time_name_;
		pptx_convert_placeHolder(Context, name, presentation_class::date_time);
	}
	
	Context.end_page();

 	if (presentation_notes_)
	{
		Context.start_page_notes();
		presentation_notes_->pptx_convert(Context);
		Context.end_page_notes();
	}  
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_footer_decl::ns = L"presentation";
const wchar_t * presentation_footer_decl::name = L"footer-decl";

void presentation_footer_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"presentation:name",	presentation_name_);
}
void presentation_footer_decl::add_text(const std::wstring & text)
{
    text_ += text; 
}
void presentation_footer_decl::add_space(const std::wstring & text)
{
    text_ += text; 
}
void presentation_footer_decl::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().add_text(text_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_date_time_decl::ns = L"presentation";
const wchar_t * presentation_date_time_decl::name = L"date-time-decl";

void presentation_date_time_decl::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"presentation:name",		presentation_name_);
	CP_APPLY_ATTR(L"presentation:source",	presentation_source_);
	CP_APPLY_ATTR(L"style:data-style-name",	style_data_style_name_);
}
void presentation_date_time_decl::add_text(const std::wstring & text)
{
    text_ += text;
}
void presentation_date_time_decl::add_space(const std::wstring & text)
{
    text_ += text;
}
void presentation_date_time_decl::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_text_context().add_text(text_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_notes::ns = L"presentation";
const wchar_t * presentation_notes::name = L"notes";

void presentation_notes::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void presentation_notes::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void presentation_notes::pptx_convert_placeHolder(oox::pptx_conversion_context & Context, std::wstring styleName, presentation_class::type PresentationClass)
{
	office_element_ptr elm = Context.root()->odf_context().drawStyles().find_by_style_name(styleName);
	//todooo если это элемент datatime -нужно вытащить формат поля

	if (!elm)return;

	int index=-1;

    const std::wstring masterName = attlist_.master_page_name_.get_value_or(L"");
	style_master_page * master = Context.root()->odf_context().pageLayoutContainer().master_page_by_name(masterName);

	//if (master)
	//	index = master->find_placeHolderIndex(PresentationClass, Context.last_idx_placeHolder);


	Context.get_slide_context().start_shape(1);
	Context.get_slide_context().set_placeHolder_type(presentation_class(PresentationClass).get_type_ms());
	Context.get_slide_context().set_placeHolder_idx(index);
	
	Context.get_text_context().start_object();
	
	if (PresentationClass == presentation_class::date_time)
	{
		Context.get_text_context().start_field(oox::datetime, L"");
	}
	
	elm->pptx_convert(Context);
	
	std::wstring text_content_ = Context.get_text_context().end_object();

	if (text_content_.length()>0)
	{
		Context.get_slide_context().set_property(_property(L"text-content",text_content_));
	}
	Context.get_slide_context().set_property(_property(L"no_rect",true));
	Context.get_slide_context().end_shape();

}

void presentation_notes::pptx_convert(oox::pptx_conversion_context & Context)
{
	const std::wstring pageStyleName	= attlist_.draw_style_name_.get_value_or(L"");
    const std::wstring layoutName		= attlist_.page_layout_name_.get_value_or(L"");
    const std::wstring masterName		= attlist_.master_page_name_.get_value_or(L"");

    _CP_LOG << L"[info][pptx] process note slide" << std::endl;

	if (attlist_.draw_style_name_)
	{
		style_instance * style_inst = Context.root()->odf_context().styleContainer().style_by_name(pageStyleName, style_family::DrawingPage, Context.process_masters_);

		if ((style_inst) && (style_inst->content()))
		{
			const style_drawing_page_properties * properties = style_inst->content()->get_style_drawing_page_properties();

			if (properties)
			{				
				oox::_oox_fill fill;
				Compute_GraphicFill(properties->content().common_draw_fill_attlist_, office_element_ptr(), 
																	Context.root()->odf_context().drawStyles() ,fill);
				Context.get_slide_context().add_background(fill);
			
	////////////////////////////////////////////////
				if ((properties->content().presentation_display_footer_) && (*properties->content().presentation_display_footer_))
					Context.get_slide_context().set_footer();
					
				if ((properties->content().presentation_display_header_) && (*properties->content().presentation_display_header_))
					Context.get_slide_context().set_header();			
				
				if ((properties->content().presentation_display_page_number_) && (*properties->content().presentation_display_page_number_))
					Context.get_slide_context().set_page_number();
				
				if ((properties->content().presentation_display_date_time_) && (*properties->content().presentation_display_date_time_))
					Context.get_slide_context().set_date_time();			
			}
		}
	}

/////////////////////////
	for (size_t i = 0; i < content_.size(); i++)
    {
		content_[i]->pptx_convert(Context);
	}

	if (attlist_.use_footer_name_)//from master_page_notes
	{
		std::wstring name = L"footer:" + *attlist_.use_footer_name_;
		pptx_convert_placeHolder(Context, name, presentation_class::footer);
	}
	if (attlist_.use_date_time_name_)//from master_page_notes
	{
		std::wstring name = L"datetime:" + *attlist_.use_date_time_name_;
		pptx_convert_placeHolder(Context, name, presentation_class::date_time);
	}

}

}
}
