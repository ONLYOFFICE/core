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


#include "ods_table_context.h"
#include "table.h"

#include "ods_conversion_context.h"
#include "logging.h"

#include <boost/algorithm/string.hpp>

#include "../../../ASCOfficeOdfFile/formulasconvert/formulasconvert.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

std::wstring getColAddress(size_t col)
{
    static const size_t r = (L'Z' - L'A' + 1);
    std::wstring res;
    size_t r0 = col / r;

    if (r0 > 0)
    {
        const std::wstring rest = getColAddress(col - r * r0);
        const std::wstring res	= getColAddress(r0-1) + rest;
        return res;
    }
    else
        return std::wstring(1, (wchar_t)(L'A' + col));
}

std::wstring getRowAddress(size_t row)
{
    return std::to_wstring(row + 1);
}

std::wstring getCellAddress(size_t col, size_t row)
{
	return getColAddress(col) + getRowAddress(row);
}

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

	if (std::wstring::npos == ref.find(L"!") )
	{
		ref = table_state_list_.back().office_table_name_ + L"!" + ref;
	}
	std::wstring odf_range = formulas_converter.convert_named_ref(ref);

	d_range->table_target_range_address_	= odf_range;
	d_range->table_name_					= name;
	//d_range->table_display_filter_buttons_= true;

	table_database_ranges_.root->add_child_element(elm);
	table_database_ranges_.elements.push_back(elm);

	table_part_state part_state;
	part_state.name = name;
	part_state.ref = ref;

	int r = ref.rfind(L":");
	if (r < 0) return;//тута однозначно .. по правилам оох

	utils::parsing_ref (ref.substr(0, r), part_state.col_start, part_state.row_start);
	utils::parsing_ref (ref.substr(r + 1, ref.size() - r), part_state.col_end, part_state.row_end);

	state().table_parts_.push_back(part_state);
}

