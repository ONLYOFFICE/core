/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "xlsx_drawing.h"
#include <xml/simple_xml_writer.h>
#include <xml/utils.h>

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
void xlsx_serialize_text(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
{
	_CP_OPT(std::wstring) strTextContent;
	odf_reader::GetProperty ( val.additional, L"text-content", strTextContent);

	if (!strTextContent)return;
	if (strTextContent->empty())return;
   
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(ns + L":txBody")
		{  
			val.serialize_bodyPr(CP_XML_STREAM());
			
			CP_XML_NODE(L"a:lstStyle");
			if (strTextContent)
			{				
				CP_XML_STREAM() << *strTextContent;
			}
		}
    }
}

void xlsx_drawing_position::serialize(std::wostream & strm, const std::wstring & ns)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE( ns + (ns.empty() ? L"" : L":") + (type == xlsx_drawing_position::from ? L"from" : L"to") )
        {
            CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"col")
            {
                CP_XML_CONTENT(position.col);
            }
            
            CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"colOff")
            {
                CP_XML_CONTENT(static_cast<size_t>(position.colOff));
            }

            CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"row")
            {
                CP_XML_CONTENT(position.row);
            }

            CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"rowOff")
            {
                CP_XML_CONTENT(static_cast<size_t>(position.rowOff));
            }  
        }
    }
}

void xlsx_serialize_image(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
{
    CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(ns + L":pic")
        {                  
            CP_XML_NODE(ns + L":nvPicPr")
            {
                CP_XML_NODE(ns + L":cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);
                    CP_XML_ATTR(L"name", val.name);

					oox_serialize_action(CP_XML_STREAM(), val.action);

				}
                CP_XML_NODE(ns + L":cNvPicPr")
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

            CP_XML_NODE(ns + L":spPr")
            {
				val.serialize_xfrm(CP_XML_STREAM());

                CP_XML_NODE(L"a:prstGeom")
                {                   
                    CP_XML_ATTR(L"prst", L"rect");
                    CP_XML_NODE(L"a:avLst");
                }
				oox_serialize_ln(CP_XML_STREAM(), val.additional);
            } 			
			xlsx_serialize_text(CP_XML_STREAM(), val, ns);
        } 
    }  // CP_XML_WRITER  
}

void xlsx_serialize_shape(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
{
	CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(ns + L":sp")
        {                  
            CP_XML_NODE(ns + L":nvSpPr")
            {
                CP_XML_NODE(ns + L":cNvPr")
                {
                    CP_XML_ATTR(L"id", val.id);//числовое значение val.rId

                    CP_XML_ATTR(L"name", val.name);

					oox_serialize_action(CP_XML_STREAM(), val.action);
                }
				CP_XML_NODE(ns + L":cNvSpPr")//non visual properies (собственно тока 1 там)
				{
					if (val.sub_type == 1)CP_XML_ATTR(L"txBox", 1);
				}
            } // xdr:nv_Pr
			CP_XML_NODE(ns + L":spPr")
            {
				val.serialize_xfrm(CP_XML_STREAM());

				val.serialize_shape(CP_XML_STREAM());
				
				oox_serialize_ln(CP_XML_STREAM(),val.additional, val.lined);
            } // xdr:spPr
			
			xlsx_serialize_text(CP_XML_STREAM(), val, ns);
        } 
    }  // CP_XML_WRITER  
}
void xlsx_serialize_object(std::wostream & strm, _xlsx_drawing & val)
{//отображательная часть
	CP_XML_WRITER(strm)    
    {
        CP_XML_NODE(L"xdr:sp")
        {                  
            CP_XML_NODE(L"xdr:nvSpPr")
            {
                CP_XML_NODE(L"xdr:cNvPr")
                {
                    CP_XML_ATTR(L"id",		val.id);	//числовое значение val.rId
                    CP_XML_ATTR(L"name",	val.name);
                }
				CP_XML_NODE(L"xdr:cNvSpPr");

            } 
			CP_XML_NODE(L"xdr:spPr")
            {
				val.serialize_xfrm(CP_XML_STREAM());

				CP_XML_NODE(L"a:prstGeom")
				{        
					CP_XML_ATTR(L"prst", L"rect");
				}	
            }
        } 
    } 
}
void xlsx_serialize_group(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(ns + L":grpSp")
		{
            CP_XML_NODE(ns + L":nvGrpSpPr")
            {
                CP_XML_NODE(ns + L":cNvPr")
                {
                    CP_XML_ATTR(L"id",		val.id);
                    CP_XML_ATTR(L"name",	val.name);
                }
				CP_XML_NODE(ns + L":cNvGrpSpPr");
            }
			CP_XML_NODE(ns + L":grpSpPr")
			{
				val.serialize_xfrm(CP_XML_STREAM());

			} 
			CP_XML_STREAM() << val.content_group_;
		}
	}
}

