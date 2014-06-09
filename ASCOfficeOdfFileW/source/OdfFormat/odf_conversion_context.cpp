#include "precompiled_cpodf.h"

#include <boost/foreach.hpp>

#include "office_spreadsheet.h"
#include "office_chart.h"

#include "office_elements_create.h"

#include "odf_conversion_context.h"
#include "odf_rels.h"


namespace cpdoccore { 
namespace odf {

//////////////////////////////////////////////////////////////////////////////////////////////////

odf_conversion_context::odf_conversion_context(package::odf_document * outputDocument) : chart_context_ (this), page_layout_context_(this)
{ 
	output_document_ = outputDocument;

	current_object_ =0;
}

odf_style_context* odf_conversion_context::styles_context()
{
	if (objects_.size() > 0)
		return objects_[current_object_].style_context.get();
	else
		return NULL;
}

odf_page_layout_context* odf_conversion_context::page_layout_context()
{
	return &page_layout_context_;
}
odf_chart_context* odf_conversion_context::chart_context()
{
	return &chart_context_;
}

odf_number_styles_context* odf_conversion_context::numbers_styles_context()	
{
	if (objects_.size() > 0 && objects_[current_object_].style_context)
			return &(objects_[current_object_].style_context->numbers_styles());
	else return NULL;
}

_mediaitems* odf_conversion_context::mediaitems()				
{
	return &objects_[current_object_].mediaitems;
}

odf_conversion_context::~odf_conversion_context()
{

}



void odf_conversion_context::end_document()
{
	rels	rels_;
	for (long i=0; i< objects_.size(); i++)
	{
		_object & object = objects_[i];
		bool isRoot = (i==0? true : false);
	
		if (object.content == NULL)continue;

		process_styles(object,isRoot);

		package::content_content_ptr content_root_ = package::content_content::create();		
		object.content->serialize(content_root_->content());		
		BOOST_FOREACH(const office_element_ptr & elm, object.content_styles)
		{
			elm->serialize(content_root_->styles());
		}

		package::content_simple_ptr content_style_ = package::content_simple::create();
		BOOST_FOREACH(const office_element_ptr & elm, object.styles)
		{// мастер-пейджы, заданные заливки (градиенты, битмапы), дефолтные стили, колонтитулы, разметки, заметки,...

			elm->serialize(content_style_->content());
		}
////////////////////////////
		package::object_files *object_files =  new package::object_files();
		if (object_files)
		{
			object_files->set_content	(content_root_);
			object_files->set_styles	(content_style_);
			object_files->set_media		(object.mediaitems);
			object_files->set_pictures	(object.mediaitems);

			if (!isRoot)object_files->local_path = object.name + L"/";
			
			object.mediaitems.dump_rels(rels_,object_files->local_path);

			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"styles.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"content.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"meta.xml"));

			if (isRoot)object_files->local_path = L"/";
			rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.") + object.content->get_name(), object_files->local_path));

			output_document_->add_object(package::element_ptr(object_files),isRoot);
		}

	}

	output_document_->set_rels(rels_);
}
void odf_conversion_context::start_chart()
{
	create_object();
	create_element(L"office", L"chart", objects_.back().content, this,true);

	chart_context_.set_styles_context(styles_context());
	chart_context_.start_chart(get_current_object_element());
}
void odf_conversion_context::start_spreadsheet()
{
	create_object();
	create_element(L"office", L"spreadsheet", objects_.back().content, this,true);
}
void odf_conversion_context::create_object()
{
	_object obj;
	
	obj.style_context = boost::make_shared<odf_style_context>();
	obj.style_context->set_odf_context(this);
	obj.name = get_next_name_object();
	
	objects_.push_back(obj);

	current_object_ = objects_.size()-1;
}
void odf_conversion_context::end_chart()
{
	chart_context_.end_chart();

	end_object();
	chart_context_.set_styles_context(styles_context());
}
void odf_conversion_context::end_spreadsheet()
{
	end_object();
}
void odf_conversion_context::end_object()
{
	current_object_ = 0;
}

office_element_ptr & odf_conversion_context::get_current_object_element()
{
	return objects_[current_object_].content;
}

std::wstring odf_conversion_context::get_next_name_object()
{
	return std::wstring(L"Object ") + boost::lexical_cast<std::wstring>(objects_.size());
}

void odf_conversion_context::process_styles(_object & object, bool isRoot)
{
	create_element(L"office", L"styles", object.styles, this,true);//общие стили
	object.style_context->process_office(object.styles.back());
	
	if (isRoot)
	{	
		create_element(L"office", L"font-face-decls", object.styles, this,true);
		
		create_element(L"office", L"automatic-styles", object.styles, this,true);
		object.style_context->process_automatic_styles(object.styles.back());

		create_element(L"office", L"master-styles", object.styles, this,true);
		object.style_context->process_master(object.styles.back());
		
		create_element(L"office", L"font-face-decls", object.content_styles, this,true);
	}

	create_element(L"office", L"automatic-styles", object.content_styles, this,true);
	object.style_context->process_automatic(object.content_styles.back());

}

}
}
