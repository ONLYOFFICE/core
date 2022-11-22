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

namespace cpdoccore { 
namespace odf_reader {

class math_msub : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSub;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msub);
CP_REGISTER_OFFICE_ELEMENT3(math_msub);
//--------------------------------------------------------------------
class math_msup : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSup;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msup);
CP_REGISTER_OFFICE_ELEMENT3(math_msup);

//--------------------------------------------------------------------
class math_msubsup : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSubSup;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msubsup);
CP_REGISTER_OFFICE_ELEMENT3(math_msubsup);

//--------------------------------------------------------------------
class math_none : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMNone;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_none);
CP_REGISTER_OFFICE_ELEMENT3(math_none);

//--------------------------------------------------------------------
class math_mprescripts : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMPreScripts;

	virtual void oox_convert(oox::math_context & Context);

private:
	virtual void add_attributes(const xml::attributes_wc_ptr & Attributes) {}
	virtual void add_child_element(xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
};

CP_REGISTER_OFFICE_ELEMENT2(math_mprescripts);
CP_REGISTER_OFFICE_ELEMENT3(math_mprescripts);

//--------------------------------------------------------------------
class math_mmultiscripts : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMMultiScripts;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;
	office_element_ptr_array pre_content_;
	bool bSeparate = false;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mmultiscripts);
CP_REGISTER_OFFICE_ELEMENT3(math_mmultiscripts);

//--------------------------------------------------------------------
class math_munderover : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMUnderOver;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_; //3 exact - base, under, over
};

CP_REGISTER_OFFICE_ELEMENT2(math_munderover);
CP_REGISTER_OFFICE_ELEMENT3(math_munderover);

//--------------------------------------------------------------------
class math_mover : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMOver;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mover);
CP_REGISTER_OFFICE_ELEMENT3(math_mover);

//--------------------------------------------------------------------
class math_munder : public office_math_element
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMUnder;

	virtual void oox_convert(oox::math_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_munder);
CP_REGISTER_OFFICE_ELEMENT3(math_munder);
//--------------------------------------------------------------------
}
}
