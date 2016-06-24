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


#include "color.h"
#include "style_ref.h"
#include "iconset_type.h"
#include "calcext_type.h"

namespace cpdoccore { 
namespace odf_writer {


class calcext_data_bar_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

	_CP_OPT(odf_types::color) calcext_axis_color_;
	_CP_OPT(odf_types::color) calcext_positive_color_;
	_CP_OPT(odf_types::color) calcext_negative_color_;
};

class calcext_condition_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

	_CP_OPT(std::wstring)	calcext_base_cell_address_;
	_CP_OPT(odf_types::style_ref)		calcext_apply_style_name_;
	_CP_OPT(std::wstring)	calcext_value_;

};
class calcext_icon_set_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

	_CP_OPT(odf_types::iconset_type) calcext_icon_set_type_;

};

class calcext_date_is_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

	_CP_OPT(odf_types::style_ref)		calcext_style_;
	_CP_OPT(std::wstring)	calcext_date_;

};
//////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief  calcext:color-scale-entry
class calcext_color_scale_entry : public office_element_impl<calcext_color_scale_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextColorScaleEntry;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::color)			calcext_color_;
	_CP_OPT(std::wstring)	calcext_value_;
	_CP_OPT(odf_types::calcext_type)	calcext_type_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_color_scale_entry);

/// \brief  calcext:formatting-entry
class calcext_formatting_entry : public office_element_impl<calcext_formatting_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextFormattingEntry;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring) calcext_value_;
	_CP_OPT(odf_types::calcext_type) calcext_type_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_formatting_entry);

/// \brief  calcext:icon-set
class calcext_icon_set : public office_element_impl<calcext_icon_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextIconSet;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	calcext_icon_set_attr calcext_icon_set_attr_;

private:
    office_element_ptr_array content_;//entries

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_icon_set);

/// \brief  calcext:data-bar
class calcext_data_bar: public office_element_impl<calcext_data_bar>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextDataBar;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	calcext_data_bar_attr calcext_data_bar_attr_;

private:
    office_element_ptr_array content_;//entries
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_data_bar)

/// \brief  calcext:color-scale
class calcext_color_scale: public office_element_impl<calcext_color_scale>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextColorScale;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

private:
    office_element_ptr_array content_;//color_scale_entries
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_color_scale)

/// \brief  calcext:date-is
class calcext_date_is: public office_element_impl<calcext_date_is>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextDateIs;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	calcext_date_is_attr calcext_date_is_attr_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_date_is)

/// \brief  calcext:condition
class calcext_condition: public office_element_impl<calcext_condition>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextCondition;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	calcext_condition_attr calcext_condition_attr_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_condition)

/// \brief  calcext:conditional-format
class calcext_conditional_format: public office_element_impl<calcext_conditional_format>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextConditionalFormat;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	_CP_OPT(std::wstring) calcext_target_range_address_;

private:
    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_conditional_format)

/// \brief  calcext:conditional-formats
class calcext_conditional_formats: public office_element_impl<calcext_conditional_formats>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextConditionalFormats;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	

private:
    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_conditional_formats)
}
}
