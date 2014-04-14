#include "precompiled_cpodf.h"

#include <boost/foreach.hpp>

#include "office_elements_create.h"

#include "odf_conversion_context.h"
#include "odf_rels.h"


namespace cpdoccore { 
namespace odf {

//////////////////////////////////////////////////////////////////////////////////////////////////

odf_conversion_context::odf_conversion_context(package::odf_document * outputDocument) : style_context_(*this)
{ 
	output_document_ = outputDocument;
}

odf_conversion_context::~odf_conversion_context()
{

}

office_element_ptr &  odf_conversion_context::getCurrentElement()
{
	if (content_.size()>0)
	{
		return content_.back();
	}
	else
	{ 
        CP_ASSERT(false);
        throw std::runtime_error("internal error");
	}
}

//void odf_conversion_context::start_font_face()
//{
//	create_element(L"office", L"font-face-decls", styles_, this,true);
//}

void odf_conversion_context::end_document()
{
///////////////////завершающая обработка

	process_styles();
//////////////////////////////////////////////////////////////////////
	package::content_content_ptr content_root_ = package::content_content::create();
	
	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->serialize(content_root_->content());
	}
	BOOST_FOREACH(const office_element_ptr & elm, content_styles_)
	{
		elm->serialize(content_root_->styles());
	}
//////////////////////////////////////////////////////////////////////////////////////////
	package::content_simple_ptr content_style_ = package::content_simple::create();
	BOOST_FOREACH(const office_element_ptr & elm, styles_)
	{// мастер-пейджы, заданные заливки (градиенты, битмапы), дефолтные стили, колонтитулы, разметки, заметки,...

		elm->serialize(content_style_->content());
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
	package::object_files *object_files =  new package::object_files();
	if (object_files)
	{
		object_files->set_content(content_root_);
		object_files->set_styles(content_style_);
		object_files->set_media(mediaitems_);
		object_files->set_pictures(mediaitems_);


		output_document_->add_object(package::element_ptr(object_files ),true);
	}

	mediaitems_.dump_rels(rels_);
	std::vector<package::content_content_ptr> objects_; //styles в объектах почти пустые .. - ссылки на картинки ... и только
														//собственно стили записываются в сам контент

	output_document_->set_rels(rels_);
}

void odf_conversion_context::add_rel(relationship const & r)
{
	rels_.add(r);
}

void odf_conversion_context::process_styles()
{
	create_element(L"office", L"font-face-decls", styles_, this,true);
	
	create_element(L"office", L"styles", styles_, this,true);//общие стили
	style_context_.process_office(styles_.back());
	
	create_element(L"office", L"automatic-styles", styles_, this,true);
	style_context_.process_automatic_styles(styles_.back());

	create_element(L"office", L"master-styles", styles_, this,true);
	style_context_.process_master(styles_.back());

///////////////////////////////
	create_element(L"office", L"font-face-decls", content_styles_, this,true);
	create_element(L"office", L"automatic-styles", content_styles_, this,true);
	style_context_.process_automatic(content_styles_.back());

}


}
}
