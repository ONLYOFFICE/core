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

#include "number_style.h"

#include "odf_document.h"

#include <xml/xmlchar.h>
#include <xml/utils.h>

#include "../../Common/CPColorUtils.h"

#include "serialize_elements.h"
#include "paragraph_elements.h"
#include "style_text_properties.h"
#include "style_map.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

std::wstring number_style_base::get_style_name() const
{
    return common_data_style_attlist_.style_name_.get_value_or(L"");
}

void number_style_base::oox_convert_impl(oox::num_format_context & Context) 
{
    std::wstring style_name = common_data_style_attlist_.style_name_.get_value_or(L"");

    Context.start_format(style_name);

    if (style_text_properties_)
    {
        if (style_text_properties * textProp = dynamic_cast<style_text_properties*>(style_text_properties_.get()))
        {
            if (textProp->content_.fo_color_)
            {
                int r = 0, g = 0, b = 0;
                if (HEXStringToRGB(textProp->content_.fo_color_->get_hex_value(), r, g, b))
                {
                    std::wostream & strm = Context.output();
                    strm << L"[" << RGBToString(r, g, b) << L"]";
                }
            }
        }
    }

	ElementType type = this->get_type();

	switch(type)
	{
	case typeNumberCurrencyStyle:	Context.type(odf_types::office_value_type::Currency);		break;
	case typeNumberPercentageStyle:	Context.type(odf_types::office_value_type::Percentage);		break;
	case typeNumberDataStyle:		Context.type(odf_types::office_value_type::Date);			break;
	case typeNumberTimeStyle:		Context.type(odf_types::office_value_type::Time);			break;
	}

	for (size_t i = 0; i < content_.size(); i++)
    {
		number_style_base	*number_style_		= dynamic_cast<number_style_base *>	(content_[i].get());
  		number_element		*number_element_	= dynamic_cast<number_element *>	(content_[i].get());
		
		if (number_style_)
		{
			Context.type(odf_types::office_value_type::Float);
			number_style_->oox_convert(Context);
		}
		if (number_element_)
		{
			number_element_->oox_convert(Context);
		}
    }
    Context.end_format();
}

void number_style_base::oox_convert(oox::num_format_context & Context)
{
    if (!style_map_.empty())
    {
		for (size_t i = 0; i < style_map_.size(); i++)
		{
            if (const style_map * styleMap = dynamic_cast<const style_map *>(style_map_[i].get()))
            {
                const std::wstring applyStyleName	= styleMap->style_apply_style_name_;
                const std::wstring condition		= styleMap->style_condition_;

				if (office_element_ptr num_style = Context.odf_context_.numberStyles().find_by_style_name(applyStyleName))
                {
                    if (number_style_base * concrete_num_style = dynamic_cast<number_style_base *>(num_style.get()))
                    {
                        concrete_num_style->oox_convert_impl(Context);

                        const std::wstring num_format = Context.get_last_format();
                        Context.add_format(condition, num_format);
                    }
                }
            }
        }
    }

    oox_convert_impl(Context);
    const std::wstring num_format = Context.get_last_format();
    Context.add_format(L"", num_format);

}


