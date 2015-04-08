#include "../odf/precompiled_cpodf.h"
#include "xlsx_tablecontext.h"
#include "xlsx_textcontext.h"
#include "xlsxconversioncontext.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/odf/odf_document.h>

#include "xlsx_table_state.h"

namespace cpdoccore {
namespace oox {

/**/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xlsx_table_state & xlsx_table_context::state()
{
    return table_state_stack_.back();
}

const xlsx_table_state & xlsx_table_context::state() const
{
    return table_state_stack_.back();
}

xlsx_table_context::
xlsx_table_context(xlsx_conversion_context & Context, xlsx_text_context & textCotnext): context_(Context),
xlsx_text_context_(textCotnext)
{        
}

void xlsx_table_context::start_table(const std::wstring & tableName, const std::wstring & tableStyleName)
{
    table_state_stack_.push_back( xlsx_table_state(context_, tableStyleName, tableName) );
}

void xlsx_table_context::end_table()
{
    table_state_stack_.pop_back();
}

std::wstring xlsx_table_context::get_current_table_name() const
{
    return state().get_current_table_name();
}

void xlsx_table_context::start_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned)
{
    state().start_cell(columnsSpanned, rowsSpanned);    
}

void xlsx_table_context::end_cell()
{    
    state().end_cell();
}

void xlsx_table_context::set_current_cell_style_id(unsigned int xfId)
{
    return state().set_current_cell_style_id(xfId);
}

int xlsx_table_context::get_current_cell_style_id()
{
    return state().get_current_cell_style_id();
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
    return state().start_covered_cell();
}

void xlsx_table_context::end_covered_cell()
{
    return state().end_covered_cell();
}
void xlsx_table_context::set_table_row_group(int count, bool collapsed, int level)
{
	return state().set_table_row_group(count, collapsed, level);
}
void xlsx_table_context::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    return state().start_row(StyleName, defaultCellStyleName);
}

void xlsx_table_context::non_empty_row()
{
    return table_state_stack_.back().non_empty_row();
}

bool xlsx_table_context::is_empty_row() const
{
    return state().is_empty_row();
}

void xlsx_table_context::end_row()
{
    return state().end_row();
}

void xlsx_table_context::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    return state().start_column(repeated, defaultCellStyleName);
}

unsigned int xlsx_table_context::columns_count()
{
    return table_state_stack_.back().columns_count();    
}

std::wstring xlsx_table_context::default_row_cell_style() const
{
    return state().default_row_cell_style();
}

std::wstring xlsx_table_context::default_column_cell_style() const
{
    return state().default_column_cell_style();
}

int xlsx_table_context::current_column() const
{
    return state().current_column();
}

int xlsx_table_context::current_row() const
{
    return state().current_row();
}

void xlsx_table_context::serialize_merge_cells(std::wostream & _Wostream)
{
    return state().serialize_merge_cells(_Wostream);
}
void xlsx_table_context::serialize_table_format(std::wostream & _Wostream)
{
    return state().serialize_table_format(_Wostream);
}
void xlsx_table_context::serialize_hyperlinks(std::wostream & _Wostream)
{
    return state().serialize_hyperlinks(_Wostream);
}
void xlsx_table_context::dump_rels_hyperlinks(rels & Rels)
{
	return state().dump_rels_hyperlinks(Rels);
}

xlsx_table_metrics & xlsx_table_context::get_table_metrics()
{
    return state().get_table_metrics();
}

xlsx_drawing_context & xlsx_table_context::get_drawing_context()
{
    return state().get_drawing_context();
}

xlsx_comments_context & xlsx_table_context::get_comments_context()
{
    return state().get_comments_context();
}
void xlsx_table_context::table_column_last_width(double w)
{
    return state().table_column_last_width(w);
}

double xlsx_table_context::table_column_last_width() const
{
    return state().table_column_last_width();
}

void xlsx_table_context::start_hyperlink()
{
    return state().start_hyperlink();
}

 std::wstring xlsx_table_context::end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display)
{
    return state().end_hyperlink(ref, href, display);
}


}
}
