#ifndef _CPDOCCORE_ODF_OFFICE_DOCUMENT_H_
#define _CPDOCCORE_ODF_OFFICE_DOCUMENT_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "office_body.h"

namespace cpdoccore { 
namespace odf_reader {

/// \class  office_document_base
class office_document_base  : public office_element
{
public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
    office_document_base();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    std::wstring office_mimetype_;
    std::wstring office_version_;
    // office-document-attrs
    // office-document-common-attrs
    office_element_ptr office_meta_; // office-meta
    office_element_ptr office_settings_;// office-settings
    office_element_ptr office_scripts_;
    office_element_ptr office_font_face_decls_;// office-font-face-decls
    
    office_element_ptr office_styles_; // office-styles
    office_element_ptr office_automatic_styles_; // office-automatic-styles
    office_element_ptr office_master_styles_; // office-master-styles
 	office_element_ptr_array manifests_; // manifests
   

public:
    office_element_ptr office_body_; // office-body

    friend class odf_document;

};


/// \class  office_document 
/// \brief  office:document 
class office_document : public office_document_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeDocument;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

};

CP_REGISTER_OFFICE_ELEMENT2(office_document);

/// \class office_document_content
/// \brief office:document-content
class office_document_content : public office_document_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeDocumentContent;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();
};

CP_REGISTER_OFFICE_ELEMENT2(office_document_content);

/// \class office_document_styles
/// \brief office:document-styles
class office_document_styles : public office_document_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeDocumentStyles;
    
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

private:

};

CP_REGISTER_OFFICE_ELEMENT2(office_document_styles);

/// \class office_document_meta
/// \brief office:document-meta
class office_document_meta : public office_document_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeDocumentMeta;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();
};

CP_REGISTER_OFFICE_ELEMENT2(office_document_meta);

/// \class office_document_settings
/// \brief office:document-settings
class office_document_settings : public office_document_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeDocumentSettings;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();
};

CP_REGISTER_OFFICE_ELEMENT2(office_document_settings);


/// \brief manifest:manifest
class manifest_manifest : public office_document_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifest;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();
};

CP_REGISTER_OFFICE_ELEMENT2(manifest_manifest);

/// \brief manifest:file-entry
class manifest_entry  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestEntry;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring media_type_;
	std::wstring full_path_;

	office_element_ptr encryption_;
};

CP_REGISTER_OFFICE_ELEMENT2(manifest_entry);


/// \brief manifest:encryption-data
class manifest_encryption_data  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_text(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestEncryptionData;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring manifest_checksum_;
	std::wstring manifest_checksum_type_;

	//childs
	//manifest:algorithm
	//manifest:key-derivation 
};

CP_REGISTER_OFFICE_ELEMENT2(manifest_encryption_data);

}
}

#endif
