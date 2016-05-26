

#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_chart_legend.h"

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

void oox_chart_legend::oox_serialize(std::wostream & _Wostream)
{
	if (content_.bEnabled == false) return;

	oox_chart_shape shape;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:legend")
        {

			CP_XML_NODE(L"c:legendPos")
			{
				CP_XML_ATTR(L"val", "r");//  "b" | "l" |  "r" |  "t"// == bottom left right top

			}
            layout_.oox_serialize(CP_XML_STREAM());		
			
			CP_XML_NODE(L"c:overlay")
			{
				CP_XML_ATTR(L"val", 0);
			}

			shape.set(content_.graphic_properties_, content_.fill_);
			shape.oox_serialize(CP_XML_STREAM());

			oox_serialize_default_text(CP_XML_STREAM(), content_.text_properties_);

		}
    }

}
}
}
