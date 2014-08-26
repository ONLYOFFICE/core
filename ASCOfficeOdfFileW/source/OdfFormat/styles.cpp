#include "precompiled_cpodf.h"

#include "styles.h"
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <iostream>
#include <boost/foreach.hpp>

#include "office_elements.h"
#include "office_elements_create.h"

#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_graphic_properties.h"
#include "style_chart_properties.h"
#include "style_section_properties.h"
#include "style_page_layout_properties.h"

//#include "style_presentation.h"



namespace cpdoccore { 
namespace odf {
    
using xml::xml_char_wc;
//
style_text_properties * style_content::get_style_text_properties() 
{
	//если запросили .. значит надо - создаем
	if (!style_text_properties_)
		create_element(L"style", L"text-properties",style_text_properties_,Context);
	
	return dynamic_cast<style_text_properties *>(style_text_properties_.get());    
}

style_paragraph_properties * style_content::get_style_paragraph_properties()
{
 	if (!style_paragraph_properties_)
		create_element(L"style", L"paragraph-properties",style_paragraph_properties_,Context);

	return dynamic_cast<style_paragraph_properties *>(style_paragraph_properties_.get());    
}

style_graphic_properties * style_content::get_style_graphic_properties() 
{
  	if (!style_graphic_properties_)
		create_element(L"style", L"graphic-properties",style_graphic_properties_,Context);

	return dynamic_cast<style_graphic_properties *>(style_graphic_properties_.get());    
}
style_table_properties * style_content::get_style_table_properties()
{
  	if (!style_table_properties_)
		create_element(L"style", L"table-properties",style_table_properties_,Context);

    return dynamic_cast<style_table_properties *>(style_table_properties_.get());    
}

style_section_properties * style_content::get_style_section_properties()
{
  	if (!style_section_properties_)
		create_element(L"style", L"section-properties",style_section_properties_,Context);

	return dynamic_cast<style_section_properties *>(style_section_properties_.get());
}

style_table_cell_properties * style_content::get_style_table_cell_properties()
{
	//если запросили .. значит надо - создаем
	if (!style_table_cell_properties_)
		create_element(L"style", L"table-cell-properties",style_table_cell_properties_,Context);
    
	return dynamic_cast<style_table_cell_properties *>(style_table_cell_properties_.get());
}

style_table_row_properties * style_content::get_style_table_row_properties() 
{
  	if (!style_table_row_properties_)
		create_element(L"style", L"table-row-properties",style_table_row_properties_,Context);

	return dynamic_cast<style_table_row_properties *>(style_table_row_properties_.get());
}

style_table_column_properties * style_content::get_style_table_column_properties() 
{
   	if (!style_table_column_properties_)
		create_element(L"style", L"table-column-properties",style_table_column_properties_,Context);

	return dynamic_cast<style_table_column_properties *>(style_table_column_properties_.get());
}

style_chart_properties * style_content::get_style_chart_properties() 
{
   	if (!style_chart_properties_)
		create_element(L"style", L"chart-properties",style_chart_properties_,Context);

	return dynamic_cast<style_chart_properties *>(style_chart_properties_.get());    
}
//style_drawing_page_properties * style_content::get_style_drawing_page_properties()
//{
//    if (!style_drawing_page_properties_)
//		create_element(L"style", L"drawing-page-properties",style_drawing_page_properties_,Context);
//
//   return dynamic_cast<style_drawing_page_properties *>(style_drawing_page_properties_.get());
//}

void style_content::add_child_element(office_element_ptr & child)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)//перезатирать???
	{
	case typeStyleTextProperties:		style_text_properties_ = child; break;
	case typeStyleParagraphProperties:	style_paragraph_properties_ = child; break; 
	case typeStyleTableProperties:		style_table_properties_ = child; break; 
	case typeStyleTableColumnProperties:style_table_column_properties_ = child; break; 
	case typeStyleTableCellProperties:	style_table_cell_properties_ = child; break; 
	case typeStyleTableRowProperties:	style_table_row_properties_ = child; break; 
	case typeStyleSectionProperties:	style_section_properties_ = child; break; 
	case typeStyleGraphicPropertis:		style_graphic_properties_ = child; break; 
	case typeStyleChartProperties:		style_chart_properties_ = child; break; 
	case typeStyleDrawingPageProperties:style_drawing_page_properties_ = child; break;
		//ruby
	}	
}

