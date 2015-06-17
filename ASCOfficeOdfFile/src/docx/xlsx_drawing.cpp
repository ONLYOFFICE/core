
#include "xlsx_drawing.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/xml/utils.h>

#include "../odf/datatypes/custom_shape_types_convert.h"


namespace cpdoccore {

namespace oox {



namespace {

std::wostream & operator << (std::wostream & strm, xlsx_drawing_position::type_t t)
{
    strm << (t == xlsx_drawing_position::from ? L"from" : L"to");
    return strm;
}

}
void xlsx_serialize_text(std::wostream & strm, const std::vector<odf_reader::_property> & properties)
{
	_CP_OPT(std::wstring) strTextContent;
	odf_reader::GetProperty(properties,L"text-content",strTextContent);

	if (!strTextContent)return;
	if (strTextContent.get().length()<1)return;
   
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"xdr:txBody")
		{  
			oox_serialize_bodyPr(CP_XML_STREAM(),properties);
			CP_XML_NODE(L"a:lstStyle");
			if (strTextContent)
			{				
				CP_XML_NODE(L"a:p")
				{
					CP_XML_STREAM() << strTextContent.get();
				}
			}
		}
    }
}

void xlsx_serialize(std::wostream & strm, const xlsx_drawing_position & val)
{
    CP_XML_WRITER(strm)
    {
        CP_XML_NODE(std::wstring(L"xdr:") + (val.type == xlsx_drawing_position::from ? L"from" : L"to") )
        {
            CP_XML_NODE(L"xdr:col")
            {
                CP_XML_CONTENT(val.position.col);
            }
            
            CP_XML_NODE(L"xdr:colOff")
            {
                CP_XML_CONTENT(static_cast<size_t>(val.position.colOff));
            }

            CP_XML_NODE(L"xdr:row")
            {
                CP_XML_CONTENT(val.position.row);
            }

            CP_XML_NODE(L"xdr:rowOff")
            {
                CP_XML_CONTENT(static_cast<size_t>(val.position.rowOff));
            }  
        }
    }
}

void xlsx_serialize_image(std::wostream & strm, _xlsx_drawing & val)
{
    CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"xdr:pic")
        {                  
            CP_XML_NODE(L"xdr:nvPicPr")
            {
                CP_XML_NODE(L"xdr:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);
                    CP_XML_ATTR(L"name", val.name);

					oox_serialize_hlink(CP_XML_STREAM(),val.hlinks);

				}
                CP_XML_NODE(L"xdr:cNvPicPr")
				{
					if (val.fill.bitmap->bCrop)
					{
						CP_XML_ATTR(L"preferRelativeResize", L"0");
						CP_XML_NODE(L"a:picLocks"){}
					}	
				}
            } 
			val.fill.bitmap->name_space = L"xdr";
			oox_serialize_fill(CP_XML_STREAM(), val.fill);

            CP_XML_NODE(L"xdr:spPr")
            {
				oox_serialize_xfrm(CP_XML_STREAM(),val);

                CP_XML_NODE(L"a:prstGeom")
                {                   
                    CP_XML_ATTR(L"prst", L"rect");
                    CP_XML_NODE(L"a:avLst");
                }
				oox_serialize_ln(CP_XML_STREAM(),val.additional);
            } 			
			xlsx_serialize_text(CP_XML_STREAM(),val.additional);
        } 
    }  // CP_XML_WRITER  
}

void xlsx_serialize_shape(std::wostream & strm, _xlsx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"xdr:sp")
        {                  
            CP_XML_NODE(L"xdr:nvSpPr")
            {
                CP_XML_NODE(L"xdr:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);//числовое значение val.rId

                    CP_XML_ATTR(L"name", val.name);

					oox_serialize_hlink(CP_XML_STREAM(),val.hlinks);
                }
				CP_XML_NODE(L"xdr:cNvSpPr")//non visual properies (собственно тока 1 там)
				{
					if (val.sub_type==1)CP_XML_ATTR(L"txBox", 1);
				}
            } // xdr:nv_Pr
			CP_XML_NODE(L"xdr:spPr")
            {
				oox_serialize_xfrm(CP_XML_STREAM(),val);

				oox_serialize_shape(CP_XML_STREAM(),val);

				oox_serialize_ln(CP_XML_STREAM(),val.additional);
            } // xdr:spPr
			
			xlsx_serialize_text(CP_XML_STREAM(),val.additional);
        } 
    }  // CP_XML_WRITER  
}

void xlsx_serialize_chart(std::wostream & strm, _xlsx_drawing & val)
{
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"xdr:graphicFrame")
        {                  
            CP_XML_NODE(L"xdr:nvGraphicFramePr")
            {
                CP_XML_NODE(L"xdr:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);
                    CP_XML_ATTR(L"name", val.name);
                }

                CP_XML_NODE(L"xdr:cNvGraphicFramePr");
            } 
			oox_serialize_xfrm(CP_XML_STREAM(),val,L"xdr");

			//oox_serialize_ln(CP_XML_STREAM(),val.additional);

            CP_XML_NODE(L"a:graphic")
            {                   
                CP_XML_NODE(L"a:graphicData")
				{
					CP_XML_ATTR(L"uri", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
					CP_XML_NODE(L"c:chart")
					{
						CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
						CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
						CP_XML_ATTR(L"r:id", val.chartId);
					}
				}
			}               
		} // xdr:graphicFrame
    }  // CP_XML_WRITER  
}

void xlsx_serialize(std::wostream & strm, _xlsx_drawing & val)
{
     CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"xdr:twoCellAnchor")
        {
			CP_XML_ATTR(L"editAs", L"oneCell");//"absolute");

			xlsx_serialize(CP_XML_STREAM(), val.from);
			xlsx_serialize(CP_XML_STREAM(), val.to);
			
			if (val.type == mediaitems::typeShape)
			{
				xlsx_serialize_shape(CP_XML_STREAM(),val);
			}
			else if (val.type == mediaitems::typeImage)
			{
				xlsx_serialize_image(CP_XML_STREAM(),val);
			}
			else if (val.type == mediaitems::typeChart)
			{
				xlsx_serialize_chart(CP_XML_STREAM(),val);
			}
			CP_XML_NODE(L"xdr:clientData");
		}

	 }
}


}
}
