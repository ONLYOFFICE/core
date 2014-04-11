#pragma once

#include "object_package.h"
#include "odf_style_context.h"
#include "odf_drawing_context.h"

namespace cpdoccore { 
namespace odf {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class odf_conversion_context : boost::noncopyable
{
public:
    odf_conversion_context(package::odf_document * outputDocument);
    virtual ~odf_conversion_context();

	virtual void start_document() = 0 ;
	void end_document();

	office_element_ptr & getCurrentElement();

	std::vector<office_element_ptr>  content_;
	std::vector<office_element_ptr>  content_styles_;
	std::vector<office_element_ptr>  styles_;
	
    void add_rel(relationship const & r);

	package::odf_document * output_document_;

	odf_style_context &		styles_context(){return style_context_;}
	odf_drawing_context&	drawing_context(){return drawing_context_;}
		
	odf_number_styles_context & numbers_styles_context() {return style_context_.numbers_styles();}


private:
	rels	rels_;
	void process_styles();

public:

	odf_drawing_context		drawing_context_;	
	odf_style_context		style_context_;

    //styles_container		& styleContainer()		{ return major_style_container_; }
	//page_layout_container & pageLayoutContainer()	{ return page_layout_container_; }
	//fonts_container		& fontContainer()		{ return fonts_container_; }
	//list_style_container	& listStyleContainer()	{ return list_style_container_; }

	//notes_configuration &	noteConfiguration()		{ return notes_configuration_; }

	//styles_lite_container &	numberStyles()		{ return number_style_container_; }
	//styles_lite_container &	drawStyles()		{ return draw_style_container_; }
	//styles_lite_container &	Templates()			{ return template_container_; }

private:
    //styles_container		major_style_container_;
	//page_layout_container	page_layout_container_;
	//fonts_container		fonts_container_;
	//list_style_container	list_style_container_;
	//notes_configuration	notes_configuration_;

	//styles_lite_container	number_style_container_;
	//styles_lite_container	draw_style_container_;
	//styles_lite_container	template_container_;
};

}
}