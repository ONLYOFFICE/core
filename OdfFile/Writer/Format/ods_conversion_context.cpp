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

#include <boost/algorithm/string.hpp>

#include "ods_conversion_context.h"
#include "office_spreadsheet.h"

#include "styles.h"
#include "style_table_properties.h"

#include "odf_text_context.h"
#include "paragraph_elements.h"
#include "odf_settings_context.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

ods_conversion_context::ods_conversion_context(package::odf_document* outputDocument) 
	: odf_conversion_context(SpreadsheetDocument, outputDocument), table_context_(*this)
{
}

void ods_conversion_context::start_document()
{
	start_spreadsheet();
	
	root_spreadsheet_ = dynamic_cast<office_spreadsheet*>(get_current_object_element().get());
}

void ods_conversion_context::set_tables_structure_lock(bool val)
{
	root_spreadsheet_->table_structure_protected_ = val;
}

void ods_conversion_context::end_document()
{
	if (table_context_.table_database_ranges_.root)
		root_spreadsheet_->add_child_element(table_context_.table_database_ranges_.root);

	if (table_context_.table_defined_expressions_.root)
		root_spreadsheet_->add_child_element(table_context_.table_defined_expressions_.root);

	if (table_context_.table_content_validations_.root)
		root_spreadsheet_->add_child_element(table_context_.table_content_validations_.root);
	
	if (table_context_.table_pivots_.root)
		root_spreadsheet_->add_child_element(table_context_.table_pivots_.root);

	odf_conversion_context::end_document();
}

void ods_conversion_context::add_autofilter(std::wstring ref)
{
	table_context_.add_autofilter(ref);
}
void ods_conversion_context::start_conditional_formats()
{
	current_table()->start_conditional_formats();
}
void ods_conversion_context::start_table_part(std::wstring name, std::wstring ref)
{
	table_context_.start_table_part(name, ref);
}
void ods_conversion_context::set_table_part_autofilter(bool val)
{
	table_context_.set_table_part_autofilter(val);
}
void ods_conversion_context::add_table_part_column(std::wstring name)
{
	table_context_.add_table_part_column(name);
}
void ods_conversion_context::end_table_part()
{
	table_context_.end_table_part();
}
void ods_conversion_context::add_defined_range(const std::wstring & name, const std::wstring & cell_range, int sheet_id, bool printable)
{
	table_context_.add_defined_range(name,cell_range, sheet_id, printable);
}
void ods_conversion_context::add_defined_expression( const std::wstring & name, const std::wstring & value, int sheet_id, bool printable)
{
	table_context_.add_defined_expression(name,value, sheet_id, printable);
}
void ods_conversion_context::add_header_footer_image(const std::wstring & name, office_element_ptr image)
{
	current_table()->mapHeaderFooterImages.insert(std::make_pair(name, image));
}
void ods_conversion_context::start_sheet()
{
	create_element(L"table", L"table", root_spreadsheet_->getContent(), this);	
	table_context_.start_table(root_spreadsheet_->getContent().back());

		drawing_context()->set_styles_context(styles_context());
		page_layout_context()->set_styles_context(styles_context());
		
	page_layout_context()->add_master_page(L"");

    current_table()->set_table_master_page(page_layout_context()->last_master() ?
                                              page_layout_context()->last_master()->get_name() : L"");
}

void ods_conversion_context::set_sheet_dimension(const std::wstring & ref)
{
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells, ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

	int max_col = 0, max_row = 0;
	for (size_t i = 0; i < ref_cells.size(); i++)
	{
		int col = -1, row = -1;
		utils::parsing_ref (ref_cells[i], col, row);

		if (col > max_col) max_col = col;
		if (col > max_row) max_row = row;
	}
	current_table()->set_table_dimension(max_col, max_row);
}

