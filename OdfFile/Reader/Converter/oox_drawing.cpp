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

#include <boost/algorithm/string.hpp>

#include "oox_drawing.h"
#include <xml/simple_xml_writer.h>
#include "../../DataTypes/custom_shape_types_convert.h"

using namespace cpdoccore;

namespace svg_path
{
	void oox_serialize(std::wostream & strm, std::vector<_polyline> & path)
	{
		CP_XML_WRITER(strm)
		{
			for (size_t i = 0; i < path.size(); i++)
			{	
				oox_serialize(strm, path[i]);
			}
		}
	}
	void oox_serialize(std::wostream & strm, _polyline & val)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(val.command)
			{
				if (val.command == L"a:ArcTo")
				{
					if (val.points.size() > 0)
					{
						CP_XML_ATTR(L"wR", (int)(val.points[0].x.get()));
						CP_XML_ATTR(L"hR", (int)(val.points[0].y.get()));
					}	
					if (val.points.size() > 1)
					{
						CP_XML_ATTR(L"stAng", (int)(val.points[1].x.get() * 60000));
						CP_XML_ATTR(L"swAng", (int)(val.points[1].y.get() * 60000));
					}
				}
				else
				{
					for (size_t i = 0; i < val.points.size(); i++)
					{	
						oox_serialize(CP_XML_STREAM(), val.points[i]);
					}
				}
			}
		}
	}
	void oox_serialize(std::wostream & strm, _point & val)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"a:pt")
			{
				if (val.x)CP_XML_ATTR(L"x", (int)(val.x.get()));
				if (val.y)CP_XML_ATTR(L"y", (int)(val.y.get()));
			}
		}
	}

}

namespace cpdoccore {

static const std::wstring _ooxShapeType[]=
{
	L"rect", //frame
	L"rect", //text box
	L"rect", //shape
	L"ellipse",
	L"ellipse", 
	L"line", 
	L"path",
	L"custGeom",//uses sub-sub type,
	L"polygon", 
	L"roundRect", 
	L"bentConnector3",
	L"curvedConnector3",
	L"",//3-D shape
	L"",
	L"polyline",
	L"cube",
	L"ellipse",	//sphere
	L""
};


namespace oox {

static const std::wstring _ooxDashStyle[]=
{
	L"none",
	L"solid",
	L"dot",
	L"dash",
	L"dash",
	L"dashDot",
	L"sysDashDotDot"
};
static const std::wstring _vmlDashStyle[]=
{
	L"none",
	L"solid",
	L"dot",
	L"dash",
	L"dash",
	L"dashdot",
	L"shortdashdotdot"
};

void oox_serialize_effects(std::wostream & strm, const std::vector<odf_reader::_property> & prop)
{
	_CP_OPT(bool)			bShadow;
	_CP_OPT(std::wstring)	strShadowColor; 
	_CP_OPT(double)			dShadowOpacity; 
	_CP_OPT(double)			dShadowOffsetX; 
	_CP_OPT(double)			dShadowOffsetY; 

	odf_reader::GetProperty(prop, L"shadow", bShadow);	
	odf_reader::GetProperty(prop, L"shadow-color", strShadowColor);	
	odf_reader::GetProperty(prop, L"shadow-opacity", dShadowOpacity);
	odf_reader::GetProperty(prop, L"shadow-offset-x",	dShadowOffsetX);	
	odf_reader::GetProperty(prop, L"shadow-offset-y", dShadowOffsetY);

	CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"a:effectLst")
        { 
			if ((bShadow) && (*bShadow))
			{
				CP_XML_NODE(L"a:outerShdw")
				{ 			
					//CP_XML_ATTR(L"blurRad", 0); 

					double offsetX = dShadowOffsetX.get_value_or(0);
					double offsetY = dShadowOffsetY.get_value_or(0);

					double dist = sqrt(offsetX * offsetX + offsetY * offsetY);
					double dir = (offsetX != 0)  ? atan(offsetY / offsetX) * 180. / 3.1415926 : 0;
					if (dir < 0) dir += 360;

					CP_XML_ATTR(L"dist", (int)(dist)); 
					CP_XML_ATTR(L"dir", (int)(dir * 60000)); 
					
					CP_XML_ATTR(L"rotWithShape", L"0"); 
					CP_XML_ATTR(L"algn", L"tl"); 

					CP_XML_NODE(L"a:srgbClr")
					{
						if (strShadowColor)
						{
							CP_XML_ATTR(L"val", *strShadowColor); 
						}
						else
						{
							CP_XML_ATTR(L"val", L"000000"); 
						}
						if (dShadowOpacity)
						{
							CP_XML_NODE(L"a:alpha")
							{
								CP_XML_ATTR(L"val", (int)(*dShadowOpacity * 1000)); 
							}
						}
					}
				}
			}
		}
	}
}

