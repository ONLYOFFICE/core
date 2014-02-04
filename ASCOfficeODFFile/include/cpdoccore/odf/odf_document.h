#ifndef _CPDOCCORE_ODF_ODF_DOCUMENT_H_
#define _CPDOCCORE_ODF_ODF_DOCUMENT_H_

#include <string>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore { 

namespace oox {
	class docx_conversion_context;
	class xlsx_conversion_context;
	class pptx_conversion_context;
}

namespace odf {

class odf_read_context;

class odf_document
{
public:
    odf_document(const std::wstring & Folder);
    ~odf_document();

public:
    odf_read_context & odf_context();
    
	void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
	void pptx_convert(oox::pptx_conversion_context & Context);
	
	const std::wstring & get_folder() const;
	
	long get_office_mime_type();
	bool get_encrypted();

    class Impl;   
    Impl * get_impl() { return impl_.get(); }

private:
    _CP_SCOPED_PTR(Impl) impl_;
            
};

}
}

#endif // #ifndef _CPDOCCORE_ODF_ODF_DOCUMENT_H_
