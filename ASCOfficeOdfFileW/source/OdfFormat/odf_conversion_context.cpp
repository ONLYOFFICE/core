#include "precompiled_cpodf.h"

#include "odf_conversion_context.h"
#include "odf_rels.h"

namespace cpdoccore { 
namespace odf {


//////////////////////////////////////////////////////////////////////////////////////////////////

odf_conversion_context::odf_conversion_context(package::odf_document * outputDocument)
{ 
	output_document_ = outputDocument;
}

odf_conversion_context::~odf_conversion_context()
{

}

void odf_conversion_context::start_document()
{
}

void odf_conversion_context::end_document()
{
	package::content_content_ptr content_root_ = package::content_content::create();


    std::wstringstream styles_root_strm;
    //odf_styles_.serialize(styles_root_strm);// мастер-пейджы, заданные заливки (градиенты, битмапы), дефолтные стили, колонтитулы, разметки, заметки,...

	package::object_files *object_files =  new package::object_files();
	if (object_files)
	{
		object_files->set_content(content_root_);
		object_files->set_styles(package::simple_element::create(L"styles.xml", styles_root_strm.str()));


		output_document_->add_object(package::element_ptr(object_files ),true);
	}
	std::vector<package::content_content_ptr> objects_; //styles в объектах почти пустые .. - ссылки на картинки ... и только
														//собственно стили записываются в сам контент

	{
		//...
	}
	output_document_->set_rels(rels_);
}

void odf_conversion_context::add_rel(relationship const & r)
{
	rels_.add(r);
}



}
}
