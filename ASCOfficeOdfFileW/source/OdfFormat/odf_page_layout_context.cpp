#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "odf_page_layout_context.h" 

#include "odf_style_context.h"
#include "ods_conversion_context.h"

#include "styles.h"
#include "style_page_layout_properties.h" 


namespace cpdoccore {
namespace odf {

odf_page_layout_context::odf_page_layout_context(odf_conversion_context * Context)
{
	odf_context_	= Context;
	style_context_	= Context->styles_context();       
}


odf_layout_state & odf_page_layout_context::last_layout()
{
	if (layout_state_list_.size() >0)
		return layout_state_list_.back();
	else
		throw;
}
odf_master_state & odf_page_layout_context::last_master()
{
	if (master_state_list_.size() >0)
		return master_state_list_.back();
	else
		throw;
}

void odf_page_layout_context::create_master_page(std::wstring oox_name)
{
	std::wstring odf_name = oox_name;	
	style_family family = style_family::MasterPage;

	office_element_ptr elm;
	create_element(L"style", L"master-page", elm, odf_context_);

	if (!elm) return;

	style_context_->add_master_style(elm);
	
	master_state_list_.push_back( odf_master_state(elm) ); 
///////////////////////////////////////
	if (odf_name.length() <1)odf_name =L"MasterPage" + boost::lexical_cast<std::wstring>(master_state_list_.size());
	master_state_list_.back().set_name(odf_name);
/////////////////////////

	//default layout
	create_layout_page();
	master_state_list_.back().set_layout_name(layout_state_list_.back().get_name());
}

void odf_page_layout_context::set_current_master_page_base()
{
	style_context_->set_current_master_page_base();
}

void odf_page_layout_context::set_styles_context(odf_style_context * Context)
{
	style_context_	= Context;       
}

void odf_page_layout_context::create_layout_page()
{
	office_element_ptr elm;
	style_family family;

	family = style_family::MasterPage;
	create_element(L"style", L"page-layout", elm, odf_context_);

	std::wstring odf_name = style_context_->find_free_name(style_family::PageLayout);
	if (!elm) return;

	style_context_->add_style(elm, true, true, style_family::PageLayout);
	
	layout_state_list_.push_back( odf_layout_state(elm) ); 
///////////////////////////////////////

	layout_state_list_.back().set_name(odf_name);
}

void odf_page_layout_context::set_page_margin(_CP_OPT(double) top, _CP_OPT(double) left, _CP_OPT(double) bottom, _CP_OPT(double) right, _CP_OPT(double) header, _CP_OPT(double) footer)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (top)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = 
												length(length(*top,length::inch).get_value_unit(length::cm),length::cm);
	if (bottom)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_= 
												length(length(*bottom,length::inch).get_value_unit(length::cm),length::cm);
	if (left)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_ = 
												length(length(*left,length::inch).get_value_unit(length::cm),length::cm);
	if (right)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_ = 
												length(length(*right,length::inch).get_value_unit(length::cm),length::cm);
}
void odf_page_layout_context::set_page_margin(_CP_OPT(length) top, _CP_OPT(length) left, _CP_OPT(length) bottom, _CP_OPT(length) right)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (top)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_ = 
												length(top->get_value_unit(length::cm),length::cm);
	if (bottom)
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_= 
												length(bottom->get_value_unit(length::cm),length::cm);
	if (left)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_left_ = 
												length(left->get_value_unit(length::cm),length::cm);
	if (right)
		props->style_page_layout_properties_attlist_.common_horizontal_margin_attlist_.fo_margin_right_ = 
												length(right->get_value_unit(length::cm),length::cm);
}
void odf_page_layout_context::set_page_gutter(_CP_OPT(length) length_)
{
	if (!length_) return;


}
void odf_page_layout_context::set_page_footer(_CP_OPT(length) length_)
{
	if (!length_) return;
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_)
	{
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_= 
			props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_->get_length() + length(length_->get_value_unit(length::cm),length::cm);
	}
	else
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_bottom_= length(length_->get_value_unit(length::cm),length::cm);
///////////////////////////////////////////////////////////////
	office_element_ptr elm;
	create_element(L"style", L"footer", elm, odf_context_);
	
	master_state_list_.back().add_footer(elm);
}
void odf_page_layout_context::set_page_header(_CP_OPT(length) length_)
{
	if (!length_) return;
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_)
	{
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_= 
			props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_->get_length() + length(length_->get_value_unit(length::cm),length::cm);
	}
	else
		props->style_page_layout_properties_attlist_.common_vertical_margin_attlist_.fo_margin_top_= length(length_->get_value_unit(length::cm),length::cm);

	office_element_ptr elm;
	create_element(L"style", L"header", elm, odf_context_);
	
	master_state_list_.back().add_header(elm);
}
void odf_page_layout_context::set_page_border_shadow(bool val)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	props->style_page_layout_properties_attlist_.common_shadow_attlist_.style_shadow_ = L"#000000 0.159cm 0.159cm";
}
void odf_page_layout_context::set_page_border(std::wstring top, std::wstring left, std::wstring bottom, std::wstring right)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_ = left;
	}
	else
	{
		if (bottom.length() >0 )props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0 )	props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) props->style_page_layout_properties_attlist_.common_border_attlist_.fo_border_right_	= right;
	}
}
void odf_page_layout_context::set_page_size(_CP_OPT(length) width, _CP_OPT(length) height)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (width)
		props->style_page_layout_properties_attlist_.fo_page_width_ = 
												length(width->get_value_unit(length::cm),length::cm);
	if (height)
		props->style_page_layout_properties_attlist_.fo_page_height_ = 
												length(height->get_value_unit(length::cm),length::cm);
}

style_page_layout_properties * odf_page_layout_context::get_properties()
{
	if (layout_state_list_.size() < 1) return NULL;
	style_page_layout_properties * props = layout_state_list_.back().get_properties();
	if (props == NULL)
	{
		office_element_ptr elm;
		create_element(L"style", L"page-layout-properties", elm, odf_context_);
		layout_state_list_.back().add_child(elm, office_element_ptr(),L"");
		props = layout_state_list_.back().get_properties();
	}
	return props;
}
void odf_page_layout_context::set_page_orientation(int type)
{
	style_page_layout_properties * props = get_properties();
	if (!props)return;

	if (type == 0)
		props->style_page_layout_properties_attlist_.style_print_orientation_ = L"landscape";
	else
		props->style_page_layout_properties_attlist_.style_print_orientation_ = L"portrait";

}

}
}