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

#include "docx_drawing.h"
#include <xml/simple_xml_writer.h>
#include "../../DataTypes/custom_shape_types_convert.h"

namespace cpdoccore {

namespace oox {


namespace {

}
void serialize_wrap_top_bottom(std::wostream & strm,  _docx_drawing const & val)
{
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"wp:wrapTopAndBottom")
		{
			if (val.margin_rect[3])CP_XML_ATTR(L"distB",val.margin_rect[3]);
			if (val.margin_rect[1])CP_XML_ATTR(L"distT",val.margin_rect[1]);
		}
	}
}

void serialize_wrap_square(std::wostream & strm, _docx_drawing const & val)
{
    std::wstring wrapText;
    if (val.styleWrap)
    {
        switch(val.styleWrap->get_type())
        {
		case odf_types::style_wrap::Parallel:
            wrapText = L"bothSides";
            break;
		case odf_types::style_wrap::Dynamic:
            wrapText = L"largest";
            break;
        default:
            wrapText = boost::lexical_cast<std::wstring>(*(val.styleWrap));
            break;
        }
    }
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"wp:wrapSquare")
		{
			if (!wrapText.empty())	CP_XML_ATTR(L"wrapText", wrapText);

			if (val.margin_rect[3])CP_XML_ATTR(L"distB",val.margin_rect[3]);
			if (val.margin_rect[1])CP_XML_ATTR(L"distT",val.margin_rect[1]);
		}
	}   
}
void serialize_null_extent(std::wostream & strm)
{
	 CP_XML_WRITER(strm)    
	{
		CP_XML_NODE(L"wp:effectExtent")
		{
			CP_XML_ATTR(L"l",0); CP_XML_ATTR(L"t",0);CP_XML_ATTR(L"r",0);CP_XML_ATTR(L"b",0);
		}
	 }
}
void serialize_wrap_extent(std::wostream & strm, _docx_drawing const & val)
{
    if (!val.styleWrap)return;

	 CP_XML_WRITER(strm)    
	{
		switch(val.styleWrap->get_type())
		{
			case odf_types::style_wrap::Parallel:
			case odf_types::style_wrap::Left:
			case odf_types::style_wrap::Right:
			case odf_types::style_wrap::Dynamic:
			case odf_types::style_wrap::None:
			{
				CP_XML_NODE(L"wp:effectExtent")
				{
					_CP_OPT(int) iVal;
					if (odf_reader::GetProperty(val.additional,L"border_width_left",iVal))		CP_XML_ATTR(L"l",iVal.get());
					if (odf_reader::GetProperty(val.additional,L"border_width_top",iVal))		CP_XML_ATTR(L"t",iVal.get());
					if (odf_reader::GetProperty(val.additional,L"border_width_right",iVal))		CP_XML_ATTR(L"r",iVal.get());
					if (odf_reader::GetProperty(val.additional,L"border_width_bottom",iVal))	CP_XML_ATTR(L"b",iVal.get());   
				}
			}break;
			default:
				serialize_null_extent( strm);  
		}
    }
//error case
}

void serialize_wrap(std::wostream & strm, _docx_drawing const & val)
{
	if (!val.styleWrap || val.styleWrap->get_type() != odf_types::style_wrap::Parallel)
	{
		serialize_wrap_extent(strm, val);
	}
	CP_XML_WRITER(strm)    
	{
		if (val.styleWrap)
		{
			switch(val.styleWrap->get_type())
			{
			case odf_types::style_wrap::Parallel: 
				{
					if (val.number_wrapped_paragraphs== 1)
					{
						serialize_null_extent(strm);
						CP_XML_NODE(L"wp:wrapNone");
					}   
					else
					{
						serialize_wrap_extent(strm, val);
						serialize_wrap_square(strm, val);
					}
				}
				break;
			case odf_types::style_wrap::Left:
			case odf_types::style_wrap::Right:
			case odf_types::style_wrap::Dynamic: //Présentation_de_tib.odt
				serialize_wrap_square(strm, val);
				break;
			case odf_types::style_wrap::RunThrough:
				CP_XML_NODE(L"wp:wrapNone");
				break;
			case odf_types::style_wrap::None:
				serialize_wrap_top_bottom(strm, val);
				break;
			default:
				CP_XML_NODE(L"wp:wrapTopAndBottom");
				break;
			}
		}
		else
		{
			CP_XML_NODE(L"wp:wrapTopAndBottom");
		}
	}
}

