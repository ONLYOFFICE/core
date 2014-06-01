#include "precompiled_cpodf.h"

#include "ods_table_context.h"
#include "table.h"

#include "ods_conversion_context.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include <cpdoccore/formulasconvert.h>

namespace cpdoccore {
namespace odf {

ods_table_context::ods_table_context(ods_conversion_context & Context): context_(Context)
{        
}

ods_table_state & ods_table_context::state()
{
    return table_state_list_.back();
}

void ods_table_context::start_defined_expressions(office_element_ptr & root_elm)
{
	table_defined_expressions_.root = root_elm;
}

void ods_table_context::add_defined_range(std::wstring & name,std::wstring & cell_range, int sheet_id)
{
	office_element_ptr elm;
	create_element(L"table", L"named-range",elm, &context_);
	
	table_named_range* named_range = dynamic_cast<table_named_range*>(elm.get());
	if (named_range == NULL)return;

	static formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_cell_range = formulas_converter.convert_named_ref(cell_range);
	std::wstring odf_base_cell = formulas_converter.find_base_cell(cell_range);

	named_range->table_name_ = name;
	named_range->table_cell_range_address_ = odf_cell_range;
	
	if (odf_base_cell.length() > 0)
		named_range->table_base_cell_address_ =  odf_base_cell;

	table_defined_expressions_.defined.push_back(elm);
	
	if (sheet_id >=0)
	{
		int i=0;
		for (std::list<ods_table_state>::iterator iter = table_state_list_.begin(); iter != table_state_list_.end(); iter++)
		{
			if (i == sheet_id)
			{
				iter->add_definded_expression(elm);
				break;
			}
			i++;
		}
	}
	else
		table_defined_expressions_.root->add_child_element(elm);

}
void ods_table_context::add_defined_expression(std::wstring & name,std::wstring & value, int sheet_id)
{
	office_element_ptr elm;
	create_element(L"table", L"named-expression",elm, &context_);
	
	table_named_expression* named_expression = dynamic_cast<table_named_expression*>(elm.get());
	if (named_expression == NULL)return;

	static formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_value = formulas_converter.convert_named_ref(value);
	std::wstring odf_base_cell = formulas_converter.find_base_cell(value);

	named_expression->table_name_ = name;
	named_expression->table_expression_ = odf_value;
	
	if (sheet_id >=0)
	{
		int i=0;
		for (std::list<ods_table_state>::iterator iter = table_state_list_.begin(); iter != table_state_list_.end(); iter++)
		{
			if (i == sheet_id)
			{
				odf_base_cell = iter->office_table_name_ + L".$A$1";
				iter->add_definded_expression(elm);
				break;
			}
			i++;
		}
	}
	else
		table_defined_expressions_.root->add_child_element(elm);

	if (odf_base_cell.length() > 0)
		named_expression->table_base_cell_address_ =  odf_base_cell;

	table_defined_expressions_.defined.push_back(elm);
}

void ods_table_context::start_table(office_element_ptr & elm)
{
	table_state_list_.push_back( ods_table_state(context_, elm) );
	
	std::wstring style_name_new = L"ta" + boost::lexical_cast<std::wstring>(table_state_list_.size());

	office_element_ptr & style = context_.styles_context()->add_or_find(style_name_new, style_family::Table, true);
	style->create_child_element(L"style", L"table-properties");
	

	state().set_table_style(style);
	state().set_table_hidden(false);

	//для свойств страницы, а не таблицы - нужно создать master-page c page layout и связать по имени со стилем таблицы
	//причем здесь, т.к. с другой стороны это ВСЕ еще свойства листа. то есть совйства листа разделить на свйства страницы и таблицы ..
	//todooo
	//????
}

void ods_table_context::end_table()
{
}

}
}