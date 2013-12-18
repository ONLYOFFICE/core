#include "precompiled_cpodf.h"
#include "pptx_drawing.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/custom_shape_types_convert.h"

namespace cpdoccore {

namespace oox {

static const int _odf_to_docx_ShapeType[]=
{ 4,4,4,34,};

static const std::wstring _docxShapeType[]=
{
	L"rect", 
	L"rect", 
	L"rect", 
	L"ellipse", 
	L"ellipse", 
	L"line", 
	L"custGeom"
};

void pptx_serialize_text(std::wostream & strm, const std::vector<odf::_property> & properties)
{
	_CP_OPT(std::wstring) strTextContent;
	odf::GetProperty(properties,L"text-content",strTextContent);

	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:txBody")
		{  
			oox_serialize_bodyPr(CP_XML_STREAM(),properties);
			if (strTextContent)
			{	
				CP_XML_STREAM() << strTextContent.get();
			}
			else
			{				
				CP_XML_NODE(L"a:p")//empty a:p
				{
					CP_XML_NODE(L"a:endParaRPr");
				}
			}
		}
    }
}
void pptx_serialize_image(std::wostream & strm, _pptx_drawing const & val)
{
    CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"p:pic")
        {                  
            CP_XML_NODE(L"p:nvPicPr")
            {
                CP_XML_NODE(L"p:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);
                    CP_XML_ATTR(L"name", val.name);

					oox_serialize_hlink(CP_XML_STREAM(),val.hlinks);

				}
                CP_XML_NODE(L"p:cNvPicPr")
				{
					if (val.fill.bitmap->bCrop)
					{
						CP_XML_NODE(L"a:picLocks"){}
					}	
				}
				CP_XML_NODE(L"p:nvPr");
            } 
			val.fill.bitmap->name_space = L"p";
			oox_serialize_fill(CP_XML_STREAM(), val.fill);

            CP_XML_NODE(L"p:spPr")
            {
				oox_serialize_xfrm(CP_XML_STREAM(),val);

                CP_XML_NODE(L"a:prstGeom")
                {                   
                    CP_XML_ATTR(L"prst", L"rect");
                    CP_XML_NODE(L"a:avLst");
                }
				oox_serialize_ln(CP_XML_STREAM(),val.additional);
            } 			
			pptx_serialize_text(CP_XML_STREAM(),val.additional);
        } 
    }  
}

void pptx_serialize_shape(std::wostream & strm, _pptx_drawing const & val)
{
	CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"p:sp")
        {                  
            CP_XML_NODE(L"p:nvSpPr")
            {
                CP_XML_NODE(L"p:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);//числовое значение val.rId
                    CP_XML_ATTR(L"name", val.name);

					oox_serialize_hlink(CP_XML_STREAM(),val.hlinks);
                }
				CP_XML_NODE(L"p:cNvSpPr")//non visual properies (собственно тока 1 там)
				{
					if (val.sub_type==1 || val.sub_type==2)CP_XML_ATTR(L"txBox", 1);
					CP_XML_NODE(L"a:spLocks")
					{
						CP_XML_ATTR(L"noGrp", 1);
					}
				}
				CP_XML_NODE(L"p:nvPr")
				{
					if (val.place_holder_type_.length()>0)CP_XML_NODE(L"p:ph"){ CP_XML_ATTR(L"type",val.place_holder_type_);}
				}
            }
			CP_XML_NODE(L"p:spPr")
            {
				oox_serialize_xfrm(CP_XML_STREAM(),val);

				oox_serialize_shape(CP_XML_STREAM(),val);
				oox_serialize_ln(CP_XML_STREAM(),val.additional);
            }
			
			pptx_serialize_text(CP_XML_STREAM(),val.additional);
        } 
    }  // CP_XML_WRITER  
}

void pptx_serialize_chart(std::wostream & strm, _pptx_drawing const & val)
{
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"p:graphicFrame")
        {                  
            CP_XML_NODE(L"p:nvGraphicFramePr")
            {
                CP_XML_NODE(L"p:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);
                    CP_XML_ATTR(L"name", val.name);
                }

                CP_XML_NODE(L"p:cNvGraphicFramePr");
				CP_XML_NODE(L"p:nvPr");
            } 
			oox_serialize_xfrm(CP_XML_STREAM(),val,L"p");

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
		} // p:graphicFrame
    }  // CP_XML_WRITER  
}

void pptx_serialize_table(std::wostream & strm, _pptx_drawing const & val)
{
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"p:graphicFrame")
        {                  
            CP_XML_NODE(L"p:nvGraphicFramePr")
            {
                CP_XML_NODE(L"p:cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);
                    CP_XML_ATTR(L"name", val.name);
                }

                CP_XML_NODE(L"p:cNvGraphicFramePr");
				CP_XML_NODE(L"p:nvPr");
            } 
			oox_serialize_xfrm(CP_XML_STREAM(),val,L"p");

			//oox_serialize_ln(CP_XML_STREAM(),val.additional);

            CP_XML_NODE(L"a:graphic")
            {                   
                CP_XML_NODE(L"a:graphicData")
				{
					CP_XML_ATTR(L"uri", L"http://schemas.openxmlformats.org/drawingml/2006/table");
					
					_CP_OPT(std::wstring) strTableContent;
					odf::GetProperty(val.additional,L"table-content",strTableContent);

					if (strTableContent)
					{
						CP_XML_STREAM() << strTableContent.get();
					}
				}
			}               
		} // p:graphicFrame
    }  // CP_XML_WRITER  
}

void pptx_serialize(std::wostream & strm, _pptx_drawing const & val)
{
	if (val.type == mediaitems::typeShape)
	{
		pptx_serialize_shape(strm,val);
	}
	else if (val.type == mediaitems::typeImage)
	{
		pptx_serialize_image(strm,val);
	}
	else if (val.type == mediaitems::typeChart)
	{
		pptx_serialize_chart(strm,val);
	}
	else if (val.type == mediaitems::typeTable)
	{
		pptx_serialize_table(strm,val);
	}
}


}
}