void style_content::create_child_element( const std::wstring & Ns, const ::std::wstring & Name)
{   
    if CP_CHECK_NAME(L"style", L"text-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_text_properties_);    
    }
    else if CP_CHECK_NAME(L"style", L"paragraph-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_paragraph_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-column-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_column_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-cell-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_cell_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"table-row-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_table_row_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"section-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_section_properties_);
    } 
    else if CP_CHECK_NAME(L"style", L"graphic-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_graphic_properties_);    
    }
    else if CP_CHECK_NAME(L"style", L"chart-properties")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_chart_properties_);    
    }
	else if CP_CHECK_NAME(L"style", L"drawing-page-properties")
	{
		CP_CREATE_ELEMENT_SIMPLE(style_drawing_page_properties_);
	}
    else
    {
        not_applicable_element(L"style-content",  Ns, Name);
    }
}
void style_content::serialize(std::wostream & strm)
{
	if (style_text_properties_)		style_text_properties_->serialize(strm);
    if (style_paragraph_properties_)style_paragraph_properties_->serialize(strm);
    if (style_section_properties_)	style_section_properties_->serialize(strm);
    if (style_ruby_properties_)		style_ruby_properties_->serialize(strm);
    if (style_table_properties_)	style_table_properties_->serialize(strm);
    if (style_table_column_properties_)style_table_column_properties_->serialize(strm);
    if (style_table_row_properties_)style_table_row_properties_->serialize(strm);
    if (style_chart_properties_)	style_chart_properties_->serialize(strm);
    if (style_graphic_properties_)	style_graphic_properties_->serialize(strm);
    if (style_table_cell_properties_)style_table_cell_properties_->serialize(strm);
	if (style_drawing_page_properties_)style_drawing_page_properties_->serialize(strm);
}

// style:default-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * default_style::ns = L"style";
const wchar_t * default_style::name = L"default-style";


void default_style::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    style_content_.create_child_element(Ns, Name);
}
void default_style::add_child_element(office_element_ptr & child)
{
	style_content_.add_child_element(child);
}
void default_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"style:family", style_family_);
			style_content_.serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_fill_image::ns = L"draw";
const wchar_t * draw_fill_image::name = L"fill-image";

void draw_fill_image::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void draw_fill_image::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
 			CP_XML_ATTR_OPT(L"draw:name",	draw_name_);
			CP_XML_ATTR_OPT(L"draw:display_name",draw_display_name_);
			
			xlink_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_marker::ns = L"draw";
const wchar_t * draw_marker::name = L"marker";

void draw_marker::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void draw_marker::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
 			CP_XML_ATTR_OPT(L"draw:name",	draw_name_);
			CP_XML_ATTR_OPT(L"draw:display_name",draw_display_name_);
			
			CP_XML_ATTR_OPT(L"svg:d",	svg_d_);
			CP_XML_ATTR_OPT(L"svg:viewBox",	svg_viewBox_);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_gradient::ns = L"draw";
const wchar_t * draw_gradient::name = L"gradient";

void draw_gradient::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void draw_gradient::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"draw:start-color",draw_start_color_);
			CP_XML_ATTR_OPT(L"draw:end-color",	draw_end_color_);

			CP_XML_ATTR_OPT(L"draw:end-intensity",	draw_end_intensity_); 
			CP_XML_ATTR_OPT(L"draw:start-intensity",draw_start_intensity_);

			CP_XML_ATTR_OPT(L"draw:cx",		draw_cy_);//%
			CP_XML_ATTR_OPT(L"draw:cy",		draw_cx_);
			
			CP_XML_ATTR_OPT(L"draw:border",	draw_border_);
			CP_XML_ATTR_OPT(L"draw:angle",	draw_angle_);
			CP_XML_ATTR_OPT(L"draw:style",	draw_style_);

 			CP_XML_ATTR_OPT(L"draw:name",	draw_name_);
			CP_XML_ATTR_OPT(L"draw:display_name",draw_display_name_);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_hatch::ns = L"draw";
