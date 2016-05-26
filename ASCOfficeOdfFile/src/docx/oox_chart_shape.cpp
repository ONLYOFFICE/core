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

void oox_serialize_default_text(std::wostream & _Wostream, std::vector<odf_reader::_property>& properties)
{
	if (properties.size() < 1)return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:txPr")
		{	
			CP_XML_NODE(L"a:bodyPr");
			CP_XML_NODE(L"a:lstStyle");
			CP_XML_NODE(L"a:p")
			{
				CP_XML_NODE(L"a:pPr")
				{
					CP_XML_NODE(L"a:defRPr")
					{							
						_CP_OPT(double) dVal;
						_CP_OPT(int) iVal;	
						_CP_OPT(std::wstring) sVal;

						if (odf_reader::GetProperty(properties, L"font-size",dVal))
							CP_XML_ATTR(L"sz", (int)(dVal.get()*100));
						
						if ((odf_reader::GetProperty(properties, L"font-style",iVal)) && (*iVal >0))
							CP_XML_ATTR(L"i", "true");
						
						if ((odf_reader::GetProperty(properties, L"font-weight",iVal)) && (*iVal >0))
							CP_XML_ATTR(L"b", "true");		
				
						if (odf_reader::GetProperty(properties, L"font-color",sVal))
							CP_XML_NODE(L"a:solidFill")	
							{
								CP_XML_NODE(L"a:srgbClr"){CP_XML_ATTR(L"val", sVal.get());}
							}
					}

				}
			}
		}
	}
}


}
}