//void number_style_base::xlsx_convert(oox::xlsx_conversion_context & Context)
//{
//    if (!style_map_.empty())
//    {
//        BOOST_FOREACH(const office_element_ptr & elm, style_map_)
//        {
//            if (const style_map * styleMap = dynamic_cast<const style_map *>(elm.get()))
//            {
//                const std::wstring applyStyleName	= styleMap->style_apply_style_name_.style_name();
//                const std::wstring condition		= styleMap->style_condition_;
//
//                odf_read_context & odfContext = Context.root()->odf_context();
//                
//				if (office_element_ptr num_style = odfContext.numberStyles().find_by_style_name(applyStyleName))
//                {
//                    if (number_style_base * concrete_num_style = dynamic_cast<number_style_base *>(num_style.get()))
//                    {
//                        concrete_num_style->oox_convert_impl(Context.get_num_format_context());
//
//                        const std::wstring num_format = Context.get_num_format_context().get_last_format();
//                        Context.get_num_format_context().add_format(condition, num_format);
//                    }
//                }
//            }
//        }
//    }
//
//    this->oox_convert_impl(Context);
//    const std::wstring num_format = Context.get_num_format_context().get_last_format();
//    Context.get_num_format_context().add_format(L"", num_format);
//
//}
//
void number_style_base::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_data_style_attlist_.add_attributes(Attributes);
}
void number_date_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"number:automatic-order", number_automatic_order_);
	
	number_style_base::add_attributes(Attributes);
}
void number_time_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"number:automatic-order", number_automatic_order_);
	
	number_style_base::add_attributes(Attributes);
}
void number_style_base::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"text-properties")
    {
        CP_CREATE_ELEMENT(style_text_properties_);
    }
    else if CP_CHECK_NAME(L"style", L"map")
    {
        CP_CREATE_ELEMENT(style_map_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);
    }
}
//-------------------------------------------------------------------
// number:number-style
const wchar_t * number_number_style::ns = L"number";
const wchar_t * number_number_style::name = L"number-style";

//-------------------------------------------------------------------
// number:boolean-style
const wchar_t * number_boolean_style::ns = L"number";
const wchar_t * number_boolean_style::name = L"boolean-style";

//-------------------------------------------------------------------
// number:text
const wchar_t * number_text::ns = L"number";
const wchar_t * number_text::name = L"text";

void number_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void number_text::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_text::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    text_.push_back( elm );
}
void number_text::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    text_.push_back( elm );
}
// number:embedded-text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_embedded_text::ns = L"number";
const wchar_t * number_embedded_text::name = L"embedded-text";

void number_embedded_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:position", number_position_);
}

void number_embedded_text::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_embedded_text::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    text_.push_back( elm );
}
void number_embedded_text::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    text_.push_back( elm );
}
void number_embedded_text::oox_convert(oox::num_format_context & Context)
{
}
// number:number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_number::ns = L"number";
const wchar_t * number_number::name = L"number";

void number_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:decimal-replacement", number_decimal_replacement_);
    CP_APPLY_ATTR(L"number:display-factor", number_display_factor_); 

    CP_APPLY_ATTR(L"number:decimal-places", number_decimal_places_);

    CP_APPLY_ATTR(L"number:min-integer-digits", number_min_integer_digits_);
    CP_APPLY_ATTR(L"number:grouping", number_grouping_);
}

void number_number::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"number", L"embedded-text")
    {
        CP_CREATE_ELEMENT(number_embedded_text_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

// number:scientific-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_scientific_number::ns = L"number";
const wchar_t * number_scientific_number::name = L"scientific-number";

void number_scientific_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:min-exponent-digits", number_min_exponent_digits_);
    CP_APPLY_ATTR(L"number:decimal-places", number_decimal_places_);

    CP_APPLY_ATTR(L"number:min-integer-digits", number_min_integer_digits_);
    CP_APPLY_ATTR(L"number:grouping", number_grouping_);
}

void number_scientific_number::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// number:currency-symbol
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_currency_symbol::ns = L"number";
const wchar_t * number_currency_symbol::name = L"currency-symbol";

void number_currency_symbol::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:language", number_language_);
    CP_APPLY_ATTR(L"number:country", number_country_);
}                      

void number_currency_symbol::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_currency_symbol::add_text(const std::wstring & Text)
{
    text_.push_back(Text);
}
void number_currency_symbol::add_space(const std::wstring & Text)
{
    text_.push_back(Text);
}
void number_currency_symbol::oox_convert(oox::num_format_context & Context)
{
    std::wostream & strm = Context.output();
    strm << L"[$";

	for (size_t i = 0; i < text_.size(); i++)
    {
        strm << XmlUtils::EncodeXmlString(text_[i]);
    }
    strm << L"]";
}


// number:currency-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_currency_style::ns = L"number";
const wchar_t * number_currency_style::name = L"currency-style";

// number:text-content
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_text_content::ns = L"number";
const wchar_t * number_text_content::name = L"text-content";