const wchar_t * draw_hatch::name = L"hatch";

void draw_hatch::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void draw_hatch::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"draw:color",		draw_color_);
			CP_XML_ATTR_OPT(L"draw:rotation",	draw_rotation_);
			
			CP_XML_ATTR_OPT(L"draw:distance",	draw_distance_);
			CP_XML_ATTR_OPT(L"draw:style",		draw_style_);

 			CP_XML_ATTR_OPT(L"draw:name",		draw_name_);
			CP_XML_ATTR_OPT(L"draw:display_name",draw_display_name_);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_opacity::ns = L"draw";
const wchar_t * draw_opacity::name = L"opacity";

void draw_opacity::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void draw_opacity::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"draw:start",	draw_start_);
			CP_XML_ATTR_OPT(L"draw:end",	draw_end_);
			
			CP_XML_ATTR_OPT(L"draw:cx",		draw_cx_);
			CP_XML_ATTR_OPT(L"draw:cy",		draw_cy_);

			CP_XML_ATTR_OPT(L"draw:border",	draw_border_);
			CP_XML_ATTR_OPT(L"draw:angle",	draw_angle_);
			CP_XML_ATTR_OPT(L"draw:style",	draw_style_);

 			CP_XML_ATTR_OPT(L"draw:name",			draw_name_);
			CP_XML_ATTR_OPT(L"draw:display_name",	draw_display_name_);
		}
	}
}
// style:style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style::ns = L"style";
const wchar_t * style::name = L"style";

void style::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"map")
    {
        CP_CREATE_ELEMENT(style_map_);
    }
    else
        style_content_.create_child_element(Ns, Name);
}

void style::add_child_element(office_element_ptr & child)
{
 	if (!child)return;

	ElementType type = child->get_type();
	
	if  (type == typeStyleMap)
    {
        style_map_.push_back(child);
    } 
    else
        style_content_.add_child_element(child);
}
void style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"style:default-outline-level", style_default_outline_level_);
			CP_XML_ATTR_OPT(L"style:parent-style-name", style_parent_style_name_);
			CP_XML_ATTR_OPT(L"style:next-style-name", style_next_style_name_);
			CP_XML_ATTR_OPT(L"style:list-style-name", style_list_style_name_);
			CP_XML_ATTR_OPT(L"style:auto-update", style_auto_update_);
			CP_XML_ATTR_OPT(L"style:data-style-name", style_data_style_name_);
			CP_XML_ATTR_OPT(L"style:class", style_class_);
			CP_XML_ATTR_OPT(L"style:master-page-name", style_master_page_name_);
			CP_XML_ATTR_OPT(L"style:display-name", style_display_name_);
			CP_XML_ATTR(L"style:family", style_family_);
			CP_XML_ATTR(L"style:name", style_name_);
			
			style_content_.serialize(CP_XML_STREAM());
		}
	}
}
// styles & draw_styles
//////////////////////////////////////////////////////////////////////////////////////////////////

