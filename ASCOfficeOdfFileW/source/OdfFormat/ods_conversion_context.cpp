/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include <boost/algorithm/string.hpp>

#include "../utils.h"

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

namespace utils
{

void calculate_size_font_symbols(_font_metrix & metrix, CApplicationFonts *appFonts)
{
    double appr_px = _graphics_utils_::calculate_size_symbol_asc(metrix.font_name,metrix.font_size,metrix.italic,metrix.bold, appFonts);
	
	if (appr_px <0.01)
	{
        appr_px = _graphics_utils_::calculate_size_symbol_win(metrix.font_name,metrix.font_size,false/*metrix.italic*/,false/*metrix.bold*/);
		appr_px = ((int)(appr_px+0.5) + 2*(int)appr_px)/3.;
	}

	if (appr_px > 0)
	{
		//pixels to pt
		metrix.approx_symbol_size = appr_px ;///1.1;//"1.2" волшебное число оО
		metrix.IsCalc = true;
	}

}
}
ods_conversion_context::ods_conversion_context(package::odf_document * outputDocument) 
		: odf_conversion_context(outputDocument), table_context_(*this), current_text_context_(NULL)
{
	font_metrix_		= _font_metrix();
}


void ods_conversion_context::start_document()
{
	start_spreadsheet();
	
	root_spreadsheet_ = dynamic_cast<office_spreadsheet*>(get_current_object_element().get());
}

void ods_conversion_context::end_document()
{
	if (table_context_.table_database_ranges_.root)
		root_spreadsheet_->add_child_element(table_context_.table_database_ranges_.root);

	if (table_context_.table_defined_expressions_.root)
		root_spreadsheet_->add_child_element(table_context_.table_defined_expressions_.root);

	odf_conversion_context::end_document();
}

void ods_conversion_context::start_autofilter(std::wstring ref)
{
	table_context_.start_autofilter(ref);
}
void ods_conversion_context::start_conditional_formats()
{
	current_table().start_conditional_formats();
}

void ods_conversion_context::add_defined_range(const std::wstring & name, const std::wstring & cell_range, int sheet_id, bool printable)
{
	table_context_.add_defined_range(name,cell_range, sheet_id, printable);
}
void ods_conversion_context::add_defined_expression( const std::wstring & name, const std::wstring & value, int sheet_id, bool printable)
{
	table_context_.add_defined_expression(name,value, sheet_id, printable);
}
void ods_conversion_context::start_sheet()
{
	create_element(L"table", L"table",root_spreadsheet_->getContent(),this);	
	table_context_.start_table(root_spreadsheet_->getContent().back());

		drawing_context()->set_styles_context(styles_context());
		page_layout_context()->set_styles_context(styles_context());
		
	page_layout_context()->start_master_page(L"");

    current_table().set_table_master_page(page_layout_context()->last_master() ?
                                              page_layout_context()->last_master()->get_name() : L"");
}

void ods_conversion_context::set_sheet_dimension(const std::wstring & ref)
{
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells, ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

	int max_col = 0, max_row = 0;
	for (long i = 0; i < ref_cells.size(); i++)
	{
		int col = -1, row = -1;
		utils::parsing_ref (ref_cells[i], col, row);

		if (col > max_col) max_col = col;
		if (col > max_row) max_row = row;
	}
	current_table().set_table_dimension(max_col, max_row);
}

void ods_conversion_context::end_sheet()
{
	if (current_table().drawing_context()->is_exist_content())
	{
		office_element_ptr shapes_root_elm;
		create_element(L"table", L"shapes",shapes_root_elm,this);

		current_table().drawing_context()->finalize(shapes_root_elm);
		
		current_table().add_child_element(shapes_root_elm);
	}

	table_context_.end_table();
	
	styles_context()->reset_defaults();

	page_layout_context()->end_master_page();
}

