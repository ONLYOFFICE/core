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
#include <xml/xmlelement.h>
#include <xml/nodetype.h>
#include <xml/simple_xml_writer.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/presentationclass.h"


namespace cpdoccore { 
namespace odf_writer {


class draw_page_attr
{
public:
    void serialize(CP_ATTR_NODE);

	_CP_OPT(std::wstring)		draw_name_;
	_CP_OPT(std::wstring)		draw_id_;
	_CP_OPT(std::wstring)		draw_style_name_;
	_CP_OPT(std::wstring)		draw_master_page_name_;

	_CP_OPT(std::wstring)		style_page_layout_name_;

	_CP_OPT(std::wstring)		presentation_use_footer_name_;
	_CP_OPT(std::wstring)		presentation_use_date_time_name_;
	_CP_OPT(std::wstring)		presentation_page_layout_name_;
};

class draw_page : public office_element_impl<draw_page>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawPage;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array	content_;
	office_element_ptr			animation_;

	draw_page_attr				attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_page);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:footer-decl
class presentation_footer_decl : public office_element_impl<presentation_footer_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typePresentationFooterDecl;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream){}

	_CP_OPT(std::wstring)	presentation_name_;
	std::wstring			text_;

};
CP_REGISTER_OFFICE_ELEMENT2(presentation_footer_decl);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:date-time-decl
class presentation_date_time_decl : public office_element_impl<presentation_date_time_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typePresentationDateTimeDecl;
    

	_CP_OPT(std::wstring)	presentation_name_;
	_CP_OPT(std::wstring)	presentation_source_;
	_CP_OPT(std::wstring)	style_data_style_name_;

	std::wstring			text_;
    
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}
	virtual void serialize(std::wostream & _Wostream){}

};
CP_REGISTER_OFFICE_ELEMENT2(presentation_date_time_decl);
//---------------------------------------------------------------------
class presentation_notes : public office_element_impl<presentation_notes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typePresentationNotes;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array	content_;
	draw_page_attr				attlist_;
};

CP_REGISTER_OFFICE_ELEMENT2(presentation_notes);

}
}
