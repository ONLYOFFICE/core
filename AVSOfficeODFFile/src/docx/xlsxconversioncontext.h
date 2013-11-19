
#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "ooxconversioncontext.h"

#include "xlsx_textcontext.h"
#include "xlsx_tablecontext.h"

#include "xlsx_sharedstrings.h"
#include "xlsx_styles.h"
#include "xlsx_output_xml.h"
#include "xlsx_num_format_context.h"
#include "xlsx_drawing_context.h"
#include "xlsx_comments_context.h"
#include "xlsx_defined_names.h"
#include "xlsx_table_metrics.h"
#include "oox_chart_context.h"

#include "mediaitems.h"


namespace cpdoccore {

namespace odf 
{
    class odf_document;
    class office_element;
}

namespace oox {

namespace package
{
    class xlsx_document;
}

class xlsx_conversion_context : boost::noncopyable
{
public:
    xlsx_conversion_context(::cpdoccore::oox::package::xlsx_document * outputDocument,
        ::cpdoccore::odf::odf_document * odfDocument);

    void start_document();
    void end_document();

    void start_chart(std::wstring const & name);
    void end_chart();
	
	void start_body();
    void end_body();

    void start_office_spreadsheet(const odf::office_element * elm);
    void end_office_spreadsheet();

    const odf::office_element * get_spreadsheet();

    void start_paragraph(const std::wstring & styleName);
    void end_paragraph();

    void start_span(const std::wstring & styleName);
    void end_span();

    bool start_table(const std::wstring & tableName, const std::wstring & tableStyleName);
    void end_table();


    void start_table_column(unsigned int repeated, const std::wstring & defaultCellStyleName, int & cMin, int & cMax);
    void table_column_last_width(double w);
    double table_column_last_width() const;
    void end_table_column();

    void start_table_row(const std::wstring & styleName, const std::wstring & defaultCellStyleName);
    void non_empty_row();
    void end_table_row();
    bool is_empty_row() const;
    void set_current_row_height(size_t height_pt);    

    void start_table_cell(const std::wstring & formula, size_t columnsSpanned, size_t rowsSpanned);
    void end_table_cell();
    
    void set_current_cell_style_id(unsigned int xfId);
    int get_current_cell_style_id();

    void start_table_covered_cell();
    void end_table_covered_cell();

    xlsx_text_context & get_text_context() { return xlsx_text_context_; }
    xlsx_table_context & get_table_context() { return xlsx_table_context_; }
    const xlsx_table_context & get_table_context() const { return xlsx_table_context_; }
    
    int current_table_column() const;
    int current_table_row() const;

    std::wstring current_cell_address() const;

    odf::odf_document * root()
    {
        return odf_document_;
    }

    std::pair<float,float> getMaxDigitSize();

    void process_styles();

    xlsx_style_manager & get_style_manager() { return xlsx_style_; }
    xlsx_xml_worksheet & current_sheet();
   
	oox_chart_context & current_chart();

    num_format_context & get_num_format_context() { return num_format_context_; }

    size_t get_default_cell_style() const { return default_style_; };

    xlsx_defined_names & get_xlsx_defined_names() { return xlsx_defined_names_; }

    xlsx_table_metrics & get_table_metrics();
    xlsx_drawing_context & get_drawing_context();
    xlsx_drawing_context_handle & get_drawing_context_handle();
	
    xlsx_comments_context & get_comments_context();
	xlsx_comments_context_handle & get_comments_context_handle();


    mediaitems & get_mediaitems() { return mediaitems_; }

    void start_hyperlink(const std::wstring & styleName);
    void end_hyperlink(std::wstring const & href);

private:
    void create_new_sheet(std::wstring const & name);
    void dump_sheet();   

private:

	package::xlsx_document * output_document_;
    const odf::office_element * spreadsheet_;
    odf::odf_document * odf_document_;    
    
    xlsx_table_context xlsx_table_context_;
	xlsx_text_context xlsx_text_context_;
   
	std::wstringstream defaultOutput_;
    std::vector<xlsx_xml_worksheet_ptr> sheets_;

	std::vector<oox_chart_context_ptr> charts_;
  
	xlsx_xml_workbook_ptr workbook_;

	std::pair<float,float> maxDigitSize_;
    
	xlsx_style_manager xlsx_style_;
    num_format_context num_format_context_;
    size_t default_style_;
    xlsx_defined_names xlsx_defined_names_;    
    mediaitems mediaitems_;
  
	xlsx_drawing_context_handle xlsx_drawing_context_handle_;
    xlsx_comments_context_handle xlsx_comments_context_handle_;
    
};

}
}