void oox_serialize_ln(std::wostream & strm, const std::vector<odf_reader::_property> & prop, bool always_draw, const std::wstring &ns)
{
	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");
	std::wstring ns_node = L"a:ln";
	
	if (ns == L"w14")
		ns_node = L"w14:textOutline";

	_CP_OPT(std::wstring)	strStrokeColor; 
	_CP_OPT(int)			iStroke;
	_CP_OPT(double)			dStrokeWidth;
	_CP_OPT(double)			dStrokeOpacity; 
	_CP_OPT(bool)			bWordArt;
	
	odf_reader::GetProperty(prop, L"wordArt", bWordArt);
	
	odf_reader::GetProperty(prop, L"stroke-color"	, strStrokeColor);	
	odf_reader::GetProperty(prop, L"stroke"			, iStroke);	
	odf_reader::GetProperty(prop, L"stroke-width"	, dStrokeWidth);
	odf_reader::GetProperty(prop, L"stroke-opacity"	, dStrokeOpacity);
	
	if ((!strStrokeColor && !iStroke && !dStrokeWidth) && !always_draw)return;

	CP_XML_WRITER(strm)
    {
        CP_XML_NODE(ns_node)
        { 
			std::wstring color, dash_style, fill = ns + L":solidFill" ;

			if (strStrokeColor) color = *strStrokeColor;

			if (iStroke)
			{
				if (iStroke.get() == 0 || bWordArt) fill = ns + L":noFill";
				else dash_style =  _ooxDashStyle[iStroke.get()];	
			}
			
			if ((dStrokeWidth) && (*dStrokeWidth >= 0) && fill != ns + L":noFill")
			{
				int val = dStrokeWidth.get() * 12700;	//in emu (1 pt = 12700)
				if (val < 10)	val = 12700;
				
				CP_XML_ATTR2(ns_att + L"w", val);
				if (color.length()<1)color = L"729FCF";
			}
		
			CP_XML_NODE(fill)
			{ 			
				if (fill != ns + L":noFill")
				{
					if ( color.empty() )
					{
						if (always_draw)	color = L"000000";
						else				color = L"FFFFFF";
					}
					
					CP_XML_NODE(ns + L":srgbClr")
					{
						CP_XML_ATTR2(ns_att + L"val", color);
						
						if (dStrokeOpacity)	
						{
							CP_XML_NODE(ns + L":alpha")
							{
								CP_XML_ATTR2(ns_att + L"val", (int)(*dStrokeOpacity * 1000));
							}
						}
					}
				}
			}
			if (fill != ns + L":noFill")
			{
				_CP_OPT(std::wstring)	strVal;

				if (dash_style.length() > 0 && dash_style != L"solid")
				{
					CP_XML_NODE(ns + L":prstDash"){CP_XML_ATTR2(ns_att + L"val", dash_style);}	
				}
				odf_reader::GetProperty(prop,L"marker-start", strVal);	
				if (strVal)
				{
					CP_XML_NODE(ns + L":headEnd"){CP_XML_ATTR2(ns_att + L"type", strVal.get());}
				}
				odf_reader::GetProperty(prop,L"marker-end",strVal);	
				if (strVal)
				{
					CP_XML_NODE(ns + L":tailEnd"){CP_XML_ATTR2(ns_att + L"type",strVal.get());}
				}
			}
		}
    }
}
void vml_serialize_text(std::wostream & strm, const std::vector<odf_reader::_property> & prop)
{
	_CP_OPT(std::wstring) strTextContent;
	odf_reader::GetProperty(prop, L"text-content", strTextContent);

	CP_XML_WRITER(strm)
	{			
		if (strTextContent)
		{
			CP_XML_NODE(L"v:textbox")
			{  
				CP_XML_ATTR(L"style", L"mso-direction-alt:auto");
				CP_XML_ATTR(L"o:singleclick", L"f");
				
				CP_XML_NODE(L"div")
				{
					CP_XML_ATTR(L"style", L"text-align:left");

					CP_XML_NODE(L"font")
					{
						CP_XML_ATTR(L"face", L"Segoe UI");
						CP_XML_ATTR(L"size", L"160");
						CP_XML_ATTR(L"color", L"#000000");

						const std::wstring & test_string = strTextContent.get();
						CP_XML_STREAM() << test_string;
					}
				}
			}
		}
	}	

}
void vml_serialize_ln(std::wostream & strm, const std::vector<odf_reader::_property> & prop)
{
	_CP_OPT(std::wstring)	strStrokeColor; 
	_CP_OPT(int)			iStroke;
	_CP_OPT(double)			dStrokeWidth;
	_CP_OPT(double)			dStrokeOpacity; 
	_CP_OPT(bool)			bWordArt;
	
	odf_reader::GetProperty(prop, L"wordArt", bWordArt);
	
	odf_reader::GetProperty(prop, L"stroke-color"	, strStrokeColor);	
	odf_reader::GetProperty(prop, L"stroke"			, iStroke);	
	odf_reader::GetProperty(prop, L"stroke-width"	, dStrokeWidth);
	odf_reader::GetProperty(prop, L"stroke-opacity"	, dStrokeOpacity);

	if (!strStrokeColor && !iStroke && !dStrokeWidth) return;

	CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"v:stroke")
        { 
			std::wstring color, dash_style ;

			if (strStrokeColor) color = *strStrokeColor;

			if (iStroke)
			{
				if (iStroke.get() != 0 ) dash_style = _vmlDashStyle[iStroke.get()];	
			}
			
			if ((dStrokeWidth) && (*dStrokeWidth >= 0))
			{
				int val = dStrokeWidth.get() * 12700;	//in emu (1 pt = 12700)
				if (val < 10)	val = 12700;
				
				CP_XML_ATTR(L"weight", val);
			}

			if (false == color.empty())
			{
				CP_XML_ATTR(L"color", L"#" + color);
			}
			if (!dash_style.empty() && dash_style != L"solid")
			{
				CP_XML_ATTR(L"dashstyle", dash_style);
			}
			//odf_reader::GetProperty(prop,L"marker-start", strVal);	
			//if (strVal)
			//{
			//}
			//odf_reader::GetProperty(prop,L"marker-end",strVal);	
			//if (strVal)
			//{
			//}

			CP_XML_ATTR(L"startarrow", L"block"); 
			CP_XML_ATTR(L"startarrowwidth", L"medium");  
			CP_XML_ATTR(L"startarrowlength", L"medium"); 
			CP_XML_ATTR(L"joinstyle", L"round");  
			CP_XML_ATTR(L"endcap", L"flat"); 
		}
    }
}
void oox_serialize_aLst(std::wostream & strm, const std::vector<odf_reader::_property> & prop, const std::wstring & shapeGeomPreset, const std::wstring &ns)
{
	std::wstring ns_att = (ns == L"a" ? L"" : ns + L":");

	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(ns + L":avLst")
		{
			_CP_OPT(std::wstring) strModifiers;
			odf_reader::GetProperty(prop, L"oox-draw-modifiers", strModifiers);
			
			if (strModifiers)
			{
				std::vector< std::wstring > values;
				boost::algorithm::split(values, strModifiers.get(), boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

				if (!values.empty() && values.back().empty()) values.pop_back();

				std::vector<std::wstring> names;
			
				if (std::wstring::npos != shapeGeomPreset.find(L"math") ||
					std::wstring::npos != shapeGeomPreset.find(L"bentConnector") || 
					std::wstring::npos != shapeGeomPreset.find(L"curvedConnector")|| 
					std::wstring::npos != shapeGeomPreset.find(L"frame"))
				{
					names.push_back(L"adj1");
				}
				else if (std::wstring::npos != shapeGeomPreset.find(L"heptagon") ||
						 std::wstring::npos != shapeGeomPreset.find(L"decagon"))
				{
					values.clear();
				}
				else if (std::wstring::npos != shapeGeomPreset.find(L"decagon"))
				{
					names.push_back(L"vf");
				}
				else if (std::wstring::npos != shapeGeomPreset.find(L"pentagon"))
				{
					names.push_back(L"hf");
					names.push_back(L"vf");
				}
				else if (std::wstring::npos != shapeGeomPreset.find(L"hexagon"))
				{
					names.push_back(L"adj");
					names.push_back(L"vf");
				}
				else if (std::wstring::npos != shapeGeomPreset.find(L"star5")|| 
						 std::wstring::npos != shapeGeomPreset.find(L"star7"))
				{
					names.push_back(L"adj");
					names.push_back(L"hf");
					names.push_back(L"vf");
				}
				else if (std::wstring::npos != shapeGeomPreset.find(L"star6") ||
						 std::wstring::npos != shapeGeomPreset.find(L"star10"))
				{
					names.push_back(L"adj");
					names.push_back(L"hf");
				}

				for (size_t i = 0; i < values.size(); i++)
				{
					if (values[i].empty()) continue;
					
					CP_XML_NODE(ns + L":gd")
					{
						if (names.size() > i)
						{
							CP_XML_ATTR2(ns_att + L"name", names[i]);
						}
						else
						{
							if (values.size() > 1)
								CP_XML_ATTR2(ns_att + L"name", L"adj" + std::to_wstring(i + 1));
							else
								CP_XML_ATTR2(ns_att + L"name", L"adj");
						}						
						CP_XML_ATTR2(ns_att + L"fmla", L"val " + values[i]);
					}
				}
			}
		}
	}
}
void _oox_drawing::serialize_bodyPr(std::wostream & strm, const std::wstring & namespace_)
{
	const std::vector<odf_reader::_property> & prop = additional;

	_CP_OPT(bool)	bWordArt;
	odf_reader::GetProperty(prop,L"wordArt", bWordArt);

	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(namespace_ + L":bodyPr")
		{
			_CP_OPT(double)dPaddingLeft, dPaddingRight, dPaddingTop, dPaddingBottom;
			odf_reader::GetProperty(prop,L"text-padding-left"	, dPaddingLeft);
			odf_reader::GetProperty(prop,L"text-padding-right"	, dPaddingRight);
			odf_reader::GetProperty(prop,L"text-padding-top"	, dPaddingTop);
			odf_reader::GetProperty(prop,L"text-padding-bottom"	, dPaddingBottom);
			
			if (dPaddingLeft)	CP_XML_ATTR(L"lIns", (int)(*dPaddingLeft));
			if (dPaddingTop)	CP_XML_ATTR(L"tIns", (int)(*dPaddingTop));
			if (dPaddingRight)	CP_XML_ATTR(L"rIns", (int)(*dPaddingRight));
			if (dPaddingBottom)	CP_XML_ATTR(L"bIns", (int)(*dPaddingBottom));

			if (inGroup == false)
			{
				_CP_OPT(int)	iWrap;
				odf_reader::GetProperty(prop, L"text-wrap"	, iWrap);
				if ((iWrap) && (*iWrap == 0)) CP_XML_ATTR(L"wrap", L"none");
			}

			_CP_OPT(int) iAlign, iVert;
			odf_reader::GetProperty(prop, L"textarea-vertical_align", iAlign);
			if (iAlign)
			{
				switch (iAlign.get())
				{
				case 0://Baseline,
					CP_XML_ATTR(L"anchor", L"dist");break;
				case 1://Top,
				case 4://Auto,
					CP_XML_ATTR(L"anchor", L"t");break;
				case 2://Middle,
					CP_XML_ATTR(L"anchor", L"ctr");break;
				case 3://Bottom,
					CP_XML_ATTR(L"anchor", L"b");break;
				case 5://Justify
					CP_XML_ATTR(L"anchor", L"just");break;
				}
			}
			odf_reader::GetProperty(prop, L"text_vert", iVert);
			if (iVert)
			{
				switch (iVert.get())
				{
				case 1:	CP_XML_ATTR(L"vert", L"vert");		break;
				case 2:	CP_XML_ATTR(L"vert", L"vert270");	break;

				}
			}			
			_CP_OPT(bool) bAutoGrowHeight;
			_CP_OPT(bool) bFitToSize;
			odf_reader::GetProperty(prop,L"fit-to-size", bFitToSize);
			odf_reader::GetProperty(prop,L"auto-grow-height", bAutoGrowHeight);

			if ((bAutoGrowHeight) && (*bAutoGrowHeight))
			{
				CP_XML_NODE(L"a:spAutoFit");
			}
			else if ((bFitToSize) && (*bFitToSize))
			{
				CP_XML_NODE(L"a:normAutofit");
				//{
				//	CP_XML_ATTR(L"lnSpcReduction", 10000);
				//}
			}
			//else CP_XML_ATTR(L"anchor", L"dist");break;
			if (bWordArt)
			{
				_CP_OPT(int) iVal;
				odf_reader::GetProperty(prop, L"oox-geom-index", iVal);
				if (iVal)
				{
					std::wstring shapeType = _OO_OOX_wordart[*iVal].oox;					
					CP_XML_NODE(L"a:prstTxWarp")
					{
						CP_XML_ATTR(L"prst", shapeType);
						
						oox_serialize_aLst(CP_XML_STREAM(), prop, shapeType);
					}
				}
			}
		}
	}
}

