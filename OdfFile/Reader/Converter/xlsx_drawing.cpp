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

#include "../../DataTypes/custom_shape_types_convert.h"
#include "../../Formulas/formulasconvert.h"

#define OBJ_Group			0x0000
#define OBJ_Line			0x0001 
#define OBJ_Rectangle		0x0002 
#define OBJ_Oval			0x0003 
#define OBJ_Arc				0x0004 
#define OBJ_Text			0x0006 
#define OBJ_OfficeArt		0x001E  
#define OBJ_Polygon			0x0009 
#define OBJ_Picture			0x0008 
#define OBJ_Chart			0x0005   
#define OBJ_Button			0x0007 
#define OBJ_CheckBox		0x000B 
#define OBJ_RadioButton		0x000C  
#define OBJ_EditBox			0x000D 
#define OBJ_Label			0x000E 
#define OBJ_DialogBox		0x000F 
#define OBJ_SpinControl		0x0010 
#define OBJ_Scrollbar		0x0011 
#define OBJ_List			0x0012 
#define OBJ_GroupBox		0x0013 
#define OBJ_DropdownList	0x0014 
#define OBJ_Note			0x0019 

namespace cpdoccore {
namespace oox {

namespace {

std::wostream & operator << (std::wostream & strm, xlsx_drawing_position::type_t t)
{
    strm << (t == xlsx_drawing_position::from ? L"from" : L"to");
    return strm;
}

}
void xml_serialize_text(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
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

std::wstring xlsx_drawing_position::vml_serialize()
{
	//emu -> pt =  1 / 12700  =  72.0 / (360000.0 * 2.54);
	return	std::to_wstring(position.col) + L"," + std::to_wstring((int)(position.colOff / 12700)) + L"," +
			std::to_wstring(position.row) + L"," + std::to_wstring((int)(position.rowOff / 12700));
}
void xlsx_drawing_position::serialize(std::wostream & strm, const std::wstring & ns_title, const std::wstring & ns)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE( ns_title + (ns_title.empty() ? L"" : L":") + (type == xlsx_drawing_position::from ? L"from" : L"to") )
        {
            //CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"col")
			CP_XML_NODE(L"xdr:col")
			{
                CP_XML_CONTENT(position.col);
            }
            
            //CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"colOff")
			CP_XML_NODE(L"xdr:colOff")
            {
                CP_XML_CONTENT(static_cast<size_t>(position.colOff));
            }

            //CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"row")
			CP_XML_NODE(L"xdr:row")
            {
                CP_XML_CONTENT(position.row);
            }

			//CP_XML_NODE(ns + (ns.empty() ? L"" : L":") + L"rowOff")
			CP_XML_NODE(L"xdr:rowOff")
            {
                CP_XML_CONTENT(static_cast<size_t>(position.rowOff));
            }  
        }
    }
}

void xml_serialize_image(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
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

					if (val.hidden) CP_XML_ATTR(L"hidden", 1);

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
			xml_serialize_text(CP_XML_STREAM(), val, ns);
        } 
    }  // CP_XML_WRITER  
}

