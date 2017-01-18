/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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


#include "ods_table_context.h"
#include "table.h"

#include "ods_conversion_context.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "../../../ASCOfficeOdfFile/formulasconvert/formulasconvert.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

ods_table_context::ods_table_context(ods_conversion_context & Context): context_(Context)
{        
}

ods_table_state & ods_table_context::state()
{
    return table_state_list_.back();
}

void ods_table_context::start_table_part(std::wstring name, std::wstring ref)
{
	if (!table_database_ranges_.root) create_element(L"table", L"database-ranges",table_database_ranges_.root,&context_);

	office_element_ptr elm;
	create_element(L"table", L"database-range",elm, &context_);
	table_database_range * d_range = dynamic_cast<table_database_range*>(elm.get());

	if (!d_range)return;

	formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_range = formulas_converter.convert_named_ref(ref);
	boost::algorithm::replace_all(odf_range, L"[", L"");
	boost::algorithm::replace_all(odf_range, L"]", L"");

	d_range->table_target_range_address_	= odf_range;
	d_range->table_name_					= name;
	//d_range->table_display_filter_buttons_= true;

	table_database_ranges_.root->add_child_element(elm);
	table_database_ranges_.elements.push_back(elm);
}

void ods_table_context::set_table_part_autofilter(bool val)
{
	if (!val) return;
	if (table_database_ranges_.elements.empty()) return;

	table_database_range * d_range = dynamic_cast<table_database_range*>(table_database_ranges_.elements.back().get());

	if (!d_range) return;

	d_range->table_display_filter_buttons_= true;
}

void ods_table_context::end_table_part()
{
}

void ods_table_context::add_autofilter(std::wstring ref)
{
	if (!table_database_ranges_.root) create_element(L"table", L"database-ranges",table_database_ranges_.root,&context_);

	office_element_ptr elm;
	create_element(L"table", L"database-range",elm,&context_);
	table_database_range * d_range = dynamic_cast<table_database_range*>(elm.get());

	if (!d_range)return;

	formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_range = formulas_converter.convert_named_ref(ref);
	boost::algorithm::replace_all(odf_range, L"[", L"");
	boost::algorithm::replace_all(odf_range, L"]", L"");

	d_range->table_target_range_address_ = odf_range;
	d_range->table_display_filter_buttons_= true;

	table_database_ranges_.root->add_child_element(elm);
	table_database_ranges_.elements.push_back(elm);

}
void ods_table_context::start_defined_expressions(office_element_ptr & root_elm)
{
	table_defined_expressions_.root = root_elm;
}

void ods_table_context::add_defined_range(const std::wstring & name, const std::wstring & cell_range, int sheet_id, bool printable)
{
	office_element_ptr elm;
	create_element(L"table", L"named-range",elm, &context_);
	
	table_named_range* named_range = dynamic_cast<table_named_range*>(elm.get());
	if (named_range == NULL)return;

	formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_range = formulas_converter.convert_named_ref(cell_range);//todo - разделить конвертацию диапазонов/рэнжей на c [] и без
	boost::algorithm::replace_all(odf_range, L"[", L"");
	boost::algorithm::replace_all(odf_range, L"]", L"");
	std::wstring odf_base_cell = formulas_converter.find_base_cell(cell_range);

	named_range->table_name_ = name;
	named_range->table_cell_range_address_ = odf_range;
	if (printable)
		named_range->table_range_usable_as_ = L"print-range";
	
	if (odf_base_cell.length() > 0)
		named_range->table_base_cell_address_ =  odf_base_cell;

	table_defined_expressions_.elements.push_back(elm);
	
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
	{
		if (!table_defined_expressions_.root)create_element(L"table", L"named-expressions",table_defined_expressions_.root,&context_);

		table_defined_expressions_.root->add_child_element(elm);
	}

}
void ods_table_context::add_defined_expression(const std::wstring & name, const std::wstring & value, int sheet_id, bool printable)
{
	office_element_ptr elm;
	create_element(L"table", L"named-expression",elm, &context_);
	
	table_named_expression* named_expression = dynamic_cast<table_named_expression*>(elm.get());
	if (named_expression == NULL)return;

	formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_value		= formulas_converter.convert_named_ref(value);
	std::wstring odf_base_cell	= formulas_converter.find_base_cell(value);

	named_expression->table_name_		= name;
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
				if ( printable)
				{
					boost::algorithm::replace_all(odf_value, L"[", L"");
					boost::algorithm::replace_all(odf_value, L"]", L"");
					boost::algorithm::replace_all(odf_value, L";", L" ");

					iter->set_print_range(odf_value);
				}
				break;
			}
			i++;
		}
	}
	else
	{
		if (!table_defined_expressions_.root)create_element(L"table", L"named-expressions",table_defined_expressions_.root,&context_);
		table_defined_expressions_.root->add_child_element(elm);
	}

	if (odf_base_cell.length() > 0)
		named_expression->table_base_cell_address_ =  odf_base_cell;


	table_defined_expressions_.elements.push_back(elm);
}

void ods_table_context::start_table(office_element_ptr & elm)
{
	table_state_list_.push_back( ods_table_state(&context_, elm) );
	
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