void number_text_content::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void number_text_content::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_text_content::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    text_.push_back( elm );
}
void number_text_content::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    text_.push_back( elm );
}
void number_text_content::oox_convert(oox::num_format_context & Context)
{
}

// number:text-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_text_style::ns = L"number";
const wchar_t * number_text_style::name = L"text-style";

// number:percentage-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_percentage_style::ns = L"number";
const wchar_t * number_percentage_style::name = L"percentage-style";

//////////////////////////////

namespace 
{
void format_number_number(
						std::wostream & strm,
						bool number_grouping_,
						int number_min_integer_digits_,
						_CP_OPT(int) number_decimal_places_
                        )    
{
    if (number_grouping_)
    {
        if (number_min_integer_digits_ >= 4)
            strm << std::wstring( number_min_integer_digits_ - 3, L'0');
        else
            strm << L'#';
        strm << L',';
        strm << std::wstring( (std::max)(0, 3 - number_min_integer_digits_), L'#');
        strm << std::wstring( (std::min)(3, number_min_integer_digits_), L'0');
    }
    else
    {
        if (number_min_integer_digits_ > 1 || number_decimal_places_)
            strm << std::wstring( number_min_integer_digits_, L'0');
        else
            strm << L"General";
    }

    if ((number_decimal_places_) && (*number_decimal_places_ > 0))
    {
        strm << L".";
        strm << std::wstring(*number_decimal_places_, L'0');
    }
}
}

void number_number::oox_convert(oox::num_format_context & Context)
{
    std::wostream & strm = Context.output();

    format_number_number(
        strm,
        number_grouping_.get_value_or(false),
        number_min_integer_digits_.get_value_or(0),
        number_decimal_places_);
}

void number_text::oox_convert(oox::num_format_context & Context)
{
	std::wstringstream strm;

    for (size_t i = 0; i < text_.size(); i++)
    {
        text_[i]->text_to_stream(strm);
    }

	std::wstring text_ = strm.str();

	if (odf_types::office_value_type::Date == Context.type() || 
		odf_types::office_value_type::Time == Context.type() ||
		odf_types::office_value_type::Currency == Context.type() || 
		odf_types::office_value_type::Percentage == Context.type())
	{
	}
	else
	{
		text_ = L"\"" + text_ + L"\""; //Book 70.ods Design of Pile Cap.ods
	}
	Context.output() << text_;
}


// number:day
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_day::ns = L"number";
const wchar_t * number_day::name = L"day";

void number_day::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_day::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void number_day::oox_convert(oox::num_format_context & Context)
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        strm << L"dd";
    }
    else
    {
        strm << L"d";
    }
}


// number:day-of-week
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_day_of_week::ns = L"number";
const wchar_t * number_day_of_week::name = L"day-of-week";

void number_day_of_week::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_day_of_week::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_day_of_week::oox_convert(oox::num_format_context & Context)
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        strm << L"dddd";
    }
    else
    {
        strm << L"ddd";
    }
}

// number:quarter
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_quarter::ns = L"number";
const wchar_t * number_quarter::name = L"quarter";

void number_quarter::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_quarter::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_quarter::oox_convert(oox::num_format_context & Context) 
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        // TODO: ??
        strm << L"";
    }
    else
    {
        // TODO: ??
        strm << L"";
    }
}

// number:month
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_month::ns = L"number";
const wchar_t * number_month::name = L"month";

void number_month::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:textual", number_textual_);
    CP_APPLY_ATTR(L"number:possessive-form", number_possessive_form_);
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_month::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_month::oox_convert(oox::num_format_context & Context) 
{
    std::wostream & strm = Context.output();
    if (!number_textual_.get_value_or(false))
    {
        if (number_style_.get_value_or(L"short") == L"long")
        {
            strm << L"mm";
        }
        else
        {
            strm << L"m";
        }
    }
    else
    {
        if (number_style_.get_value_or(L"short") == L"long")
        {
            strm << L"mmmm";
        }
        else
        {
            strm << L"mmm";
        }
    }
}

// number:year
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_year::ns = L"number";
const wchar_t * number_year::name = L"year";

