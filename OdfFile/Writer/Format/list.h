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

#include <CPOptional.h>
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

    static const ElementType type = typeTextNumber;
    

public:
    text_number() {}

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    std::wstring string_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_number);


// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_list_item : public office_element_impl<text_list_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextListItem;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
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

typedef shared_ptr<text_list_item>::Type	text_list_item_ptr;
typedef std::vector<text_list_item_ptr>		text_list_item_ptr_array;

// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_list_header;
typedef shared_ptr<text_list_header>::Type text_list_header_ptr;

class text_list_header : public office_element_impl<text_list_header>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextListHeader;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
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

