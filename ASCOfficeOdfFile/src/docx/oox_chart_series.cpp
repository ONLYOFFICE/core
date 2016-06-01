

#include <boost/foreach.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/style_text_properties.h"

#include "oox_chart_series.h"
#include "../formulasconvert/formulasconvert.h"

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

oox_chart_series::oox_chart_series()
{
	values_[0].type	= L"c:cat";//подписи
	values_[1].type	= L"c:val";
	values_[2].type	= L"c:xVal";
	values_[3].type	= L"c:yVal";
	values_[4].type	= L"c:bubbleSize";//заместо оси х!!!!

	iSymbolMarkerType_	= 0;
	bLocalTable_		= false;
}
void oox_chart_series::setName(std::wstring &value)
{
	name_=value;
}

void oox_chart_series::setFormula(int ind, std::wstring &value, std::wstring & formatCode, bool link_to_source)
{
	formulasconvert::odf2oox_converter converter;

	if (ind == 0)
	{
		long res = value.find(L"local-table");
		if (res >=0) return;
		
		values_[ind].strRef_.formula	= converter.convert_chart_distance(value);
		values_[ind].strRef_.present	= true;
		values_[ind].present			= true;
	}
	else
	{
		long res = value.find(L"local-table");
		if (res >=0 && !bLocalTable_ ) return; //в xlsx низя .... нужно сделать тогда отдельную  table.xml

		values_[ind].numRef_.formula		= converter.convert_chart_distance(value);
		values_[ind].numRef_.present		= true;
		values_[ind].numRef_.formatCode		= formatCode;
		values_[ind].numRef_.link_to_source	= link_to_source;
		values_[ind].present				= true;
	}
}

void oox_chart_series::set_cache_only (bool val)
{
	bLocalTable_ = val;
}