void ods_conversion_context::end_sheet()
{
	if (current_table()->controls_context()->is_exist_content())
	{
		office_element_ptr forms_root_elm;
		create_element(L"office", L"forms", forms_root_elm, this);

		current_table()->controls_context()->finalize(forms_root_elm);
		
		current_table()->add_child_element(forms_root_elm);
	}
	if (current_table()->drawing_context()->is_exist_content())
	{
		office_element_ptr shapes_root_elm;
		create_element(L"table", L"shapes", shapes_root_elm, this);

		current_table()->drawing_context()->finalize(shapes_root_elm);
		
		current_table()->add_child_element(shapes_root_elm);
	}

	table_context_.end_table();
	
	styles_context()->reset_defaults();
}
void ods_conversion_context::add_row_repeated()
{
	current_table()->add_row_repeated();
}
void ods_conversion_context::start_row(int _start_row, int repeated, int level, bool _default)
{
	if (_start_row > current_table()->current_row() + 1)
	{
		int repeated_default = _start_row - current_table()->current_row() - 1;

		add_default_row(repeated_default);
	}
//-------------------------------------------------------------------------------------------
	while (level < current_table()->current_level())
	{
		current_table()->end_group();
	}
	while (level > current_table()->current_level())
	{
		office_element_ptr row_group_elm;
		create_element(L"table", L"table-row-group",row_group_elm,this);
		current_table()->start_group(row_group_elm);
	}
//-------------------------------------------------------------------------------------------
	bool bBreak = false;

	for (size_t i = 0; i < current_table()->row_breaks_.size(); i++)
	{
		if (_start_row == current_table()->row_breaks_[i])
		{
			if (repeated > 1)
			{
				start_row(_start_row, 1, level, _default);
				end_row();
				
				start_row(_start_row + 1, repeated - 1, level, _default);
				return;
			}
			else bBreak = true;
			break;
		}
		else if (_start_row < current_table()->row_breaks_[i] && current_table()->row_breaks_[i] < _start_row + repeated)
		{
			start_row(_start_row, current_table()->row_breaks_[i] - _start_row, level, _default); 
			end_row();
			
			start_row(current_table()->row_breaks_[i], 1, level, _default); 
			end_row();
			
			start_row(current_table()->row_breaks_[i] + 1, _start_row + repeated - current_table()->row_breaks_[i] - 1, level, _default); 
			return;
		}
	}
//-------------------------------------------------------------------------------------------
	office_element_ptr	style_elm;
	if ( _default && !bBreak)
	{
		style_elm = styles_context()->find_odf_style_default(style_family::TableRow);
	}
	else
	{
		styles_context()->create_style(L"",style_family::TableRow, true, false, -1);
		style_elm = styles_context()->last_state()->get_office_element();
		
		style* _style = dynamic_cast<style*>(style_elm.get());
		if (!_style)return;		

		style_table_row_properties * row_properties = _style->content_.get_style_table_row_properties();
 		if (row_properties == NULL)return; //error ????

		if (bBreak)
			row_properties->style_table_row_properties_attlist_.common_break_attlist_.fo_break_before_ = fo_break(fo_break::Page);
		else
			row_properties->style_table_row_properties_attlist_.common_break_attlist_.fo_break_before_ = fo_break(fo_break::Auto);
	}

	office_element_ptr row_elm;
	create_element(L"table", L"table-row",row_elm,this);
	
	current_table()->add_row(row_elm, repeated, style_elm);

	if ( _default)
	{
		//std::wstring style_cell_name= styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
		//current_table()->set_row_default_cell_style(style_cell_name);
	}
}
void ods_conversion_context::end_row()
{
	//add default last cells
	int repeated = 1024;// max dimension columns???
	
	current_table()->add_default_cell(repeated);

}
//////////////////////
void ods_conversion_context::start_comment(int col, int row, std::wstring & author)
{
	current_table()->start_comment(col, row, author);
	start_text_context();
////////////////
	office_element_ptr paragr_elm;
	create_element(L"text", L"p", paragr_elm, this);
	
	text_context()->start_paragraph(paragr_elm);
}
void ods_conversion_context::end_comment()
{
	if (text_context())text_context()->end_paragraph();

	current_table()->end_comment(text_context());
	end_text_context();
}
void ods_conversion_context::set_comment_color(const std::wstring & color)
{
	current_table()->set_comment_color(color);
}
void ods_conversion_context::set_comment_visible(bool val)
{
	current_table()->set_comment_visible(val);
}
void ods_conversion_context::set_comment_rect(double l, double t, double w, double h)//in mm
{
	current_table()->set_comment_rect(l,t,w,h);
}
/////////////////////////////
void ods_conversion_context::add_hyperlink(const std::wstring & ref, const std::wstring & link, const std::wstring & display, const std::wstring & location)
{
//////////////////////////////////////////////////////////////////
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells, ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
	if (ref_cells.size() > 1)
	{
	//в ооx можно воткнуть на диапазон одну ссылку, в оо нельзя - ссылку вствляем, текст не меням
		int start_col = -1, start_row = -1;
		int end_col = -1, end_row = -1;
		
		utils::parsing_ref (ref_cells[0], start_col, start_row);
		utils::parsing_ref (ref_cells[1], end_col, end_row);
		
		for (long col = start_col; col <= end_col; col++)
		{ 
			for (long row = start_row; row <= end_row; row++)
			{
				current_table()->add_hyperlink(ref, col, row, link, location);
				//ссылка одна, а вот отображаемый текст - разный
			}
		}
	}
	else
	{
		int col = -1, row = -1;
		utils::parsing_ref (ref_cells[0], col, row);
		current_table()->add_hyperlink(ref, col, row, link, location);
	}
}
void ods_conversion_context::start_pivot_table(const std::wstring &name)
{
	return table_context_.start_pivot_table(name);
}
void ods_conversion_context::end_pivot_table()
{
	table_context_.end_pivot_table();
}
bool ods_conversion_context::start_data_validation(const std::wstring & ref, int type)
{
	return table_context_.start_data_validation(ref, type);
}
void ods_conversion_context::end_data_validation()
{
	table_context_.end_data_validation();
}
void ods_conversion_context::set_data_validation_allow_empty(bool val)
{
	table_context_.set_data_validation_allow_empty(val);
}
void ods_conversion_context::set_data_validation_content(const std::wstring &val1, const std::wstring &val2)
{
	table_context_.set_data_validation_content(val1, val2);
}
void ods_conversion_context::set_data_validation_operator(int val)
{
	table_context_.set_data_validation_operator(val);
}
void ods_conversion_context::set_data_validation_error(const std::wstring &title, const std::wstring &content, bool display, int type)
{
	table_context_.set_data_validation_error(title, content, display, type);
}
void ods_conversion_context::set_data_validation_promt(const std::wstring &title, const std::wstring &content, bool display)
{
	table_context_.set_data_validation_promt(title, content, display);
}
void ods_conversion_context::add_merge_cells(const std::wstring & ref)
{
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells, ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

	if (ref_cells.size() != 2) return;//тута однозначно .. по правилам оохml

	int start_col = -1, start_row = -1;
	int end_col = -1, end_row = -1;

	utils::parsing_ref (ref_cells[0], start_col, start_row);
	utils::parsing_ref (ref_cells[1], end_col, end_row);

	current_table()->set_merge_cells(start_col, start_row, end_col, end_row);

}

