#include "precompiled_cpodf.h"

#include "odf_style_context.h"
//#include "ods_textcontext.h"
#include "ods_conversion_context.h"

#include "logging.h"
#include "styles.h"

#include <boost/foreach.hpp>
#include <iostream>

namespace cpdoccore {
namespace odf {

odf_style_context::odf_style_context(odf_conversion_context & Context/*, xlsx_text_context & textCotnext*/): context_(Context)
{        
}

odf_style_state & odf_style_context::state()
{
    return style_state_list_.back();
}

const odf_style_state & odf_style_context::state() const
{
    return style_state_list_.back();
}

void odf_style_context::start_style(std::wstring name,const style_family family, bool automatic )
{
	int id = style_state_list_.size();

	office_element_ptr elm;
	create_element(L"style", L"style", elm, &context_);
	
	style_state_list_.push_back( odf_style_state(context_, elm, family) ); 
///////////////////////////////////////	
	state().set_name(name);
	state().set_automatic(automatic);

	state().style_oox_id_ = id;
}

void odf_style_context::process_automatic(office_element_ptr root )
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->automatic_== true && it->odf_style_)
			root->add_child_element(it->odf_style_);
	}
}

void odf_style_context::process_office(office_element_ptr root )
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->automatic_== false && it->master_ == false && it->odf_style_)
			root->add_child_element(it->odf_style_);
	}
}
void odf_style_context::process_master(office_element_ptr root )
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->master_ == true && it->odf_style_)
			root->add_child_element(it->odf_style_);
	}
}
//office_element_ptr & odf_style_context::find_by_id()
//{
//	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
//	{
//		if (it->odf_style_)
//		{
//			if (it->odf_style_->style_family_ == family && it->style_oox_id_ == id)
//				return it->get_office_element();
//		}
//	}
//	return office_element_ptr();
//}
void odf_style_context::end_style()
{
	state().convert();
}

}
}