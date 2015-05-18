
#include "oox_title.h"
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/style_text_properties.h"

namespace cpdoccore {
namespace oox {

void oox_title::oox_serialize(std::wostream & _Wostream)
{
	if (content_.content_.length()<1)return;
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:title")
        {
			CP_XML_NODE(L"c:tx")
			{
				CP_XML_NODE(L"c:rich")
				{
					CP_XML_NODE(L"a:bodyPr"){}
					CP_XML_NODE(L"a:lstStyle"){}
					CP_XML_NODE(L"a:p")
					{
						CP_XML_NODE(L"a:pPr")
						{
							CP_XML_NODE(L"a:defRPr"){}
						}
						CP_XML_NODE(L"a:r")
						{
							CP_XML_NODE(L"a:rPr")
							{
								_CP_OPT(double) dVal;
								_CP_OPT(std::wstring) sVal;
								_CP_OPT(int) iVal;	

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
							CP_XML_NODE(L"a:t")
							{
								CP_XML_CONTENT(content_.content_);
							}
						}
					}
				
				}
			}
            layout_.oox_serialize(CP_XML_STREAM());
			CP_XML_NODE(L"c:overlay")
			{
				CP_XML_ATTR(L"val", 0);
			}
		}
    }
}
}
}
