

#include <boost/foreach.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/style_text_properties.h"

#include "oox_chart_series.h"
#include "../formulasconvert/formulasconvert.h"

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

class oox_marker_type
{
public:
	static void oox_serialize(std::wostream & _Wostream,int type =0)
	{
		CP_XML_WRITER(_Wostream)
		{
			CP_XML_NODE(L"c:marker")
			{
				CP_XML_NODE(L"c:symbol")
				{
					switch (type)
					{
					case 11:	CP_XML_ATTR(L"val", L"star"); break;
					case 10:	CP_XML_ATTR(L"val", L"circle"); break;
					case 15:	CP_XML_ATTR(L"val", L"dash"); break;
					case 3:		CP_XML_ATTR(L"val", L"diamond"); break;
					case 14:	CP_XML_ATTR(L"val", L"dot"); break;
					case 8:		CP_XML_ATTR(L"val", L"picture"); break;
					case 13:	CP_XML_ATTR(L"val", L"plus"); break;
					case 2:		CP_XML_ATTR(L"val", L"square"); break;
					case 9:		CP_XML_ATTR(L"val", L"triangle"); break;
					case 12:	CP_XML_ATTR(L"val", L"x"); break;
					case 0:	//none
					case 1:	//auto
					default:
						CP_XML_ATTR(L"val", L"none");			
					}
				}
			}
		}
	}
};

void oox_chart_series::setName(std::wstring &value)
{
	name_=value;
}

void oox_chart_series::setFormula(int ind, std::wstring &value)
{
	formulasconvert::odf2oox_converter converter;

	if (ind == 4)
	{
		long res = value.find(L"local-table");
		if (res >=0) return;
		
		values_[ind].strRef_.formula=converter.convert_chart_distance(value);
		values_[ind].strRef_.present = true;
		values_[ind].present = true;
	}
	else
	{
		values_[ind].numRef_.formula=converter.convert_chart_distance(value);
		values_[ind].numRef_.present = true;
		values_[ind].present = true;
	}
}

