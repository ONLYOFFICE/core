
#include "xlsx_tablecontext.h"
#include "xlsx_textcontext.h"
#include "xlsxconversioncontext.h"
#include "xlsx_table_state.h"
#include "xlsx_utils.h"

#include "../formulasconvert/formulasconvert.h"

#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/simple_xml_writer.h>


namespace cpdoccore {
namespace oox {

//-----------------------------------------------------------------------------------------------------------------------

xlsx_table_state_ptr & xlsx_table_context::state()
{
    return xlsx_table_states_.back();
}

const xlsx_table_state_ptr & xlsx_table_context::state() const
{
    return xlsx_table_states_.back();
}

void xlsx_table_context::start_database_range(std::wstring tableName, std::wstring ref)
{	
	formulasconvert::odf2oox_converter convert;	
	ref	= convert.convert_named_ref(ref);

	std::wstring ref1, ref2;
	int pos = ref.find(L":");

	std::wstring xlsx_table_name;

	if (pos >= 0)
	{
		xlsx_data_ranges_.push_back(xlsx_data_range_ptr(new xlsx_data_range()));
		xlsx_data_ranges_.back()->table_name = tableName;
		
		ref1 = ref.substr(0, pos );
		ref2 = ref.substr(pos + 1);
		
		pos = ref1.find(L"!");							
		if (pos > 0)
		{
			xlsx_table_name = ref1.substr(0, pos);
			ref1 = ref1.substr(pos + 1);
		}
		
		pos = ref2.find(L"!");
		if (pos > 0) ref2 = ref2.substr(pos + 1);
	
		xlsx_data_ranges_.back()->ref = ref1 + L":" + ref2;

		size_t col, row;
		
		getCellAddressInv(ref1, col, row);
		xlsx_data_ranges_.back()->cell_start = std::pair<int, int>(col,row);

		getCellAddressInv(ref2, col, row);
		xlsx_data_ranges_.back()->cell_end = std::pair<int, int>(col,row);
	}

	if (!xlsx_table_name.empty())
	{
		boost::algorithm::replace_all(xlsx_table_name, L"'", L"");
		xlsx_data_ranges_map_.insert(std::pair<std::wstring, int> (xlsx_table_name, xlsx_data_ranges_.size() - 1));
	}

}
void xlsx_table_context::set_database_orientation (bool val)
{
	if (xlsx_data_ranges_.empty()) return;

	xlsx_data_ranges_.back()->byRow = val;
}
void xlsx_table_context::set_database_header (bool val)
{
	if (xlsx_data_ranges_.empty()) return;

	xlsx_data_ranges_.back()->withHeader = val;
}
void xlsx_table_context::set_database_filter (bool val)
{
	if (xlsx_data_ranges_.empty()) return;

	xlsx_data_ranges_.back()->filter = val;
}
void xlsx_table_context::end_database_range()
{
}

void xlsx_table_context::add_database_sort(int field_number, int order)
{
	xlsx_data_ranges_.back()->bySort.push_back(std::pair<int, bool>(field_number, order == 1 ? false : true ));
}


xlsx_table_context::
xlsx_table_context(xlsx_conversion_context * Context, xlsx_text_context & textContext): xlsx_conversion_context_(Context),
xlsx_text_context_(textContext)
{        
}

void xlsx_table_context::start_table(std::wstring tableName, std::wstring tableStyleName)
{
	xlsx_table_state_ptr  state = boost::make_shared<xlsx_table_state>(xlsx_conversion_context_, tableStyleName, tableName);
    xlsx_table_states_.push_back( state);
}

void xlsx_table_context::end_table()
{
    xlsx_table_states_.pop_back();
}

std::wstring xlsx_table_context::get_current_table_name() const
{
    return state()->get_current_table_name();
}

void xlsx_table_context::start_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned)
{
    state()->start_cell(columnsSpanned, rowsSpanned);    
}

void xlsx_table_context::end_cell()
{    
    state()->end_cell();
}

void xlsx_table_context::set_current_cell_style_id(unsigned int xfId)
{
    return state()->set_current_cell_style_id(xfId);
}

int xlsx_table_context::get_current_cell_style_id()
{
    return state()->get_current_cell_style_id();
}

void xlsx_table_context::start_cell_content()
{
    xlsx_text_context_.start_cell_content();
}

int xlsx_table_context::end_cell_content()
{
    return xlsx_text_context_.end_cell_content();
}

void xlsx_table_context::start_covered_cell()
{
    return state()->start_covered_cell();
}

void xlsx_table_context::end_covered_cell()
{
    return state()->end_covered_cell();
}
void xlsx_table_context::set_table_row_group(int count, bool collapsed, int level)
{
	return state()->set_table_row_group(count, collapsed, level);
}
void xlsx_table_context::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    return state()->start_row(StyleName, defaultCellStyleName);
}

