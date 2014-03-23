#pragma once

#include <string>
#include <vector>
//
//#include "ods_row_spanned.h"
//#include "ods_merge_cells.h"
//#include "ods_table_metrics.h"
//#include "ods_drawing_context.h"
//#include "ods_comments_context.h"
//#include "ods_hyperlinks.h"

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore {
namespace odf {

class ods_conversion_context;
class table_table;
class style;
class color;

struct ods_element_state
{
	office_element_ptr elm;
	
	int repeated;
	std::wstring style_name;
	office_element_ptr style_elm;
};

struct ods_cell_state : ods_element_state
{
	std::wstring ref;
	int col;
	int row;
};

class ods_table_state
{
public:
	ods_table_state(ods_conversion_context & Context, office_element_ptr & elm);
		void set_table_name(std::wstring);
		void set_table_style(office_element_ptr & _style);
		void set_table_hidden(bool Val);
		void set_table_tab_color(_CP_OPT(color) & _color);

	void add_column(office_element_ptr & elm, int repeated ,office_element_ptr & style);
		void set_column_width(int width);
		void set_column_optimal_width(bool val);
		void set_column_default_cell_style(std::wstring & style_name);

	void add_row(office_element_ptr & elm, int repeated ,office_element_ptr & style);//const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
		void set_row_hidden(bool Val);
		//void set_row_collapsed(bool Val);
		void set_row_optimal_height(bool val);
		void set_row_height(double height);
	//void set_table_row_group(int count, bool collapsed, int level);
		void set_row_default_cell_style(std::wstring & style_name);


	//void start_cell(/*size_t columnsSpanned, size_t rowsSpanned*/);
	void start_cell(office_element_ptr & elm ,office_element_ptr & style);
	void end_cell();
	void add_default_cell(office_element_ptr & cell, int repeated);
	void set_cell_ref (std::wstring & ref, int col, int row);
	void set_cell_format_value(int format);
	void set_cell_type(int type);
	void set_cell_value(std::wstring & value);

 //   void start_covered_cell();
 //   void end_covered_cell();

	office_element_ptr & current_row_element();
	office_element_ptr & current_cell_element();

 //   void set_current_cell_style_id(unsigned int xfId);
 //   int get_current_cell_style_id();

	int current_column() const;
	int current_row() const;

 //   unsigned int current_columns_spaned() const;
 //   unsigned int current_rows_spanned(unsigned int Column) const;
	unsigned int columns_count() const;
 //   
 //   xlsx_table_metrics & get_table_metrics() { return xlsx_table_metrics_; }
 //   xlsx_drawing_context & get_drawing_context() { return xlsx_drawing_context_; }
 //   xlsx_comments_context & get_comments_context() { return xlsx_comments_context_; }

 //   void table_column_last_width(double w) { table_column_last_width_ = w; }
 //   double table_column_last_width() const { return table_column_last_width_; };

 //   void start_hyperlink();
	//std::wstring end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

	//void serialize_table_format(std::wostream & _Wostream);
	//void serialize_merge_cells(std::wostream & _Wostream);
 //   void serialize_hyperlinks(std::wostream & _Wostream);
 //   void dump_rels_hyperlinks(rels & Rels);

	//struct _group_row
	//{
	//	bool enabled;
	//	int count;
	//	int level;
	//	bool collapsed;
	//}group_row_;
private:
    ods_conversion_context & context_;   
	
	office_element_ptr	office_table_;	
	style*				office_table_style_;//??? может хранить как office_element_ptr ???

	//std::wstring table_style_;
	//std::wstring tableName_;
 //   std::wstring table_row_style_;
 //   std::vector<std::wstring> column_default_cell_style_name_;
	std::wstring row_default_cell_style_name_;
 //   std::wstring cell_style_;
	int current_table_column_;
	int current_table_row_;
 //   unsigned int columns_spanned_num_;
 //   std::wstring columns_spanned_style_;
 //   std::vector<xlsx_row_spanned> rows_spanned_;
	std::vector<ods_element_state> columns_;
	std::vector<ods_element_state> rows_;
	
	std::vector<ods_cell_state> cells_;
	unsigned int columns_count_;
 //   xlsx_merge_cells merge_cells_; 
 //   xlsx_table_metrics xlsx_table_metrics_;
 //   xlsx_drawing_context xlsx_drawing_context_;
 //   xlsx_comments_context xlsx_comments_context_;


	//bool empty_row_;
 //   double table_column_last_width_;
 //   xlsx_hyperlinks xlsx_hyperlinks_;

};


}
}