void xlsx_serialize_chart(std::wostream & strm, _xlsx_drawing & val)
{//отображательная часть
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"xdr:graphicFrame")
        {                  
            CP_XML_NODE(L"xdr:nvGraphicFramePr")
            {
                CP_XML_NODE(L"xdr:cNvPr")
                {
                    CP_XML_ATTR(L"id",		val.id);
                    CP_XML_ATTR(L"name",	val.name);
                }

                CP_XML_NODE(L"xdr:cNvGraphicFramePr");
            } 
			val.serialize_xfrm(CP_XML_STREAM(), L"xdr");
			//oox_serialize_ln(CP_XML_STREAM(), val.additional);

            CP_XML_NODE(L"a:graphic")
            {                   
                CP_XML_NODE(L"a:graphicData")
				{
					CP_XML_ATTR(L"uri", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
					CP_XML_NODE(L"c:chart")
					{
						CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
						CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
						CP_XML_ATTR(L"r:id", val.objectId);
					}
				}
			}               
		}
    }
}
void xlsx_serialize(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
{
	if (val.type == typeShape)
	{
		xlsx_serialize_shape(strm, val, ns);
	}
	else if (val.type == typeImage)
	{
		xlsx_serialize_image(strm, val, ns);
	}
	else if (val.type == typeChart)
	{
		xlsx_serialize_chart(strm, val);
	}
	else if (val.type == typeGroupShape)
	{
		xlsx_serialize_group(strm, val, ns);
	}
	else if (val.type == typeOleObject ||
			 val.type == typeMsObject || 
			 val.type == typeControl)
	{
		xlsx_serialize_object(strm, val);
	}
}

void _xlsx_drawing::serialize(std::wostream & strm, const std::wstring & ns)
{
	if (inGroup) 
		return xlsx_serialize(strm, *this, ns);
	
	CP_XML_WRITER(strm)    
    {
		if (type_anchor == 1)
		{
			CP_XML_NODE(ns + L":twoCellAnchor")
			{
				CP_XML_ATTR(L"editAs", L"oneCell");//"absolute");oneCell

				from_.serialize	(CP_XML_STREAM());
				to_.serialize	(CP_XML_STREAM());

				xlsx_serialize	(CP_XML_STREAM(), *this, ns);				
				CP_XML_NODE(ns + L":clientData");
			}
		}
		else if (type_anchor == 2)
		{
			CP_XML_NODE(ns + L":absoluteAnchor")
			{
				CP_XML_NODE(ns + L":pos")
				{
					CP_XML_ATTR(L"x", x);
					CP_XML_ATTR(L"y", y);
				}
				CP_XML_NODE(ns + L":ext")
				{
					CP_XML_ATTR(L"cx", cx);
					CP_XML_ATTR(L"cy", cy);
				}
				xlsx_serialize(CP_XML_STREAM(), *this, ns);				
				CP_XML_NODE(ns + L":clientData");
			}
		}
		else if (type_anchor == 3)
		{
			CP_XML_NODE(ns + L":relSizeAnchor")
			{
				CP_XML_NODE(ns + L":from")
				{
					CP_XML_NODE(ns + L":x")
					{
						CP_XML_STREAM() << ((double)x / *owner_cx_);
					}
					CP_XML_NODE(ns + L":y")
					{
						CP_XML_STREAM() << ((double)y / *owner_cy_);
					}
				}
				CP_XML_NODE(ns + L":to")
				{
					_INT32 x1 = x + cx;
					_INT32 y1 = y + cy;

					CP_XML_NODE(ns + L":x")
					{
						CP_XML_STREAM() << ((double)x1 / *owner_cx_);
					}
					CP_XML_NODE(ns + L":y")
					{
						CP_XML_STREAM() << ((double)y1 / *owner_cy_);
					}
				}
				xlsx_serialize(CP_XML_STREAM(), *this, ns);				
			}			
		}

	 }
}


void _xlsx_drawing::serialize_object (std::wostream & strm)
{
	if (type != typeOleObject && type != typeMsObject) return;

	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"oleObject")
		{
			CP_XML_ATTR(L"r:id",	objectId);
			CP_XML_ATTR(L"shapeId", id);
			CP_XML_ATTR(L"progId",	objectProgId);
			
			CP_XML_NODE(L"objectPr")
			{
				CP_XML_ATTR(L"defaultSize", 0);
				if (fill.bitmap)
				{
					CP_XML_ATTR(L"r:id", fill.bitmap->rId);
				}
				CP_XML_NODE(L"anchor")
				{
					CP_XML_ATTR(L"moveWithCells", 1);

					from_.serialize	(CP_XML_STREAM(), L"");
					to_.serialize	(CP_XML_STREAM(), L"");

				}
			}
		}
	}

}

void _xlsx_drawing::serialize_control (std::wostream & strm)
{
	if (type != typeControl) return;
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"control")
		{
			CP_XML_ATTR(L"r:id",	objectId);
			CP_XML_ATTR(L"shapeId", id);
			//CP_XML_ATTR(L"name",	objectProgId);
			
			CP_XML_NODE(L"controlPr")
			{
				CP_XML_ATTR(L"defaultSize", 0);
				if (fill.bitmap)
				{
					CP_XML_ATTR(L"r:id", fill.bitmap->rId);
				}
				CP_XML_NODE(L"anchor")
				{
					CP_XML_ATTR(L"moveWithCells", 1);

					from_.serialize	(CP_XML_STREAM(), L"");
					to_.serialize	(CP_XML_STREAM(), L"");

				}
			}
		}
	}

}

}
}
