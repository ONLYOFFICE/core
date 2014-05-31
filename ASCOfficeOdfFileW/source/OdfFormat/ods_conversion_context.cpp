#include "precompiled_cpodf.h"

#include "../utils.h"

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

void calculate_size_font_symbols(_font_metrix & metrix)
{
	double appr_px = 0;//(int)_gdi_graphics_::calculate_size_symbol_asc(metrix.font_name,metrix.font_size,metrix.italic,metrix.bold);
	
	if (appr_px <0.01)
		appr_px = (int)_gdi_graphics_::calculate_size_symbol(metrix.font_name,metrix.font_size,false/*metrix.italic*/,false/*metrix.bold*/);

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
	font_metrix_ = _font_metrix();
}


void ods_conversion_context::start_document()
{
	start_spreadsheet();
	
	root_spreadsheet_ = dynamic_cast<office_spreadsheet*>(get_current_object_element().get());
}
void ods_conversion_context::start_defined_expressions()
{
	create_element(L"table", L"named-expressions",root_spreadsheet_->getContent(),this);
	
	table_context_.start_defined_expressions(root_spreadsheet_->getContent().back());

}
void ods_conversion_context::add_defined_range(std::wstring & name,std::wstring & cell_range, int sheet_id)
{
	table_context_.add_defined_range(name,cell_range, sheet_id);
}
void ods_conversion_context::add_defined_expression(std::wstring & name,std::wstring & value, int sheet_id)
{
	table_context_.add_defined_expression(name,value, sheet_id);
}
void ods_conversion_context::start_sheet(std::wstring & name)
{
	create_element(L"table", L"table",root_spreadsheet_->getContent(),this);
	
	table_context_.start_table(root_spreadsheet_->getContent().back(),name);

	drawing_context()->set_styles_context(styles_context());
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
	if (current_table().drawing_context()->is_exist_content())
	{
		office_element_ptr shapes_root_elm;
		create_element(L"table", L"shapes",shapes_root_elm,this);

		current_table().drawing_context()->finalize(shapes_root_elm);
		
		current_table().add_child_element(shapes_root_elm);
	}

	table_context_.end_table();
	
	styles_context()->reset_defaults();
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
		style_elm = styles_context()->last_state().get_office_element();
		
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
		//std::wstring style_cell_name= styles_context()->find_odf_style_name_default(odf::style_family::TableCell);
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
		
		current_table().add_default_cell(repeated);
	}

	office_element_ptr style_elm;

	std::wstring style_cell_name;
	office_value_type::type format_value_type = office_value_type::Custom;
	
	if ( xfd_style >=0)
	{
		odf_style_state  *style_state=NULL;
		styles_context()->find_odf_style_state(xfd_style, style_family::TableCell,style_state);
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
	utils::calculate_size_font_symbols(font_metrix_);
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
	int repeat = max(current_table().dimension_row,1024) - current_table().current_row();
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
		style_elm = styles_context()->last_state().get_office_element();
		
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
		std::wstring style_cell_name= styles_context()->find_odf_style_name_default(odf::style_family::TableCell);
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
void ods_conversion_context::add_text_content(std::wstring & text)
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

void ods_conversion_context::start_image(std::wstring & image_file_name)
{
	std::wstring odf_ref_name ;
	
	mediaitems()->add_or_find(image_file_name,_mediaitems::typeImage,odf_ref_name);

	current_table().drawing_context()->start_image(odf_ref_name);
}
double ods_conversion_context:: convert_symbol_width(double val)
{
	double pixels = (double)(((256. * val + (int)(128. / font_metrix_.approx_symbol_size)) / 256.) * font_metrix_.approx_symbol_size);

	return pixels* 72./96;
}
}
}
