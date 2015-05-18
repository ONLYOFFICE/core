

#include <boost/foreach.hpp>

#include <cpdoccore/xml/simple_xml_writer.h>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/utils.h>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPColorUtils.h>

#include "number_style.h"

#include "paragraph_elements.h"
#include "style_text_properties.h"
#include "style_map.h"

namespace cpdoccore { 
namespace odf {

std::wstring number_style_base::get_style_name() const
{
    if (common_data_style_attlist_.style_name_)
        return common_data_style_attlist_.style_name_->style_name();
    else
        return L"";
}
void number_style_base::set_style_name(std::wstring & name) 
{
    common_data_style_attlist_.style_name_ = style_ref(name);
}

void number_style_base::serialize(std::wostream & strm)
{
 	if (style_text_properties_) 
		style_text_properties_->serialize(strm);
 
	BOOST_FOREACH(office_element_ptr & elm, style_map_)
	{
		elm->serialize(strm);
    }
	BOOST_FOREACH(office_element_ptr & elm, content_)
	{
		elm->serialize(strm);
	}
}
void number_style_base::serialize(CP_ATTR_NODE)
{
	common_data_style_attlist_.serialize(CP_GET_XML_NODE());
}
void number_date_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:automatic-order", number_automatic_order_);
	
			number_style_base::serialize(CP_GET_XML_NODE());	//attr
			number_style_base::serialize(CP_XML_STREAM());		//nodes
		}
	}
}
void number_time_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:automatic-order", number_automatic_order_);
	
			number_style_base::serialize(CP_GET_XML_NODE());	//attr
			number_style_base::serialize(CP_XML_STREAM());		//nodes
		}
	}
}
void number_style_base::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
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
void number_style_base::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeStyleTextProperties)
	{
		style_text_properties_ = child_element;
	}
	else if (type == typeStyleMap)
	{
		style_map_.push_back(child_element);
	}
	else
		content_.push_back(child_element);

}
// number:number-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_number_style::ns = L"number";
const wchar_t * number_number_style::name = L"number-style";

void number_number_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			number_style_base::serialize(CP_GET_XML_NODE());	//attr
			number_style_base::serialize(CP_XML_STREAM());		//nodes
		}
	}
}
// number:text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_text::ns = L"number";
const wchar_t * number_text::name = L"text";

void number_text::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			BOOST_FOREACH(office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}		
		}
	}
}


void number_text::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}

// number:embedded-text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_embedded_text::ns = L"number";
const wchar_t * number_embedded_text::name = L"embedded-text";

void number_embedded_text::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:position", number_position_);
			
			BOOST_FOREACH(office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}		
		}
	}
}



void number_embedded_text::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}

// number:number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_number::ns = L"number";
const wchar_t * number_number::name = L"number";

void number_number::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:decimal-replacement", number_decimal_replacement_);
			CP_XML_ATTR_OPT(L"number:display-factor", number_display_factor_); 

			CP_XML_ATTR_OPT(L"number:decimal-places", number_decimal_places_);

			CP_XML_ATTR_OPT(L"number:min-integer-digits", number_min_integer_digits_);
			CP_XML_ATTR_OPT(L"number:grouping", number_grouping_);
			
			BOOST_FOREACH(office_element_ptr & elm, number_embedded_text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

void number_number::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"number", L"embedded-text")
    {
        CP_CREATE_ELEMENT(number_embedded_text_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
void number_number::add_child_element(office_element_ptr & child)
{
 	ElementType type = child->get_type();

    if (type == typeNumberEmbeddedText)
	{
        number_embedded_text_.push_back(child);
    }
    else
	{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
}

// number:scientific-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_scientific_number::ns = L"number";
const wchar_t * number_scientific_number::name = L"scientific-number";

void number_scientific_number::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:min-exponent-digits", number_min_exponent_digits_);
			CP_XML_ATTR_OPT(L"number:decimal-places", number_decimal_places_);

			CP_XML_ATTR_OPT(L"number:min-integer-digits", number_min_integer_digits_);
			CP_XML_ATTR_OPT(L"number:grouping", number_grouping_);
		}
	}
}


// number:currency-symbol
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_currency_symbol::ns = L"number";
const wchar_t * number_currency_symbol::name = L"currency-symbol";

void number_currency_symbol::serialize(std::wostream & strm)
{
 	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:language", number_language_);
			CP_XML_ATTR_OPT(L"number:country", number_country_);
			
			for (long i=0; i < text_.size(); i++)
				CP_XML_STREAM() << text_[i];
		}
	}
}                      


void number_currency_symbol::add_text(const std::wstring & Text)
{
    text_.push_back(Text);
}

// number:currency-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_currency_style::ns = L"number";
const wchar_t * number_currency_style::name = L"currency-style";

void number_currency_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			number_style_base::serialize(CP_GET_XML_NODE());	//attr
			number_style_base::serialize(CP_XML_STREAM());		//nodes
		}
	}
}
// number:text-content
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_text_content::ns = L"number";
const wchar_t * number_text_content::name = L"text-content";

