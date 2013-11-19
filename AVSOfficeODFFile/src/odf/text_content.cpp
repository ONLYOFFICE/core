#include "precompiled_cpodf.h"
#include "text_content.h"

#include <string>
#include <boost_string.h>
#include <boost/foreach.hpp>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/serialize.h>

namespace cpdoccore { 
namespace odf {

namespace text {

void paragraph_attrs::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_style_name_ = style_ref( Attributes->get_val< ::std::wstring >(L"text:style-name").get_value_or(L"") );
    
    const ::std::wstring classNames = Attributes->get_val< ::std::wstring >(L"text:class-names").get_value_or(L"");
    if (classNames.size())
    {
        ::std::vector< ::std::wstring > classNamesArray;
        boost::algorithm::split(classNamesArray, classNames, boost::algorithm::is_any_of(L" "));

        BOOST_FOREACH(const ::std::wstring & name, classNamesArray)
        {
            text_class_names_.push_back( style_ref(name) );        
        }
    }

    text_cond_style_name_ = style_ref( Attributes->get_val< ::std::wstring >(L"text:cond-style-name").get_value_or(L"") );
    
}


}
}
}