void styles::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"style", L"style")
    {
        CP_CREATE_ELEMENT_SIMPLE(style_style_);
    } 
    else if CP_CHECK_NAME(L"text", L"list-style")
    {
        CP_CREATE_ELEMENT_SIMPLE(text_list_style_);
    }
    else if (
        CP_CHECK_NAME(L"number", L"number-style") ||
        CP_CHECK_NAME(L"number", L"currency-style") ||
        CP_CHECK_NAME(L"number", L"percentage-style") ||
        CP_CHECK_NAME(L"number", L"date-style") ||
        CP_CHECK_NAME(L"number", L"time-style") ||
        CP_CHECK_NAME(L"number", L"number-boolean-style")
        )
    {
        CP_CREATE_ELEMENT_SIMPLE(number_styles_);
    }   
    else
    {
        CP_NOT_APPLICABLE_ELM_SIMPLE(L"styles");
    }
}
void styles::add_child_element(office_element_ptr & child, odf_conversion_context * Context)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)
	{
	case typeStyleStyle:	
		style_style_.push_back(child);
		break;
	case typeTextListStyle:	
		text_list_style_.push_back(child);
		break;
	case typeNumberTimeStyle:	
	case typeNumberDataStyle:	
	case typeNumberPercentageStyle:	
	case typeNumberCurrencyStyle:	
	case typeNumberNumberStyle:	
		//boolean
		number_styles_.push_back(child);
		break; 
	}
}
void styles::serialize(std::wostream & strm)//content from different element
{
	BOOST_FOREACH(office_element_ptr & elm, number_styles_)
    {
		elm->serialize(strm);
	}
	BOOST_FOREACH(office_element_ptr & elm, style_style_)
    {
		elm->serialize(strm);
	}	
	BOOST_FOREACH(office_element_ptr & elm, text_list_style_)
    {
		elm->serialize(strm);
	}	
}
void templates::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-template")
    {
        CP_CREATE_ELEMENT_SIMPLE(table_templates_);
    } 
}
void templates::add_child_element(office_element_ptr & child)
{
 	if (!child)return;

	ElementType type = child->get_type();
	
	if (type == typeTableTemplate)
    {
        table_templates_.push_back(child);
    }
}
void templates::serialize(std::wostream & strm)
{
	BOOST_FOREACH(office_element_ptr & elm, table_templates_)
	{
		elm->serialize(strm);
	}	
}
void draw_styles::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"draw", L"gradient")
    {
        CP_CREATE_ELEMENT_SIMPLE(draw_gradient_);
    } 
    else if CP_CHECK_NAME(L"draw", L"hatch")
    {
        CP_CREATE_ELEMENT_SIMPLE(draw_hatch_);
    }
    else if CP_CHECK_NAME(L"draw", L"fill-image")
    {
        CP_CREATE_ELEMENT_SIMPLE(draw_fill_image_);
    }
    else if CP_CHECK_NAME(L"draw", L"opacity")
	{
         CP_CREATE_ELEMENT_SIMPLE(draw_opacity_);  
	}
	else if CP_CHECK_NAME(L"draw", L"marker")
	{
         CP_CREATE_ELEMENT_SIMPLE(draw_marker_);
	}
    else if CP_CHECK_NAME(L"draw", L"stroke-dash")
	{
         CP_CREATE_ELEMENT_SIMPLE(draw_stroke_dash_);
	}
    else
    {
        CP_NOT_APPLICABLE_ELM_SIMPLE(L"draw_styles");
    }
}

void draw_styles::add_child_element(office_element_ptr & child, odf_conversion_context * Context)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)
	{
	case typeStyleDrawGradient:		draw_gradient_.push_back(child); break;
	case typeStyleDrawHatch:		draw_hatch_.push_back(child); break;
	case typeStyleDrawFillImage:	draw_fill_image_.push_back(child); break;
	case typeStyleDrawOpacity:		draw_opacity_.push_back(child); break;
	case typeStyleDrawMarker:		draw_marker_.push_back(child); break;
	case typeStyleDrawStrokeDash:	draw_stroke_dash_.push_back(child); break;
	}	
}

void draw_styles::serialize(std::wostream & strm)
{
	BOOST_FOREACH(office_element_ptr & elm, draw_fill_image_)
    {
		elm->serialize(strm);
	}
	BOOST_FOREACH(office_element_ptr & elm, draw_hatch_)
    {
		elm->serialize(strm);
	}	
	BOOST_FOREACH(office_element_ptr & elm, draw_gradient_)
    {
		elm->serialize(strm);
	}	
	BOOST_FOREACH(office_element_ptr & elm, draw_opacity_)
    {
		elm->serialize(strm);
	}	
	BOOST_FOREACH(office_element_ptr & elm, draw_marker_)
    {
		elm->serialize(strm);
	}
	BOOST_FOREACH(office_element_ptr & elm, draw_stroke_dash_)
    {
		elm->serialize(strm);
	}
}
// office:automatic-styles
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_automatic_styles::ns = L"office";
const wchar_t * office_automatic_styles::name = L"automatic-styles";