void ods_conversion_context::start_cell(std::wstring & ref, int xfd_style)
{
	int col = 0, row = 0, spanned_rows = 0, spanned_cols = 0;
	utils::parsing_ref ( ref, col, row);

	bool bCovered = false;
	bool bSpanned = current_table()->isSpannedCell(col, row, spanned_cols, spanned_rows);

	if (!bSpanned)
	{
		bCovered = current_table()->isCoveredCell(col, row);
	}

	if (col > current_table()->current_column() + 1)
	{
		int repeated = col - current_table()->current_column() - 1;
		
		current_table()->add_default_cell(repeated);//, bCovered);
	}

	office_element_ptr style_elm;

	std::wstring style_cell_name;
	office_value_type::type format_value_type = office_value_type::Custom;
	
	if ( xfd_style >=0)
	{
		odf_style_state_ptr  style_state;
		styles_context()->find_odf_style_state(xfd_style, style_family::TableCell, style_state, false, true);
		if (style_state)
		{
			style_elm = style_state->get_office_element();
			style_cell_name = style_state->get_name();
			
			int number_format = style_state->get_number_format();//oox
			if (number_format >0)
				format_value_type = numbers_styles_context()->add_or_find(number_format).ods_type;
		}
		else
		{//error
		}
	}

	office_element_ptr cell_elm;

	if (bCovered)
	{
		create_element(L"table", L"covered-table-cell", cell_elm, this);
	}
	else
	{
		create_element(L"table", L"table-cell", cell_elm, this);
	}
	
	current_table()->start_cell(cell_elm, style_elm);

	if (bSpanned)
	{
		current_table()->set_cell_spanned(spanned_cols, spanned_rows);
	}
	if (!bCovered)
	{
		current_table()->set_cell_format_value(format_value_type);
	}
}