void _docx_drawing::serialize_text(std::wostream & strm)
{
	_CP_OPT(std::wstring) strTextContent;
	odf_reader::GetProperty(additional, L"text-content", strTextContent);

	CP_XML_WRITER(strm)
	{			
		if (strTextContent)
		{
			CP_XML_NODE(L"wps:txbx")
			{  
				CP_XML_NODE(L"w:txbxContent")
				{				
					const std::wstring & test_string = strTextContent.get();
					CP_XML_STREAM() << test_string;
				}
			}
		}
	}	

	serialize_bodyPr(strm, L"wps");
}

//--------------------------------------------------------------------
void docx_serialize_media_child(std::wostream & strm, _docx_drawing & val)
{
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"pic:pic")
		{
			CP_XML_ATTR(L"xmlns:pic",L"http://schemas.openxmlformats.org/drawingml/2006/picture");
			{
				CP_XML_NODE(L"pic:nvPicPr")
				{
					CP_XML_NODE(L"pic:cNvPr")
					{
						CP_XML_ATTR(L"id", val.id + 1);
						CP_XML_ATTR(L"name", val.name);
					
						//oox_serialize_action(CP_XML_STREAM(), val.action);
					}
					CP_XML_NODE(L"pic:cNvPicPr")
					{
						CP_XML_NODE(L"a:picLocks") CP_XML_ATTR(L"noChangeAspect",1);
					}
					CP_XML_NODE(L"pic:nvPr")
					{
						std::wstring strNode; 
						
						if		(val.type == typeVideo)	strNode = L"a:videoFile"; 
						else if (val.type == typeAudio) strNode = L"a:audioFile"; 

						if (false == strNode.empty())
						{
							CP_XML_NODE(strNode)
							{
								CP_XML_ATTR(L"r:link",	val.objectId);
							}
						}
						if (false == val.extId.empty())
						{
							CP_XML_NODE(L"a:extLst")
							{
								CP_XML_NODE(L"a:ext")
								{
									CP_XML_ATTR(L"uri",	L"{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}");
									CP_XML_NODE(L"wp15:media")
									{	
										CP_XML_ATTR(L"xmlns:wp15", L"http://schemas.microsoft.com/office/word/2012/wordprocessingDrawing");
										if (val.extExternal)
										{
											CP_XML_ATTR(L"r:link",	val.extId);
										}
										else
										{
											CP_XML_ATTR(L"r:embed",	val.extId);
										}
									}
								}
							}
						}
					}
				}    
			}
			val.fill.bitmap->name_space = L"pic";
			oox_serialize_fill(CP_XML_STREAM(), val.fill);

			CP_XML_NODE(L"pic:spPr")
			{
				val.serialize_xfrm(CP_XML_STREAM());

				CP_XML_NODE(L"a:prstGeom")
				{
					CP_XML_ATTR(L"prst", L"rect");
					CP_XML_NODE(L"a:avLst");
				}

				oox_serialize_ln(CP_XML_STREAM(), val.additional);
				oox_serialize_effects(CP_XML_STREAM(), val.additional);
			}
		}
	}
}
//--------------------------------------------------------------------
void docx_serialize_image_child(std::wostream & strm, _docx_drawing & val)
{
    CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"pic:pic")
		{
			CP_XML_ATTR(L"xmlns:pic",L"http://schemas.openxmlformats.org/drawingml/2006/picture");
			{
				CP_XML_NODE(L"pic:nvPicPr")
				{
					CP_XML_NODE(L"pic:cNvPr")
					{
						//CP_XML_ATTR(L"desc text",L"");
						CP_XML_ATTR(L"id", val.id + 1);
						CP_XML_ATTR(L"name", val.name);
					
						//oox_serialize_action(CP_XML_STREAM(), val.action);
					}
					CP_XML_NODE(L"pic:cNvPicPr")
					{
						CP_XML_NODE(L"a:picLocks") CP_XML_ATTR(L"noChangeAspect",1);
					}
				}    
			}
			val.fill.bitmap->name_space = L"pic";
			oox_serialize_fill(CP_XML_STREAM(), val.fill);

			CP_XML_NODE(L"pic:spPr")
			{
				val.serialize_xfrm(CP_XML_STREAM());

				CP_XML_NODE(L"a:prstGeom")
				{
					CP_XML_ATTR(L"prst", L"rect");
					CP_XML_NODE(L"a:avLst");
				}

				oox_serialize_ln(CP_XML_STREAM(), val.additional);
				oox_serialize_effects(CP_XML_STREAM(), val.additional);
			}
		}
	}
}
void docx_serialize_shape_child(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"wps:wsp")
		{
			CP_XML_NODE(L"wps:cNvPr")
			{
				CP_XML_ATTR(L"id",	val.id + 1);
				CP_XML_ATTR(L"name",val.name);
			}
			CP_XML_NODE(L"wps:cNvSpPr")
			{
				if (val.sub_type==1 || val.sub_type==0)//frame
				{
					CP_XML_ATTR(L"txBox", 1);
				}
				if (val.inGroup == false)
				{
					CP_XML_NODE(L"a:spLocks")
					{
						CP_XML_ATTR(L"noChangeAspect", 1);
					}
				}
			}		
			CP_XML_NODE(L"wps:spPr")
			{
				val.serialize_xfrm	(CP_XML_STREAM());
				val.serialize_shape	(CP_XML_STREAM());

				oox_serialize_ln(CP_XML_STREAM(), val.additional);
				oox_serialize_effects(CP_XML_STREAM(), val.additional);
			} 
			val.serialize_text(CP_XML_STREAM());
		}
	}
}
void docx_serialize_group_child(std::wostream & strm, _docx_drawing & val)
{
	std::wstring	node_name = L"wpg:wgp";
	if (val.inGroup)node_name = L"wpg:grpSp";
	
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(node_name)
		{
			CP_XML_NODE(L"wpg:cNvGrpSpPr")
			{
				CP_XML_ATTR(L"bwMode", L"auto");
			}

			CP_XML_NODE(L"wpg:grpSpPr")
			{
				val.serialize_xfrm(CP_XML_STREAM());

			} 
			CP_XML_STREAM() << val.content_group_;
		}
	}
}

