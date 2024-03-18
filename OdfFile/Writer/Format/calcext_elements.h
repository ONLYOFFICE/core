/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <iosfwd>
#include <vector>
#include <CPOptional.h>
#include <xml/nodetype.h>
#include <xml/simple_xml_writer.h>

#include "office_elements_create.h"

#include "../DataTypes/color.h"
#include "../DataTypes/iconset_type.h"
#include "../DataTypes/calcext_type.h"
#include "../DataTypes/timeperiod.h"
#include "../DataTypes/sparklines.h"
#include "../DataTypes/length.h"
#include "../DataTypes/bool.h"

namespace cpdoccore { 
namespace odf_writer {


class calcext_data_bar_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

    _CP_OPT(odf_types::color) axis_color_;
	_CP_OPT(odf_types::color) positive_color_;
	_CP_OPT(odf_types::color) negative_color_;
    _CP_OPT(std::wstring) axis_position_;
    _CP_OPT(odf_types::Bool) gradient_;
    _CP_OPT(unsigned int) min_length_;
    _CP_OPT(unsigned int) max_length_;
};

class calcext_condition_attr
{
public:
   	void serialize(CP_ATTR_NODE);   

	_CP_OPT(std::wstring)	calcext_base_cell_address_;
	_CP_OPT(std::wstring)	calcext_apply_style_name_;
	_CP_OPT(std::wstring)	calcext_value_;
    _CP_OPT(int)	        loext_stdDev_;
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

	_CP_OPT(std::wstring)	calcext_style_;
	_CP_OPT(odf_types::time_period)	calcext_date_;

};
class calcext_sparkline_group_attr
{
public:
    void serialize(CP_ATTR_NODE);

    _CP_OPT(std::wstring) id_;
    _CP_OPT(std::wstring) ref_;
    _CP_OPT(odf_types::sparkline_type) type_;
    _CP_OPT(odf_types::length) line_width_;
    _CP_OPT(odf_types::Bool) first_;
    _CP_OPT(odf_types::Bool) last_;
    _CP_OPT(odf_types::Bool) high_;
    _CP_OPT(odf_types::Bool) low_;
    _CP_OPT(odf_types::Bool) display_hidden_;
    _CP_OPT(odf_types::Bool) right_to_left_;
    _CP_OPT(odf_types::Bool) display_x_axis_;
    _CP_OPT(odf_types::Bool) date_axis_;
    _CP_OPT(odf_types::Bool) negative_;
    _CP_OPT(odf_types::Bool) markers_;

    _CP_OPT(odf_types::sparkline_empty) display_empty_cells_as_;
    _CP_OPT(odf_types::sparkline_axis_type) min_axis_type_;
    _CP_OPT(odf_types::sparkline_axis_type) max_axis_type_;
    _CP_OPT(double) manual_min_;
    _CP_OPT(double) manual_max_;

    _CP_OPT(odf_types::color) color_series_;
    _CP_OPT(odf_types::color) color_negative_;
    _CP_OPT(odf_types::color) color_axis_;
    _CP_OPT(odf_types::color) color_markers_;
    _CP_OPT(odf_types::color) color_first_;
    _CP_OPT(odf_types::color) color_last_;
    _CP_OPT(odf_types::color) color_high_;
    _CP_OPT(odf_types::color) color_low_;
};
//////////////////////////////////////////////////////////////////////////////////////////////////

///  calcext:color-scale-entry
class calcext_color_scale_entry : public office_element_impl<calcext_color_scale_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextColorScaleEntry;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::color)			calcext_color_;
	_CP_OPT(std::wstring)				calcext_value_;
	_CP_OPT(odf_types::calcext_type)	calcext_type_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_color_scale_entry);

///  calcext:formatting-entry
class calcext_formatting_entry : public office_element_impl<calcext_formatting_entry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextFormattingEntry;    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)				value_;
	_CP_OPT(odf_types::calcext_type)    type_;
    _CP_OPT(odf_types::Bool)            show_value_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_formatting_entry);

///  calcext:icon-set
class calcext_icon_set : public office_element_impl<calcext_icon_set>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextIconSet;    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	calcext_icon_set_attr		attr_;
    _CP_OPT(odf_types::Bool)    show_value_;
    office_element_ptr_array	content_;//entries

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_icon_set);

///  calcext:data-bar
class calcext_data_bar: public office_element_impl<calcext_data_bar>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextDataBar;    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	calcext_data_bar_attr		attr_;
    _CP_OPT(odf_types::Bool)    show_value_;
    office_element_ptr_array	content_;//entries
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_data_bar)

///  calcext:color-scale
class calcext_color_scale: public office_element_impl<calcext_color_scale>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextColorScale;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;	//color_scale_entries
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_color_scale)

///  calcext:date-is
class calcext_date_is: public office_element_impl<calcext_date_is>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextDateIs;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	calcext_date_is_attr attr_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_date_is)

///  calcext:condition
class calcext_condition: public office_element_impl<calcext_condition>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextCondition;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_child_element( const office_element_ptr & child_element){}

	virtual void serialize(std::wostream & _Wostream);

	calcext_condition_attr attr_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_condition)

///  calcext:conditional-format
class calcext_conditional_format: public office_element_impl<calcext_conditional_format>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextConditionalFormat;    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	
	_CP_OPT(std::wstring) calcext_target_range_address_;

private:
    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_conditional_format)

///  calcext:conditional-formats
class calcext_conditional_formats: public office_element_impl<calcext_conditional_formats>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeCalcextConditionalFormats;
    

	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);
	

private:
    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(calcext_conditional_formats)

//  calcext:sparkline-groups
class calcext_sparkline_groups : public office_element_impl<calcext_sparkline_groups>
{
public:
    static const wchar_t* ns;
    static const wchar_t* name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextSparklineGroups;
  
    virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name);
    virtual void add_child_element(const office_element_ptr& child_element);

    virtual void serialize(std::wostream& _Wostream);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_sparkline_groups)

//  calcext:sparkline-group
class calcext_sparkline_group : public office_element_impl<calcext_sparkline_group>
{
public:
    static const wchar_t* ns;
    static const wchar_t* name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextSparklineGroup;

    virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name);
    virtual void add_child_element(const office_element_ptr& child_element);

    virtual void serialize(std::wostream& _Wostream);

    calcext_sparkline_group_attr attr_;
    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_sparkline_group)

//  calcext:sparklines
class calcext_sparklines : public office_element_impl<calcext_sparklines>
{
public:
    static const wchar_t* ns;
    static const wchar_t* name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextSparklines;

    virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name);
    virtual void add_child_element(const office_element_ptr& child_element);

    virtual void serialize(std::wostream& _Wostream);

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_sparklines)

//  calcext:sparkline
class calcext_sparkline : public office_element_impl<calcext_sparkline>
{
public:
    static const wchar_t* ns;
    static const wchar_t* name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeCalcextSparklines;

    virtual void create_child_element(const std::wstring& Ns, const std::wstring& Name) {}
    virtual void add_child_element(const office_element_ptr& child_element) {}

    virtual void serialize(std::wostream& _Wostream);

    _CP_OPT(std::wstring) data_range_;
    _CP_OPT(std::wstring) cell_address_;
};
CP_REGISTER_OFFICE_ELEMENT2(calcext_sparkline)

}
}
