#include <sstream>
#include <boost/noncopyable.hpp>

#include "common.h"

#include "xlsx_textcontext.h"
#include "xlsx_tablecontext.h"
#include "xlsx_drawing_context.h"
#include "xlsx_chart_context.h"

#include "xlsx_output_xml.h"

#include "external_items.h"

namespace oox {

namespace package
{
    class xlsx_document;
}
class xlsx_conversion_context : boost::noncopyable
{
public:
    xlsx_conversion_context(package::xlsx_document * outputDocument);
    ~xlsx_conversion_context();

    //void set_font_directory(std::wstring pathFonts);

    void start_document();
    void end_document();

	bool start_table(const std::wstring & name);
    void end_table();

    void start_chart();
    void end_chart();

	std::wostream				& shared_strings()			{ return xlsx_shared_strings_; }    
 	std::wostream				& defined_names()			{ return xlsx_defined_names_; }    
	
	xlsx_text_context           & get_text_context()		{ return xlsx_text_context_; }
    xlsx_table_context          & get_table_context()		{ return xlsx_table_context_; }
    xlsx_xml_worksheet          & current_sheet();
 
//   const xlsx_table_context    & get_table_context() const { return xlsx_table_context_; }
	oox_chart_context			& current_chart();

	xlsx_drawing_context        & get_drawing_context();
	xlsx_drawing_context_handle & get_drawing_context_handle();	
 //	xlsx_comments_context       & get_comments_context();
//	xlsx_comments_context_handle & get_comments_context_handle();

	external_items & get_mediaitems() { return mediaitems_; }
private:

    void create_new_sheet(std::wstring const & name);

    package::xlsx_document				*output_document_;

	external_items						mediaitems_;

    std::vector<xlsx_xml_worksheet_ptr> sheets_;
    std::vector<oox_chart_context_ptr>  charts_;
  
    //std::wstringstream                  defaultOutput_;
    //std::pair<float,float>              maxDigitSize_;
    //num_format_context                  num_format_context_;
    //size_t                              default_style_;
  
    xlsx_table_context					xlsx_table_context_;
    xlsx_text_context					xlsx_text_context_;

	std::wstringstream					xlsx_shared_strings_;
	std::wstringstream					xlsx_defined_names_;

    xlsx_drawing_context_handle			xlsx_drawing_context_handle_;
    //xlsx_comments_context_handle    xlsx_comments_context_handle_;
    
};

}