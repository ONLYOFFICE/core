#include "precompiled_cpodf.h"

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
	_CP_OPT(std::wstring) strVal;
 	_CP_OPT(double) dVal;
 	_CP_OPT(int) iVal;
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:spPr")
        {
			oox_serialize_fill(CP_XML_STREAM(),fill);
	
			oox_serialize_ln(CP_XML_STREAM(),content_);
		}
    }

}
}
}
