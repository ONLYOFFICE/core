#pragma once

#include "object_package.h"

#include "odf_style_context.h"
#include "odf_drawing_context.h"
#include "odf_chart_context.h"

#include "mediaitems.h"

namespace cpdoccore { 
namespace odf {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class odf_conversion_context : boost::noncopyable
{
	struct _object
	{
		std::wstring					 name;

		office_element_ptr				 content;
		std::vector<office_element_ptr>  content_styles;
		std::vector<office_element_ptr>  styles;	
	
		odf_style_context_ptr			 style_context;	
		_mediaitems						 mediaitems;
	};

public:
    odf_conversion_context(package::odf_document * outputDocument);
    virtual ~odf_conversion_context();

	virtual void	start_document() = 0 ;
	void			end_document();
	
	package::odf_document * output_document_;


	virtual odf_drawing_context		* drawing_context() = 0;
	virtual odf_text_context		* text_context() = 0;

	odf_style_context				* styles_context();
	odf_number_styles_context		* numbers_styles_context();
	_mediaitems						* mediaitems();

	void start_chart();
	void end_chart();

	void start_spreadsheet();
	void end_spreadsheet();
	
	void create_object();
	void end_object();

	std::wstring			get_next_name_object();
	office_element_ptr &	get_current_object_element();

private:
	std::vector<_object>	objects_;//"0" = root

	odf_chart_context		chart_context_;

	void process_styles(_object & object, bool isRoot);
	
	int	 current_object_;

	//page_layout_container & pageLayoutContainer()	{ return page_layout_container_; }
	//fonts_container		& fontContainer()		{ return fonts_container_; }
	//list_style_container	& listStyleContainer()	{ return list_style_container_; }

	//notes_configuration &	noteConfiguration()		{ return notes_configuration_; }

	//styles_lite_container &	Templates()			{ return template_container_; }


    //styles_container		major_style_container_;
	//page_layout_container	page_layout_container_;
	//fonts_container		fonts_container_;
	//list_style_container	list_style_container_;
	//notes_configuration	notes_configuration_;

	//styles_lite_container	template_container_;
};

}
}