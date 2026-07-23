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
#include <vector>
#include <CPOptional.h>
#include <xml/nodetype.h>
#include <xml/simple_xml_writer.h>

#include "office_elements_create.h"

#include "../../DataTypes/bool.h"
#include "../../DataTypes/length.h"

namespace cpdoccore { 
namespace odf_writer {
	
class office_annotation_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

	_CP_OPT(odf_types::length)	svg_y_;
	_CP_OPT(odf_types::length)	svg_x_;
 	_CP_OPT(odf_types::length)	svg_width_;
	_CP_OPT(odf_types::length)	svg_height_;

	_CP_OPT(odf_types::length)	caption_point_y_;
	_CP_OPT(odf_types::length)	caption_point_x_;

	_CP_OPT(std::wstring)		name_;
	_CP_OPT(odf_types::Bool)	display_;

	_CP_OPT(std::wstring)		draw_text_style_name_;
	_CP_OPT(std::wstring)		draw_style_name_;
};

class office_annotation : public office_element_impl<office_annotation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeOfficeAnnotation;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	office_annotation_attr attr_;

private:
    office_element_ptr_array content_;

    office_element_ptr dc_date_;
    office_element_ptr dc_creator_;


};
CP_REGISTER_OFFICE_ELEMENT2(office_annotation);

class office_annotation_end : public office_element_impl<office_annotation_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeOfficeAnnotationEnd;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);
	
	office_annotation_attr attr_;
//
//private:
//    office_element_ptr_array content_;
//
//    office_element_ptr dc_date_;
//    office_element_ptr dc_creator_;


};
CP_REGISTER_OFFICE_ELEMENT2(office_annotation_end);

//----------------------------------------------------------------------------------------------------------
class officeooo_annotation : public office_element_impl<officeooo_annotation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type		= typeOfficeAnnotation;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

private:
	office_annotation_attr		attr_;
	office_element_ptr_array	content_;

    office_element_ptr			dc_date_;
    office_element_ptr			dc_creator_;
};
CP_REGISTER_OFFICE_ELEMENT2(officeooo_annotation);
}
}