void docx_serialize_chart_child(std::wostream & strm, _docx_drawing & val)
{	 
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"c:chart")
		{
			CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"r:id", val.objectId);
		}
	}
}

void docx_serialize_common(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"wp:docPr")
		{
			CP_XML_ATTR(L"name",	val.name);
			CP_XML_ATTR(L"id",		0xf000 + val.id + 1);
			
			oox_serialize_action(CP_XML_STREAM(), val.action);
		}

		CP_XML_NODE(L"wp:cNvGraphicFramePr")
		{
			//CP_XML_NODE(L"a:graphicFrameLocks")
			//{
			//	CP_XML_ATTR(L"xmlns:a",L"http://schemas.openxmlformats.org/drawingml/2006/main");
			//	CP_XML_ATTR(L"noChangeAspect",1);
			//}
		}
		CP_XML_NODE(L"a:graphic")
		{
			CP_XML_ATTR(L"xmlns:a",L"http://schemas.openxmlformats.org/drawingml/2006/main");
			CP_XML_NODE(L"a:graphicData")
			{
				if (val.type == typeShape)
				{
					CP_XML_ATTR(L"uri",L"http://schemas.microsoft.com/office/word/2010/wordprocessingShape");
					docx_serialize_shape_child(CP_XML_STREAM(), val);
				}
				else if (val.type == typeGroupShape)
				{
					CP_XML_ATTR(L"uri", L"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup");
					docx_serialize_group_child(CP_XML_STREAM(), val);
				}				
				else if (val.type == typeAudio || val.type == typeVideo || val.type == typeMedia)
				{
					CP_XML_ATTR(L"uri",L"http://schemas.openxmlformats.org/drawingml/2006/picture");
					docx_serialize_media_child(CP_XML_STREAM(), val);
				}
				else if (val.type == typeImage)
				{
					CP_XML_ATTR(L"uri",L"http://schemas.openxmlformats.org/drawingml/2006/picture");
					docx_serialize_image_child(CP_XML_STREAM(), val);
				}
				else if (val.type == typeChart)
				{
					CP_XML_ATTR(L"uri", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
					docx_serialize_chart_child(CP_XML_STREAM(), val);
				}
	        } 
		}
	}
}
void docx_serialize_child(std::wostream & strm, _docx_drawing & val)
{
	if (val.type == typeShape)
	{
		docx_serialize_shape_child(strm,val);
	}
	else if (val.type == typeImage)
	{
		docx_serialize_image_child(strm,val);
	}
	else if (val.type == typeChart)
	{
		docx_serialize_chart_child(strm,val);
	}
	else if (val.type == typeGroupShape)
	{
		docx_serialize_group_child(strm,val);
	}
}

