#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "ooxconversioncontext.h"

#include "pptx_text_context.h"
#include "pptx_slide_context.h"

//#include "xlsx_sharedstrings.h"
//#include "xlsx_styles.h"
#include "pptx_output_xml.h"
//#include "xlsx_num_format_context.h"
//#include "xlsx_drawing_context.h"
//#include "xlsx_comments_context.h"
//#include "xlsx_defined_names.h"
//#include "xlsx_table_metrics.h"
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
    class pptx_document;
}

class pptx_conversion_context : boost::noncopyable
{
public:
    pptx_conversion_context(::cpdoccore::oox::package::pptx_document * outputDocument,
        ::cpdoccore::odf::odf_document * odfDocument);

    void start_document();
    void end_document();

    void start_chart(std::wstring const & name);
    void end_chart();
	
	void start_body();
    void end_body();

    void start_office_presentation();
    void end_office_presentation();

    //void start_paragraph(const std::wstring & styleName);
    //void end_paragraph();

    //void start_span(const std::wstring & styleName);
    //void end_span();

    bool start_page(const std::wstring & pageName, 
							const std::wstring & pageStyleName,
							const std::wstring & pageLayoutName,
							const std::wstring & pageMasterName);
	void end_page();
	
	bool start_layout(	int layout_index);
	void end_layout();

	bool start_master(int master_index);
 	void end_master();
   
	void start_theme(std::wstring & name);
	void end_theme();

	pptx_slide_context & get_slide_context() { return pptx_slide_context_; }

    odf::odf_document * root()
    {
        return odf_document_;
    }


    pptx_xml_slide & current_slide();
	pptx_xml_slideLayout & current_layout();
	pptx_xml_slideMaster & current_master();
    pptx_xml_theme & current_theme();
    pptx_xml_presentation & current_presentation();//собственно она одна
  
	oox_chart_context & current_chart();
	pptx_text_context & get_text_context() { return pptx_text_context_; }

 //   num_format_context & get_num_format_context() { return num_format_context_; }

 //   xlsx_defined_names & get_xlsx_defined_names() { return xlsx_defined_names_; }

 //   xlsx_table_metrics & get_table_metrics();
 //   xlsx_drawing_context & get_drawing_context();
 //   xlsx_drawing_context_handle & get_drawing_context_handle();
	//
 //   xlsx_comments_context & get_comments_context();
	//xlsx_comments_context_handle & get_comments_context_handle();


    mediaitems & get_mediaitems() { return pptx_slide_context_.get_mediaitems(); }

 //   void start_hyperlink(const std::wstring & styleName);
 //   void end_hyperlink(std::wstring const & href);
	void process_master_pages();
	void process_layouts();
	void process_styles();
	void process_theme();

private:

    void create_new_slide(std::wstring const & name);
	void create_new_slideLayout(int id);
	void create_new_slideMaster(int id);

	package::pptx_document * output_document_;
  
    odf::odf_document * odf_document_;    
    
    pptx_slide_context pptx_slide_context_;
	pptx_text_context pptx_text_context_;

	std::vector<pptx_xml_slide_ptr> slides_;
	std::vector<pptx_xml_slideMaster_ptr> slideMasters_;
	std::vector<pptx_xml_slideLayout_ptr> slideLayouts_;

	std::vector<oox_chart_context_ptr> charts_;
	
	pptx_xml_theme_ptr theme_;
	pptx_xml_presentation presentation_;

	std::wstring current_master_page_name_;
	std::wstring current_layout_page_name_;

};

}
}
