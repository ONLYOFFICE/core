#include "precompiled_cpodf.h"

#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_chart_legend.h"

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {


void oox_chart_legend::oox_serialize_default_text(std::wostream & _Wostream)
{
	if (content_.text_properties_.size()<1)return;

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

						if (odf::GetProperty(content_.text_properties_,L"font-size",dVal))
							CP_XML_ATTR(L"sz", (int)(dVal.get()*100));
						
						if ((odf::GetProperty(content_.text_properties_,L"font-style",iVal)) && (*iVal >0))
							CP_XML_ATTR(L"i", "true");
						if ((odf::GetProperty(content_.text_properties_,L"font-weight",iVal)) && (*iVal >0))
							CP_XML_ATTR(L"b", "true");		
				
						if (odf::GetProperty(content_.text_properties_,L"font-color",sVal))
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

void oox_chart_legend::oox_serialize(std::wostream & _Wostream)
{
	oox_chart_shape shape;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:legend")
        {
            layout_.oox_serialize(CP_XML_STREAM());
			CP_XML_NODE(L"c:overlay")
			{
				CP_XML_ATTR(L"val", 0);
			}
			CP_XML_NODE(L"c:legendPos")
			{
				CP_XML_ATTR(L"val", "r");//  "b" | "l" |  "r" |  "t"// == bottom left right top

			}
			shape.set(content_.graphic_properties_,content_.fill_);
			shape.oox_serialize(CP_XML_STREAM());
			
			//oox_chart_shape shape;
			//shape.set(content_.graphic_properties_,content_.fill_);

			oox_serialize_default_text(CP_XML_STREAM());

		}
    }

}
}
}
