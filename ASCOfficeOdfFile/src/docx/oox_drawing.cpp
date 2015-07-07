
#include "oox_drawing.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/svg_parser.h"
#include "../odf/datatypes/custom_shape_types_convert.h"

namespace cpdoccore {

void svg_path::oox_serialize(std::wostream & strm, std::vector<svg_path::_polyline> & path)
{
	CP_XML_WRITER(strm)
	{
		BOOST_FOREACH(svg_path::_polyline & p, path)
		{	
			oox_serialize(strm, p);
		}
	}
}
void svg_path::oox_serialize(std::wostream & strm, svg_path::_polyline const & val)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(val.command)
		{
			BOOST_FOREACH(svg_path::_point const & p, val.points)
			{		
				oox_serialize(CP_XML_STREAM() ,p);
			}
		}
    }
}

void svg_path::oox_serialize(std::wostream & strm, svg_path::_point const & val)
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
	_CP_OPT(std::wstring)	strStrokeOpacity; 
	_CP_OPT(bool)			bWordArt;
	
	odf_reader::GetProperty(prop, L"wordArt", bWordArt);
	
	odf_reader::GetProperty(prop, L"stroke-color"	, strStrokeColor);	
	odf_reader::GetProperty(prop, L"stroke"			, iStroke);	
	odf_reader::GetProperty(prop, L"stroke-width"	, dStrokeWidth);
	odf_reader::GetProperty(prop, L"stroke-opacity"	, strStrokeOpacity);

	if ((!strStrokeColor && !iStroke && !dStrokeWidth) && !always_draw)return;

	if ((iStroke) && (*iStroke==0)) return; //none

	CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"a:ln")
        { 
			std::wstring color, dash_style, fill = L"a:solidFill" ;

			if (strStrokeColor) color = strStrokeColor.get();

			if (iStroke)
			{
				if (iStroke.get() == 0 || bWordArt) fill = L"a:noFill";
				else dash_style =  _ooxDashStyle[iStroke.get()];	
			}
			
			if ((dStrokeWidth) && (dStrokeWidth.get()>= 0))
			{
				CP_XML_ATTR(L"w",static_cast<size_t>(dStrokeWidth.get() * 12700));//in emu (1 pt = 12700)
				if (color.length()<1)color = L"729FCF";
			}
		
			CP_XML_NODE(fill)
			{ 			
				if (fill != L"a:noFill")
				{
					if (color.length()<1 && always_draw) color = L"000000";
					else if (color.length()<1) color = L"ffffff";
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",color);
						if (strStrokeOpacity)CP_XML_NODE(L"a:alpha"){CP_XML_ATTR(L"val",strStrokeOpacity.get());}

					}
				}
			}
			_CP_OPT(std::wstring)	strVal;

			if (dash_style.length() >0 && dash_style != L"solid")
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
void oox_serialize_aLst(std::wostream & strm, const std::vector<odf_reader::_property> & prop)
{
	_CP_OPT(int)	iShapeIndex;
	_CP_OPT(bool)	bWordArt;
	
	odf_reader::GetProperty(prop, L"wordArt"				, bWordArt);
	odf_reader::GetProperty(prop, L"odf-custom-draw-index"	, iShapeIndex);

	int count_values = 0, min_value = 0, max_value = 0;

	if (!bWordArt && iShapeIndex)
	{
		count_values	= _OO_OOX_custom_shapes[*iShapeIndex].count_values;
		min_value		= _OO_OOX_custom_shapes[*iShapeIndex].min;
		max_value		= _OO_OOX_custom_shapes[*iShapeIndex].max;
	}
	else if (iShapeIndex)
	{
		count_values	= _OO_OOX_wordart[*iShapeIndex].count_values;
		min_value		= _OO_OOX_wordart[*iShapeIndex].min;
		max_value		= _OO_OOX_wordart[*iShapeIndex].max;
	}

	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:avLst")
		{
			_CP_OPT(std::wstring) strVal;
			if (odf_reader::GetProperty(prop,L"draw-modifiers",strVal) && iShapeIndex)
			{
				std::vector< std::wstring > values;
				boost::algorithm::split(values, strVal.get(), boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

				if( count_values >0 && values.size()>0 && count_values < 3)//временное ограниечение .. хз как там свойства путаются
				{//если не заданы доп свойства - нефиг мучится
					int i=1;

					_CP_OPT(int) iMax,iMin;
					odf_reader::GetProperty(prop,L"draw-modifiers-min",iMin);
					odf_reader::GetProperty(prop,L"draw-modifiers-max",iMax);
					values.resize(count_values);

					BOOST_FOREACH(std::wstring  & v, values)
					{
						CP_XML_NODE(L"a:gd")
						{
							if (values.size() >1)
								CP_XML_ATTR(L"name",(L"adj" + boost::lexical_cast<std::wstring>(i++)));
							else
								CP_XML_ATTR(L"name",L"adj");
							double val=0;
							if (v.length()>0)val= boost::lexical_cast<double>(v);
							
							if (iMin && iMax && iShapeIndex)
							{
								if (min_value < max_value)
								{
									double W = *iMax - *iMin;
									val = (val- (*iMin))/W * (max_value - min_value) + min_value;
								}
							}


							CP_XML_ATTR(L"fmla",L"val " + boost::lexical_cast<std::wstring>(static_cast<int>(val)));
						}
					}
				}
			}
		}
	}
}
void oox_serialize_bodyPr(std::wostream & strm, const std::vector<odf_reader::_property> & prop, const std::wstring & namespace_)
{
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

			_CP_OPT(int)	iWrap;
			odf_reader::GetProperty(prop,L"text-wrap"	, iWrap);
			if ((iWrap) && (*iWrap == 0))CP_XML_ATTR(L"wrap", L"none");

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
				CP_XML_NODE(L"a:spAutoFit")
				{
					CP_XML_ATTR(L"lnSpcReduction", 10000);
				}
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

void oox_serialize_shape(std::wostream & strm, _oox_drawing & val)
{
	_CP_OPT(std::wstring) strVal;
	_CP_OPT(double)			dVal;

 	std::wstring	shapeType;
	_CP_OPT(bool)	bWordArt;
	
	odf_reader::GetProperty(val.additional,L"wordArt", bWordArt);
	
	if (val.sub_type == 7)//custom 
	{
		_CP_OPT(int) iVal;
		odf_reader::GetProperty(val.additional, L"odf-custom-draw-index",iVal);
		if (iVal)shapeType = _OO_OOX_custom_shapes[*iVal].oox;	

		if (shapeType == L"textBox")
		{
			val.sub_type = 1;
			shapeType = L"rect";
		}
	}
	else if (val.sub_type<9 && val.sub_type>=0)
	{
		shapeType =	_ooxShapeType[val.sub_type];
	} 
	
	if (shapeType.length()<1)
	{
		shapeType	 = L"rect";
		val.sub_type = 2;
	}
	if (bWordArt) val.sub_type = 1;

	CP_XML_WRITER(strm)
    {
		if (val.sub_type == 6 || val.sub_type == 8)
		{
			CP_XML_NODE(L"a:custGeom")
			{        
				oox_serialize_aLst(CP_XML_STREAM(),val.additional);
				CP_XML_NODE(L"a:ahLst");
				CP_XML_NODE(L"a:gdLst");
				CP_XML_NODE(L"a:rect")
				{
					CP_XML_ATTR(L"b",L"b");
					CP_XML_ATTR(L"l",0);
					CP_XML_ATTR(L"r",L"r");
					CP_XML_ATTR(L"t",0);
				}
				//<a:rect b="b" l="0" r="r" t="0"/>
				if (odf_reader::GetProperty(val.additional,L"custom_path",strVal))
				{
					CP_XML_NODE(L"a:pathLst")
					{ 	
						CP_XML_NODE(L"a:path")
						{
							CP_XML_ATTR(L"w", val.cx);
							CP_XML_ATTR(L"h", val.cy);
							CP_XML_STREAM() << strVal.get();
						}
					}
				}         
			}
		}
		else
		{
			CP_XML_NODE(L"a:prstGeom")//автофигура
			{        
				CP_XML_ATTR(L"prst", shapeType);
				if (!bWordArt) oox_serialize_aLst(CP_XML_STREAM(), val.additional);
			}					
		}
		if (bWordArt)
		{
			_oox_fill no_fill;
			oox_serialize_fill(strm, no_fill);
		}
		else
			oox_serialize_fill(strm, val.fill);

	}
}

void oox_serialize_xfrm(std::wostream & strm, _oox_drawing & val, std::wstring name_space)
{
	CP_XML_WRITER(strm)
    {
		std::wstring xfrm = name_space + L":xfrm";

		_CP_OPT(double) dRotate;
		odf_reader::GetProperty(val.additional,L"svg:rotate",dRotate);
	
		_CP_OPT(double) dSkewX;
		odf_reader::GetProperty(val.additional,L"svg:skewX",dSkewX);		

		_CP_OPT(double) dSkewY;
		odf_reader::GetProperty(val.additional,L"svg:skewY",dSkewY);	

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
				double d =360-dRotateAngle.get()*180./3.14159265358979323846;
				d *= 60000; //60 000 per 1 gr - 19.5.5 oox 
				CP_XML_ATTR(L"rot", (int)d);
			}
			_CP_OPT(bool)bVal;
			if (odf_reader::GetProperty(val.additional,L"flipH",bVal))
				CP_XML_ATTR(L"flipH", bVal.get());

			if (odf_reader::GetProperty(val.additional,L"flipV",bVal))
				CP_XML_ATTR(L"flipV", bVal.get());

			CP_XML_NODE(L"a:off") 
			{
				CP_XML_ATTR(L"x", val.x);
				CP_XML_ATTR(L"y", val.y);
			}

			if (val.cx >0 || val.cy >0)
			{
				CP_XML_NODE(L"a:ext")
				{
					CP_XML_ATTR(L"cx", val.cx);
					CP_XML_ATTR(L"cy", val.cy);
				}
			}else
			{
                _CP_LOG << L"[error!!!] not set size object\n";
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
			if (h.object == true)
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