void ods_conversion_context::end_cell()
{
	current_table()->end_cell();
	end_text_context();
}

void ods_conversion_context::start_columns()
{
}
void ods_conversion_context::end_columns()
{
	//add default last column  - ЕСЛИ они не прописаны в исходном (1024 - от  балды)
	//вопрос - если и добавлять то  с каким стилем???
	//if (current_table()->current_column() < 1 )
	//	add_column(current_table()->current_column() + 1,1024, 0, true);
	//else
    int repeat = (std::max)(current_table()->dimension_columns, 1024) - current_table()->current_column();
	if (repeat < 0) repeat = 1;
	
	add_column(current_table()->current_column() + 1, repeat, 0, true);
}
void ods_conversion_context::start_rows()
{
}
void ods_conversion_context::add_default_row(int repeated)
{
	if (repeated < 1) return;

	if (repeated > 1)
	{
		int row_comment_repeated = 1;
		int row_comment = current_table()->is_row_comment(current_table()->current_row() + 1, repeated);
		int row_validation_repeated = repeated;
		int row_validation = current_table()->is_row_validation(current_table()->current_row() + 1, row_validation_repeated);

		int row_split = row_comment; int row_split_repeated = 1;
		if (row_validation > 0)
		{
			if (row_split < 0 || row_validation < row_split)
			{
				row_split = row_validation;
				row_split_repeated = row_validation_repeated;
			}
		}
		if (row_split > current_table()->current_row() && row_split_repeated != repeated)
		{//делим на 3 - до, с --, после;			
			int r = current_table()->current_row();

			add_default_row(row_split - r - 1);
			add_default_row(row_split_repeated);
			add_default_row(repeated + r + row_split_repeated - row_split - 1);

			return;
		}
	}
	
	if (repeated > 0 && current_table()->get_last_row_repeated() < 1024)
	{
		start_row(current_table()->current_row() + 1, repeated, 0, true);
		end_row();
	}
}
void ods_conversion_context::end_rows()
{
	//add default last row
    int repeated = (std::max)(current_table()->dimension_row, 64) - current_table()->current_row();
	if (repeated < 0) repeated = 1;

	add_default_row(repeated);
}
void ods_conversion_context::add_column(int start_column, int repeated, int level, bool _default)
{
	if (start_column > current_table()->current_column() + 1)
	{
		int repeated_default = start_column - current_table()->current_column() - 1;
		add_column(start_column - repeated_default, repeated_default, 0, true);
	}
//-------------------------------------------------------------------------------------------
	while (level < current_table()->current_level())
	{
		current_table()->end_group();
	}
	while (level > current_table()->current_level())
	{
		office_element_ptr column_group_elm;
		create_element(L"table", L"table-column-group", column_group_elm, this);
		current_table()->start_group(column_group_elm);
	}
//-------------------------------------------------------------------------------------------
	bool bBreak = false;

	for (size_t i = 0; i < current_table()->column_breaks_.size(); i++)
	{
		if (start_column == current_table()->column_breaks_[i])
		{
			if (repeated > 1)
			{
				add_column(start_column, 1, level, _default);
				add_column(start_column + 1, repeated - 1, level, _default);
				return;
			}
			else bBreak = true;
			break;
		}
		else if (start_column < current_table()->column_breaks_[i] && current_table()->column_breaks_[i] < start_column + repeated)
		{
			add_column(start_column, current_table()->column_breaks_[i] - start_column, level, _default);
			add_column(current_table()->column_breaks_[i], 1, level, _default);
			add_column(current_table()->column_breaks_[i] + 1, start_column + repeated - current_table()->column_breaks_[i] - 1, level, _default);
			return;
		}
	}
//-------------------------------------------------------------------------------------------
	office_element_ptr	style_elm;
	if ( _default && !bBreak)
	{
		style_elm = styles_context()->find_odf_style_default(style_family::TableColumn);
	}
	else
	{
		//по сути в этом стиле раличные опции ширины колонок тока .. а если свойства совпадают - можно сгенерить один, хотя выше и указано что стили разные.
		//то есть в оо разделяют оох стиль на 2 (для колонки собственно, и описалово ячеек в колонки)
		styles_context()->create_style(L"", style_family::TableColumn, true, false, -1);
		style_elm = styles_context()->last_state()->get_office_element();
		
		style* _style = dynamic_cast<style*>(style_elm.get());
		if (!_style)return;		

		style_table_column_properties * column_properties = _style->content_.get_style_table_column_properties();
 		if (column_properties == NULL)return; //error ????

		if (bBreak)
			column_properties->style_table_column_properties_attlist_.common_break_attlist_.fo_break_before_ = fo_break(fo_break::Page);
		else
			column_properties->style_table_column_properties_attlist_.common_break_attlist_.fo_break_before_ = fo_break(fo_break::Auto);
	}

	office_element_ptr column_elm;
	create_element(L"table", L"table-column", column_elm, this);
	
	current_table()->add_column(column_elm, repeated, style_elm);

	if (_default)
	{
		std::wstring style_cell_name= styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
		current_table()->set_column_default_cell_style(style_cell_name);
	}
}

