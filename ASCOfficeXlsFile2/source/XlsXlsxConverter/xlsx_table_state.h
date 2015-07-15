#pragma once

#include <vector>

#include "xlsx_row_spanned.h"
#include "xlsx_merge_cells.h"
#include "xlsx_table_metrics.h"
#include "xlsx_drawing_context.h"
#include "xlsx_comments_context.h"
#include "xlsx_hyperlinks.h"

namespace cpdoccore {
namespace oox {

class xlsx_conversion_context;

class xlsx_table_state
{
public:
    xlsx_table_state(xlsx_conversion_context & Context, const std::wstring & StyleName, const std::wstring & tableName);
    std::wstring current_style() const { return table_style_; }
    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName);
    void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
    void non_empty_row();
    bool is_empty_row() const;
    void end_row();
    std::wstring current_row_style() const;
    std::wstring default_row_cell_style() const;
    std::wstring default_column_cell_style() const;

	void set_table_row_group(int count, bool collapsed, int level);
    void start_cell(size_t columnsSpanned, size_t rowsSpanned);
    void end_cell();

    void start_covered_cell();
    void end_covered_cell();

    void set_current_cell_style_id(unsigned int xfId);
    int get_current_cell_style_id();

    int current_column() const;
    int current_row() const;

    unsigned int current_columns_spaned() const;
    unsigned int current_rows_spanned(unsigned int Column) const;
    unsigned int columns_count() const;
    
    xlsx_table_metrics & get_table_metrics() { return xlsx_table_metrics_; }
    xlsx_drawing_context & get_drawing_context() { return xlsx_drawing_context_; }
    xlsx_comments_context & get_comments_context() { return xlsx_comments_context_; }

    void table_column_last_width(double w) { table_column_last_width_ = w; }
    double table_column_last_width() const { return table_column_last_width_; };

    void start_hyperlink();
	std::wstring end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

	void serialize_table_format(std::wostream & _Wostream);
	void serialize_merge_cells(std::wostream & _Wostream);
    void serialize_hyperlinks(std::wostream & _Wostream);
    void dump_rels_hyperlinks(rels & Rels);

    std::wstring get_current_table_name() const { return tableName_; }

	struct _group_row
	{
		bool enabled;
		int count;
		int level;
		bool collapsed;
	}group_row_;
private:
    xlsx_conversion_context & context_;    
    std::wstring table_style_;
    std::wstring tableName_;
    std::wstring table_row_style_;
    std::vector<std::wstring> column_default_cell_style_name_;
    std::wstring row_default_cell_style_name_;
    std::wstring cell_style_;
    int current_table_column_;
    int current_table_row_;
    unsigned int columns_spanned_num_;
    std::wstring columns_spanned_style_;
    std::vector<xlsx_row_spanned> rows_spanned_;
    std::vector<unsigned int> columns_;
    unsigned int columns_count_;
    xlsx_merge_cells merge_cells_; 
    xlsx_table_metrics xlsx_table_metrics_;
    xlsx_drawing_context xlsx_drawing_context_;
    xlsx_comments_context xlsx_comments_context_;


	bool empty_row_;
    double table_column_last_width_;
    xlsx_hyperlinks xlsx_hyperlinks_;

};


}
}

