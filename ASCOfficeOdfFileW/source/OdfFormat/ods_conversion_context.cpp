#include "precompiled_cpodf.h"


#include "ods_conversion_context.h"
#include "office_spreadsheet.h"

#include "styles.h"
#include "style_table_properties.h"

#include "odf_text_context.h"
#include "paragraph_elements.h"

namespace cpdoccore { 
namespace odf {

namespace utils
{
size_t getColAddressInv(const std::wstring & a_)
{
    std::wstring a = a_;
    ::boost::algorithm::to_upper(a);
    static const size_t r = (L'Z' - L'A' + 1);
    size_t mul = 1;
    bool f = true;
    size_t res = 0;
    BOOST_REVERSE_FOREACH(const wchar_t c, a)
    {
        size_t v = c - L'A';
        if (f)
            f = false;
        else
            v += 1;
        res += v * mul;
        mul *= r;
    }
    return res;
}
size_t getRowAdderssInv(const std::wstring & a_)
{
	int sz = a_.length();
	if (a_.length()>0)
	{
	   return boost::lexical_cast<size_t>(a_)-1;
	}
	else
		return 0;
}
void splitCellAddress(const std::wstring & a_, std::wstring & col, std::wstring & row)
{   
	std::wstring a = a_;

	std::reverse(a.begin(), a.end());
    ::boost::algorithm::replace_all(a, L"$", L"");
    //::boost::algorithm::replace_all(a, L"'", L"");
	::boost::algorithm::to_upper(a);
	

	BOOST_FOREACH(wchar_t c, a)
    {
		if (c >= L'0' && c <= L'9')
			row +=c;
		else
			col += c;
    }
	std::reverse(col.begin(), col.end());
	std::reverse(row.begin(), row.end());
}
void parsing_ref (const std::wstring & ref, int & col,int & row)
{
	std::wstring strCol, strRow;
	splitCellAddress(ref,strCol,strRow);

	col = getColAddressInv(strCol)+1;
	row = getRowAdderssInv(strRow)+1;

}
}
ods_conversion_context::ods_conversion_context(package::odf_document * outputDocument) 
		: odf_conversion_context(outputDocument), table_context_(*this), current_text_context_(NULL)
{
}


void ods_conversion_context::start_document()
{
	create_element(L"office", L"spreadsheet", content_, this,true);

	current_spreadsheet_ = dynamic_cast<office_spreadsheet*>(content_.back().get());
}


void ods_conversion_context::start_sheet(std::wstring & name)
{
	create_element(L"table", L"table",current_spreadsheet_->getContent(),this);
	
	table_context_.start_table(current_spreadsheet_->getContent().back(),name);
	
}

void ods_conversion_context::set_sheet_dimension(std::wstring & ref)
{
 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells,ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

	int max_col = 0, max_row = 0;
	for (long i=0; i<ref_cells.size(); i++)
	{
		int col = -1, row = -1;
		utils::parsing_ref (ref_cells[i], col, row);

		if (col > max_col) max_col = col;
		if (col > max_row) max_row = row;
	}
	current_table().set_table_dimension(max_col,max_row);
}

void ods_conversion_context::end_sheet()
{
	table_context_.end_table();
	
	styles_context().reset_defaults();
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
		style_elm = styles_context().find_odf_style_default(style_family::TableRow);
	}
	else
	{
		styles_context().create_style(L"",style_family::TableRow, true, false, -1);
		style_elm = styles_context().last_state().get_office_element();
		
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
		//std::wstring style_cell_name= styles_context().find_odf_style_name_default(odf::style_family::TableCell);
		//current_table().set_row_default_cell_style(style_cell_name);
	}
}
void ods_conversion_context::end_row()
{
	//add default last cells
	int repeated = 1024;// max dimension columns???
	
	office_element_ptr default_cell_elm;
	create_element(L"table", L"table-cell",default_cell_elm,this);

	current_table().add_default_cell(default_cell_elm, repeated);

}

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

void ods_conversion_context::add_merge_cells(std::wstring & ref)
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
		
		office_element_ptr default_cell_elm;
		create_element(L"table", L"table-cell",default_cell_elm,this);

		current_table().add_default_cell(default_cell_elm, repeated);
	}

	office_element_ptr style_elm;

	std::wstring style_cell_name;
	office_value_type::type format_value_type = office_value_type::Custom;
	
	if ( xfd_style >=0)
	{
		odf_style_state  *style_state=NULL;
		styles_context().find_odf_style_state(xfd_style, style_family::TableCell,style_state);
		if (style_state)
		{
			style_elm = style_state->get_office_element();
			style_cell_name = style_state->get_name();
			
			int number_format = style_state->get_number_format();//oox
			if (number_format >0)
				format_value_type = numbers_styles_context().add_or_find(number_format).ods_type;
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
	int repeat = current_table().dimension_columns - current_table().current_column();
	if (repeat < 0) repeat = 1;
	
	add_column(current_table().current_column()+1,repeat,0,true);
}
void ods_conversion_context::start_rows()
{
}
void ods_conversion_context::end_rows()
{
	//add default last row
	int repeat = std::max(current_table().dimension_row,1024) - current_table().current_row();
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
		style_elm = styles_context().find_odf_style_default(style_family::TableColumn);
	}
	else
	{
		//по сути в этом стиле раличные опции ширины колонок тока .. а если свойства совпадают - можно сгенерить один, хотя выше и указано что стили разные.
		//то есть в оо разделяют оох стиль на 2 (для колонки собственно, и описалово ячеек в колонки)
		styles_context().create_style(L"",style_family::TableColumn, true, false, -1);
		style_elm = styles_context().last_state().get_office_element();
		
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
		std::wstring style_cell_name= styles_context().find_odf_style_name_default(odf::style_family::TableCell);
		current_table().set_column_default_cell_style(style_cell_name);
	}
}
void ods_conversion_context::start_text_context()
{
	current_text_context_ = new odf_text_context(&styles_context(),this);

}
void ods_conversion_context::end_text_context()
{
	if (current_text_context_)
		delete current_text_context_;
	current_text_context_ = NULL;
}
void ods_conversion_context::add_text_content(std::wstring & text)
{
	if (current_text_context_)
	{
		current_text_context_->add_text_content(text);
	}
}
void ods_conversion_context::start_cell_text()
{
	end_text_context();
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

}
}
