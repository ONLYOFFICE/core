#pragma once

#include <string>
#include "ods_table_state.h"


namespace cpdoccore {
namespace odf {

class ods_conversion_context;
class ods_text_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

/// \class ods_table_context
class ods_table_context
{
public:
    ods_table_context(ods_conversion_context & Context/*, ods_text_context & textCotnext*/);

public:
    void start_table(office_element_ptr & elm);
    void end_table();

    //std::wstring get_current_table_name() const;
    
    //void start_cell(const std::wstring & formula,
    //                size_t columnsSpanned,
    //                size_t rowsSpanned);

    //void end_cell();

 //   std::wstring default_row_cell_style() const;
 //   std::wstring default_column_cell_style() const;

 //   void start_covered_cell();
 //   void end_covered_cell();

 //   void start_cell_content();    
 //   int end_cell_content();

 //   void set_current_cell_style_id(unsigned int xfId);
 //   int get_current_cell_style_id();

 //   int current_column() const;
 //   int current_row() const;

	//void set_table_row_group(int count, bool collapsed, int level);
 //   void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
 //   void non_empty_row();
 //   bool is_empty_row() const;
 //   void end_row();

 //   void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName);

 //   size_t depth() const { return table_state_stack_.size(); }

 //   unsigned int columns_count();

 //   void serialize_merge_cells(std::wostream & _Wostream);

	//void serialize_table_format(std::wostream & _Wostream);

	//xlsx_table_metrics & get_table_metrics();
 // 
	//xlsx_drawing_context & get_drawing_context();

 //   xlsx_comments_context & get_comments_context();
	//
 //   void table_column_last_width(double w);
 //   double table_column_last_width() const;

    ods_table_state & state();
    const ods_table_state & state() const;

 //   void start_hyperlink();
	//std::wstring end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

 //  void dump_rels_hyperlinks(rels & Rels);
 //  void serialize_hyperlinks(std::wostream & _Wostream);

private:

    ods_conversion_context & context_;
    std::list<ods_table_state> table_state_list_;
   
	
	
	//ods_text_context & xlsx_text_context_;
};


}
}