void ods_conversion_context::start_row(int _start_row, int repeated, int level, bool _default)
{
	if (_start_row > current_table().current_row()+1)
	{
		int repeated_default = _start_row - current_table().current_row()-1;
		
		start_row(_start_row-repeated_default, repeated_default,0, true);
		end_row();
	}
/////////////////////////////////////////////////////////////////
	while (level < current_table().current_level())
	{
		current_table().end_group();
	}
	while (level > current_table().current_level())
	{
		office_element_ptr row_group_elm;
		create_element(L"table", L"table-row-group",row_group_elm,this);
		current_table().start_group(row_group_elm);
	}
/////////////////////////////////////////////////////////////////

	office_element_ptr	style_elm;
	if ( _default)
	{
		style_elm = styles_context()->find_odf_style_default(style_family::TableRow);
	}
	else
	{
		styles_context()->create_style(L"",style_family::TableRow, true, false, -1);
		style_elm = styles_context()->last_state()->get_office_element();
		
		style* _style = dynamic_cast<style*>(style_elm.get());
		if (!_style)return;		

		style_table_row_properties * row_properties = _style->style_content_.get_style_table_row_properties();
 		if (row_properties == NULL)return; //error ????

		row_properties->style_table_row_properties_attlist_.common_break_attlist_.fo_break_before_ = fo_break(fo_break::Auto);
	}

	office_element_ptr row_elm;
	create_element(L"table", L"table-row",row_elm,this);
	
	current_table().add_row(row_elm, repeated, style_elm);

	if ( _default)
	{
		//std::wstring style_cell_name= styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
		//current_table().set_row_default_cell_style(style_cell_name);
	}
}
void ods_conversion_context::end_row()
{
	//add default last cells
	int repeated = 1024;// max dimension columns???
	
	current_table().add_default_cell(repeated);

}
//////////////////////
void ods_conversion_context::start_comment(int col, int row, std::wstring & author)
{
	current_table().start_comment(col,row,author);
	start_text_context();
////////////////
	office_element_ptr paragr_elm;
	create_element(L"text", L"p",paragr_elm,this);
	
	current_text_context_->start_paragraph(paragr_elm);
}
void ods_conversion_context::end_comment()
{
	if (current_text_context_)current_text_context_->end_paragraph();

	current_table().end_comment(current_text_context_);
	end_text_context();
}

void ods_conversion_context::set_comment_rect(double l, double t, double w, double h)//in mm
{
	current_table().set_comment_rect(l,t,w,h);
}
/////////////////////////////
void ods_conversion_context::add_hyperlink(std::wstring & ref, std::wstring & link, std::wstring & display)
{
//////////////////////////////////////////////////////////////////
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells,ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
	if (ref_cells.size()>1)
	{
	//в ооx можно воткнуть на диапазон одну ссылку, в оо нельзя - ссылку вствляем, текст не меням
		int start_col = -1, start_row = -1;
		int end_col = -1, end_row = -1;
		
		utils::parsing_ref (ref_cells[0], start_col, start_row);
		utils::parsing_ref (ref_cells[1], end_col,	  end_row);
		
		for (long col = start_col; col <= end_col; col++)
		{ 
			for (long row = start_row; row <= end_row; row++)
			{
				current_table().add_hyperlink(ref,col,row,link);
				//ссылка одна, а вот отображаемый текст - разный
			}
		}
	}
	else
	{
		int col = -1, row = -1;
		utils::parsing_ref (ref_cells[0], col, row);
		current_table().add_hyperlink(ref,col,row,link);
	}
}

void ods_conversion_context::add_merge_cells(const std::wstring & ref)
{
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells,ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

	if (ref_cells.size() !=2) return;//тута однозначно .. по правилам оох

	int start_col = -1, start_row = -1;
	int end_col = -1, end_row = -1;

	utils::parsing_ref (ref_cells[0], start_col, start_row);
	utils::parsing_ref (ref_cells[1], end_col,	  end_row);

	current_table().set_merge_cells(start_col,start_row, end_col, end_row);

}