void office_automatic_styles::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"page-layout" == Name)
    {
        CP_CREATE_ELEMENT(style_page_layout_);
    }
    else
    {
        styles_.create_child_element(Ns, Name, getContext());            
    }
}

void office_automatic_styles::add_child_element(office_element_ptr & child)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)
	{
	case typeStylePageLayout:		
		style_page_layout_.push_back(child); 
		break;
	default:
		styles_.add_child_element(child, getContext());    
		break;
	}
}
void office_automatic_styles::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(office_element_ptr elm, style_page_layout_)
			{
				elm->serialize(CP_XML_STREAM());
			}
	
			styles_.serialize(CP_XML_STREAM());
		}
	}
}
// office:master-styles
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_master_page_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:name",				style_name_);
    CP_XML_ATTR_OPT(L"style:display-name",		style_display_name_);
    CP_XML_ATTR_OPT(L"style:page-layout-name",	style_page_layout_name_);
    CP_XML_ATTR_OPT(L"draw:style-name",			draw_style_name_);
    CP_XML_ATTR_OPT(L"style:next-style-name",	style_next_style_name_);
}


const wchar_t * office_master_styles::ns = L"office";
const wchar_t * office_master_styles::name = L"master-styles";

void office_master_styles::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"master-page" == Name)
        CP_CREATE_ELEMENT(style_master_page_);
    else if (L"style" == Ns && L"handout-master" == Name)
        CP_CREATE_ELEMENT(style_handout_master_);
    else if (L"style" == Ns && L"draw-layer-set" == Name)
        CP_CREATE_ELEMENT(draw_layer_set_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

void office_master_styles::add_child_element(office_element_ptr & child)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)
	{
	case typeStyleMasterPage:		
		style_master_page_.push_back(child); 
		break;
	case typeStyleHandoutMasterPage:		
		style_handout_master_ = child; 
		break;
	case typeStyleDrawLayerSet:		
		draw_layer_set_ = child; 
		break;
	}
}
void office_master_styles::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (draw_layer_set_)
				draw_layer_set_->serialize(CP_XML_STREAM());
			
			BOOST_FOREACH(office_element_ptr elm, style_master_page_)
			{
				elm->serialize(CP_XML_STREAM());
			}
			
			if (style_handout_master_)
				style_handout_master_->serialize(CP_XML_STREAM());

		}
	}
}

// office:styles
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_styles::ns = L"office";
const wchar_t * office_styles::name = L"styles";


