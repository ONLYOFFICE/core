#ifndef _CPDOCCORE_ODF_TABLE_CALCULATION_SETTINGS_H_
#define _CPDOCCORE_ODF_TABLE_CALCULATION_SETTINGS_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

/// \class  table_calculation_settings
/// \brief  table:calculation-settings
class table_calculation_settings : public office_element_impl<table_calculation_settings>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableCalculationSettings;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    _CP_OPT(bool) table_case_sensitive_;
    _CP_OPT(bool) table_precision_as_shown_;
    _CP_OPT(bool) table_search_criteria_must_apply_to_whole_cell_;
    _CP_OPT(bool) table_automatic_find_labels_;
    _CP_OPT(bool) table_use_regular_expressions_;
    _CP_OPT(unsigned int) table_null_year_;

    office_element_ptr table_null_date_;
            
};

CP_REGISTER_OFFICE_ELEMENT2(table_calculation_settings);

/// \class  table_null_date
/// \brief  table:null-date
class table_null_date : public office_element_impl<table_null_date>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNullDate;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    _CP_OPT(std::wstring) table_value_type_;
    _CP_OPT(std::wstring) table_date_value_type_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_null_date);

/// \class  table_iteration
/// \brief  table:iteration
/// table-iteration
class table_iteration : public office_element_impl<table_iteration>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNullDate;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    _CP_OPT(std::wstring) table_status_;
    _CP_OPT(unsigned int) table_steps_;
    _CP_OPT(double) table_maximum_difference_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_iteration);

}
}

#endif