void xml_serialize_shape(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
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

					if (val.hidden) CP_XML_ATTR(L"hidden", 1);
						
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
			
			xml_serialize_text(CP_XML_STREAM(), val, ns);
        } 
    }  // CP_XML_WRITER  
}
void xml_serialize_object(std::wostream & strm, _xlsx_drawing & val)
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
 					
					if (val.hidden) CP_XML_ATTR(L"hidden", 1);
					
					if (false == val.objectId.empty())
					{
						CP_XML_NODE(L"a:extLst")
						{
							CP_XML_NODE(L"a:ext")
							{
								CP_XML_ATTR(L"uri", L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}");
								CP_XML_NODE(L"a14:compatExt")
								{
									CP_XML_ATTR(L"spid", L"_x0000_s" + std::to_wstring(val.id));
								}
							}
						}
					}
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
void xml_serialize_group(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
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
 				
					if (val.hidden) CP_XML_ATTR(L"hidden", 1);
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

void xml_serialize_chart(std::wostream & strm, _xlsx_drawing & val)
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
 					
					if (val.hidden) CP_XML_ATTR(L"hidden", 1);
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
void xml_serialize(std::wostream & strm, _xlsx_drawing & val, const std::wstring & ns)
{
	if (val.type == typeShape)
	{
		xml_serialize_shape(strm, val, ns);
	}
	else if (val.type == typeImage)
	{
		xml_serialize_image(strm, val, ns);
	}
	else if (val.type == typeChart)
	{
		xml_serialize_chart(strm, val);
	}
	else if (val.type == typeGroupShape)
	{
		xml_serialize_group(strm, val, ns);
	}
	else if (val.type == typeOleObject ||
			 val.type == typeMsObject || 
			 val.type == typeControl)
	{
		xml_serialize_object(strm, val);
	}
}


void _xlsx_drawing::serialize(std::wostream & strm, const std::wstring & ns)
{
	if (inGroup) 
		return xml_serialize(strm, *this, ns);
	
	CP_XML_WRITER(strm)    
    {
		if (type_anchor == 1)
		{
			CP_XML_NODE(ns + L":twoCellAnchor")
			{
				CP_XML_ATTR(L"editAs", L"oneCell");//"absolute");oneCell

				from_.serialize	(CP_XML_STREAM());
				to_.serialize	(CP_XML_STREAM());

				xml_serialize	(CP_XML_STREAM(), *this, ns);				
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
				xml_serialize(CP_XML_STREAM(), *this, ns);				
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
				xml_serialize(CP_XML_STREAM(), *this, ns);				
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

					from_.serialize	(CP_XML_STREAM(), L"", L"");
					to_.serialize	(CP_XML_STREAM(), L"", L"");
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

					from_.serialize	(CP_XML_STREAM(), L"", L"xdr");
					to_.serialize	(CP_XML_STREAM(), L"", L"xdr");

				}
			}
		}
	}
}
void _xlsx_drawing::serialize_vml(std::wostream & strm)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"v:shape")
		{
			CP_XML_ATTR(L"id", L"_x0000_s" + std::to_wstring(id));
			CP_XML_ATTR(L"type", sub_type == OBJ_Note ? L"#_x0000_t202" : L"#_x0000_t201");

			std::wstring color;
			
			if (fill.solid)
				color = fill.solid->color;

			if (!color.empty())
				CP_XML_ATTR(L"fillcolor", L"#" + color);

			CP_XML_ATTR(L"stroked", /*lined ? */L"t"/* : L"f"*/);

			std::wstring style = L"position:absolute";
			//style +="margin-left:414.3pt;margin-top:70.2pt;width:144.15pt;height:96.75pt";
			
			CP_XML_ATTR(L"style", style);
			CP_XML_NODE(L"v:shadow")
			{
				CP_XML_ATTR(L"on", L"t");
				CP_XML_ATTR(L"obscured", L"t");
				CP_XML_ATTR(L"color", L"black");
			}
			//CP_XML_NODE(L"w10:wrap")
			//{
			//	CP_XML_ATTR(L"type", L"none");
			//}
			vml_serialize_fill(CP_XML_STREAM(), fill);

			vml_serialize_ln(CP_XML_STREAM(), additional);

			vml_serialize_text(CP_XML_STREAM(), additional);
			
			CP_XML_NODE(L"x:ClientData")
			{
				switch(sub_type)
				{
				case OBJ_Button:		CP_XML_ATTR(L"ObjectType", L"Button");	break;
				case OBJ_CheckBox:		CP_XML_ATTR(L"ObjectType", L"Checkbox");break;
				case OBJ_RadioButton:	CP_XML_ATTR(L"ObjectType", L"Radio");	break;
				case OBJ_EditBox:		CP_XML_ATTR(L"ObjectType", L"Edit");	break;
				case OBJ_Label:			CP_XML_ATTR(L"ObjectType", L"Label");	break;
				case OBJ_DialogBox:		CP_XML_ATTR(L"ObjectType", L"Dialog");	break;
				case OBJ_SpinControl:	CP_XML_ATTR(L"ObjectType", L"Spin");	break;
				case OBJ_Scrollbar:		CP_XML_ATTR(L"ObjectType", L"Scroll");	break;
				case OBJ_List:			CP_XML_ATTR(L"ObjectType", L"List");	break;
				case OBJ_DropdownList:	CP_XML_ATTR(L"ObjectType", L"Drop");	break;
				case OBJ_Note:			CP_XML_ATTR(L"ObjectType", L"Note");	break;
				}

				CP_XML_NODE(L"x:MoveWithCells");
				CP_XML_NODE(L"x:SizeWithCells");
				CP_XML_NODE(L"x:Anchor")
				{
					CP_XML_STREAM() << from_.vml_serialize() << L"," << to_.vml_serialize();
				}
				CP_XML_NODE(L"x:AutoFill")
				{
					CP_XML_STREAM() << L"False";
				}

				_CP_OPT(int) base_col, base_row;
				GetProperty(additional, L"base_col", base_col);
				GetProperty(additional, L"base_row", base_row);

				if (base_row)
				{
					CP_XML_NODE(L"x:Row")
					{
						CP_XML_STREAM() << *base_row;
					}
				}
				if (base_col)
				{
					CP_XML_NODE(L"x:Column")
					{
						CP_XML_STREAM() << *base_col;
					}
				}
				_CP_OPT(std::wstring) sVal;
				GetProperty(additional, L"linked_cell", sVal);
				if (sVal)
				{
					formulasconvert::odf2oox_converter converter;
					std::wstring fmla = converter.convert_named_ref(*sVal, true, L" ", true);
					CP_XML_NODE(L"x:FmlaLink")
					{
						CP_XML_STREAM() << fmla;
					}
				}
				GetProperty(additional, L"cell_range", sVal);
				if (sVal)
				{
					formulasconvert::odf2oox_converter converter;
					std::wstring fmla = converter.convert_named_ref(*sVal, true, L" ", true);
					CP_XML_NODE(L"x:FmlaRange")
					{
						CP_XML_STREAM() << fmla;
					}
				}	
				_CP_OPT(bool) visible;
				GetProperty(additional, L"visible", visible);
				if (visible)
				{
					if (*visible == false)
					{
						CP_XML_NODE(L"x:Visible")
						{
							CP_XML_STREAM() << L"False";
						}
					}
					else
						CP_XML_NODE(L"x:Visible");
				}
				
				_CP_OPT(int) nVal;
				GetProperty(additional, L"min_value", nVal);
				if (nVal)
				{
					CP_XML_NODE(L"x:Min")
					{
						CP_XML_STREAM() << *nVal;
					}
				}
				GetProperty(additional, L"max_value", nVal);
				if (nVal)
				{
					CP_XML_NODE(L"x:Max")
					{
						CP_XML_STREAM() << *nVal;
					}
				}
				GetProperty(additional, L"step", nVal);
				if (nVal)
				{
					CP_XML_NODE(L"x:Inc")
					{
						CP_XML_STREAM() << *nVal;
					}
				}
				GetProperty(additional, L"page_step", nVal);
				if (nVal)
				{
					CP_XML_NODE(L"x:Page")
					{
						CP_XML_STREAM() << *nVal;
					}
				}
				GetProperty(additional, L"value", sVal);
				if (sVal)
				{
					CP_XML_NODE(L"x:Val")
					{
						CP_XML_STREAM() << *sVal;
					}
				}

				GetProperty(additional, L"orientation", sVal);
				if (sVal)
				{
					if (*sVal == L"horizontal")	CP_XML_NODE(L"x:Horiz");
					else						CP_XML_NODE(L"x:Vert");
				}
			}
		}
	}
}

}
}
