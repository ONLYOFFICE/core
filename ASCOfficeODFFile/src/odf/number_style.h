#ifndef _CPDOCCORE_ODF_NUMBER_STYLE_H_
#define _CPDOCCORE_ODF_NUMBER_STYLE_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "common_attlists.h"

namespace cpdoccore { 
namespace odf {

class number_style_base : public office_element
{
public:
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    void xlsx_convert_impl(oox::xlsx_conversion_context & Context);
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_text(const std::wstring &) {}
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    std::wstring get_style_name() const;

private:
    common_data_style_attlist common_data_style_attlist_;
    office_element_ptr style_text_properties_;
    office_element_ptr_array content_;
    office_element_ptr_array style_map_;
};

/// \class  number_number_style
/// \brief  number:number-style
///         number-number-style
class number_number_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberNumberStyle;
    CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_number_style);

/// \class  number_currency_style
/// \brief  number:currency-style
///         number-currency-style
class number_currency_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberCurrencyStyle;
    CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_currency_style);

/// \class  number_text_style
/// \brief  number:text-style
///         number-text-style
class number_text_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberTextStyle;
    CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_text_style);

/// \class  number_date_style
/// \brief  number:date-style
///         number-date-style
class number_date_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberDataStyle;
    CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;
};

CP_REGISTER_OFFICE_ELEMENT2(number_date_style);

/// \class  number_percentage_style
/// \brief  number:percentage-style
///         number-percentage-style
class number_percentage_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberPercentageStyle;
    CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;
                    
};

CP_REGISTER_OFFICE_ELEMENT2(number_percentage_style);

/// \class  number_time_style
/// \brief  number:time-style
///         number-time-style
class number_time_style : public number_style_base
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberTimeStyle;
    CPDOCCORE_DEFINE_VISITABLE();
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;                    
};

CP_REGISTER_OFFICE_ELEMENT2(number_time_style);

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

/// \class  number_text
/// \brief  number:text
///         number-text
class number_text : public office_element_impl<number_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberText;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array text_;
};

CP_REGISTER_OFFICE_ELEMENT2(number_text);

/// \class  number_number
/// \brief  number:number
///         number-number
class number_number : public office_element_impl<number_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberNumber;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element(xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    // number-number-attlist
    _CP_OPT(std::wstring) number_decimal_replacement_;
    _CP_OPT(double) number_display_factor_;

    // common-decimal-places-attlist
    _CP_OPT(int) number_decimal_places_;

    // common-number-attlist
    _CP_OPT(int) number_min_integer_digits_;
    _CP_OPT(bool) number_grouping_;
    
    office_element_ptr_array number_embedded_text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(number_number);

/// \class  number_embedded_text
/// \brief  number:embedded-text
///         number-embedded-text
class number_embedded_text : public office_element_impl<number_embedded_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberEmbeddedText;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    _CP_OPT(int) number_position_;
    office_element_ptr_array text_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_embedded_text);

/// \class  number_scientific_number
/// \brief  number:scientific-number
///         number-scientific-number
class number_scientific_number : public office_element_impl<number_scientific_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberScientificNumber;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    // number-scientific-number-attlist
    _CP_OPT(int) number_min_exponent_digits_;

    // common-decimal-places-attlist
    _CP_OPT(int) number_decimal_places_;

    // common-number-attlist
    _CP_OPT(int) number_min_integer_digits_;
    _CP_OPT(bool) number_grouping_;        
    
};

CP_REGISTER_OFFICE_ELEMENT2(number_scientific_number);

/// \class  number_currency_symbol
/// \brief  number:currency-symbol
///         number-currency-symbol
class number_currency_symbol : public office_element_impl<number_currency_symbol>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberCurrencySymbol;
    CPDOCCORE_DEFINE_VISITABLE();

    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    _CP_OPT(std::wstring) number_language_;
    _CP_OPT(std::wstring) number_country_;

    std::vector<std::wstring> text_;
   
};

CP_REGISTER_OFFICE_ELEMENT2(number_currency_symbol);

/// \class  number_text_content
/// \brief  number:text-content
///         number-text-content
class number_text_content : public office_element_impl<number_text_content>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberTextContent;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array text_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(number_text_content);

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
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_day_of_week);

/// \class  number_quarter
/// \brief  number:quarter
///         number-quarter
class number_quarter : public office_element_impl<number_quarter>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberQuarter;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_quarter);


/// \class  number_day
/// \brief  number:day
///         number-day
class number_day : public office_element_impl<number_day>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberDay;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_day);

/// \class  number_month
/// \brief  number:month
///         number-month
class number_month : public office_element_impl<number_month>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberMonth;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(bool) number_textual_;
    _CP_OPT(bool) number_possessive_form_;
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_month);

/// \class  number_year
/// \brief  number:year
///         number-year
class number_year : public office_element_impl<number_year>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberYear;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_year);

/// \class  number_hours
/// \brief  number:hours
///         number-hours
class number_hours : public office_element_impl<number_hours>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberHours;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_hours);

/// \class  number_minutes
/// \brief  number:minutes
///         number-minutes
class number_minutes : public office_element_impl<number_minutes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberMinutes;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_minutes);

/// \class  number_seconds
/// \brief  number:seconds
///         number-seconds
class number_seconds : public office_element_impl<number_seconds>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberSeconds;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(std::wstring) number_style_;
    _CP_OPT(int) number_decimal_places_;
    _CP_OPT(std::wstring) number_calendar_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_seconds);

/// \class  number_am_pm
/// \brief  number:am-pm
///         number-am-pm
class number_am_pm : public office_element_impl<number_am_pm>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberAmPm;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

};

CP_REGISTER_OFFICE_ELEMENT2(number_am_pm);

/// \class  number_fraction
/// \brief  number:fraction
///         number-fraction
class number_fraction : public office_element_impl<number_fraction>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeNumberFraction;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    _CP_OPT(int) number_min_integer_digits_;
    _CP_OPT(bool) number_grouping_;
    _CP_OPT(std::wstring) number_calendar_;
    
    _CP_OPT(int) number_denominator_value_;
    _CP_OPT(int) number_min_numerator_digits_;
    _CP_OPT(int) number_min_denominator_digits_;

};

CP_REGISTER_OFFICE_ELEMENT2(number_fraction);

}
}

#endif
