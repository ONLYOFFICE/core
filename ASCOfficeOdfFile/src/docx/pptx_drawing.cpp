/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "pptx_drawing.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/datatypes/custom_shape_types_convert.h"

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

void pptx_serialize_text(std::wostream & strm, _pptx_drawing & val)
{
	_CP_OPT(std::wstring) strTextContent;
	odf_reader::GetProperty ( val.additional, L"text-content", strTextContent);

	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:txBody")
		{  
			oox_serialize_bodyPr(CP_XML_STREAM(), val);
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
void pptx_serialize_image(std::wostream & strm, _pptx_drawing & val)
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
			//_CP_OPT(std::wstring) strTextContent;
			//odf::GetProperty(properties,L"text-content",strTextContent);
			//pptx_serialize_text(CP_XML_STREAM(),val.additional);
			//на картинке тект нельзя... - выше сменили тип на рект с заливкой
        } 
    }  
}

void pptx_serialize_shape(std::wostream & strm, _pptx_drawing & val)
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
					if (val.place_holder_type_.length()>0)
					{
						CP_XML_NODE(L"p:ph")
						{
							CP_XML_ATTR(L"type",val.place_holder_type_);
							if (val.place_holder_idx_ > 0)	CP_XML_ATTR(L"idx", val.place_holder_idx_);
							
							if (val.place_holder_type_ == L"dt")	{	CP_XML_ATTR(L"sz", L"half");	}
							if (val.place_holder_type_ == L"ftr")	{	CP_XML_ATTR(L"sz", L"quarter");	}
							if (val.place_holder_type_ == L"sldNum"){	CP_XML_ATTR(L"sz", L"quarter");	}
						}
					}
				}
            }
			CP_XML_NODE(L"p:spPr")
			{			
				_CP_OPT(bool) bNoRect;
				odf_reader::GetProperty(val.additional,L"no_rect",bNoRect);

				if (!bNoRect)
				{					
					oox_serialize_xfrm(CP_XML_STREAM(),val);

					oox_serialize_shape(CP_XML_STREAM(),val);
					oox_serialize_ln(CP_XML_STREAM(), val.additional);
				}
			}
			pptx_serialize_text(CP_XML_STREAM(), val);
		}
    }  // CP_XML_WRITER  
}

void pptx_serialize_chart(std::wostream & strm, _pptx_drawing & val)
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

void pptx_serialize_table(std::wostream & strm, _pptx_drawing & val)
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
					odf_reader::GetProperty(val.additional,L"table-content",strTableContent);

					if (strTableContent)
					{
						CP_XML_STREAM() << strTableContent.get();
					}
				}
			}               
		} // p:graphicFrame
    }  // CP_XML_WRITER  
}

void pptx_serialize(std::wostream & strm, _pptx_drawing & val)
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
