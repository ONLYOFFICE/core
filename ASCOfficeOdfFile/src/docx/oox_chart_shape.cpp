

#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>


#include "oox_chart_shape.h"
#include "oox_drawing.h"

namespace cpdoccore {
namespace oox {


void oox_chart_shape::oox_serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:spPr")
        {
			oox_serialize_fill(CP_XML_STREAM(),fill_);
	
			oox_serialize_ln(CP_XML_STREAM(),content_);
		}
    }

}

void oox_chart_shape::set(std::vector<odf_reader::_property> & prop,_oox_fill & fill)
{
	content_ = prop;
	fill_ = fill;
}

}
}
