#pragma once

#include <string>
#include "odf_style_state.h"


namespace cpdoccore {
namespace odf {

//class ods_conversion_context;
//class odt_conversion_context;
class odf_conversion_context;


class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

/// \class ods_table_context
class odf_style_context
{
public:
    odf_style_context(odf_conversion_context & Context/*, ods_text_context & textCotnext*/);

    void create_style(std::wstring name, const style_family style_family, bool automatic = false, int oox_id = -1);

	office_element_ptr & add_or_find(std::wstring name, const style_family family, bool automatic = false, int id = -1);

	void process_automatic(office_element_ptr root );
	void process_master(office_element_ptr root );
	void process_office(office_element_ptr root );

     
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

    odf_style_state & state();
    const odf_style_state & state() const;

 //   void start_hyperlink();
	//std::wstring end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display);

 //  void dump_rels_hyperlinks(rels & Rels);
 //  void serialize_hyperlinks(std::wostream & _Wostream);

private:

    odf_conversion_context & context_;
    std::list<odf_style_state> style_state_list_;//тут описалово и доступ к элементам из content_

};


}
}
