/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "office_document.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include "../docx/docx_conversion_context.h"

namespace cpdoccore { 
namespace odf_reader {



std::wostream & office_document_base::text_to_stream(::std::wostream & _Wostream) const
{
    office_body_->text_to_stream(_Wostream);
    return _Wostream;    
}

office_document_base::office_document_base()
{}

void office_document_base::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"office:mimetype", office_mimetype_, std::wstring(L""));
    CP_APPLY_ATTR(L"office:version", office_version_, std::wstring(L""));
}

void office_document_base::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"office", L"body")
        CP_CREATE_ELEMENT(office_body_);
    else if CP_CHECK_NAME(L"office", L"automatic-styles")
        CP_CREATE_ELEMENT(office_automatic_styles_);
    else if CP_CHECK_NAME(L"office", L"styles")
        CP_CREATE_ELEMENT(office_styles_);
    else if CP_CHECK_NAME(L"office", L"font-face-decls")
        CP_CREATE_ELEMENT(office_font_face_decls_);
    else if CP_CHECK_NAME(L"office", L"master-styles")
        CP_CREATE_ELEMENT(office_master_styles_);
    else if CP_CHECK_NAME(L"office", L"scripts")
        CP_CREATE_ELEMENT(office_scripts_);
    else if CP_CHECK_NAME(L"office", L"settings")
        CP_CREATE_ELEMENT(office_settings_);
    else if CP_CHECK_NAME(L"manifest", L"file-entry")
        CP_CREATE_ELEMENT(manifests_);
	else
        CP_NOT_APPLICABLE_ELM();
}

void office_document_base::add_text(const std::wstring & Text)
{
}

void office_document_base::docx_convert(oox::docx_conversion_context & Context) 
{
    if (office_body_)
        office_body_->docx_convert(Context);
}

void office_document_base::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    if (office_body_)
        office_body_->xlsx_convert(Context);
}
void office_document_base::pptx_convert(oox::pptx_conversion_context & Context)
{
    if (office_body_)
        office_body_->pptx_convert(Context);
}

// office:document
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_document::ns = L"office";
const wchar_t * office_document::name = L"document";

// office:document-content
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_document_content::ns = L"office";
const wchar_t * office_document_content::name = L"document-content";

// office:document-styles
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_document_styles::ns = L"office";
const wchar_t * office_document_styles::name = L"document-styles";

// office:document-meta
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_document_meta::ns = L"office";
const wchar_t * office_document_meta::name = L"document-meta";

// office:document-settings
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_document_settings::ns = L"office";
const wchar_t * office_document_settings::name = L"document-settings";

// manifest:manifest
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_manifest::ns = L"manifest";
const wchar_t * manifest_manifest::name = L"manifest";

// manifest:file-entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_entry::ns = L"manifest";
const wchar_t * manifest_entry::name = L"file-entry";

void manifest_entry::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"manifest:media-type", media_type_, std::wstring(L""));
    CP_APPLY_ATTR(L"manifest:full-path", full_path_, std::wstring(L""));
}
void manifest_entry::add_child_element(cpdoccore::xml::sax *Reader, const std::wstring &Ns, const std::wstring &Name)
{
	if CP_CHECK_NAME(L"manifest", L"encryption-data")
        CP_CREATE_ELEMENT(encryption_);
}
// manifest:file-entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * manifest_encryption_data::ns = L"manifest";
const wchar_t * manifest_encryption_data::name = L"encryption-data";

void manifest_encryption_data::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"manifest:checksum", manifest_checksum_, std::wstring(L""));
    CP_APPLY_ATTR(L"manifest:checksum-type", manifest_checksum_type_, std::wstring(L""));

}
}
}

