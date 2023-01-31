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
