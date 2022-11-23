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