void xlsx_table_context::non_empty_row()
{
    return state()->non_empty_row();
}

bool xlsx_table_context::is_empty_row() const
{
    return state()->is_empty_row();
}

void xlsx_table_context::end_row()
{
    return state()->end_row();
}

void xlsx_table_context::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    return state()->start_column(repeated, defaultCellStyleName);
}

unsigned int xlsx_table_context::columns_count()
{
    return state()->columns_count();    
}

std::wstring xlsx_table_context::default_row_cell_style() const
{
    return state()->default_row_cell_style();
}

std::wstring xlsx_table_context::default_column_cell_style() const
{
    return state()->default_column_cell_style();
}

int xlsx_table_context::current_column() const
{
    return state()->current_column();
}

int xlsx_table_context::current_row() const
{
    return state()->current_row();
}


void xlsx_table_context::serialize_sort(std::wostream & _Wostream)
{
	if (xlsx_data_ranges_.empty()) return;

	std::pair<std::multimap<std::wstring, int>::iterator, std::multimap<std::wstring, int>::iterator> range;

	range = xlsx_data_ranges_map_.equal_range(state()->tableName_);

	for (std::multimap<std::wstring, int>::iterator it = range.first; it != range.second; it++)
	{
		xlsx_data_ranges_[it->second]->serialize_sort(_Wostream);
	}
}
void xlsx_table_context::serialize_autofilter(std::wostream & _Wostream)
{
	if (xlsx_data_ranges_.empty()) return;

	std::wstring ref;

	std::pair<int, int> cell_start	(-1, -1);
	std::pair<int, int> cell_end	(-1, -1);

	std::pair<std::multimap<std::wstring, int>::iterator, std::multimap<std::wstring, int>::iterator> range;

	range = xlsx_data_ranges_map_.equal_range(state()->tableName_);

	for (std::multimap<std::wstring, int>::iterator it = range.first; it != range.second; it++)
	{
		if (xlsx_data_ranges_[it->second]->filter)
		{
			if (cell_start.first < 0 || xlsx_data_ranges_[it->second]->cell_start.first < cell_start.first )
				cell_start.first = xlsx_data_ranges_[it->second]->cell_start.first;
			
			if (cell_start.second < 0 || xlsx_data_ranges_[it->second]->cell_start.second < cell_start.second )
				cell_start.second = xlsx_data_ranges_[it->second]->cell_start.second;
			
			if (cell_end.first < 0 || xlsx_data_ranges_[it->second]->cell_end.first > cell_end.first )
				cell_end.first = xlsx_data_ranges_[it->second]->cell_end.first;
			
			if (cell_end.second < 0 || xlsx_data_ranges_[it->second]->cell_end.second > cell_end.second )
				cell_end.second = xlsx_data_ranges_[it->second]->cell_end.second;	

			ref = xlsx_data_ranges_[it->second]->ref + L";";
		}
	}
	if (cell_end.first < 0  || cell_start.first < 0) return;

	ref.erase(ref.size() - 1, 1);

	CP_XML_WRITER(_Wostream)
	{			
		CP_XML_NODE(L"autoFilter")
		{
			//в автофильтре тока простые диапазоны .. для сложных нужно выделять tablePart - todooo
			CP_XML_ATTR(L"ref", getCellAddress(cell_start.first, cell_start.second) + L":" + getCellAddress(cell_end.first, cell_end.second));
			//CP_XML_ATTR(L"ref", ref);
		}
	}
}
void xlsx_table_context::serialize_merge_cells(std::wostream & _Wostream)
{
    return state()->serialize_merge_cells(_Wostream);
}
void xlsx_table_context::serialize_table_format(std::wostream & _Wostream)
{
    return state()->serialize_table_format(_Wostream);
}
void xlsx_table_context::serialize_hyperlinks(std::wostream & _Wostream)
{
    return state()->serialize_hyperlinks(_Wostream);
}
void xlsx_table_context::dump_rels_hyperlinks(rels & Rels)
{
	return state()->dump_rels_hyperlinks(Rels);
}

xlsx_table_metrics & xlsx_table_context::get_table_metrics()
{
    return state()->get_table_metrics();
}

xlsx_drawing_context & xlsx_table_context::get_drawing_context()
{
    return state()->get_drawing_context();
}

xlsx_comments_context & xlsx_table_context::get_comments_context()
{
    return state()->get_comments_context();
}
void xlsx_table_context::table_column_last_width(double w)
{
    return state()->table_column_last_width(w);
}

double xlsx_table_context::table_column_last_width() const
{
    return state()->table_column_last_width();
}

void xlsx_table_context::start_hyperlink()
{
    return state()->start_hyperlink();
}

 std::wstring xlsx_table_context::end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display)
{
    return state()->end_hyperlink(ref, href, display);
}


}
}
