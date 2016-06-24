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

#include <iosfwd>
#include <vector>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/nodetype.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "office_elements_create.h"

#include "bool.h"
#include "length.h"
#include "style_ref.h"

namespace cpdoccore { 
namespace odf_writer {


class office_annotation_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

public:
	_CP_OPT(odf_types::length) svg_y_;
	_CP_OPT(odf_types::length) svg_x_;
 	_CP_OPT(odf_types::length) svg_width_;
	_CP_OPT(odf_types::length) svg_height_;

	_CP_OPT(odf_types::length) caption_point_y_;
	_CP_OPT(odf_types::length) caption_point_x_;

	_CP_OPT(odf_types::Bool) display_;

	 _CP_OPT(odf_types::style_ref) draw_text_style_name_;
	 _CP_OPT(odf_types::style_ref) draw_style_name_;

	 _CP_OPT(std::wstring) name_;
};

/// \brief  dc:date
class dc_date : public office_element_impl<dc_date>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDcDate;
    CPDOCCORE_DEFINE_VISITABLE()
	
	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

	std::wstring content_;

};
CP_REGISTER_OFFICE_ELEMENT2(dc_date);

/// \brief  dc:creator
class dc_creator : public office_element_impl<dc_creator>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDcCreator;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

	std::wstring content_;

};
CP_REGISTER_OFFICE_ELEMENT2(dc_creator);

/// \brief  office:annotation 
class office_annotation : public office_element_impl<office_annotation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeAnnotation;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	office_annotation_attr office_annotation_attr_;

private:
    office_element_ptr_array content_;

    office_element_ptr dc_date_;
    office_element_ptr dc_creator_;


};
CP_REGISTER_OFFICE_ELEMENT2(office_annotation);

/// \brief  office:annotation-end
class office_annotation_end : public office_element_impl<office_annotation_end>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeAnnotationEnd;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);
	
	office_annotation_attr office_annotation_attr_;
//
//private:
//    office_element_ptr_array content_;
//
//    office_element_ptr dc_date_;
//    office_element_ptr dc_creator_;


};
CP_REGISTER_OFFICE_ELEMENT2(office_annotation_end);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//officeooo:annotation
class officeooo_annotation : public office_element_impl<officeooo_annotation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeAnnotation;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);


private:
    office_element_ptr_array content_;
	office_annotation_attr office_annotation_attr_;

    office_element_ptr dc_date_;
    office_element_ptr dc_creator_;


};
CP_REGISTER_OFFICE_ELEMENT2(officeooo_annotation);
}
}
