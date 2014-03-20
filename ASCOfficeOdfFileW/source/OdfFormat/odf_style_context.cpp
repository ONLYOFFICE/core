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

odf_style_state & odf_style_context::last_state()
{
    return style_state_list_.back();
}

const odf_style_state & odf_style_context::last_state() const
{
    return style_state_list_.back();
}

void odf_style_context::create_style(std::wstring oox_name,const style_family family, bool automatic,bool root,int oox_id)
{
	if (oox_id < 0) oox_id = style_state_list_.size();
	
	std::wstring odf_name = oox_name;
	
	if (odf_name.length() <1)odf_name = find_free_name(family);

	office_element_ptr elm;
	create_element(L"style", L"style", elm, &context_);
	
	style_state_list_.push_back( odf_style_state(context_, elm, family) ); 

///////////////////////////////////////	
	last_state().set_name(odf_name);
	last_state().set_automatic(automatic);
	last_state().set_root(root);

	last_state().style_oox_id_ = oox_id;
	last_state().style_oox_name_ = oox_name;
}
void odf_style_context::process_automatic_styles(office_element_ptr root )
{//автоматические стили для стилей
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->automatic_== true && it->root_== true && it->odf_style_)
			root->add_child_element(it->odf_style_);
	}
}
void odf_style_context::process_automatic(office_element_ptr root )
{//автоматические стили для элементов
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (/*it->automatic_== true && */it->root_== false && it->odf_style_)
			root->add_child_element(it->odf_style_);
	}
}
void odf_style_context::process_master(office_element_ptr root )
{
	for (std::list<odf_style_state>::iterator it = master_state_list_.begin(); it != master_state_list_.end(); it++)
	{
		root->add_child_element(it->odf_style_);
	}
}
void odf_style_context::process_office(office_element_ptr root )
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->automatic_== false && it->root_ == true && it->odf_style_)
			root->add_child_element(it->odf_style_);
	}
}
std::wstring & odf_style_context::find_odf_style_name(int oox_id_style, const style_family family)
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->odf_style_)
		{
			if (it->style_family_ == family)
			{
				if (oox_id_style >=0 && it->style_oox_id_ == oox_id_style)	return it->get_name();
			}
				
		}
	}
	return std::wstring();
}
office_element_ptr & odf_style_context::find_odf_style(int oox_id_style, const style_family family)
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->odf_style_)
		{
			if (it->style_family_ == family)
			{
				if (oox_id_style >=0 && it->style_oox_id_ == oox_id_style)	return it->get_office_element();
			}
				
		}
	}
	return office_element_ptr();
}
std::wstring odf_style_context::get_name_family(const style_family & family)
{
	switch(family.get_type())
	{
        case style_family::None:		return L"none";
		case style_family::Paragraph:	return L"P";
        case style_family::Text:		return L"T";
        case style_family::Section:		return L"sct";
        case style_family::Table:		return L"ta";
        case style_family::TableColumn: return L"co";
        case style_family::TableRow:	return L"ro";
        case style_family::TableCell:	return L"ce";
        case style_family::TablePage:	return L"tbl";//??
        case style_family::Chart:		return L"ch";
        case style_family::Default:		return L"def";
        case style_family::DrawingPage: return L"dp";
        case style_family::Graphic:		return L"gr";
        case style_family::Presentation:return L"pr";
        case style_family::Control:		return L"ctrl";//??
        case style_family::Ruby:		return L"rb";//??
	}
	return L"none";
}
std::wstring odf_style_context::find_free_name(const style_family & family)
{
	std::wstring name = get_name_family(family);
	int count =1;

	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if ((it->odf_style_) && (it->style_family_ == family))
		{
			count++;
		}
	}
	name = name + boost::lexical_cast<std::wstring>(count);
	return name;
}
office_element_ptr & odf_style_context::add_or_find(std::wstring name, const style_family family, bool automatic , bool root, int oox_id)
{
	for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); it++)
	{
		if (it->odf_style_)
		{
			if (it->style_family_ == family)
			{
				if (oox_id >=0 && it->style_oox_id_ == oox_id)					return it->get_office_element();
				if ((name.length() >=0 && it->odf_style_->get_name() == name) && it->root_ == root)	return it->get_office_element();
				//во избежания имена генерим уникальные (в принципе для root и остальных - можно одинаковые)
			}
				
		}
	}
	create_style(name,family, automatic, root,oox_id);
	
	return last_state().get_office_element();
}


}
}