void docx_serialize_wps(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"w:drawing")
		{
			std::wstring	 strWp = L"wp:anchor";
			if (val.isInline)strWp = L"wp:inline";
			CP_XML_NODE(strWp)
			{		
				if (val.isInline)
				{
					CP_XML_NODE(L"wp:extent")
					{
						if (val.cx > 0 || val.cy > 0)
						{
							CP_XML_ATTR(L"cx", val.cx);
							CP_XML_ATTR(L"cy", val.cy);
						}

					}
					serialize_null_extent(CP_XML_STREAM());
				}
				else//anchor
				{
					CP_XML_ATTR(L"simplePos",0);
					CP_XML_ATTR(L"locked",0);
					CP_XML_ATTR(L"layoutInCell",1);
					CP_XML_ATTR(L"allowOverlap",1);

					CP_XML_ATTR(L"relativeHeight" ,val.relativeHeight);
					CP_XML_ATTR(L"behindDoc" , val.behindDoc);
			        
					CP_XML_ATTR(L"distL", val.margin_rect[0]);
					CP_XML_ATTR(L"distT", val.margin_rect[1]);
					CP_XML_ATTR(L"distR", val.margin_rect[2]);
					CP_XML_ATTR(L"distB", val.margin_rect[3]);

					CP_XML_NODE(L"wp:simplePos")
					{
						CP_XML_ATTR(L"x",0);
						CP_XML_ATTR(L"y",0);
					}

					CP_XML_NODE(L"wp:positionH")
					{
						std::wstring relativeFrom = L"margin";
						if (val.styleHorizontalRel) relativeFrom =val.styleHorizontalRel->get_type_str();
						
						CP_XML_ATTR(L"relativeFrom", relativeFrom);

						if (val.styleHorizontalPos &&
							val.styleHorizontalPos->get_type() != odf_types::horizontal_pos::FromLeft &&
							val.styleHorizontalPos->get_type() != odf_types::horizontal_pos::Outside)
						{
							CP_XML_NODE(L"wp:align") {CP_XML_STREAM() << (*val.styleHorizontalPos);}

						}
						else
						{
							CP_XML_NODE(L"wp:posOffset") {CP_XML_STREAM() << val.posOffsetH;}
						}			     
					}

					CP_XML_NODE(L"wp:positionV")
					{
						std::wstring relativeFrom = L"margin";
						if (val.styleVerticalRel)relativeFrom = val.styleVerticalRel->get_type_str();    

						CP_XML_ATTR(L"relativeFrom", relativeFrom);

						if (val.styleVerticalPos && 
							val.styleVerticalPos->get_type() != odf_types::vertical_pos::FromTop &&
							val.styleVerticalPos->get_type() != odf_types::vertical_pos::Below)
						{
							CP_XML_NODE(L"wp:align")
							{
								CP_XML_STREAM() << val.styleVerticalPos->get_type_str();
							}
						}
						else
						{
							CP_XML_NODE(L"wp:posOffset")
							{
								CP_XML_STREAM() << val.posOffsetV;                    
							}
						}
					}
					if (val.cx > 0 || val.cy > 0)
					{
						CP_XML_NODE(L"wp:extent")
						{
							CP_XML_ATTR(L"cx", val.cx); 
							CP_XML_ATTR(L"cy", val.cy);
						}
					}

					serialize_wrap(CP_XML_STREAM(), val);
				}
					
				docx_serialize_common(CP_XML_STREAM(), val);

				if (val.pctWidth)
				{
					std::wstring relativeFrom = L"margin";
					if (val.styleHorizontalRel) relativeFrom =val.styleHorizontalRel->get_type_str();

					if (relativeFrom == L"column") relativeFrom = L"margin";
					
					CP_XML_NODE(L"wp14:sizeRelH")
					{
						CP_XML_ATTR(L"relativeFrom", relativeFrom);
						CP_XML_NODE(L"wp14:pctWidth")
						{
							CP_XML_STREAM() << (val.pctWidth.get() * 1000);
						}
					}				
				}
				if (val.pctHeight)
				{
					std::wstring relativeFrom = L"paragraph";
					if (val.styleVerticalRel)relativeFrom = val.styleVerticalRel->get_type_str();					
					
					if (relativeFrom == L"paragraph") relativeFrom = L"margin";     

					CP_XML_NODE(L"wp14:sizeRelV")
					{
						CP_XML_ATTR(L"relativeFrom", relativeFrom);
						CP_XML_NODE(L"wp14:pctHeight")
						{
							CP_XML_STREAM() << (val.pctHeight.get() * 1000);
						}
					}
				}
			}
		}
	}
}