void oox_chart_series::parse_properties()
{
	_CP_OPT(int)			intVal;
	_CP_OPT(bool)			boolVal;
	_CP_OPT(std::wstring)	strVal;
	odf_reader::GetProperty(content_.properties_, L"symbol-type", intVal);   //    noneSymbol,    autoSymbol,     namedSymbol

	if (intVal)
	{
		iSymbolMarkerType_ = intVal.get() ;
		if (iSymbolMarkerType_ == 2)
		{
			odf_reader::GetProperty(content_.properties_, L"symbol-name", intVal);   //    квадратики, звездочки ...
			
			if (intVal)	iSymbolMarkerType_	= intVal.get() ;
			else		iSymbolMarkerType_	= 0;//выключим
		}
	}
	//odf_reader::GetProperty(content_.properties_, L"data-label-symbol", boolVal);
	//if (boolVal)
	//{
	//	if (!data_labels) data_labels_ = oox_data_labels();
	//	//data_labels_->
	//}
	odf_reader::GetProperty(content_.properties_, L"data-label-text", boolVal);
	if (boolVal)
	{
		if (!data_labels_) data_labels_ = oox_data_labels();
		
		data_labels_->set_showCatName(*boolVal); 
	}
	odf_reader::GetProperty(content_.properties_, L"data-label-number", intVal);
	if (intVal)
	{
		if (!data_labels_)	data_labels_ = oox_data_labels();
		
		if (*intVal == 1)	data_labels_->set_showVal(true); 
		if (*intVal == 2)	data_labels_->set_showPercent(true); 		
	}
}
void oox_chart_series::setValues(int ind, std::vector<std::wstring> & values)
{
	values_[ind].present = true;

	if (ind == 0)	values_[ind].strRef_.present = true;
	else			values_[ind].numRef_.present = true;

	BOOST_FOREACH(std::wstring & v, values)
	{
		boost::algorithm::trim(v);
		if (ind == 0)
		{
			values_[ind].strRef_.str_cache.push_back(v);
			values_[ind].strRef_.str_cache_count++;
		}
		else
		{
			values_[ind].numRef_.num_cache.push_back(v);
			values_[ind].numRef_.num_cache_count++;
		}
	}
}
void oox_chart_series::oox_serialize_marker(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE(L"c:marker")
		{
			CP_XML_NODE(L"c:symbol")
			{
				switch (iSymbolMarkerType_)
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
			if (iSymbolMarkerType_ > 0)
			{
				oox_chart_shape shape;
				
				shape.set(content_.graphic_properties_, content_.fill_);
				shape.oox_serialize(_Wostream);
			}
		}
	}
}
void oox_chart_series::oox_serialize_common(std::wostream & _Wostream)
{
	parse_properties();

	oox_chart_shape shape;
	
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
		shape.set(content_.graphic_properties_, content_.fill_);
		shape.oox_serialize(_Wostream);

		for (int i=0; i < 5; i++)
		{
			if (values_[i].present)
			{
				CP_XML_NODE(values_[i].type)
				{
					if (values_[i].numRef_.present && !bLocalTable_)
					{
						CP_XML_NODE(L"c:numRef")
						{
							CP_XML_NODE(L"c:f")
							{
								if (values_[i].numRef_.formula.empty())
									CP_XML_CONTENT(L"label");
								else
									CP_XML_CONTENT(values_[i].numRef_.formula);
							}
						
							if (values_[i].numRef_.num_cache_count>0)
							{
								CP_XML_NODE(L"c:numCache")
								{
									CP_XML_NODE(L"c:formatCode")
									{
										CP_XML_CONTENT(values_[i].numRef_.formatCode);
									}
									CP_XML_NODE(L"c:ptCount")
									{
										CP_XML_ATTR(L"val", values_[i].numRef_.num_cache_count);
									}
									int j=0;
									BOOST_FOREACH(std::wstring & v, values_[i].numRef_.num_cache)
									{								
										if (v == L"NaN") continue; 
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
					else if (values_[i].numRef_.num_cache_count > 0)
					{
						CP_XML_NODE(L"c:numLit")
						{
							CP_XML_NODE(L"c:formatCode")
							{
								CP_XML_CONTENT(values_[i].numRef_.formatCode);
							}
							CP_XML_NODE(L"c:ptCount")
							{
								CP_XML_ATTR(L"val", values_[i].numRef_.num_cache_count);
							}
							int j=0;
							BOOST_FOREACH(std::wstring & v, values_[i].numRef_.num_cache)
							{				
								if (v == L"NaN") continue; 
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
					else if (values_[i].strRef_.present > 0 && !bLocalTable_)
					{
						CP_XML_NODE(L"c:strRef")
						{
							CP_XML_NODE(L"c:f")
							{
								if (values_[i].strRef_.formula.empty())
									CP_XML_CONTENT(L"label");
								else
									CP_XML_CONTENT(values_[i].strRef_.formula);
							}
							CP_XML_NODE(L"c:strCache")
							{
								CP_XML_NODE(L"c:ptCount")
								{
									CP_XML_ATTR(L"val", values_[i].strRef_.str_cache_count);
								}
								int j=0;
								BOOST_FOREACH(std::wstring & v, values_[i].strRef_.str_cache)
								{								
									CP_XML_NODE(L"c:pt")
									{
										CP_XML_ATTR(L"idx", j++);
										CP_XML_NODE(L"c:v")
										{
											CP_XML_CONTENT(v);
										}
									}
								}
							}
						}
					}
					else if (values_[i].strRef_.str_cache_count > 0)
					{
						CP_XML_NODE(L"c:strLit")
						{
							CP_XML_NODE(L"c:ptCount")
							{
								CP_XML_ATTR(L"val", values_[i].strRef_.str_cache_count);
							}
							int j=0;
							BOOST_FOREACH(std::wstring & v, values_[i].strRef_.str_cache)
							{								
								CP_XML_NODE(L"c:pt")
								{
									CP_XML_ATTR(L"idx", j++);
									CP_XML_NODE(L"c:v")
									{
										CP_XML_CONTENT(v);
									}
								}
							}
						}
					}
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
		if (!content_.text_properties_.empty())
		{
			if (!data_labels_) data_labels_ = oox_data_labels();

			data_labels_->set_common_dLbl(content_.text_properties_);
		}

		bool bEmpty_dPt = true;
		for (int i = 0 ; i < content_.points_.size(); i++)
		{
			if (content_.points_[i].bEnabled)
			{
				bEmpty_dPt = false;
				break;
			}
		}

		int indPoint = 0;
		for (int i = 0 ; !bEmpty_dPt && i < content_.points_.size(); i++)
		{
			for (int j = 0 ; j < content_.points_[i].repeated_; j++)
			{
				CP_XML_NODE(L"c:dPt")
				{
					CP_XML_NODE(L"c:idx")
					{
						CP_XML_ATTR(L"val", indPoint++);
					}
					if (content_.points_[i].graphic_properties_.size() > 0 && content_.points_[i].fill_.type >= 0)
					{
						shape.set( content_.points_[i].graphic_properties_, content_.points_[i].fill_);
						shape.oox_serialize(CP_XML_STREAM());
					}

					if (!content_.points_[i].text_properties_.empty())
					{
						if (!data_labels_) data_labels_ = oox_data_labels();

						data_labels_->add_dLbl(indPoint - 1, content_.points_[i].text_properties_);
					}
				}
			}
		}
		
		if (data_labels_)
		{
			data_labels_->oox_serialize(_Wostream);
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

			oox_serialize_marker(CP_XML_STREAM());
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
	content_.fill_.clear();		//авто подбор цветов

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());

			_CP_OPT(int) iVal;
			odf_reader::GetProperty(content_.properties_, L"pie-offset", iVal); 
			
			if (iVal)
			{
				CP_XML_NODE(L"c:explosion")
				{
					CP_XML_ATTR(L"val", *iVal);			
				}
			}
			
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_scatter_series::create()
{
    return boost::make_shared<oox_scatter_series>();
}
void oox_scatter_series::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:ser")
		{
			oox_serialize_common(CP_XML_STREAM());
			
			oox_serialize_marker(CP_XML_STREAM());
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

_CP_PTR(oox_chart_series) oox_bubble_series::create()
{
    return boost::make_shared<oox_bubble_series>();
}
void oox_bubble_series::oox_serialize(std::wostream & _Wostream)
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

			oox_serialize_marker(CP_XML_STREAM());
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
