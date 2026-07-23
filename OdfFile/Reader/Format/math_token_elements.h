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

#include "math_elements.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {

class math_mi : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMI;

	virtual void oox_convert(oox::math_context & Context);

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
private:
    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text			(const std::wstring & Text);

	odf_types::common_math_style_attlist	common_attlist_;
   
	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mi);
CP_REGISTER_OFFICE_ELEMENT3(math_mi);
//--------------------------------------------------------------------
class math_mo : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMO;

	virtual void oox_convert(oox::math_context & Context);

	virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	_CP_OPT(bool)				fence_;
	_CP_OPT(bool)				stretchy_;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	odf_types::common_math_style_attlist	common_attlist_;

	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mo);
CP_REGISTER_OFFICE_ELEMENT3(math_mo);
//--------------------------------------------------------------------
class math_mn : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMN;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	odf_types::common_math_style_attlist	common_attlist_;

	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mn);
CP_REGISTER_OFFICE_ELEMENT3(math_mn);
//--------------------------------------------------------------------
class math_mtext : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMText;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	odf_types::common_math_style_attlist	common_attlist_;

	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mtext);
CP_REGISTER_OFFICE_ELEMENT3(math_mtext);
//--------------------------------------------------------------------
class math_mspace : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSpace;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	odf_types::common_math_style_attlist	common_attlist_;

	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mspace);
CP_REGISTER_OFFICE_ELEMENT3(math_mspace);
//--------------------------------------------------------------------
class math_ms : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMS;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

 	odf_types::common_math_style_attlist	common_attlist_;

	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_ms);
CP_REGISTER_OFFICE_ELEMENT3(math_ms);
//--------------------------------------------------------------------
class math_mglyph : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMGlyph;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

	odf_types::common_math_style_attlist	common_attlist_;

	office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mglyph);
CP_REGISTER_OFFICE_ELEMENT3(math_mglyph);
//--------------------------------------------------------------------
}
}