void office_styles::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (CP_CHECK_NAME(L"style",		L"style") ||
        CP_CHECK_NAME(L"text",		L"list-style") ||        
		CP_CHECK_NAME(L"number",	L"number-style") ||
        CP_CHECK_NAME(L"number",	L"currency-style") || 
        CP_CHECK_NAME(L"number",	L"text-style") ||
        CP_CHECK_NAME(L"number",	L"percentage-style") ||
        CP_CHECK_NAME(L"number",	L"date-style") ||
        CP_CHECK_NAME(L"number",	L"boolean-style")
        )
    {
        styles_.create_child_element( Ns, Name, getContext());
    }
    else if (L"style" == Ns && L"default-style" == Name)
        CP_CREATE_ELEMENT(style_default_style_);

    else if (L"style" == Ns && L"presentation-page-layout" == Name)
        CP_CREATE_ELEMENT(style_presentation_page_layout_);

    else if(CP_CHECK_NAME(L"draw",		L"gradient") ||
			CP_CHECK_NAME(L"draw",		L"fill-image") ||
			CP_CHECK_NAME(L"draw",		L"hatch") ||
			CP_CHECK_NAME(L"draw",		L"marker") ||
			CP_CHECK_NAME(L"draw",		L"opacity") ||
			CP_CHECK_NAME(L"draw",		L"stroke-dash") ||
			CP_CHECK_NAME(L"svg",		L"linearGradient") ||
			CP_CHECK_NAME(L"svg",		L"radialGradient") 
			)
	{
        draw_styles_.create_child_element( Ns, Name, getContext());
	}
     else if(CP_CHECK_NAME(L"table",	L"table-template"))
	{
        templates_.create_child_element( Ns, Name, getContext());
	}
	 else if (L"text" == Ns && L"outline-style" == Name)
        CP_CREATE_ELEMENT(text_outline_style_);
    else if (L"text" == Ns && L"notes-configuration" == Name)
        CP_CREATE_ELEMENT(text_notes_configuration_);
    else if (L"text" == Ns && L"bibliography-configuration" == Name)
        CP_CREATE_ELEMENT(text_bibliography_configuration_);
    else if (L"text" == Ns && L"linenumbering-configuration" == Name)
        CP_CREATE_ELEMENT(text_linenumbering_configuration_);
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void office_styles::add_child_element(office_element_ptr & child)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)
	{
	case typeStyleStyle:	
	case typeTextListStyle:	

	case typeNumberTimeStyle:	
	case typeNumberDataStyle:	
	case typeNumberPercentageStyle:	
	case typeNumberCurrencyStyle:	
	case typeNumberNumberStyle:	
		styles_.add_child_element(child, getContext());
		break;
	case typeStyleDrawGradient:	
	case typeStyleDrawHatch:	
	case typeStyleDrawFillImage:
	case typeStyleDrawOpacity:	
	case typeStyleDrawMarker:	
	case typeStyleDrawStrokeDash:
		draw_styles_.add_child_element(child, getContext());
		break;
	case typeStyleDefaultStyle:		
		style_default_style_.push_back(child); 
		break;
	case typeStylePresentationPageLayout:		
		style_presentation_page_layout_.push_back(child); 
		break;
	case typeTableTemplate:		
		templates_.add_child_element(child);
		break;
	//....
	}
}
void office_styles::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(office_element_ptr & elm, style_default_style_)
			{
				elm->serialize(CP_XML_STREAM());
			}
			
			draw_styles_.serialize(CP_XML_STREAM());

			templates_.serialize(CP_XML_STREAM());
			
			styles_.serialize(CP_XML_STREAM());
		}
	}
}

// style:header
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header::ns = L"style";
const wchar_t * style_header::name = L"header";

void style_header::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    content().header_footer_content_.create_child_element( Ns, Name, getContext());
}
void style_header::add_child_element(office_element_ptr & child)
{
	content().header_footer_content_.add_child_element( child);
}
void style_header::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			content().header_footer_content_.serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer::ns = L"style";
const wchar_t * style_footer::name = L"footer";


void style_footer::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    content().header_footer_content_.create_child_element(Ns, Name, getContext());
}
void style_footer::add_child_element(office_element_ptr & child)
{
    content().header_footer_content_.add_child_element( child);
}
void style_footer::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			content().header_footer_content_.serialize(CP_XML_STREAM());
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header_first::ns = L"style";
const wchar_t * style_header_first::name = L"header-first";

void style_header_first::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    content().header_footer_content_.create_child_element(Ns, Name, getContext());
}
void style_header_first::add_child_element(office_element_ptr & child)
{
    content().header_footer_content_.add_child_element(child);
}
void style_header_first::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			content().header_footer_content_.serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer_first::ns = L"style";
const wchar_t * style_footer_first::name = L"footer-first";

void style_footer_first::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    content().header_footer_content_.create_child_element( Ns, Name, getContext());
}
void style_footer_first::add_child_element( office_element_ptr & child)
{
    content().header_footer_content_.add_child_element( child);
}
void style_footer_first::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			content().header_footer_content_.serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header_left::ns = L"style";
const wchar_t * style_header_left::name = L"header-left";

void style_header_left::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    content().header_footer_content_.create_child_element( Ns, Name, getContext());
}
void style_header_left::add_child_element(office_element_ptr & child)
{
    content().header_footer_content_.add_child_element( child);
}

void style_header_left::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			content().header_footer_content_.serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer_left::ns = L"style";
const wchar_t * style_footer_left::name = L"footer-left";

void style_footer_left::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    content().header_footer_content_.create_child_element( Ns, Name, getContext());
}
void style_footer_left::add_child_element(office_element_ptr & child)
{
    content().header_footer_content_.add_child_element(child);
}
void style_footer_left::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			content().header_footer_content_.serialize(CP_XML_STREAM());
		}
	}
}

