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
#include "text_elements.h"
#include "table_data_pilot_tables.h"

#include "ods_conversion_context.h"
#include "logging.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/make_shared.hpp>

#include "../../Formulas/formulasconvert.h"

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

ods_table_context::ods_table_context(ods_conversion_context & Context): context_(Context), count_validations_(0)
{        
}

ods_table_state_ptr & ods_table_context::state()
{
    return table_state_list_.back();
}

void ods_table_context::start_table_part(const std::wstring &name, std::wstring ref)
{
	if (!table_database_ranges_.root) create_element(L"table", L"database-ranges",table_database_ranges_.root,&context_);

	office_element_ptr elm;
	create_element(L"table", L"database-range",elm, &context_);
	table_database_range * d_range = dynamic_cast<table_database_range*>(elm.get());

	if (!d_range)return;

	formulasconvert::oox2odf_converter formulas_converter;

	if (std::wstring::npos == ref.find(L"!") )
	{
		bool bQuotes = (std::wstring::npos != table_state_list_.back()->office_table_name_.find(L" "));

		ref = (bQuotes ? L"'" : L"" ) + table_state_list_.back()->office_table_name_ + (bQuotes ? L"'" : L"" ) + L"!" + ref;
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

	state()->table_parts_.push_back(part_state);
}

void ods_table_context::add_table_part_column(std::wstring name)
{
	if (state()->table_parts_.empty()) return;

	size_t column = state()->table_parts_.back().columns.size();

	std::wstring sCol = L"$" + utils::getColAddress(state()->table_parts_.back().col_start + column - 1);

	bool bQuotes = (std::wstring::npos != table_state_list_.back()->office_table_name_.find(L" "));

	std::wstring ref = L"$";
	
	ref += (bQuotes ? L"'" : L"") + table_state_list_.back()->office_table_name_ + (bQuotes ? L"'" : L"") + L".";

	ref += sCol + std::to_wstring(state()->table_parts_.back().row_start);
	ref += L":";
	ref += sCol + std::to_wstring(state()->table_parts_.back().row_end);

	state()->table_parts_.back().columns.push_back(std::make_pair(name, ref));
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
	if (ref.empty()) return;

	if (!table_database_ranges_.root) create_element(L"table", L"database-ranges", table_database_ranges_.root, &context_);

	office_element_ptr elm;
	create_element(L"table", L"database-range", elm, &context_);
	table_database_range * d_range = dynamic_cast<table_database_range*>(elm.get());

	if (!d_range)return;

	if (std::wstring::npos == ref.find(L"!") )
	{
		bool bQuotes = (std::wstring::npos != table_state_list_.back()->office_table_name_.find(L" "));

		ref = (bQuotes ? L"'" : L"") + table_state_list_.back()->office_table_name_ + (bQuotes ? L"'" : L"") + L"!" + ref;
	}
	formulasconvert::oox2odf_converter formulas_converter;

	std::wstring odf_range = formulas_converter.convert_named_ref(ref);

	d_range->table_target_range_address_ = odf_range;
	d_range->table_display_filter_buttons_= true;

	d_range->table_name_ = L"__Anonymous_Sheet_DB__" + std::to_wstring(table_database_ranges_.elements.size() + 1);

	table_database_ranges_.root->add_child_element(elm);
	table_database_ranges_.elements.push_back(elm);

}
void ods_table_context::start_pivot_table(const std::wstring &name)
{
	if (!table_pivots_.root) create_element(L"table", L"data-pilot-tables", table_pivots_.root, &context_);

	office_element_ptr elm;
	create_element(L"table", L"data-pilot-table", elm, &context_);
	table_data_pilot_table *pilot_table = dynamic_cast<table_data_pilot_table*>(elm.get());

	if (!pilot_table) return;

	pilot_table->table_name_ = name;

	table_pivots_.root->add_child_element(elm);

	state()->start_pilot_table(elm);
}
void ods_table_context::end_pivot_table()
{
	state()->end_pilot_table();
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
	validation_state.name	= L"DataValidation_" + std::to_wstring(++count_validations_);
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

	validation->table_base_cell_address_	= state()->office_table_name_ + L"." + getCellAddress(validation_state.refs[0].col_start - 1, validation_state.refs[0].row_start - 1);
	validation->table_name_					= validation_state.name;

	table_content_validations_.root->add_child_element(elm);
	table_content_validations_.elements.push_back(elm);

	state()->data_validations_.push_back(validation_state);

	return true;
}
void ods_table_context::set_data_validation_allow_empty(bool val)
{
	if (state()->data_validations_.empty()) return;
	
	table_content_validation *validation = dynamic_cast<table_content_validation*>(state()->data_validations_.back().elm.get());
	validation->table_allowempty_cell_ = val;
}
void ods_table_context::set_data_validation_operator(int val)
{
	if (state()->data_validations_.empty()) return;

	state()->data_validations_.back().operator_ = val;
}
void ods_table_context::set_data_validation_content( std::wstring oox_formula1, std::wstring oox_formula2)
{
	if (state()->data_validations_.empty()) return;
	if (oox_formula1.empty() && oox_formula2.empty()) return;
	
	std::wstring odf_formula1, odf_formula2;
	if (false == oox_formula1.empty() && oox_formula1[0] == L'\"' && oox_formula1[oox_formula1.length() - 1] == L'\"')
	{
		oox_formula1 = oox_formula1.substr(1, oox_formula1.length() - 2);

		std::vector<std::wstring> arItems;
		boost::algorithm::split(arItems, oox_formula1, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
		
		for (size_t i = 0; i < arItems.size(); ++i)
		{
			odf_formula1 += L"\"" + arItems[i] + L"\"" + (i < arItems.size() - 1 ? L";" : L"");
		}
	}
	else
	{
		formulasconvert::oox2odf_converter formulas_converter;

		odf_formula1 = formulas_converter.convert_formula(oox_formula1);

		if (false == odf_formula1.empty())
		{
			odf_formula1 = odf_formula1.substr(4);
		}
    }
    if (false == oox_formula2.empty() && oox_formula2[0] == L'\"' && oox_formula2[oox_formula2.length() - 1] == L'\"')
    {
        oox_formula2 = oox_formula1.substr(1, oox_formula2.length() - 2);
        
        std::vector<std::wstring> arItems;
        std::wstring str = oox_formula2.substr(1, oox_formula1.length() - 2);
        boost::algorithm::split(arItems, str,
                                boost::algorithm::is_any_of(L","),
                                boost::algorithm::token_compress_on);
        
        for (size_t i = 0; i < arItems.size(); ++i)
        {
            odf_formula2 += L"\"" + arItems[i] + L"\"" + (i < arItems.size() - 1 ? L";" : L"");
        }
    }
    else
    {
		formulasconvert::oox2odf_converter formulas_converter;

		odf_formula2 = formulas_converter.convert_formula(oox_formula2);

		if (false == odf_formula2.empty())
		{
			odf_formula2 = odf_formula2.substr(4);
		}
	}
	table_content_validation *validation = dynamic_cast<table_content_validation*>(state()->data_validations_.back().elm.get());
	
	std::wstring odf_condition;

	if (state()->data_validations_.back().type == 5) odf_condition = L"cell-content-text-length";
	else odf_condition = L"cell-content";

	switch(state()->data_validations_.back().operator_)
	{
		case 1: // SimpleTypes::spreadsheet::operatorNotBetween
			odf_condition += L"-is-not-between(" + odf_formula1 + L"," + odf_formula2 + L")"; break;
		case 2: // SimpleTypes::spreadsheet::operatorEqual
			odf_condition += L"()==" + odf_formula1; break;
		case 3: // SimpleTypes::spreadsheet::operatorNotEqual
			odf_condition += L"()!=" + odf_formula1; break;
		case 4: // SimpleTypes::spreadsheet::operatorLessThan
			odf_condition += L"()<" + odf_formula1; break;
		case 5: // SimpleTypes::spreadsheet::operatorLessThanOrEqual
			odf_condition += L"()<=" + odf_formula1; break;
		case 6: // SimpleTypes::spreadsheet::operatorGreaterThan
			odf_condition += L"()>" + odf_formula1; break;
		case 7: // SimpleTypes::spreadsheet::operatorGreaterThanOrEqual
			odf_condition += L"()>=" + odf_formula1; break;
		case 0: // SimpleTypes::spreadsheet::operatorBetween
		default:
			odf_condition +=  + L"-is-between(" + odf_formula1 + L"," + odf_formula2 + L")"; break;
	}
	switch (state()->data_validations_.back().type)
	{
		case 0://SimpleTypes::spreadsheet::validationTypeNone:
		{
			odf_condition.clear();
		}break;
		case 1://SimpleTypes::spreadsheet::validationTypeCustom:
		{
			odf_condition = L"of:is-true-formula(" + odf_formula1 + L")";
		}break;
		case 2://SimpleTypes::spreadsheet::validationTypeDate:
		{
			odf_condition = L"of:cell-content-is-date() and " + odf_condition;
		}break;
		case 3://SimpleTypes::spreadsheet::validationTypeDecimal:
		{
			odf_condition = L"of:cell-content-is-decimal-number() and " + odf_condition;
		}break;
		case 4://SimpleTypes::spreadsheet::validationTypeList:	
		{
			odf_condition = L"of:cell-content-is-in-list(" + odf_formula1 + L")";
		}break;
		case 6://SimpleTypes::spreadsheet::validationTypeTime:
		{
			odf_condition = L"of:cell-content-is-time() and " + odf_condition;
		}break;
		case 7://SimpleTypes::spreadsheet::validationTypeWhole:
		{
			odf_condition = L"of:cell-content-is-whole-number() and " + odf_condition;
		}break;
		case 5://SimpleTypes::spreadsheet::validationTypeTextLength:
		default:
		{
			odf_condition = L"of:" + odf_condition;
		}break;	
	}
	state()->data_validations_.back().condition = odf_condition;
	
	validation->table_condition_ = odf_condition;
}
void ods_table_context::set_data_validation_error(const std::wstring &title, const std::wstring &content, bool display, int type)
{
	if (state()->data_validations_.empty()) return;

	office_element_ptr elm;
	create_element(L"table", L"error-message", elm, &context_);

	table_content_validations_.elements.back()->add_child_element(elm);

	table_error_message *error_message = dynamic_cast<table_error_message*>(elm.get());

	if (error_message)
	{
		error_message->table_message_type_ = (odf_types::message_type::type)type;
		error_message->table_display_ = display;
		if (false == title.empty()) error_message->table_title_ = title;

		//error_message->message_type_

		if (false == content.empty())
		{
			error_message->create_child_element(L"text", L"p");
			text_p *p = dynamic_cast<text_p*>(error_message->content_.back().get());
			if (p)
			{
				p->add_text(content);
			}
		}
	}
}
void ods_table_context::set_data_validation_promt(const std::wstring &title, const std::wstring &content, bool display)
{
	if (state()->data_validations_.empty()) return;

	office_element_ptr elm;
	create_element(L"table", L"help-message", elm, &context_);
	
	table_content_validations_.elements.back()->add_child_element(elm);
	
	table_help_message *help_message = dynamic_cast<table_help_message*>(elm.get());

	if (help_message)
	{
		help_message->table_display_ = display;
		
		if (false == title.empty()) help_message->table_title_ = title;

		if (false == content.empty())
		{
			help_message->create_child_element(L"text", L"p");
			text_p *p = dynamic_cast<text_p*>(help_message->content_.back().get());
			if (p)
			{
				p->add_text(content);
			}
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
	{
		named_range->table_range_usable_as_ = L"print-range";

		XmlUtils::replace_all( odf_range, L"[", L"");
		XmlUtils::replace_all( odf_range, L"]", L"");
		XmlUtils::replace_all( odf_range, L";", L" ");

		table_state_list_[sheet_id]->set_table_print_ranges(odf_range);
	}
	
	if (false == odf_base_cell.empty())
		named_range->table_base_cell_address_ = odf_base_cell;

	table_defined_expressions_.elements.push_back(elm);
	
	if (sheet_id >=0)
	{
		for (size_t i = 0; i < table_state_list_.size(); ++i)
		{
			if (i == sheet_id)
			{
				table_state_list_[i]->add_definded_expression(elm);
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
		odf_base_cell = L"$" + table_state_list_[sheet_id]->office_table_name_ + L".$A$1";
		table_state_list_[sheet_id]->add_definded_expression(elm);
		
		if ( printable)
		{
			XmlUtils::replace_all( odf_value, L"[", L"");
			XmlUtils::replace_all( odf_value, L"]", L"");
			XmlUtils::replace_all( odf_value, L";", L" ");

			table_state_list_[sheet_id]->set_table_print_ranges(odf_value);
		}
	}
	else
	{
		if (!table_defined_expressions_.root)create_element(L"table", L"named-expressions", table_defined_expressions_.root, &context_);
		table_defined_expressions_.root->add_child_element(elm);
	}

	if (!odf_base_cell.empty())
		named_expression->table_base_cell_address_ =  odf_base_cell;

	table_defined_expressions_.elements.push_back(elm);
}

void ods_table_context::start_table(office_element_ptr & elm)
{
	table_state_list_.push_back( boost::make_shared<ods_table_state>(&context_, elm) );
	
	std::wstring style_name_new = L"ta" + boost::lexical_cast<std::wstring>(table_state_list_.size());

	office_element_ptr & style = context_.styles_context()->add_or_find(style_name_new, style_family::Table, true);
	style->create_child_element(L"style", L"table-properties");
	
	state()->set_table_style(style);
	state()->set_table_hidden(false);

	//для свойств страницы, а не таблицы - нужно создать master-page c page layout и связать по имени со стилем таблицы
	//причем здесь, т.к. с другой стороны это ВСЕ еще свойства листа. то есть совйства листа разделить на свйства страницы и таблицы ..
	//todooo
	//????
}

void ods_table_context::end_table()
{
	state()->check_spanned_cells();
}

}
}
