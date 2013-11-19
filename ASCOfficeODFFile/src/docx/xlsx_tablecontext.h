#ifndef _CPDOCCORE_XLSX_TABLE_CONTEXT_
#define _CPDOCCORE_XLSX_TABLE_CONTEXT_

#include <string>
#include "ooxtablerowspanned.h"
#include "xlsx_table_state.h"

namespace cpdoccore {
namespace oox {

class xlsx_conversion_context;
class xlsx_text_context;

/// \class xlsx_table_context
class xlsx_table_context
{
public:
    xlsx_table_context(xlsx_conversion_context & Context, xlsx_text_context & textCotnext);

public:
    void start_table(const std::wstring & tableName, const std::wstring & tableStyleName);
    void end_table();

    std::wstring get_current_table_name() const;
    
    void start_cell(const std::wstring & formula,
                    size_t columnsSpanned,
                    size_t rowsSpanned);

    void end_cell();

    std::wstring default_row_cell_style() const;
    std::wstring default_column_cell_style() const;

    void start_covered_cell();
    void end_covered_cell();

    void start_cell_content();    
    int end_cell_content();

    void set_current_cell_style_id(unsigned int xfId);
    int get_current_cell_style_id();

    int current_column() const;
    int current_row() const;

    void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
    void non_empty_row();
    bool is_empty_row() const;
    void end_row();

    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName);

    size_t depth() const { return table_state_stack_.size(); }

    unsigned int columns_count();

    void serialize_merge_cells(std::wostream & _Wostream);

	void serialize_table_format(std::wostream & _Wostream);

	xlsx_table_metrics & get_table_metrics();
  
	xlsx_drawing_context & get_drawing_context();

    xlsx_comments_context & get_comments_context();
	
    void table_column_last_width(double w);
    double table_column_last_width() const;

    xlsx_table_state & state();
    const xlsx_table_state & state() const;

    void start_hyperlink();
	std::wstring end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

   void dump_rels_hyperlinks(rels & Rels);
   void serialize_hyperlinks(std::wostream & _Wostream);

private:
    xlsx_conversion_context & context_;
    xlsx_text_context & xlsx_text_context_;
    std::list<xlsx_table_state> table_state_stack_;
};


}
}

#endif