/// style:header-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_header_style::ns = L"style";
const wchar_t * style_header_style::name = L"header-style";

void style_header_style::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"header-footer-properties" == Name)
    {
        CP_CREATE_ELEMENT(style_header_footer_properties_);        
    } 
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_header_style::add_child_element(office_element_ptr & child)
{
 	if (!child)return;

	ElementType type = child->get_type();

	if (type == typeStyleHeaderFooterProperties)
        style_header_footer_properties_  = child;

}
void style_header_style::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {	 
			if (style_header_footer_properties_)	style_header_footer_properties_->serialize(CP_XML_STREAM());
		}
	}
}
/// style:footer-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_footer_style::ns = L"style";
const wchar_t * style_footer_style::name = L"footer-style";

void style_footer_style::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"header-footer-properties" == Name)
    {
        CP_CREATE_ELEMENT(style_header_footer_properties_);        
    } 
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_footer_style::add_child_element(office_element_ptr & child)
{
 	if (!child)return;

	ElementType type = child->get_type();

	if (type == typeStyleHeaderFooterProperties)
        style_header_footer_properties_  = child;

}
void style_footer_style::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {	 
			if (style_header_footer_properties_)	style_header_footer_properties_->serialize(CP_XML_STREAM());
		}
	}
}
/// style:page-layout
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_page_layout_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:name",				style_name_);
    CP_XML_ATTR_OPT(L"style:page-usage",		style_page_usage_);
}
const wchar_t * style_page_layout::ns = L"style";
const wchar_t * style_page_layout::name = L"page-layout";

void style_page_layout::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"header-style" == Name)
    {
        CP_CREATE_ELEMENT(style_header_style_);        
    } 
    else if (L"style" == Ns && L"footer-style" == Name)
    {
        CP_CREATE_ELEMENT(style_footer_style_);
    }
    else if (L"style" == Ns && L"page-layout-properties" == Name)
    {
        CP_CREATE_ELEMENT(style_page_layout_properties_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_page_layout::add_child_element( office_element_ptr & child)
{
	if (!child)return;

	ElementType type = child->get_type();

	switch(type)
	{
	case typeStyleHeaderStyle:	
		style_header_style_ = child;
		break;
	case typeStyleFooterStyle:
        style_footer_style_ = child;
		break;
	case typeStylePageLayout:
		style_page_layout_properties_ = child;
		break;
	}
}
void style_page_layout::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			style_page_layout_attlist_.serialize( CP_GET_XML_NODE());
			
			if (style_page_layout_properties_)	style_page_layout_properties_->serialize(CP_XML_STREAM());
			if (style_header_style_)			style_header_style_->serialize(CP_XML_STREAM());
			if (style_footer_style_)			style_footer_style_->serialize(CP_XML_STREAM());
		}
	}
}
/// style:footnote-sep
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_footnote_sep_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:width", style_width_);
    CP_XML_ATTR_OPT(L"style:rel-width", style_rel_width_);
    CP_XML_ATTR_OPT(L"style:color", style_color_);
    CP_XML_ATTR_OPT(L"style:line-style", style_line_style_);
    CP_XML_ATTR_OPT(L"style:type", style_adjustment_); // default Left
    CP_XML_ATTR_OPT(L"style:distance-before-sep", style_distance_before_sep_);
    CP_XML_ATTR_OPT(L"style:distance-after-sep", style_distance_after_sep_);
}

const wchar_t * style_footnote_sep::ns = L"style";
const wchar_t * style_footnote_sep::name = L"footnote-sep";

void style_footnote_sep::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void style_footnote_sep::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			style_footnote_sep_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}



/// style:master-page
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_master_page::ns = L"style";
const wchar_t * style_master_page::name = L"master-page";

