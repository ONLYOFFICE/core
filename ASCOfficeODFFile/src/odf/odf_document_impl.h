#ifndef CPDOCCORE_ODF_DOCUMENT_IMPL_H_
#define CPDOCCORE_ODF_DOCUMENT_IMPL_H_

#include <cpdoccore/odf/odf_document.h>
#include <string>
#include "odf_content_xml.h"

#ifdef _MSC_VER
#pragma once
#endif

namespace cpdoccore { 

namespace oox {
class docx_conversion_context;
class xlsx_conversion_context;
}

namespace odf {

class odf_read_context;
typedef shared_ptr<odf_read_context>::Type odf_read_context_ptr;

class content_xml_t;
typedef shared_ptr<content_xml_t>::Type content_xml_t_ptr;

/// \class  odf_document::Impl
class odf_document::Impl
{
public:
    Impl(const std::wstring & Folder);
    odf_read_context & odf_context();
   
	void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);

    const std::wstring & get_folder() const { return base_folder_; }
    const office_element * get_content() const;

	long get_office_mime_type() {return office_mime_type_;}
    
private:
    odf_read_context_ptr context_;
    void parse_styles();
    void parse_fonts();
	void parse_manifests();

private:
    content_xml_t_ptr content_xml_;
    content_xml_t_ptr styles_xml_;
    content_xml_t_ptr meta_xml_;
    content_xml_t_ptr settings_xml_;
    content_xml_t_ptr manifest_xml_;

private:
    std::wstring base_folder_;
	long office_mime_type_;
            
};

}
}

#endif
