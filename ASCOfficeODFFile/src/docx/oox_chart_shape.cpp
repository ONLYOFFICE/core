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
			std::wstring fillType = L"a:solidFill";

			if (odf::GetProperty(content_,L"fill",strVal))
			{
				if ((strVal) && (strVal.get() == L"none"))		fillType = L"a:noFill";
				if ((strVal) && (strVal.get() == L"hatch"))		fillType = L"a:pattFill";
				if ((strVal) && (strVal.get() == L"gradient"))	fillType = L"a:gradFill";
				if ((strVal) && (strVal.get() == L"bitmap"))	fillType = L"a:imageFill";
			}

			odf::GetProperty(content_,L"fill-color",strVal);
			if (strVal && fillType != L"a:noFill")
			{		
				CP_XML_NODE(fillType)
				{ 
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",strVal.get());
						if (odf::GetProperty(content_,L"opacity",dVal))
						{
							CP_XML_NODE(L"a:alpha")
							{
								CP_XML_ATTR(L"val",boost::lexical_cast<std::wstring>((int)(dVal.get())) + L"%");
							}
						}
					}
				}
			}

		
			oox_serialize_ln(CP_XML_STREAM(),content_);
		}
    }

}
}
}