void oox_chart_series::parse_properties()
{
	_CP_OPT(int)	iSymbolType;
	odf_reader::GetProperty(content_.properties_, L"symbol-type",iSymbolType);   //    noneSymbol,    autoSymbol,     namedSymbol

	if (iSymbolType)
	{
		iSymbolMarkerType_ = iSymbolType.get() ;
		if (iSymbolMarkerType_ == 2)
		{
			odf_reader::GetProperty(content_.properties_, L"symbol-name",iSymbolType);   //    квадратики, звездочки ...
			if (iSymbolType)iSymbolMarkerType_ = iSymbolType.get() ;
			else iSymbolMarkerType_ = 0;//выключим
		}
	}
}
void oox_chart_series::setValues(int ind, std::vector<std::wstring> & values)
{
	values_[ind].present = true;

	if (ind == 4)values_[ind].strRef_.present = true;
	else values_[ind].numRef_.present = true;

	BOOST_FOREACH(std::wstring & v, values)
	{
		boost::algorithm::trim(v);
		if (ind == 4)
		{
			values_[ind].strRef_.str_cash.push_back(v);
			values_[ind].strRef_.str_cash_count++;
		}
		else
		{
			values_[ind].numRef_.num_cash.push_back(v);
			values_[ind].numRef_.num_cash_count++;
		}
	}
}
void oox_chart_series::oox_serialize_common(std::wostream & _Wostream)
{
	parse_properties();

	oox_chart_shape shape;
	shape.set(content_.graphic_properties_, content_.fill_);
	
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:idx")
		{
			CP_XML_ATTR(L"val", id_);
		}
		CP_XML_NODE(L"c:order")
		{
			CP_XML_ATTR(L"val", id_);

		}
		shape.oox_serialize(_Wostream);

		for (int i=0; i < 4; i++)
		{
			if (values_[i].present)
			{
				CP_XML_NODE(values_[i].type)
				{
					if (values_[i].numRef_.present)
					{
						CP_XML_NODE(L"c:numRef")
						{
							CP_XML_NODE(L"c:f")
							{
								CP_XML_CONTENT(values_[i].numRef_.formula);
							}
						
							if (values_[i].numRef_.num_cash_count>0)
							{
								CP_XML_NODE(L"c:numCache")//необязательно
								{
									CP_XML_NODE(L"c:formatCode")
									{
										CP_XML_CONTENT(L"General");//????
									}
									CP_XML_NODE(L"c:ptCount")
									{
										CP_XML_ATTR(L"val", values_[i].numRef_.num_cash_count);
									}
									int j=0;
									BOOST_FOREACH(std::wstring & v, values_[i].numRef_.num_cash)
									{								
										CP_XML_NODE(L"c:pt")
										{
											CP_XML_ATTR(L"idx", j++);
											double val = 0;

											try { val = boost::lexical_cast<double>(v);}
											catch(...){}			
											CP_XML_NODE(L"c:v")
											{
												CP_XML_CONTENT(val);
											}
										}
									}
								}
							}
						}
					}
					//if (values_[i].strRef_.present)
					//{
					//	CP_XML_NODE(L"c:strRef")
					//	{
					//		CP_XML_NODE(L"c:f")
					//		{
					//			CP_XML_CONTENT(values_[i].strRef_.formula);
					//		}
					//	
					//		if (values_[i].strRef_.str_cash_count>0)
					//		{
					//			CP_XML_NODE(L"c:strCache")//необязательно
					//			{
					//				CP_XML_NODE(L"c:ptCount")
					//				{
					//					CP_XML_ATTR(L"val", values_[i].strRef_.str_cash_count);
					//				}
					//				int j=0;
					//				BOOST_FOREACH(std::wstring & v, values_[i].strRef_.str_cash)
					//				{								
					//					CP_XML_NODE(L"c:pt")
					//					{
					//						CP_XML_ATTR(L"idx", j++);
					//						CP_XML_NODE(L"c:v")
					//						{
					//							CP_XML_CONTENT(v);
					//						}
					//					}
					//				}
					//			}
					//		}
					//	}
					//}
				}
			}
		}		
		if (content_.regression_curve_.line_properties_.size()>0)
		{
			std::wstring typeTrendline= L"log"; //"exp" | "linear" | "log" | "movingAvg" | "poly" | "power"
            _oox_fill oox_fill;
            shape.set(content_.regression_curve_.line_properties_, oox_fill);
			
			_CP_OPT(int) iType;
			odf_reader::GetProperty(content_.properties_, L"regression-type",iType);   //     none, linear, logarithmic, exponential, power
			if (iType)
			{
				switch(iType.get()){case 1: typeTrendline= L"linear";break;case 3: typeTrendline= L"exp";break;case 4: typeTrendline= L"power";break;}
			}

			CP_XML_NODE(L"c:trendline")
			{
				CP_XML_NODE(L"c:trendlineType")
				{
					CP_XML_ATTR(L"val", typeTrendline);
				}
				CP_XML_NODE(L"c:dispEq")
				{
					CP_XML_ATTR(L"val",content_.regression_curve_.bEquation);
				}

				CP_XML_NODE(L"c:dispRSqr")
				{
					CP_XML_ATTR(L"val",content_.regression_curve_.bREquation);
				}
				if (content_.regression_curve_.equation_properties_.graphic_properties_.size()>0)
				{
					CP_XML_NODE(L"c:trendlineLbl")
					{
						shape.set(content_.regression_curve_.equation_properties_.graphic_properties_,content_.regression_curve_.equation_properties_.fill_);
						shape.oox_serialize(CP_XML_STREAM());
					}
				}		

                _oox_fill oox_fill;
                shape.set( content_.regression_curve_.line_properties_, oox_fill);
				shape.oox_serialize(CP_XML_STREAM());


			}
		}
	}
}
	//backward (Backward) §21.2.2.12
	//dispEq (Display Equation) §21.2.2.43
	//dispRSqr (Display R Squared Value) §21.2.2.44
	//extLst (Chart Extensibility) §21.2.2.64
	//forward (Forward) §21.2.2.73
	//intercept (Intercept) §21.2.2.85
	//name (Trendline Name) §21.2.2.116
	//order (Polynomial Trendline Order) §21.2.2.129
	//period (Period) §21.2.2.135
	//spPr (Shape Properties) §21.2.2.197
	//trendlineLbl (Trendline Label) §21.2.2.212
	//trendlineType (Trendline Type)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_bar_series::create()
{
    return boost::make_shared<oox_bar_series>();
}
void oox_bar_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_line_series::create()
{
    return boost::make_shared<oox_line_series>();
}

void oox_line_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());

			oox_marker_type::oox_serialize(CP_XML_STREAM(),iSymbolMarkerType_);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_pie_series::create()
{
    return boost::make_shared<oox_pie_series>();
}
void oox_pie_series::oox_serialize(std::wostream & _Wostream)
{
	content_.graphic_properties_.clear(); //авто подбор цветов
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) xlsx_scatter_series::create()
{
    return boost::make_shared<xlsx_scatter_series>();
}
void xlsx_scatter_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());
			
			oox_marker_type::oox_serialize(CP_XML_STREAM(),iSymbolMarkerType_);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) xlsx_bubble_series::create()
{
    return boost::make_shared<xlsx_bubble_series>();
}
void xlsx_bubble_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			//CP_XML_NODE(L"c:invertIfNegative")
			//{
			//	CP_XML_ATTR(L"val", 0);//from 0 to 300
			//}
			oox_serialize_common(CP_XML_STREAM());
			
			//CP_XML_NODE(L"c:bubbleSize")
			//{
			//	for (int i=0;i<3;i++)
			//	{
			//		if (values_[i].present && values_[i].numRef_.present)
			//		{
			//			CP_XML_NODE(L"c:numRef")
			//			{
			//				CP_XML_NODE(L"c:f")
			//				{
			//					CP_XML_CONTENT(values_[i].numRef_.formula);
			//				}
			//			}
			//		}
			//	}
			//}
			//CP_XML_NODE(L"c:bubble3D")
			//{
			//	CP_XML_ATTR(L"val", 0);
			//}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_area_series::create()
{
    return boost::make_shared<oox_area_series>();
}

void oox_area_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());

		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_radar_series::create()
{
    return boost::make_shared<oox_radar_series>();
}
void oox_radar_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());

			oox_marker_type::oox_serialize(CP_XML_STREAM(),iSymbolMarkerType_);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