void docx_serialize_vml(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"w:pict")
		{
			CP_XML_NODE(L"v:rect")
			{
				std::wstring strStyle =	  L"position:absolute;\
visibility:visible;\
mso-wrap-style:square;\
mso-position-horizontal:absolute;\
mso-position-horizontal-relative:text;\
mso-position-vertical:absolute;\
mso-position-vertical-relative:text;";
				strStyle += L"margin-left:" + boost::lexical_cast<std::wstring>(val.x / 12700.) + L"pt;";
				strStyle += L"margin-top:"  + boost::lexical_cast<std::wstring>(val.y / 12700.) + L"pt;";
				strStyle += L"width:"		+ boost::lexical_cast<std::wstring>(val.cx / 12700.) + L"pt;"; 
				strStyle += L"height:"		+ boost::lexical_cast<std::wstring>(val.cy / 12700.) + L"pt;"; 
				strStyle += L"z-index:"		+ std::to_wstring(0xF000800 - val.id);
			
				CP_XML_ATTR(L"id",				L"Rect"		+ std::to_wstring(val.id));
				CP_XML_ATTR(L"o:spid",			L"_x0000_s" + std::to_wstring(1024 + val.id));
				CP_XML_ATTR(L"style",			strStyle);
				CP_XML_ATTR(L"fillcolor",		L"#4f81bd [3204]");
				CP_XML_ATTR(L"strokecolor",		L"#243f60 [1604]");
				CP_XML_ATTR(L"strokeweight",	L"1pt");
			}
		}
	}
}