void ods_conversion_context::start_cell(std::wstring & ref, int xfd_style)
{
	int col=0, row=0;
	utils::parsing_ref ( ref, col,row);

	if (col > current_table().current_column()+1)
	{
		int repeated = col - current_table().current_column() -1;
		
		current_table().add_default_cell(repeated);
	}

	office_element_ptr style_elm;

	std::wstring style_cell_name;
	office_value_type::type format_value_type = office_value_type::Custom;
	
	if ( xfd_style >=0)
	{
		odf_style_state_ptr  style_state;
		styles_context()->find_odf_style_state(xfd_style, style_family::TableCell,style_state, false, true);
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
	create_element(L"table", L"table-cell",cell_elm,this);
	
	current_table().start_cell(cell_elm, style_elm);
	
	current_table().set_cell_format_value(format_value_type);
}

void ods_conversion_context::end_cell()
{
	current_table().end_cell();
	end_text_context();
}
void ods_conversion_context::calculate_font_metrix(std::wstring name, double size, bool italic, bool bold)
{
	if (font_metrix_.IsCalc)return;

	font_metrix_.font_size = size;
	font_metrix_.italic = italic;
	font_metrix_.bold = bold;
	font_metrix_.font_name = name;

////////////////////////////////////////////
    utils::calculate_size_font_symbols(font_metrix_, applicationFonts_);
}

void ods_conversion_context::start_columns()
{
}
void ods_conversion_context::end_columns()
{
	//add default last column  - ЕСЛИ они не прописаны в исходном (1024 - от  балды)
	//вопрос - если и добавлять то  с каким стилем???
	//if (current_table().current_column() < 1 )
	//	add_column(current_table().current_column()+1,1024,0,true);
	//else
    int repeat = (std::max)(current_table().dimension_columns,1024) - current_table().current_column();
	if (repeat < 0) repeat = 1;
	
	add_column(current_table().current_column()+1,repeat,0,true);
}
void ods_conversion_context::start_rows()
{
}
void ods_conversion_context::end_rows()
{
	//add default last row
    int repeat = (std::max)(current_table().dimension_row,64) - current_table().current_row();
	if (repeat < 0) repeat = 1;

	start_row(current_table().current_row()+1,repeat,0,true);
	end_row();
}

void ods_conversion_context::add_column(int start_column, int repeated, int level, bool _default)
{
	if (start_column > current_table().current_column()+1)
	{
		int repeated_default = start_column - current_table().current_column()-1;
		add_column(start_column-repeated_default,repeated_default,0,true);
	}
/////////////////////////////////////////////////////////////////
	while (level < current_table().current_level())
	{
		current_table().end_group();
	}
	while (level > current_table().current_level())
	{
		office_element_ptr column_group_elm;
		create_element(L"table", L"table-column-group",column_group_elm,this);
		current_table().start_group(column_group_elm);
	}

	office_element_ptr	style_elm;
	if ( _default)
	{
		style_elm = styles_context()->find_odf_style_default(style_family::TableColumn);
	}
	else
	{
		//по сути в этом стиле раличные опции ширины колонок тока .. а если свойства совпадают - можно сгенерить один, хотя выше и указано что стили разные.
		//то есть в оо разделяют оох стиль на 2 (для колонки собственно, и описалово ячеек в колонки)
		styles_context()->create_style(L"",style_family::TableColumn, true, false, -1);
		style_elm = styles_context()->last_state()->get_office_element();
		
		style* _style = dynamic_cast<style*>(style_elm.get());
		if (!_style)return;		

		style_table_column_properties * column_properties = _style->style_content_.get_style_table_column_properties();
 		if (column_properties == NULL)return; //error ????

		column_properties->style_table_column_properties_attlist_.common_break_attlist_.fo_break_before_ = fo_break(fo_break::Auto);
	}

	office_element_ptr column_elm;
	create_element(L"table", L"table-column",column_elm,this);
	
	current_table().add_column(column_elm, repeated, style_elm);

	if (_default)
	{
		std::wstring style_cell_name= styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
		current_table().set_column_default_cell_style(style_cell_name);
	}
}
void ods_conversion_context::start_text_context()
{
	current_text_context_ = new odf_text_context(this);

}
void ods_conversion_context::end_text_context()
{
	if (current_text_context_)
		delete current_text_context_;
	current_text_context_ = NULL;
}
void ods_conversion_context::add_text_content(const std::wstring & text)
{
	if (current_text_context_)
	{
		current_text_context_->add_text_content(text);
	}
}
void ods_conversion_context::start_cell_text()
{
	start_text_context();
////////////
	office_element_ptr paragr_elm;
	create_element(L"text", L"p",paragr_elm,this);
	
	current_text_context_->start_paragraph(paragr_elm);

	if (current_table().is_cell_hyperlink())
	{
		ods_hyperlink_state & state = current_table().current_hyperlink();
		
		office_element_ptr text_a_elm;
		create_element(L"text", L"a", text_a_elm, this);

		text_a* text_a_ = dynamic_cast<text_a*>(text_a_elm.get());
		if (text_a_ == NULL)return;

		text_a_->common_xlink_attlist_.type_ = xlink_type(xlink_type::Simple);
		text_a_->common_xlink_attlist_.href_ = state.link;
		
		current_text_context_->start_element(text_a_elm); // может быть стоит сделать собственый???
	}
}

void ods_conversion_context::end_cell_text()
{
	if (current_text_context_)
	{
		if (current_table().is_cell_hyperlink())	current_text_context_->end_element();
		
		current_text_context_->end_paragraph();
	}
}
void ods_conversion_context::start_drawings()
{
}
void ods_conversion_context::end_drawings()
{
	current_table().drawing_context()->clear();
}
void ods_conversion_context::start_image(const std::wstring & image_file_name)
{
	std::wstring odf_ref_name ;
	
	mediaitems()->add_or_find(image_file_name,_mediaitems::typeImage,odf_ref_name);

	current_table().drawing_context()->start_image(odf_ref_name);
}
double ods_conversion_context::convert_symbol_width(double val)
{
	//width = ((int)((column_width * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	//width = (int)(((256. * width + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels
	//
	//_dxR = dxR / 1024. * width * 9525.;  // to emu
	
	val = ((int)((val * font_metrix_.approx_symbol_size + 5) / font_metrix_.approx_symbol_size * 256 )) / 256.;

	double pixels = (int)(((256. * val + ((int)(128. / font_metrix_.approx_symbol_size ))) / 256. ) * font_metrix_.approx_symbol_size ); //in pixels

	return pixels * 0.75; //* 9525. * 72.0 / (360000.0 * 2.54);
}

void ods_conversion_context::start_table_view( int view_id )
{
	settings_context()->set_current_view(view_id);
	settings_context()->start_table(current_table().office_table_name_);
}

void ods_conversion_context::end_table_view()
{
	settings_context()->end_table();
	settings_context()->set_current_view(-1);
}


}
}