void ods_conversion_context::add_text_content(const std::wstring & text)
{
	if (false == text_context_.empty())
	{
		text_context_.back()->add_text_content(text);
	}
}
odf_drawing_context* ods_conversion_context::drawing_context() 
{
	if (false == drawing_context_.empty())
	{
		return drawing_context_.back().get();
	}
	else 
	{
		return current_table()->drawing_context();
	}
}
odf_text_context* ods_conversion_context::text_context()
{
	if (false == text_context_.empty())
	{
		return text_context_.back().get();
	}
	else
	{
		return NULL;
	}
}
void ods_conversion_context::add_text(const std::wstring &text)
{
	office_element_ptr paragr_elm;
	create_element(L"text", L"p", paragr_elm, this);
	
	text_context()->start_paragraph(paragr_elm);

	text_context()->add_text_content(text);

	text_context()->end_paragraph();
}

void ods_conversion_context::start_cell_text()
{
	start_text_context();
////////////
	office_element_ptr paragr_elm;
	create_element(L"text", L"p", paragr_elm, this);
	
	text_context()->start_paragraph(paragr_elm);

	if (current_table()->is_cell_hyperlink())
	{
		ods_hyperlink_state & state = current_table()->current_hyperlink();
		
		office_element_ptr text_a_elm;
		create_element(L"text", L"a", text_a_elm, this);

		text_a* text_a_ = dynamic_cast<text_a*>(text_a_elm.get());
		if (text_a_ == NULL)return;

		text_a_->common_xlink_attlist_.type_ = xlink_type(xlink_type::Simple);
		text_a_->common_xlink_attlist_.href_ = state.link;
		
		text_context()->start_element(text_a_elm); // может быть стоит сделать собственый???
		// libra дурит если в табличках будет вложенный span в гиперлинк ... оО (хотя это разрешено в спецификации!!!)

		text_context()->single_paragraph_ = true;
	}
}