void docx_serialize_alt_content(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"mc:AlternateContent")
		{
			CP_XML_NODE(L"mc:Choice")
			{
				CP_XML_ATTR(L"Requires", L"wps"); 
				docx_serialize_wps(CP_XML_STREAM(), val);
			}
			CP_XML_NODE(L"mc:Fallback")
			{
				docx_serialize_vml(CP_XML_STREAM(), val);
			}
		}
	}
}
void docx_serialize_object(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"w:object")
		{
			CP_XML_NODE(L"v:shape")
			{
				CP_XML_ATTR(L"id", L"ole_" + val.objectId); 

				CP_XML_ATTR(L"o:ole", "");
				std::wstring style_str;	// = L"width:730.6pt; height:261.8pt";

				style_str += L"width:"	+ boost::lexical_cast<std::wstring>(val.cx / 12700.) + L"pt;";
				style_str += L"height:" + boost::lexical_cast<std::wstring>(val.cy / 12700.) + L"pt;";

				CP_XML_ATTR(L"style", style_str);

				if (val.fill.bitmap)
				{
					CP_XML_NODE(L"v:imagedata")
					{
						CP_XML_ATTR(L"o:title", val.name);
						CP_XML_ATTR(L"r:id",	val.fill.bitmap->rId);
					}
				}
			}
			CP_XML_NODE(L"o:OLEObject")
			{
				CP_XML_ATTR(L"Type",		L"Embed");
				CP_XML_ATTR(L"ProgID",		val.objectProgId);
				CP_XML_ATTR(L"ShapeID",		L"ole_" + val.objectId); 
				CP_XML_ATTR(L"DrawAspect",	L"Content" );
				CP_XML_ATTR(L"ObjectID",	0x583A3000 + val.id );
				CP_XML_ATTR(L"r:id",		val.objectId); 
			}
		}
	}
}

void docx_serialize_control(std::wostream & strm, _docx_drawing & val)
{
	CP_XML_WRITER(strm)    
    {
		CP_XML_NODE(L"w:pict")
		{
			CP_XML_NODE(L"v:shape")
			{
				CP_XML_ATTR(L"id", L"control_" + val.objectId); 

				std::wstring style_str;	// = L"width:730.6pt; height:261.8pt";

				style_str += L"width:"	+ boost::lexical_cast<std::wstring>(val.cx / 12700.) + L"pt;";
				style_str += L"height:" + boost::lexical_cast<std::wstring>(val.cy / 12700.) + L"pt;";

				CP_XML_ATTR(L"style", style_str);

				if (val.fill.bitmap)
				{
					CP_XML_NODE(L"v:imagedata")
					{
						CP_XML_ATTR(L"o:title", val.name);
						CP_XML_ATTR(L"r:id",	val.fill.bitmap->rId);
					}
				}
			}
			CP_XML_NODE(L"w:control")
			{
				CP_XML_ATTR(L"w:name",		val.name);
				CP_XML_ATTR(L"w:shapeid",	L"control_" + val.objectId); 
				CP_XML_ATTR(L"r:id",		val.objectId); 
			}
		}
	}
}

void _docx_drawing::serialize(std::wostream & strm/*, bool insideOtherDrawing*/)
{
	if (type == typeUnknown) return;

	if (inGroup)
		return docx_serialize_child(strm, *this);
	
	if (type == typeMsObject ||
		type == typeOleObject)
	{
		docx_serialize_object(strm, *this);
	}
	else if (type == typeControl)
	{
		docx_serialize_control(strm, *this);
	}
	else
	{
		//if (insideOtherDrawing)
			docx_serialize_wps(strm, *this);
		//else
		//	docx_serialize_alt_content(strm, val);
	}

}



}
}
