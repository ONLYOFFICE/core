/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

//  office:meta
class office_meta : public office_element_impl<office_meta>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeMeta;
    CPDOCCORE_DEFINE_VISITABLE();

    office_element_ptr_array	meta_user_defined_;
    office_element_ptr			meta_generator_;
    office_element_ptr			meta_document_statistic_;
	office_element_ptr			meta_creation_date_;
	office_element_ptr			meta_template_;
	office_element_ptr			meta_printed_by_;
	office_element_ptr			meta_print_date_;
	office_element_ptr			meta_keyword_;
	office_element_ptr			meta_initial_creator_;
	office_element_ptr			meta_hyperlinkbehaviour_;
	office_element_ptr			meta_editing_duration_;
	office_element_ptr			meta_editing_cycles_;
	office_element_ptr			meta_auto_reloads_;
	office_element_ptr			dc_date_;
	office_element_ptr			dc_description_;
	office_element_ptr			dc_language_;
	office_element_ptr			dc_subject_;
	office_element_ptr			dc_title_;
	office_element_ptr			dc_creator_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
	virtual void add_text(const std::wstring & Text){} 
};
CP_REGISTER_OFFICE_ELEMENT2(office_meta);

//  base
class meta_common : public office_element_impl<meta_common>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeOfficeMetaCommon;
	CPDOCCORE_DEFINE_VISITABLE();

	std::wstring content_;

private:
	virtual void add_attributes(const xml::attributes_wc_ptr & Attributes) {}
	virtual void add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
	virtual void add_text(const std::wstring & Text);
};
//  meta:initial-creator
class meta_initial_creator : public meta_common
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const ElementType type = typeOfficeMetaInitialCreator;
};
CP_REGISTER_OFFICE_ELEMENT2(meta_initial_creator);
//  meta:keyword
class meta_keyword : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaKeyword;
};
CP_REGISTER_OFFICE_ELEMENT2(meta_keyword);
//  meta:creation-date
class meta_creation_date : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaCreationDate;
};
CP_REGISTER_OFFICE_ELEMENT2(meta_creation_date);
//  meta:generator
class meta_generator : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaGenerator;
};
CP_REGISTER_OFFICE_ELEMENT2(meta_generator);
//  meta:template
class meta_template : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaTemplate;
};
CP_REGISTER_OFFICE_ELEMENT2(meta_template);
//  meta:editing-cycles
class meta_editing_cycles : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeOfficeMetaEditingCycles;
};
CP_REGISTER_OFFICE_ELEMENT2(meta_editing_cycles);
//--------------------------------------------------------------------------
//  dc:creator
class dc_creator : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcCreator;
};
CP_REGISTER_OFFICE_ELEMENT2(dc_creator);
//  dc:description
class dc_description : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcDescription;
};
CP_REGISTER_OFFICE_ELEMENT2(dc_description);
//  dc:language
class dc_language : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcLanguage;
};
CP_REGISTER_OFFICE_ELEMENT2(dc_language);
//  dc:date
class dc_date : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcDate;
};
CP_REGISTER_OFFICE_ELEMENT2(dc_date);
//  dc:title
class dc_title : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcTitle;
};
CP_REGISTER_OFFICE_ELEMENT2(dc_title);
//  dc:subject
class dc_subject : public meta_common
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeDcSubject;
};
CP_REGISTER_OFFICE_ELEMENT2(dc_subject);
//-----------------------------------------------------------------------------
//  meta:document-statistic
class meta_document_statistic : public office_element_impl<meta_document_statistic>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeMetaDocumentStatistic;
    CPDOCCORE_DEFINE_VISITABLE();

	_CP_OPT(int)	image_count_;
	_CP_OPT(int)	object_count_;
	_CP_OPT(int)	page_count_;
	_CP_OPT(int)	paragraph_count_;
	_CP_OPT(int)	word_count_;
	_CP_OPT(int)	character_count_;
	_CP_OPT(int)	non_whitespace_character_count_;
	_CP_OPT(int)	draw_count_;
	_CP_OPT(int)	ole_object_count_;
	_CP_OPT(int)	frame_count_;
	_CP_OPT(int)	table_count_;
	_CP_OPT(int)	row_count_;
	_CP_OPT(int)	cell_count_;
	_CP_OPT(int)	sentence_count_;
	_CP_OPT(int)	syllable_count_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_text(const std::wstring & Text){}
};
CP_REGISTER_OFFICE_ELEMENT2(meta_document_statistic);

//  meta:user-defined
class meta_user_defined : public office_element_impl<meta_user_defined>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeMetaUserDefined;
    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring meta_name_;
 	std::wstring content_;
	_CP_OPT(odf_types::office_value_type) meta_value_type_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_text(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(meta_user_defined);


}
}