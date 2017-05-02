/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include <boost/algorithm/string.hpp>

#include "oox_drawing.h"
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../odf/datatypes/custom_shape_types_convert.h"

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

void oox_serialize_ln(std::wostream & strm, const std::vector<odf_reader::_property> & prop, bool always_draw)
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

	if ((!strStrokeColor && !iStroke && !dStrokeWidth) && !always_draw)return;

	CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"a:ln")
        { 
			std::wstring color, dash_style, fill = L"a:solidFill" ;

			if (strStrokeColor) color = *strStrokeColor;

			if (iStroke)
			{
				if (iStroke.get() == 0 || bWordArt) fill = L"a:noFill";
				else dash_style =  _ooxDashStyle[iStroke.get()];	
			}
			
			if ((dStrokeWidth) && (*dStrokeWidth >= 0) && fill != L"a:noFill")
			{
				int val = dStrokeWidth.get() * 12700;	//in emu (1 pt = 12700)
				if (val < 10)	val = 12700;
				
				CP_XML_ATTR(L"w", val);
				if (color.length()<1)color = L"729FCF";
			}
		
			CP_XML_NODE(fill)
			{ 			
				if (fill != L"a:noFill")
				{
					if ( color.empty() )
					{
						if (always_draw)	color = L"000000";
						else				color = L"ffffff";
					}
					
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",color);
						
						if (dStrokeOpacity)	
						{
							CP_XML_NODE(L"a:alpha")
							{
								CP_XML_ATTR(L"val", (int)(*dStrokeOpacity * 1000));
							}
						}
					}
				}
			}
			if (fill != L"a:noFill")
			{
				_CP_OPT(std::wstring)	strVal;

				if (dash_style.length() > 0 && dash_style != L"solid")
				{
					CP_XML_NODE(L"a:prstDash"){CP_XML_ATTR(L"val", dash_style);}	
				}
				odf_reader::GetProperty(prop,L"marker-start", strVal);	
				if (strVal)
				{
					CP_XML_NODE(L"a:headEnd"){CP_XML_ATTR(L"type", strVal.get());}
				}
				odf_reader::GetProperty(prop,L"marker-end",strVal);	
				if (strVal)
				{
					CP_XML_NODE(L"a:tailEnd"){CP_XML_ATTR(L"type",strVal.get());}
				}
			}
		}
    }
}
void oox_serialize_aLst(std::wostream & strm, const std::vector<odf_reader::_property> & prop)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:avLst")
		{
			_CP_OPT(bool) bModifiers;
			_CP_OPT(std::wstring) strModifiers;
			odf_reader::GetProperty(prop, L"bModifiers",		bModifiers);
			odf_reader::GetProperty(prop, L"oox-draw-modifiers", strModifiers);
			if (strModifiers)
			{
				std::vector< std::wstring > values;
				boost::algorithm::split(values, strModifiers.get(), boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

				if (!values.empty() && values.back().empty()) values.pop_back();

				for (size_t i = 0; i < values.size(); i++)
				{
					if (values[i].empty()) continue;
					CP_XML_NODE(L"a:gd")
					{
						if (values.size() > 1 || bModifiers)
							//весьма странное .. для некоторых модификаторов (напр math...) нужно указывать множественность их
							CP_XML_ATTR(L"name", L"adj" + std::to_wstring(i+1));
						else
							CP_XML_ATTR(L"name", L"adj");
						
						CP_XML_ATTR(L"fmla", L"val " + values[i]);
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
			if (dPaddingRight)	CP_XML_ATTR(L"rIns", (int)(*dPaddingRight));
			if (dPaddingTop)	CP_XML_ATTR(L"tIns", (int)(*dPaddingTop));
			if (dPaddingBottom)	CP_XML_ATTR(L"bIns", (int)(*dPaddingBottom));

			if (inGroup == false)
			{
				_CP_OPT(int)	iWrap;
				odf_reader::GetProperty(prop, L"text-wrap"	, iWrap);
				if ((iWrap) && (*iWrap == 0)) CP_XML_ATTR(L"wrap", L"none");
			}

			_CP_OPT(int) iAlign;
			odf_reader::GetProperty(prop,L"textarea-vertical_align",iAlign);
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
				CP_XML_NODE(L"a:noAutofit");
				//CP_XML_NODE(L"a:spAutoFit")
				//{
				//	CP_XML_ATTR(L"lnSpcReduction", 10000);
				//}
			}
			//else CP_XML_ATTR(L"anchor", L"dist");break;
			if (bWordArt)
			{
				_CP_OPT(int) iVal;
				odf_reader::GetProperty(prop, L"odf-custom-draw-index",iVal);
				if (iVal)
				{
					std::wstring shapeType = _OO_OOX_wordart[*iVal].oox;					
					CP_XML_NODE(L"a:prstTxWarp")
					{
						CP_XML_ATTR(L"prst", shapeType);
						oox_serialize_aLst(CP_XML_STREAM(), prop);
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
	else if (sub_type < 9 && sub_type >= 0)
	{
		shapeGeomPreset = _ooxShapeType[sub_type]; //odf -> oox
	} 
	
	if (bWordArt) sub_type = 1;


	CP_XML_WRITER(strm)
    {
		if (sub_type == 6 || sub_type == 8)
		{
			CP_XML_NODE(L"a:custGeom")
			{        
				oox_serialize_aLst(CP_XML_STREAM(), additional);
				
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
				odf_reader::GetProperty(additional, L"custom_path_w", w);
				odf_reader::GetProperty(additional, L"custom_path_h", h);
					
				CP_XML_NODE(L"a:pathLst")
				{ 	
					CP_XML_NODE(L"a:path")
					{
						CP_XML_ATTR(L"w", w ? *w : cx);
						CP_XML_ATTR(L"h", h ? *h : cy);
						
						CP_XML_STREAM() << *sCustomPath;
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
					if (std::wstring::npos != shapeGeomPreset.find(L"mathPlus"))
					{
						additional.push_back(odf_reader::_property(L"bModifiers", true));
					}
					oox_serialize_aLst(CP_XML_STREAM(), additional);
				}
			}					
		}
		if (bWordArt)
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

		_CP_OPT(double) dRotateAngle;
		
		if (dRotate || dSkewX || dSkewY)
		{
			double tmp=0;
			if (dRotate)tmp += *dRotate;
			//if (dSkewX)tmp += *dSkewX;
			//if (dSkewY)tmp += (*dSkewY) + 3.1415926;

			dRotateAngle = tmp;
		}
		
		CP_XML_NODE(xfrm)
		{      
			if (dRotateAngle)
			{
				double d =360 - dRotateAngle.get() * 180. / 3.14159265358979323846;
				d *= 60000; //60 000 per 1 gr - 19.5.5 oox 
				CP_XML_ATTR(L"rot", (int)d);
			}
			_CP_OPT(bool)bVal;
			if (odf_reader::GetProperty(additional,L"flipH", bVal))
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
				CP_XML_NODE(L"a:chExt")
				{
					CP_XML_ATTR(L"cx", cx);
					CP_XML_ATTR(L"cy", cy);
				}
			}
		}
    }
}
void oox_serialize_hlink(std::wostream & strm, std::vector<_hlink_desc> const & val)
{
    CP_XML_WRITER(strm)
    {
		BOOST_FOREACH(const _hlink_desc & h, val)
		{
			if (h.in_object == true)
			{
				CP_XML_NODE(L"a:hlinkClick")
				{
					CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
					CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
					
					CP_XML_ATTR(L"r:id", h.hId);
				}
				break;
			}
		}
	}
}



}
}
