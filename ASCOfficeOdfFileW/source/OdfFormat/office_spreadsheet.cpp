#include "precompiled_cpodf.h"
#include "office_spreadsheet.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

// office:spreadsheet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_spreadsheet::ns = L"office";
const wchar_t * office_spreadsheet::name = L"spreadsheet";

void office_spreadsheet::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    create_element(Ns,Name,content_,getContext());
}
void office_spreadsheet::add_child_element(office_element_ptr & child_element)
{
    content_.push_back(child_element);
}

void office_spreadsheet::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_spreadsheet::serialize(std::wostream & _Wostream)
{

}

}
}