void style_master_page::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"header" == Name)
        CP_CREATE_ELEMENT(style_header_);
	else if (L"style" == Ns && L"header-first" == Name)
        CP_CREATE_ELEMENT(style_header_first_);
	else if (L"style" == Ns && L"header-left" == Name)
        CP_CREATE_ELEMENT(style_header_left_);

	else if (L"style" == Ns && L"footer" == Name)
        CP_CREATE_ELEMENT(style_footer_);
	else if (L"style" == Ns && L"footer-first" == Name)
        CP_CREATE_ELEMENT(style_footer_first_);
	else if (L"style" == Ns && L"footer-left" == Name)
        CP_CREATE_ELEMENT(style_footer_left_);

    else if (L"office" == Ns && L"forms" == Name)
        CP_CREATE_ELEMENT(office_forms_);
    else if (L"style" == Ns && L"style" == Name)
        CP_CREATE_ELEMENT(style_style_);
    else if (L"presentation" == Ns && L"notes" == Name)
        CP_CREATE_ELEMENT(presentation_notes_); 
    else
    {
        CP_CREATE_ELEMENT(content_);   
    }
}

void style_master_page::add_child_element(office_element_ptr & child)
{
	if (!child)return;

	ElementType type = child->get_type();
	switch(type)
	{
	case typeStyleHeader:
        style_header_ = child; 
		break;
	case typeStyleHeaderFirst:
		style_header_first_ = child;      
		break;
	case typeStyleHeaderLeft:
		style_header_left_ = child;      
		break;
	case typeStyleFooter:
		style_footer_ = child;      
		break;
	case typeStyleFooterFirst:
		style_footer_first_ = child;      
		break;
	case typeStyleFooterLeft:
		style_footer_left_ = child;      
		break;
	case typeOfficeForms:
		office_forms_ = child;      
		break;
	case typePresentationNotes:
        presentation_notes_ = child;     
	case typeStyleStyle:
		style_style_.push_back(child);
		break;
	default:
		content_.push_back(child);
		break;
    }
}
void style_master_page::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			style_master_page_attlist_.serialize( CP_GET_XML_NODE());
			
			if (style_footer_)			style_footer_->serialize(CP_XML_STREAM());
			if (style_header_)			style_header_->serialize(CP_XML_STREAM());
			
			if (style_header_left_)		style_header_left_->serialize(CP_XML_STREAM());
			if (style_header_first_)	style_header_first_->serialize(CP_XML_STREAM());
			
			if (style_footer_left_)		style_footer_left_->serialize(CP_XML_STREAM());
			if (style_footer_first_)	style_footer_first_->serialize(CP_XML_STREAM());
			
			if (office_forms_)			office_forms_->serialize(CP_XML_STREAM());
			
			BOOST_FOREACH(const office_element_ptr & elm, style_style_)
			{
				elm->serialize(CP_XML_STREAM());
			}
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
int style_master_page::find_placeHolderIndex(presentation_class::type placeHolder,int & last_idx)
{
	int idx = -1;

	//int i=0;
	//int size = content_.size();
	//
	//while(true)
 //   {
	//	if (i>=size)break;
	//	if (content_[i]->get_type() == odf::typeDrawFrame)
	//	{
	//		draw_frame* frame = dynamic_cast<draw_frame *>(content_[i].get());

	//		if (frame->idx_in_owner<0)frame->idx_in_owner = last_idx++;

	//		if ((frame) && (frame->common_presentation_attlist_.presentation_class_) && 
	//					   (frame->common_presentation_attlist_.presentation_class_->get_type()== placeHolder))
	//		{
	//			idx = frame->idx_in_owner;
	//			break;
	//		}
	//	}
	//	i++;
 //   }
	return idx;
}

/// text:notes-configuration
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_notes_configuration::ns = L"text";
const wchar_t * text_notes_configuration::name = L"notes-configuration";

void text_notes_configuration::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"note-continuation-notice-forward")
        CP_CREATE_ELEMENT(text_note_continuation_notice_forward_);
    else if CP_CHECK_NAME(L"text", L"note-continuation-notice-backward")
        CP_CREATE_ELEMENT(text_note_continuation_notice_backward_);
    else
        CP_NOT_APPLICABLE_ELM();    
}

/// style:presentation-page-layout
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_presentation_page_layout::ns = L"style";
const wchar_t * style_presentation_page_layout::name = L"presentation-page-layout";

void style_presentation_page_layout::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"presentation" == Ns && L"placeholder" == Name)
    {
        CP_CREATE_ELEMENT(content_);        
    } 
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

}
}
