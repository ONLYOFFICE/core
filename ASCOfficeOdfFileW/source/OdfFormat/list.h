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
#pragma once

#include <cpdoccore/CPOptional.h>
#include "office_elements_create.h"

namespace cpdoccore {
namespace odf_writer {

// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_number : public office_element_impl<text_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNumber;
    CPDOCCORE_DEFINE_VISITABLE()

public:
    text_number() {}

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    ::std::wstring string_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_number);


// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_list_item;
typedef shared_ptr<text_list_item>::Type text_list_item_ptr;
typedef ::std::vector<text_list_item_ptr> text_list_item_ptr_array;

class text_list_item : public office_element_impl<text_list_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListItem;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

public:
    text_list_item() {} 

    virtual void add_text(const std::wstring & Text) {} ;

    _CP_OPT(unsigned int)		text_start_value_;

    office_element_ptr          text_number_;
    office_element_ptr_array    content_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_list_item);

// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_list_header;
typedef shared_ptr<text_list_header>::Type text_list_header_ptr;

class text_list_header : public office_element_impl<text_list_header>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListHeader;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

public:
    text_list_header() {} 

    virtual void add_text(const std::wstring & Text) {} ;

private:
    office_element_ptr          text_number_;
    office_element_ptr_array    content_;


};

CP_REGISTER_OFFICE_ELEMENT2(text_list_header);


}
}

