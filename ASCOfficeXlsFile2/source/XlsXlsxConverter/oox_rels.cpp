
#include "oox_rels.h"

#include <boost/foreach.hpp>
#include "namespaces.h"

#include "simple_xml_writer.h"

namespace oox {

const wchar_t * relationship::ns = L"";
const wchar_t * relationship::name = L"Relationship";

::std::wostream & relationship::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Relationship")
        {
            CP_XML_ATTR(L"Id", id());
            CP_XML_ATTR(L"Type", type());
            CP_XML_ATTR(L"Target", target());

            if (!target_mode().empty())
                CP_XML_ATTR(L"TargetMode", target_mode());
        }    
    }

    //_Wostream << L"<Relationship ";
    //CP_XML_SERIALIZE_ATTR(L"Id", id());
    //CP_XML_SERIALIZE_ATTR(L"Type", type());
    //CP_XML_SERIALIZE_ATTR(L"Target", target());
    //
    //if (!target_mode().empty())
    //{
    //    CP_XML_SERIALIZE_ATTR(L"TargetMode", target_mode());
    //}

    //_Wostream << L" />";
    return _Wostream;
}

const wchar_t * rels::ns = L"";
const wchar_t * rels::name = L"Relationships";

::std::wostream & rels::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Relationships")
        {
            CP_XML_ATTR(L"xmlns", xmlns::rels.value);

            BOOST_FOREACH(const relationship & r, relationship_)
            {
                r.xml_to_stream(CP_XML_STREAM());
            }
        } // "Relationships"
    }

    //_Wostream << L"<Relationships ";
    //CP_XML_SERIALIZE_ATTR(L"xmlns", xmlns::rels.value);
    //_Wostream << L">";

    //BOOST_FOREACH(const relationship & r, relationship_)
    //{
    //    r.xml_to_stream(_Wostream);
    //}

    //_Wostream << L"</Relationships>";
    return _Wostream;
}

void rels::add(relationship const & r)
{
    relationships().push_back(r);
}

}

