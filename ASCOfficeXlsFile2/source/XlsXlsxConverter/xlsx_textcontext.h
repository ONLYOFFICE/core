#pragma once

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "common.h"


namespace oox {
	
class xlsx_conversion_context;

class xlsx_text_context: boost::noncopyable
{
public:
    //xlsx_text_context();
	xlsx_text_context();
    ~xlsx_text_context();
public:
	void set_local_styles_container	(/*odf_reader::styles_container *local_styles_*/);
	void set_cell_text_properties	(/*odf_reader::text_format_properties_content *text_properties*/);    
    
    void			start_paragraph(const std::wstring & styleName);
    void			end_paragraph();

    void			start_span(const std::wstring & styleName);
    void			end_span();
    std::wstring	end_span2();

    void			start_cell_content();
    int				end_cell_content();

	void			start_comment_content();
	std::wstring	end_comment_content();
	
	void			start_drawing_content();
	std::wstring	end_drawing_content();

	void			start_hyperlink();
	void			end_hyperlink(std::wstring hId);

	void add_text(const std::wstring & text);

	bool is_drawing_context();

private:
    class Impl;
    boost::scoped_ptr<Impl> impl_;

   
};

}