void _oox_drawing::serialize_shape(std::wostream & strm)
{
	_CP_OPT(int)			iOoxShapeIndex;
	_CP_OPT(bool)			bWordArt, bOoxShape;
	_CP_OPT(std::wstring)	sCustomPath;
	
	odf_reader::GetProperty(additional, L"wordArt",			bWordArt);
	odf_reader::GetProperty(additional, L"oox-geom-index",	iOoxShapeIndex);
	odf_reader::GetProperty(additional, L"oox-geom",		bOoxShape);

	odf_reader::GetProperty(additional, L"custom_path", sCustomPath);
	
	std::wstring shapeGeomPreset;

	if (sub_type == 7)//custom 
	{
		if (iOoxShapeIndex)
			shapeGeomPreset = _OO_OOX_custom_shapes[*iOoxShapeIndex].oox;	
		else if (sCustomPath)
			sub_type = 6; //path

		if (shapeGeomPreset == L"textBox")
		{
			sub_type = 1;
			shapeGeomPreset = L"rect";
		}
	}
	else if (sub_type <= 16 && sub_type >= 0)
	{
		shapeGeomPreset = _ooxShapeType[sub_type]; //odf -> oox
	} 
	
	if (bWordArt) sub_type = 1;


	CP_XML_WRITER(strm)
    {
		if (sub_type == 6 || sub_type == 8 || sub_type == 14)
		{
			CP_XML_NODE(L"a:custGeom")
			{        
				std::vector<std::wstring> names;
				oox_serialize_aLst(CP_XML_STREAM(), additional, L"");
				
				CP_XML_NODE(L"a:ahLst");
				CP_XML_NODE(L"a:gdLst");
				CP_XML_NODE(L"a:rect")
				{
					CP_XML_ATTR(L"b", L"b");
					CP_XML_ATTR(L"l", 0);
					CP_XML_ATTR(L"r", L"r");
					CP_XML_ATTR(L"t", 0);
				}
				//<a:rect b="b" l="0" r="r" t="0"/>
				_CP_OPT(int) w, h;
				_CP_OPT(bool) stroke;
				odf_reader::GetProperty(additional, L"custom_path_w", w);
				odf_reader::GetProperty(additional, L"custom_path_h", h);

				odf_reader::GetProperty(additional, L"custom_path_s", stroke);
					

				CP_XML_NODE(L"a:pathLst")
				{ 	
					CP_XML_NODE(L"a:path")
					{
						int path_w = w ? *w : cx;
						int path_h = h ? *h : cy;

						if (path_w < 1) path_w = 1024;  
						if (path_h < 1) path_h = 1024; 

						CP_XML_ATTR(L"w", path_w);
						CP_XML_ATTR(L"h", path_h);

						if (stroke)
						{
							CP_XML_ATTR(L"stroke", *stroke ? 1 : 0);
						}
						
						if (sCustomPath)
						{	
							CP_XML_STREAM() << *sCustomPath;
						}
					}
				}       
			}
		}
		else
		{
			if (shapeGeomPreset.empty())
			{
				shapeGeomPreset	= L"rect";
				sub_type	= 2;
			}
			CP_XML_NODE(L"a:prstGeom")//автофигура
			{        
				CP_XML_ATTR(L"prst", shapeGeomPreset);
				if (!bWordArt) 
				{
					oox_serialize_aLst(CP_XML_STREAM(), additional, shapeGeomPreset);
				}
			}					
		}
		if (bWordArt || lined)
		{
			_oox_fill no_fill;
			oox_serialize_fill(strm, no_fill);
		}
		else
			oox_serialize_fill(strm, fill);

	}
}

