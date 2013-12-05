#pragma once

#include <string>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>

#include <cpdoccore/xml/attributes.h>

#include "oox_conversion_context.h"

namespace cpdoccore {
namespace odf
{
	class styles_container;
	class odf_read_context;
	typedef boost::shared_ptr<styles_container> styles_container_ptr;
};
namespace oox {
	
class pptx_conversion_context;

class pptx_text_context: boost::noncopyable
{
public:
	pptx_text_context(odf::odf_read_context & odf_context_, pptx_conversion_context & pptx_contxt_);
    ~pptx_text_context();

	void set_local_styles_container(odf::styles_container*  local_styles_);
    
	void add_text(const std::wstring & text);
    
    void start_paragraph(const std::wstring & styleName);
    void end_paragraph();

    void start_span(const std::wstring & styleName);
    void end_span();
    std::wstring end_span2();

	void start_comment_content();
	std::wstring end_comment_content();
	
	void start_drawing_content();
	std::wstring end_drawing_content();

	void start_hyperlink();
	void end_hyperlink(std::wstring hId);

	bool is_drawing_context();

    void start_list(const std::wstring & StyleName, bool Continue = false);
    void end_list();

	void start_list_item(bool restart = false);
	void end_list_item();

	styles_context & get_styles_context();   

private:

	class Impl;
    _CP_SCOPED_PTR(Impl) impl_;

   
};

}
}