void number_text_content::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			BOOST_FOREACH(office_element_ptr & elm, text_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

void number_text_content::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text_text::create(Text) ;
    text_.push_back( elm );
}

// number:text-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_text_style::ns = L"number";
const wchar_t * number_text_style::name = L"text-style";

void number_text_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			number_style_base::serialize(CP_GET_XML_NODE());	//attr
			number_style_base::serialize(CP_XML_STREAM());		//nodes
		}
	}
}
// number:percentage-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_percentage_style::ns = L"number";
const wchar_t * number_percentage_style::name = L"percentage-style";

void number_percentage_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			number_style_base::serialize(CP_GET_XML_NODE());	//attr
			number_style_base::serialize(CP_XML_STREAM());		//nodes
		}
	}
}
//////////////////////////////

namespace 
{
	//todooo - сделать наоборот string -> в свойства
void format_number_number(
                          std::wostream & strm,
                          bool number_grouping_,
                          int number_min_integer_digits_,
                          int number_decimal_places_
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
        if (number_min_integer_digits_ > 0)
            strm << std::wstring( number_min_integer_digits_, L'0');
        else
            strm << L"#";
    }

    if (number_decimal_places_)
    {
        strm << L".";
        strm << std::wstring(number_decimal_places_, L'0');
    }
}
}


// number:day
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_day::ns = L"number";
const wchar_t * number_day::name = L"day";

void number_day::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}

// number:day-of-week
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_day_of_week::ns = L"number";
const wchar_t * number_day_of_week::name = L"day-of-week";

void number_day_of_week::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}

// number:quarter
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_quarter::ns = L"number";
const wchar_t * number_quarter::name = L"quarter";

void number_quarter::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}

// number:month
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_month::ns = L"number";
const wchar_t * number_month::name = L"month";

void number_month::serialize(std::wostream & strm)
{
 	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:textual", number_textual_);
			CP_XML_ATTR_OPT(L"number:possessive-form", number_possessive_form_);
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}

// number:year
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_year::ns = L"number";
const wchar_t * number_year::name = L"year";

void number_year::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
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

void number_hours::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}


// number:minutes
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_minutes::ns = L"number";
const wchar_t * number_minutes::name = L"minutes";

void number_minutes::serialize(std::wostream & strm)
{
 	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}

// number:seconds
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_seconds::ns = L"number";
const wchar_t * number_seconds::name = L"seconds";

void number_seconds::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {  
			CP_XML_ATTR_OPT(L"number:style", number_style_);
			CP_XML_ATTR_OPT(L"number:decimal-places", number_decimal_places_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);
		}
	}
}

// number:am-pm
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_am_pm::ns = L"number";
const wchar_t * number_am_pm::name = L"am-pm";

void number_am_pm::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE();
	}
}

// number:fraction
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number_fraction::ns = L"number";
const wchar_t * number_fraction::name = L"fraction";

void number_fraction::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"number:min-integer-digits", number_min_integer_digits_);
			CP_XML_ATTR_OPT(L"number:grouping", number_grouping_);
			CP_XML_ATTR_OPT(L"number:calendar", number_calendar_);    
			CP_XML_ATTR_OPT(L"number:denominator-value", number_denominator_value_);
			CP_XML_ATTR_OPT(L"number:min-numerator-digits", number_min_numerator_digits_);
			CP_XML_ATTR_OPT(L"number:min-denominator-digits", number_min_denominator_digits_);
		}
	}
}

//void number_fraction::xlsx_convert(oox::xlsx_conversion_context & Context) 
//{
//    std::wostream & strm = Context.get_num_format_context().output();
//
//    format_number_number(strm, 
//        number_grouping_.get_value_or(false),
//        number_min_integer_digits_.get_value_or(0),
//        0);
//
//    if (number_min_numerator_digits_.get_value_or(0) &&
//        number_min_denominator_digits_.get_value_or(0))
//    {
//        strm << L' ';
//        strm << std::wstring(number_min_numerator_digits_.get_value_or(0), L'?');
//        strm << L'/';
//        
//        if (!number_denominator_value_)
//            strm << std::wstring(number_min_denominator_digits_.get_value_or(0), L'?');    
//        else
//            strm << *number_denominator_value_;
//    }
//}
//
//void number_scientific_number::xlsx_convert(oox::xlsx_conversion_context & Context) 
//{
//    std::wostream & strm = Context.get_num_format_context().output();
//    format_number_number(
//        strm,
//        number_grouping_.get_value_or(false),
//        number_min_integer_digits_.get_value_or(0),
//        number_decimal_places_.get_value_or(0)
//        );
//
//    if (int e = number_min_exponent_digits_.get_value_or(0))
//    {
//        strm << L"E+" << std::wstring(e, L'0');    
//    }
//}
//
}
}
