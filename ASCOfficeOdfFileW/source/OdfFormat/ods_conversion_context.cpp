#include "precompiled_cpodf.h"


#include "ods_conversion_context.h"

#include "office_spreadsheet.h"

#include "styles.h"
#include "style_table_properties.h"

namespace cpdoccore { 
namespace odf {


ods_conversion_context::ods_conversion_context(package::odf_document * outputDocument) 
		: odf_conversion_context(outputDocument), ods_table_context_(*this)
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
	
	ods_table_context_.start_table(current_spreadsheet_->getContent().back(),name);
	
}

void ods_conversion_context::end_sheet()
{
	ods_table_context_.end_table();
	
	styles_context().reset_defaults();
}
void ods_conversion_context::start_row(int _start_row, int repeated, bool _default)
{
	if (_start_row > ods_table_context_.state().current_row()+1)
	{
		int repeated_default = _start_row - ods_table_context_.state().current_row()-1;
		
		start_row(_start_row-repeated_default, repeated_default, true);
		end_row();
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
	
	ods_table_context_.state().add_row(row_elm, repeated, style_elm);

	if ( _default)
	{
		//std::wstring style_cell_name= styles_context().find_odf_style_name_default(odf::style_family::TableCell);
		//current_table().set_row_default_cell_style(style_cell_name);
	}
}
void ods_conversion_context::end_row()
{
	//add default last cells
	int repeated = 1024;
	
	office_element_ptr default_cell_elm;
	create_element(L"table", L"table-cell",default_cell_elm,this);

	current_table().add_default_cell(default_cell_elm, repeated);

}
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

void ods_conversion_context::start_cell(std::wstring & ref, int xfd_style)
{
	int col=0, row=0;
	parsing_ref ( ref, col,row);

	if (col > current_table().current_column()+1)
	{
		int repeated = col - current_table().current_column() -1;
		
		office_element_ptr default_cell_elm;
		create_element(L"table", L"table-cell",default_cell_elm,this);

		current_table().add_default_cell(default_cell_elm, repeated);
	}

	office_element_ptr style_elm;
	int number_format =0;

	std::wstring style_cell_name;
	if ( xfd_style >=0)
	{
		odf_style_state  *style_state=NULL;
		styles_context().find_odf_style_state(xfd_style, style_family::TableCell,style_state);
		if (style_state)
		{
			style_elm = style_state->get_office_element();
			number_format = style_state->get_number_format();
			style_cell_name = style_state->get_name();
		}
		else
		{//error
		}
	}

	office_element_ptr cell_elm;
	create_element(L"table", L"table-cell",cell_elm,this);
	
	current_table().start_cell(cell_elm, style_elm);
	
	current_table().set_cell_ref(ref,col,row);	
	current_table().set_cell_format_value(number_format);
}

void ods_conversion_context::start_columns()
{
}
void ods_conversion_context::end_columns()
{
	//add default last column  - ≈—Ћ» они не прописаны в исходном (1024 - от  балды)
	//вопрос - если и добавл€ть то  с каким стилем???
	if (ods_table_context_.state().current_column() < 1 )
		add_column(ods_table_context_.state().current_column()+1,1024,0,true);
}
void ods_conversion_context::start_rows()
{
}
void ods_conversion_context::end_rows()
{
	//add default last row
	start_row(ods_table_context_.state().current_row()+1,1024,true);
	end_row();
}
void ods_conversion_context::add_column(int start_column, int repeated, int level, bool _default)
{
	if (start_column > ods_table_context_.state().current_column()+1)
	{
		int repeated_default = start_column - ods_table_context_.state().current_column()-1;
		add_column(start_column-repeated_default,repeated_default,0,true);
	}
/////////////////////////////////////////////////////////////////
	while (level < current_table().current_level())
	{
		current_table().end_column_group();
	}
	while (level > current_table().current_level())
	{
		office_element_ptr column_group_elm;
		create_element(L"table", L"table-column-group",column_group_elm,this);
		current_table().start_column_group(column_group_elm);
	}

	office_element_ptr	style_elm;
	if ( _default)
	{
		style_elm = styles_context().find_odf_style_default(style_family::TableColumn);
	}
	else
	{
		//по сути в этом стиле раличные опции ширины колонок тока .. а если свойства совпадают - можно сгенерить один, хот€ выше и указано что стили разные.
		//то есть в оо раздел€ют оох стиль на 2 (дл€ колонки собственно, и описалово €чеек в колонки)
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


}
}
