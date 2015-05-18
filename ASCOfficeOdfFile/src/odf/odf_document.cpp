
#include <cpdoccore/odf/odf_document.h>

#include "odf_document_impl.h"
#include "../progressCallback.h"

namespace cpdoccore { 
namespace odf {

odf_document::odf_document(const std::wstring & Folder, const ProgressCallback* CallBack) : impl_(new Impl(Folder,CallBack))
{}

odf_document::~odf_document()
{}

odf_read_context & odf_document::odf_context()
{
    return impl_->odf_context();
}

bool odf_document::docx_convert(oox::docx_conversion_context & Context)
{
    return impl_->docx_convert(Context);
}

bool odf_document::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    return impl_->xlsx_convert(Context);
}
bool odf_document::pptx_convert(oox::pptx_conversion_context & Context)
{
    return impl_->pptx_convert(Context);
}

const std::wstring & odf_document::get_folder() const
{
    return impl_->get_folder();
}

long odf_document::get_office_mime_type() 
{
	return impl_->get_office_mime_type();
}
bool odf_document::get_encrypted() 
{
	return impl_->get_encrypted();
}
bool odf_document::UpdateProgress(long Complete)
{
	if (!impl_) return true;
	return impl_->UpdateProgress(Complete);
}

}
}