void ods_table_context::add_table_part_column(std::wstring name)
{
	if (state().table_parts_.empty()) return;

	size_t column = state().table_parts_.back().columns.size();

	std::wstring sCol = utils::getColAddress(state().table_parts_.back().col_start + column - 1);

	std::wstring ref;//table name ????

	ref += sCol + std::to_wstring(state().table_parts_.back().row_start);
	ref += L":";
	ref += sCol + std::to_wstring(state().table_parts_.back().row_end);

	state().table_parts_.back().columns.push_back(std::make_pair(name, ref));
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

	d_range->table_target_range_address_ = odf_range;
	d_range->table_display_filter_buttons_= true;

	table_database_ranges_.root->add_child_element(elm);
	table_database_ranges_.elements.push_back(elm);

}
bool ods_table_context::start_data_validation( const std::wstring &strRef, int type)
{
	if (!table_content_validations_.root) create_element(L"table", L"content-validations", table_content_validations_.root, &context_);

	office_element_ptr elm;
	create_element(L"table", L"content-validation", elm, &context_);
	table_content_validation *validation = dynamic_cast<table_content_validation*>(elm.get());

	if (!validation) return false;

	std::vector<std::wstring> arRefs;
	boost::algorithm::split(arRefs, strRef, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
	
	data_validation_state validation_state;
	validation_state.name	= L"DataValidation_" + std::to_wstring(state().data_validations_.size() + 1);
	validation_state.elm	= elm;
	validation_state.type	= type;
	
	for (size_t i = 0; i < arRefs.size(); i++)
	{
		data_validation_state::_ref ref;
		ref.ref = arRefs[i];
		
		size_t r = arRefs[i].rfind(L":");
		if (r == std::wstring::npos)
		{
			utils::parsing_ref (arRefs[i].substr(0, r), ref.col_start, ref.row_start);
			ref.col_end = ref.col_start;
			ref.row_end = ref.row_start;
		}
		else
		{
			utils::parsing_ref (arRefs[i].substr(0, r), ref.col_start, ref.row_start);
			utils::parsing_ref (arRefs[i].substr(r + 1, arRefs[i].size() - r), ref.col_end, ref.row_end);
		}

		validation_state.refs.push_back(ref);	
	}

	if (validation_state.refs.empty()) return false;

	validation->table_base_cell_address_	= state().office_table_name_ + L"." + getCellAddress(validation_state.refs[0].col_start, validation_state.refs[0].row_start);
	validation->table_name_					= validation_state.name;

	table_content_validations_.root->add_child_element(elm);
	table_content_validations_.elements.push_back(elm);

	state().data_validations_.push_back(validation_state);

	return true;
}
void ods_table_context::set_data_validation_allow_empty(bool val)
{
	table_content_validation *validation = dynamic_cast<table_content_validation*>(state().data_validations_.back().elm.get());
	validation->table_allowempty_cell_ = val;
}
void ods_table_context::set_data_validation_content( const std::wstring &oox_formula)
{
	if (oox_formula.empty()) return;
	
	std::wstring odf_formula;
	if (oox_formula[0] == L'\"' && oox_formula[oox_formula.length() - 1] == L'\"')
	{
		std::vector<std::wstring> arItems;
		boost::algorithm::split(arItems, oox_formula.substr(1, oox_formula.length() - 2), boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
		
		for (size_t i = 0; i < arItems.size(); ++i)
		{
			odf_formula += L"\"" + arItems[i] + L"\"" + (i < arItems.size() - 1 ? L";" : L"");
		}
	}
	else
	{
		formulasconvert::oox2odf_converter formulas_converter;

		odf_formula = formulas_converter.convert_formula(oox_formula);

		if (false == odf_formula.empty())
		{
			odf_formula = odf_formula.substr(4);
		}
	}
	table_content_validation *validation = dynamic_cast<table_content_validation*>(state().data_validations_.back().elm.get());
	
	switch (state().data_validations_.back().type)
	{
	case 0://SimpleTypes::spreadsheet::validationTypeNone:
		break;
	case 1://SimpleTypes::spreadsheet::validationTypeCustom:
		break;
	case 2://SimpleTypes::spreadsheet::validationTypeDate:
		{
			odf_formula = L"of:cell-content-is-date(" + odf_formula + L")";
		}break;
	case 3://SimpleTypes::spreadsheet::validationTypeDecimal:
		{
			odf_formula = L"of:cell-content-is-decimal-number(" + odf_formula + L")";
		}break;
	case 4://SimpleTypes::spreadsheet::validationTypeList:	
		{
			odf_formula = L"of:cell-content-is-in-list(" + odf_formula + L")";
		}break;
	case 5://SimpleTypes::spreadsheet::validationTypeTextLength:
		break;
	case 6://SimpleTypes::spreadsheet::validationTypeTime:
		{
			odf_formula = L"of:cell-content-is-time(" + odf_formula + L")";
		}break;
	case 7://SimpleTypes::spreadsheet::validationTypeWhole:
		{
			odf_formula = L"of:cell-content-is-whole-number(" + odf_formula + L")";
		}break;
	}
	state().data_validations_.back().condition = odf_formula;
	validation->table_condition_ = odf_formula;
}
void ods_table_context::set_data_validation_error(const std::wstring &title, const std::wstring &content)
{
	office_element_ptr elm;
	create_element(L"table", L"error-message", elm, &context_);

	table_content_validations_.elements.back()->add_child_element(elm);

	table_error_message *error_message = dynamic_cast<table_error_message*>(elm.get());

	if (error_message)
	{
		error_message->table_display_ = true;
		if (false == title.empty()) error_message->table_title_ = title;

		if (false == content.empty())
		{
		}
	}
}
void ods_table_context::set_data_validation_promt(const std::wstring &title, const std::wstring &content)
{
	office_element_ptr elm;
	create_element(L"table", L"help-message", elm, &context_);
	
	table_content_validations_.elements.back()->add_child_element(elm);
	
	table_help_message *help_message = dynamic_cast<table_help_message*>(elm.get());

	if (help_message)
	{
		help_message->table_display_ = true;
		if (false == title.empty()) help_message->table_title_ = title;
		if (false == content.empty())
		{
		}	
	}
}
void ods_table_context::end_data_validation()
{
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
	
	std::wstring odf_base_cell = formulas_converter.get_table_name();

	named_range->table_name_ = name;
	named_range->table_cell_range_address_ = odf_range;
	if (printable)
		named_range->table_range_usable_as_ = L"print-range";
	
	if (false == odf_base_cell.empty())
		named_range->table_base_cell_address_ =  odf_base_cell;

	table_defined_expressions_.elements.push_back(elm);
	
	if (sheet_id >=0)
	{
		for (size_t i = 0; i < table_state_list_.size(); ++i)
		{
			if (i == sheet_id)
			{
				table_state_list_[i].add_definded_expression(elm);
				break;
			}
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
	formulasconvert::oox2odf_converter formulas_converter;
	
	bool simple_range = formulas_converter.is_simple_ref(value);

	if (simple_range)//если простой - range, составной - выражение
		return add_defined_range (name, value, sheet_id, printable);

	office_element_ptr elm;
	create_element(L"table", L"named-expression",elm, &context_);
	
	table_named_expression* named_expression = dynamic_cast<table_named_expression*>(elm.get());
	if (named_expression == NULL)return;


	std::wstring odf_value		= formulas_converter.convert_named_formula(value);
	std::wstring odf_base_cell	= formulas_converter.get_table_name();

	named_expression->table_name_		= name;
	named_expression->table_expression_ = odf_value;
	
	if (sheet_id >=0 && sheet_id < table_state_list_.size())
	{
		odf_base_cell = L"$" + table_state_list_[sheet_id].office_table_name_ + L".$A$1";
		table_state_list_[sheet_id].add_definded_expression(elm);
		
		if ( printable)
		{
			XmlUtils::replace_all( odf_value, L"[", L"");
			XmlUtils::replace_all( odf_value, L"]", L"");
			XmlUtils::replace_all( odf_value, L";", L" ");

			table_state_list_[sheet_id].set_print_range(odf_value);
		}
	}
	else
	{
		if (!table_defined_expressions_.root)create_element(L"table", L"named-expressions", table_defined_expressions_.root,&context_);
		table_defined_expressions_.root->add_child_element(elm);
	}

	if (!odf_base_cell.empty())
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
	state().check_spanned_cells();
}

}
}