void number_year::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_year::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_year::oox_convert(oox::num_format_context & Context)
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        strm << L"yyyy";
    }
    else
    {
        strm << L"yy";
    }
}

// number:date-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_date_style::ns = L"number";
const wchar_t * number_date_style::name = L"date-style";

// number:time-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_time_style::ns = L"number";
const wchar_t * number_time_style::name = L"time-style";

// number:hours
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_hours::ns = L"number";
const wchar_t * number_hours::name = L"hours";

void number_hours::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_hours::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_hours::oox_convert(oox::num_format_context & Context) 
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        strm << L"hh";
    }
    else
    {
        // TODO
        strm << L"hh";
    }
}

// number:minutes
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_minutes::ns = L"number";
const wchar_t * number_minutes::name = L"minutes";

void number_minutes::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_minutes::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_minutes::oox_convert(oox::num_format_context & Context)
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        strm << L"mm";
    }
    else
    {
        // TODO
        strm << L"mm";
    }
}

// number:seconds
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_seconds::ns = L"number";
const wchar_t * number_seconds::name = L"seconds";

void number_seconds::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:style", number_style_);
    CP_APPLY_ATTR(L"number:decimal-places", number_decimal_places_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);
}

void number_seconds::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_seconds::oox_convert(oox::num_format_context & Context) 
{
    std::wostream & strm = Context.output();
    if (number_style_.get_value_or(L"short") == L"long")
    {
        strm << L"ss";
    }
    else
    {
        // TODO
        strm << L"ss";
    }

    if (int dec = number_decimal_places_.get_value_or(0))
    {
        strm << L".";
        strm << std::wstring(dec, L'0');                    
    }
}

// number:am-pm
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_am_pm::ns = L"number";
const wchar_t * number_am_pm::name = L"am-pm";

void number_am_pm::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void number_am_pm::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_am_pm::oox_convert(oox::num_format_context & Context) 
{
    std::wostream & strm = Context.output();
    strm << L"AM/PM";
}

// number:boolean
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_boolean::ns = L"number";
const wchar_t * number_boolean::name = L"boolean";

void number_boolean::oox_convert(oox::num_format_context & Context)
{
	Context.type(odf_types::office_value_type::Boolean);
}
// number:fraction
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_fraction::ns		= L"number";
const wchar_t * number_fraction::name	= L"fraction";

void number_fraction::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"number:min-integer-digits", number_min_integer_digits_);
    CP_APPLY_ATTR(L"number:grouping", number_grouping_);
    CP_APPLY_ATTR(L"number:calendar", number_calendar_);    
    CP_APPLY_ATTR(L"number:denominator-value", number_denominator_value_);
    CP_APPLY_ATTR(L"number:min-numerator-digits", number_min_numerator_digits_);
    CP_APPLY_ATTR(L"number:min-denominator-digits", number_min_denominator_digits_);
}

void number_fraction::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void number_fraction::oox_convert(oox::num_format_context & Context) 
{
	Context.type(odf_types::office_value_type::Fraction);

    std::wostream & strm = Context.output();

    format_number_number(strm, 
        number_grouping_.get_value_or(false),
        number_min_integer_digits_.get_value_or(0), boost::none);

    if (number_min_numerator_digits_.get_value_or(0) &&
        number_min_denominator_digits_.get_value_or(0))
    {
        strm << L' ';
        strm << std::wstring(number_min_numerator_digits_.get_value_or(0), L'?');
        strm << L'/';
        
        if (!number_denominator_value_)
            strm << std::wstring(number_min_denominator_digits_.get_value_or(0), L'?');    
        else
            strm << *number_denominator_value_;
    }
}

void number_scientific_number::oox_convert(oox::num_format_context & Context) 
{
	Context.type(odf_types::office_value_type::Scientific);

    std::wostream & strm = Context.output();
    format_number_number(
        strm,
        number_grouping_.get_value_or(false),
        number_min_integer_digits_.get_value_or(0),
        number_decimal_places_);

    if (int e = number_min_exponent_digits_.get_value_or(0))
    {
        strm << L"E+" << std::wstring(e, L'0');    
    }
}

}
}
