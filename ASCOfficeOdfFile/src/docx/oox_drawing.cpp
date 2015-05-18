
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

void oox_serialize_ln(std::wostream & strm, const std::vector<odf::_property> & prop)
{
	_CP_OPT(std::wstring) strVal; 
	_CP_OPT(int) iVal;
	_CP_OPT(double) dVal;
	
	odf::GetProperty(prop,L"stroke-color",strVal);	
	odf::GetProperty(prop,L"stroke",iVal);	
	odf::GetProperty(prop,L"stroke-width",dVal);

	if (!strVal && !iVal && !dVal)return;
	CP_XML_WRITER(strm)
    {
        CP_XML_NODE(L"a:ln")
        { 
			std::wstring color, dash_style, fill = L"a:solidFill" ;

			if (strVal)color = strVal.get();

			if (iVal)
			{
				if (iVal.get() == 0)fill = L"a:noFill";
				else dash_style =  _ooxDashStyle[iVal.get()];	
			}
			
			if ((dVal) && (dVal.get()> 0))
			{
				CP_XML_ATTR(L"w",static_cast<size_t>(dVal.get() * 12700));//in emu (1 pt = 12700)
				if (color.length()<1)color = L"729FCF";
			}
		
			CP_XML_NODE(fill)
			{ 			
				if (fill != L"a:noFill")
				{
					if (color.length()<1)color = L"ffffff";
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",color);
						odf::GetProperty(prop,L"stroke-opacity",strVal);
						if (strVal)CP_XML_NODE(L"a:alpha"){CP_XML_ATTR(L"val",strVal.get());}

					}
				}
			}
			if (dash_style.length() >0)
			{
				CP_XML_NODE(L"a:prstDash"){CP_XML_ATTR(L"val",dash_style);}	
			}
			odf::GetProperty(prop,L"marker-start",strVal);	
			if (strVal)
			{
				CP_XML_NODE(L"a:headEnd"){CP_XML_ATTR(L"type",strVal.get());}
			}
			odf::GetProperty(prop,L"marker-end",strVal);	
			if (strVal)
			{
				CP_XML_NODE(L"a:tailEnd"){CP_XML_ATTR(L"type",strVal.get());}
			}
		}
    }
}
void oox_serialize_aLst(std::wostream & strm, const std::vector<odf::_property> & prop)
{
	_CP_OPT(int) iShapeIndex;
	odf::GetProperty(prop,L"draw-type-index",iShapeIndex);

	if (!iShapeIndex)return;

	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:avLst")
		{
			_CP_OPT(std::wstring) strVal;
			if (odf::GetProperty(prop,L"draw-modifiers",strVal))
			{
				std::vector< std::wstring > values;
				boost::algorithm::split(values, strVal.get(), boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

				if(_OO_OOX_custom_shapes[*iShapeIndex].count_values >0 && values.size()>0 &&
					_OO_OOX_custom_shapes[*iShapeIndex].count_values <3)//временное ограниечение .. хз как там свойства путаются
				{//если не заданы доп свойства - нефиг мучится
					int i=1;

					_CP_OPT(int) iMax,iMin;
					odf::GetProperty(prop,L"draw-modifiers-min",iMin);
					odf::GetProperty(prop,L"draw-modifiers-max",iMax);
					values.resize(_OO_OOX_custom_shapes[*iShapeIndex].count_values);

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
								if (_OO_OOX_custom_shapes[*iShapeIndex].min <
									_OO_OOX_custom_shapes[*iShapeIndex].max)
								{
									double W = *iMax - *iMin;
									val = (val- (*iMin))/W *
										(_OO_OOX_custom_shapes[*iShapeIndex].max-_OO_OOX_custom_shapes[*iShapeIndex].min)+_OO_OOX_custom_shapes[*iShapeIndex].min;
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
void oox_serialize_bodyPr(std::wostream & strm, const std::vector<odf::_property> & prop)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:bodyPr")
		{
			_CP_OPT(int) iAlign;

			odf::GetProperty(prop,L"textalign-vertical",iAlign);
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
			//else CP_XML_ATTR(L"anchor", L"dist");break;

		}
	}
}
void oox_serialize_shape(std::wostream & strm, _oox_drawing const & val)
{
	_CP_OPT(std::wstring) strVal;
	_CP_OPT(double) dVal;

 	std::wstring shapeType;
	
	if (val.sub_type == 7)//custom 
	{
		_CP_OPT(int) iVal;
		odf::GetProperty(val.additional,L"draw-type-index",iVal);
		if (iVal)shapeType = _OO_OOX_custom_shapes[*iVal].oox;	
	}
	else if (val.sub_type<9 && val.sub_type>=0)
	{
		shapeType =	_ooxShapeType[val.sub_type];
	} 
	
	if (shapeType.length()<1)shapeType = L"rect";

	CP_XML_WRITER(strm)
    {
		if (val.sub_type == 6 || val.sub_type == 8)//рисованая по точкам хрень - полигоны и пути
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
				if (odf::GetProperty(val.additional,L"custom_path",strVal))
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
				oox_serialize_aLst(CP_XML_STREAM(),val.additional);
			}					
		}
		oox_serialize_fill(strm,val.fill);

	}
}

void oox_serialize_xfrm(std::wostream & strm, _oox_drawing const & val, std::wstring name_space)
{
	CP_XML_WRITER(strm)
    {
		std::wstring xfrm = name_space + L":xfrm";

		_CP_OPT(double) dRotate;
		odf::GetProperty(val.additional,L"svg:rotate",dRotate);
	
		_CP_OPT(double) dSkewX;
		odf::GetProperty(val.additional,L"svg:skewX",dSkewX);		

		_CP_OPT(double) dSkewY;
		odf::GetProperty(val.additional,L"svg:skewY",dSkewY);	

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
			if (odf::GetProperty(val.additional,L"flipH",bVal))
				CP_XML_ATTR(L"flipH", bVal.get());

			if (odf::GetProperty(val.additional,L"flipV",bVal))
				CP_XML_ATTR(L"flipV", bVal.get());

			CP_XML_NODE(L"a:off") 
			{
				CP_XML_ATTR(L"x", static_cast<size_t>(val.x));
				CP_XML_ATTR(L"y", static_cast<size_t>(val.y));
			}

			if (val.cx >0 || val.cy >0)
			{
				CP_XML_NODE(L"a:ext")
				{
					CP_XML_ATTR(L"cx", static_cast<size_t>(val.cx));
					CP_XML_ATTR(L"cy", static_cast<size_t>(val.cy));
				}
			}else
			{
				_CP_LOG(info) << L"[error!!!] not set size object\n";
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