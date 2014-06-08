#include "precompiled_cpodf.h"

#include "odf_page_layout_context.h" 

#include "odf_style_context.h"
#include "ods_conversion_context.h"

#include "logging.h"
#include "styles.h"

#include <boost/foreach.hpp>
#include <iostream>

namespace cpdoccore {
namespace odf {

odf_page_layout_context::odf_page_layout_context()
{        
}

void odf_page_layout_context::set_odf_context(odf_conversion_context * Context)
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

	if (odf_name.length() <1)odf_name = style_context_->find_free_name(style_family::MasterPage);
	if (!elm) return;

	style_context_->add_master_style(elm);
	
	master_state_list_.push_back( odf_master_state(elm) ); 
///////////////////////////////////////

	master_state_list_.back().set_name(odf_name);

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


}
}