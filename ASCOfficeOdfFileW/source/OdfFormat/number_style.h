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
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "common_attlists.h"

namespace cpdoccore { 
namespace odf_writer {

class number_style_base : public office_element
{
public:
   
    CPDOCCORE_DEFINE_VISITABLE()
    
    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);
	
	virtual void serialize(std::wostream & strm);
	void serialize(CP_ATTR_NODE);

    std::wstring	get_style_name() const;
	void			set_style_name(std::wstring & name);

    odf_types::common_data_style_attlist common_data_style_attlist_;
   
	office_element_ptr style_text_properties_;
    office_element_ptr_array content_;
    office_element_ptr_array style_map_;
};

/// \brief  number:number-style
class number_number_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberNumberStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_
	
	virtual void serialize(std::wostream & strm);

};

CP_REGISTER_OFFICE_ELEMENT2(number_number_style)

/// \brief  number:currency-style
class number_currency_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberCurrencyStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_

	virtual void serialize(std::wostream & strm);
};

CP_REGISTER_OFFICE_ELEMENT2(number_currency_style)

/// \brief  number:text-style
class number_text_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberTextStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_

	virtual void serialize(std::wostream & strm);
};

CP_REGISTER_OFFICE_ELEMENT2(number_text_style)


/// \brief  number:date-style

class number_date_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberDataStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_

	virtual void serialize(std::wostream & strm);

	_CP_OPT(bool) number_automatic_order_;
};

CP_REGISTER_OFFICE_ELEMENT2(number_date_style)


/// \brief  number:percentage-style

class number_percentage_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberPercentageStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_

	virtual void serialize(std::wostream & strm);
};

CP_REGISTER_OFFICE_ELEMENT2(number_percentage_style)

/// \brief  number:time-style
class number_time_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberTimeStyle;
    CPDOCCORE_DEFINE_VISITABLE()
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_

	_CP_OPT(bool) number_automatic_order_;

	virtual void serialize(std::wostream & strm);
};

CP_REGISTER_OFFICE_ELEMENT2(number_time_style)

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

/// \brief  number:text
class number_text : public office_element_impl<number_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberText;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){}

    virtual void serialize(std::wostream & strm);

    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array text_;
};

CP_REGISTER_OFFICE_ELEMENT2(number_text)

/// \brief  number:number
class number_number : public office_element_impl<number_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberNumber;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	// number-number-attlist
    _CP_OPT(std::wstring) number_decimal_replacement_;
    _CP_OPT(double) number_display_factor_;

    // common-decimal-places-attlist
    _CP_OPT(int) number_decimal_places_;

    // common-number-attlist
    _CP_OPT(int) number_min_integer_digits_;
    _CP_OPT(odf_types::Bool) number_grouping_;
    
    office_element_ptr_array number_embedded_text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(number_number)

/// \brief  number:embedded-text
class number_embedded_text : public office_element_impl<number_embedded_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberEmbeddedText;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    virtual void add_text(const std::wstring & Text);

	_CP_OPT(int) number_position_;
    office_element_ptr_array text_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_embedded_text)

/// \brief  number:scientific-number
class number_scientific_number : public office_element_impl<number_scientific_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberScientificNumber;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    // number-scientific-number-attlist
    _CP_OPT(int) number_min_exponent_digits_;

    // common-decimal-places-attlist
    _CP_OPT(int) number_decimal_places_;

    // common-number-attlist
    _CP_OPT(int) number_min_integer_digits_;
    _CP_OPT(bool) number_grouping_;        
    
};

CP_REGISTER_OFFICE_ELEMENT2(number_scientific_number)

/// \brief  number:currency-symbol
class number_currency_symbol : public office_element_impl<number_currency_symbol>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberCurrencySymbol;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    virtual void add_text(const std::wstring & Text);

    _CP_OPT(std::wstring) number_language_;
    _CP_OPT(std::wstring) number_country_;

    std::vector<std::wstring> text_;
   
};

CP_REGISTER_OFFICE_ELEMENT2(number_currency_symbol)

/// \brief  number:text-content
class number_text_content : public office_element_impl<number_text_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberTextContent;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}

    virtual void serialize(std::wostream & strm);

    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(number_text_content)

/// \class  number_day_of_week
/// \brief  number:day-of-week
///         number-day-of-week
class number_day_of_week : public office_element_impl<number_day_of_week>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberDayOfWeek;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_day_of_week)


/// \brief  number:quarter
class number_quarter : public office_element_impl<number_quarter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberQuarter;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_quarter)

/// \brief  number:day

class number_day : public office_element_impl<number_day>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberDay;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_day)

/// \brief  number:month
class number_month : public office_element_impl<number_month>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberMonth;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(odf_types::Bool) number_textual_;
    _CP_OPT(bool) number_possessive_form_;
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_month)


/// \brief  number:year
class number_year : public office_element_impl<number_year>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberYear;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_year)

/// \brief  number:hours
class number_hours : public office_element_impl<number_hours>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberHours;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_hours);

/// \brief  number:minutes
class number_minutes : public office_element_impl<number_minutes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberMinutes;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_minutes)

/// \brief  number:seconds
class number_seconds : public office_element_impl<number_seconds>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberSeconds;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(int) number_decimal_places_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_seconds)

/// \brief  number:am-pm
class number_am_pm : public office_element_impl<number_am_pm>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberAmPm;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);


};

CP_REGISTER_OFFICE_ELEMENT2(number_am_pm)

/// \brief  number:fraction
class number_fraction : public office_element_impl<number_fraction>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberFraction;
    CPDOCCORE_DEFINE_VISITABLE()

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name){CP_NOT_APPLICABLE_ELM();}
	virtual void serialize(std::wostream & strm);

    _CP_OPT(int) number_min_integer_digits_;
    _CP_OPT(bool) number_grouping_;
    _CP_OPT(std::wstring) number_calendar_;
    
    _CP_OPT(int) number_denominator_value_;
    _CP_OPT(int) number_min_numerator_digits_;
    _CP_OPT(int) number_min_denominator_digits_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_fraction)

}
}
