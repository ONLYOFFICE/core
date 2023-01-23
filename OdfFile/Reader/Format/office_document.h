/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include <iosfwd>

#include "office_elements_create.h"
#include "office_body.h"

namespace cpdoccore { 
namespace odf_reader {

class office_document_base  : public office_element
{
public:
    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);
    
	office_document_base();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text) {}
	virtual void add_space(const std::wstring & Text) {}

public:
	_CP_OPT(std::wstring) office_mimetype_;
	_CP_OPT(std::wstring) office_version_;
	_CP_OPT(std::wstring) office_class_; //old version (1.1)
    
    office_element_ptr			office_body_;				// office-body

    office_element_ptr			office_meta_;				// office-meta
    office_element_ptr			office_settings_;			// office-settings
    office_element_ptr			office_scripts_;
    office_element_ptr			office_font_face_decls_;	// office-font-face-decls
    
    office_element_ptr			office_styles_;				// office-styles
    office_element_ptr			office_automatic_styles_;	// office-automatic-styles
    office_element_ptr			office_master_styles_;		// office-master-styles
 	office_element_ptr_array	manifests_;					// manifests   

    friend class odf_document;
};


//  office_document 
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

// office_document_content
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

// office_document_styles
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

// office_document_meta
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

// office_document_settings
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


// manifest:manifest
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

// manifest:file-entry
class manifest_entry  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}
	virtual void add_space(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestEntry;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring		media_type_;
	std::wstring		full_path_;
	int					size;

	office_element_ptr	encryption_data_;
};

CP_REGISTER_OFFICE_ELEMENT2(manifest_entry);

// manifest:encryption-data
class manifest_encryption_data  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){}
	virtual void add_space(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestEncryptionData;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring checksum_;
	std::wstring checksum_type_;

	office_element_ptr algorithm_;
	office_element_ptr key_derivation_;
	office_element_ptr start_key_generation_;
};

CP_REGISTER_OFFICE_ELEMENT2(manifest_encryption_data);

// manifest:algorithm
class manifest_algorithm  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_text(const std::wstring & Text){}
	virtual void add_space(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestAlgorithm;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring algorithm_name_;
	std::wstring initialisation_vector_;
};
CP_REGISTER_OFFICE_ELEMENT2(manifest_algorithm);

// manifest:key_derivation
class manifest_key_derivation  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_text(const std::wstring & Text){}
	virtual void add_space(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestKeyDerivation;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	key_derivation_name_;
	int				key_size_;
	int				iteration_count_;
	std::wstring	salt_;
};
CP_REGISTER_OFFICE_ELEMENT2(manifest_key_derivation);

// manifest:start-key-generation
class manifest_start_key_generation  : public office_element
{
public:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_text(const std::wstring & Text){}
	virtual void add_space(const std::wstring & Text){}

	static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeManifestStartKeyGeneration;

    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring	start_key_generation_name_;
	int				key_size_;
};
CP_REGISTER_OFFICE_ELEMENT2(manifest_start_key_generation);

}
}
