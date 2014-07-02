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

	office_element_ptr elm;
	style_family family = style_family::MasterPage;
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

	style_context_->add_style(elm, true, true);
	
	layout_state_list_.push_back( odf_layout_state(elm) ); 
///////////////////////////////////////

	layout_state_list_.back().set_name(odf_name);
}

void odf_page_layout_context::set_page_margin(_CP_OPT(double) top, _CP_OPT(double) left, _CP_OPT(double) bottom, _CP_OPT(double) right, _CP_OPT(double) header, _CP_OPT(double) footer)
{
	if (layout_state_list_.size() < 1) return;
	style_page_layout_properties * props = layout_state_list_.back().get_properties();
	if (props == NULL)
	{
		office_element_ptr elm;
		create_element(L"style", L"page-layout-properties", elm, odf_context_);
		layout_state_list_.back().add_child(elm, office_element_ptr(),L"");
		props = layout_state_list_.back().get_properties();
	}
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

void odf_page_layout_context::set_page_orientation(int type)
{
	if (layout_state_list_.size() < 1) return;
	style_page_layout_properties * props = layout_state_list_.back().get_properties();
	if (props == NULL)
	{
		office_element_ptr elm;
		create_element(L"style", L"page-layout-properties", elm, odf_context_);
		layout_state_list_.back().add_child(elm, office_element_ptr(),L"");
		props = layout_state_list_.back().get_properties();
	}
	if (!props)return;

	if (type == 2)
		props->style_page_layout_properties_attlist_.style_print_orientation_ = L"landscape";
	else
		props->style_page_layout_properties_attlist_.style_print_orientation_ = L"portrait";

}

}
}