void _oox_drawing::serialize_xfrm(std::wostream & strm, const std::wstring & name_space, bool always_position)
{
	CP_XML_WRITER(strm)
    {
		std::wstring xfrm = name_space + L":xfrm";

		_CP_OPT(double) dRotate;
		odf_reader::GetProperty(additional, L"svg:rotate", dRotate);
	
		_CP_OPT(double) dSkewX;
		odf_reader::GetProperty(additional, L"svg:skewX", dSkewX);		

		_CP_OPT(double) dSkewY;
		odf_reader::GetProperty(additional, L"svg:skewY", dSkewY);	
		
		CP_XML_NODE(xfrm)
		{      
			if (dRotate)
			{
				double d = 360 - dRotate.get() * 180. / 3.14159265358979323846;
				CP_XML_ATTR(L"rot", (int)( d * 60000) ); //60 000 per 1 gr - 19.5.5 oox 
			}
			_CP_OPT(bool)bVal;
			if (odf_reader::GetProperty(additional, L"flipH", bVal))
				CP_XML_ATTR(L"flipH", bVal.get());

			if (odf_reader::GetProperty(additional,L"flipV", bVal))
				CP_XML_ATTR(L"flipV", bVal.get());

			CP_XML_NODE(L"a:off") 
			{
				if (inGroup || always_position)
				{	
					CP_XML_ATTR(L"x", x);
					CP_XML_ATTR(L"y", y);
				}
				else
				{
					CP_XML_ATTR(L"x", 0);
					CP_XML_ATTR(L"y", 0);
				}
			}

			if (cx > 0 || cy > 0)
			{
				CP_XML_NODE(L"a:ext")
				{
					CP_XML_ATTR(L"cx", cx);
					CP_XML_ATTR(L"cy", cy);
				}
			}else
			{
                _CP_LOG << L"[error!!!] not set size object\n";
			}
			
			if (type == typeGroupShape)
			{		
				CP_XML_NODE(L"a:chOff")
				{
					CP_XML_ATTR(L"x", 0);
					CP_XML_ATTR(L"y", 0);
				}
				if (cx > 0 || cy > 0)
				{
					CP_XML_NODE(L"a:chExt")
					{
						CP_XML_ATTR(L"cx", cx);
						CP_XML_ATTR(L"cy", cy);
					}
				}
			}
		}
    }
}
void oox_serialize_action(std::wostream & strm, _action_desc const & val)
{
	if (val.enabled == false) return;

    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:hlinkClick")
		{
			if (!val.action.empty())
				CP_XML_ATTR(L"action", val.action);
			
			if (val.highlightClick)
				CP_XML_ATTR(L"highlightClick", val.highlightClick);
		
			CP_XML_ATTR(L"r:id", val.hId);
			
			if (!val.hSoundId.empty())
			{
				CP_XML_NODE(L"a:snd")
				{
					CP_XML_ATTR(L"r:embed", val.hSoundId);
					CP_XML_ATTR(L"name", L"sound");
				}
			}
			//CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			//CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
		}
	}
}



}
}