void ods_conversion_context::end_cell_text()
{
	if (text_context())
	{
		if (current_table()->is_cell_hyperlink()) text_context()->end_element();
		
		text_context()->end_paragraph();
	}
}

void ods_conversion_context::add_external_reference(const std::wstring & ref)
{
	ods_external_state external;

	external.ref = ref;
	externals_.push_back(external);
}

void ods_conversion_context::start_table_view( int view_id )
{
	settings_context()->set_current_view(view_id);
	settings_context()->start_table(current_table()->office_table_name_);
}

void ods_conversion_context::end_table_view()
{
	settings_context()->end_table();
	settings_context()->set_current_view(-1);
}
bool ods_conversion_context::start_header(int type)// 0 - odd, 1 - first, 2 - even
{
    if (page_layout_context()->add_header(type) == false)   return false;
    if (page_layout_context()->last_master() == NULL)       return false;

	start_text_context();
	text_context()->set_styles_context(page_layout_context()->get_local_styles_context());

    text_context()->start_element(page_layout_context()->last_master()->get_last_element());

	if (false == current_table()->mapHeaderFooterImages.empty())
	{
		std::wstring mask = std::wstring(L"H") + (type == 1 ? L"FIRST" : (type == 2 ? L"EVEN" : L""));

		std::map<std::wstring, office_element_ptr>::iterator pFind;

		pFind = current_table()->mapHeaderFooterImages.find(L"C" + mask);
		if (pFind == current_table()->mapHeaderFooterImages.end())
		{
			pFind = current_table()->mapHeaderFooterImages.find(L"L" + mask);
			if (pFind == current_table()->mapHeaderFooterImages.end())
			{
				pFind = current_table()->mapHeaderFooterImages.find(L"R" + mask);
			}
		}
		
		if (pFind != current_table()->mapHeaderFooterImages.end())
		{
			page_layout_context()->set_header_footer_image(pFind->second);
		}
	}
	return true;
}
bool ods_conversion_context::start_footer(int type)
{
    if (page_layout_context()->add_footer(type) == false)   return false;
    if (page_layout_context()->last_master() == NULL)       return false;

	start_text_context();
	text_context()->set_styles_context(page_layout_context()->get_local_styles_context());
	
    text_context()->start_element(page_layout_context()->last_master()->get_last_element());

	if (false == current_table()->mapHeaderFooterImages.empty())
	{
		std::wstring mask = std::wstring(L"F") + (type == 1 ? L"FIRST" : (type == 2 ? L"EVEN" : L""));

		std::map<std::wstring, office_element_ptr>::iterator pFind;

		pFind = current_table()->mapHeaderFooterImages.find(L"C" + mask);
		if (pFind == current_table()->mapHeaderFooterImages.end())
		{
			pFind = current_table()->mapHeaderFooterImages.find(L"L" + mask);
			if (pFind == current_table()->mapHeaderFooterImages.end())
			{
				pFind = current_table()->mapHeaderFooterImages.find(L"R" + mask);
			}
		}
		
		if (pFind != current_table()->mapHeaderFooterImages.end())
		{
			page_layout_context()->set_header_footer_image(pFind->second);
		}
	}
	return true;
}
void ods_conversion_context::end_header_footer()
{
	if (text_context()->current_level_.size() > 1)
	{
		text_context()->end_paragraph();
		end_header_footer_region();
	}
	text_context()->end_element();
	end_text_context();
}
void ods_conversion_context::start_header_footer_region(int type)
{
	if (text_context()->current_level_.size() > 1)
	{
		text_context()->end_paragraph();
		end_header_footer_region();
	}

	office_element_ptr region_elm_;

	if (type == 1)
	{
		create_element(L"style", L"region-left", region_elm_, this);
	}
	else if (type == 2)
	{
		create_element(L"style", L"region-center", region_elm_, this);
	}
	else if (type == 3)
	{
		create_element(L"style", L"region-right", region_elm_, this);
	}
	
	text_context()->start_element(region_elm_);
	text_context()->start_paragraph(false);
}
void ods_conversion_context::end_header_footer_region()
{
	text_context()->end_element();
}

}
}
