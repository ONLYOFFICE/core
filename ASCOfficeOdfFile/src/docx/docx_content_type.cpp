#include "../odf/precompiled_cpodf.h"
#include "docx_content_type.h"
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>
#include <boost/foreach.hpp>
#include "namespaces.h"
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace oox {

const wchar_t * default_content_type::ns = L"";
const wchar_t * default_content_type::name = L"Default";

::std::wostream & default_content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Default")
        {
            CP_XML_ATTR(L"Extension", extension());
            CP_XML_ATTR(L"ContentType", content_type());        
        }    
    }

    //_Wostream << L"<Default ";
    //CP_XML_SERIALIZE_ATTR(L"Extension", extension());
    //CP_XML_SERIALIZE_ATTR(L"ContentType", content_type());
    //_Wostream << L" />";
    return _Wostream;
}

const wchar_t * override_content_type::ns = L"";
const wchar_t * override_content_type::name = L"Override";

::std::wostream & override_content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Override")
        {
            CP_XML_ATTR(L"PartName", part_name());
            CP_XML_ATTR(L"ContentType", content_type());
        }
    }

    //_Wostream << L"<Override ";
    //CP_XML_SERIALIZE_ATTR(L"PartName", part_name());
    //CP_XML_SERIALIZE_ATTR(L"ContentType", content_type());
    //_Wostream << L" />";
    return _Wostream;
}

const wchar_t * content_type::ns = L"";
const wchar_t * content_type::name = L"Types";

::std::wostream & content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Types")
        {
            CP_XML_ATTR(L"xmlns", xmlns::types.value);

            
            BOOST_FOREACH(const xml::element_wc & elm, default_)
            {
                elm.xml_to_stream(CP_XML_STREAM());
            }

            BOOST_FOREACH(const xml::element_wc & elm, override_)
            {
                elm.xml_to_stream(CP_XML_STREAM());
            }            
        }
    }

    //_Wostream << L"<Types ";
    //CP_XML_SERIALIZE_ATTR(L"xmlns", xmlns::types.value);
    //_Wostream << L">";

    //BOOST_FOREACH(const xml::element_wc & elm, default_)
    //{
    //    elm.xml_to_stream(_Wostream);        
    //}

    //BOOST_FOREACH(const xml::element_wc & elm, override_)
    //{
    //    elm.xml_to_stream(_Wostream);        
    //}

    //_Wostream << L"</Types>";
    return _Wostream;
}

}
}
