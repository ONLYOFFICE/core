

#include <boost/foreach.hpp>

#include "odf_rels.h"

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {


void relationship::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"manifest:file-entry")
        {
            CP_XML_ATTR(L"manifest:full-path", target());
            CP_XML_ATTR(L"manifest:media-type", type());
        }    
    }
}

void rels::serialize(std::wostream & _Wostream)
{
    BOOST_FOREACH(relationship & r, relationship_)
    {
        r.serialize(_Wostream);
	}
}

void rels::add(relationship const & r)
{
    relationships().push_back(r);
}

}
}
