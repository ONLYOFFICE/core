#include "../odf/precompiled_cpodf.h"
#include "oox_types_chart.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

void oox_chart::set_formula_series(int ind, std::wstring val)
{
	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setFormula(ind,val);
}
void oox_chart::set_name(std::wstring val)
{
	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setName(val);
}
void oox_chart::set_values_series(int ind,std::vector<std::wstring> & val)
{
	if (val.size()<1)return;

	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setValues(ind,val);
}
void oox_chart::set_properties(std::vector<odf::_property> g)
{
	_CP_OPT(int) iVal;

	_CP_OPT(bool) bStacked;
	_CP_OPT(bool) bPercent;

	odf::GetProperty(g, L"stacked",bStacked);
	odf::GetProperty(g, L"percentage",bPercent);

	if ( (bStacked) && (bStacked.get()))
	{
		grouping_ = L"stacked";
	}

	if ( (bPercent) && (bPercent.get()))
	{
		grouping_ = L"percentStacked";	
	}
	//solid-type - трехмерные
}
void oox_chart::oox_serialize_common(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:varyColors")
		{
			CP_XML_ATTR(L"val", 1);
		}
		if (grouping_.length()>1)
		{
			CP_XML_NODE(L"c:grouping")
			{	
				CP_XML_ATTR(L"val", grouping_);
			}
		}
		BOOST_FOREACH(oox_series_ptr const & s, series_)
		{
			s->oox_serialize(_Wostream);
		}
		BOOST_FOREACH(int const & i, axisId_)
		{
			CP_XML_NODE(L"c:axId")
			{	
				CP_XML_ATTR(L"val", i);
			}
		}	
		data_labels_.oox_serialize(_Wostream);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_bar_chart::add_series(int id)
{
	series_.push_back(oox_bar_series::create());
	series_.back()->id_= id;
}
void oox_bar_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);

	odf::GetProperty(g, L"vertical",bVertical);
	odf::GetProperty(g, L"connect-bars",bConnectBars);
	
	odf::GetProperty(g, L"gap-width",iGapWidth);
	odf::GetProperty(g, L"overlap",iOverlap);
}

void oox_bar_chart::set_additional_properties(std::vector<odf::_property> g)
{
	odf::GetProperty(g, L"gap-width",iGapWidth);
	odf::GetProperty(g, L"overlap",iOverlap);
}

void oox_bar_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:barChart")
        {
			CP_XML_NODE(L"c:barDir")
			{
				if ((bVertical) && (bVertical.get()))
					CP_XML_ATTR(L"val", L"bar");
				else
					CP_XML_ATTR(L"val", L"col"); 
			}
			int res = 0;
			int Overlap =0;
			if ((res = grouping_.find(L"tacked"))>=0)
			{
				Overlap =100;
			}
			if (iOverlap)
			{
				Overlap = iOverlap.get();
			}
			CP_XML_NODE(L"c:overlap")//-100 to 100
			{
				CP_XML_ATTR(L"val",Overlap); 
			}
			if (iGapWidth)
			{
				CP_XML_NODE(L"c:gapWidth")
				{
					CP_XML_ATTR(L"val",iGapWidth.get()); 
				}
			}
			oox_serialize_common(CP_XML_STREAM());
		}
    }
}
_CP_PTR(oox_chart) oox_bar_chart::create()
{
    return boost::make_shared<oox_bar_chart>();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_line_chart::add_series(int id)
{
	series_.push_back(oox_line_series::create());
	series_.back()->id_= id;
}
void oox_line_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);
	
	odf::GetProperty(g, L"interplation",iInterpolation);
	odf::GetProperty(g, L"lines",bLines);
}
void oox_line_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:lineChart")
        {
			oox_serialize_common(CP_XML_STREAM());
		
		}
    }
}
_CP_PTR(oox_chart) oox_line_chart::create()
{
    return boost::make_shared<oox_line_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_area_chart::add_series(int id)
{
	series_.push_back(oox_area_series::create());
	series_.back()->id_= id;

}
void oox_area_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_area_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:areaChart")
        {
			oox_serialize_common(CP_XML_STREAM());
			
		}
    }
}
_CP_PTR(oox_chart) oox_area_chart::create()
{
    return boost::make_shared<oox_area_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_bubble_chart::add_series(int id)
{
	series_.push_back(xlsx_bubble_series::create());
	series_.back()->id_= id;

}
void oox_bubble_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_bubble_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:bubbleChart")
        {
			oox_serialize_common(CP_XML_STREAM());
			//CP_XML_NODE(L"c:bubbleScale")
			//{
			//	CP_XML_ATTR(L"val", 100);//from 0 to 300
			//}
			//CP_XML_NODE(L"c:showNegBubbles")
			//{
			//	CP_XML_ATTR(L"val", 0);//from 0 to 300
			//}
		}
    }
}
_CP_PTR(oox_chart) oox_bubble_chart::create()
{
    return boost::make_shared<oox_bubble_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_doughnut_chart::add_series(int id)
{
	series_.push_back(oox_pie_series::create());
	series_.back()->id_= id;

}
void oox_doughnut_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_doughnut_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:doughnutChart")
        {
			oox_serialize_common(CP_XML_STREAM());
				
			CP_XML_NODE(L"c:firstSliceAng")
			{	
				CP_XML_ATTR(L"val", 0);
			}			
			CP_XML_NODE(L"c:holeSize")
			{	
				CP_XML_ATTR(L"val", 50);
			}
		}
	}
}
_CP_PTR(oox_chart) oox_doughnut_chart::create()
{
    return boost::make_shared<oox_doughnut_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_pie_chart::add_series(int id)
{
	series_.push_back(oox_pie_series::create());
	series_.back()->id_= id;
}
void oox_pie_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_pie_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:pieChart")
        {
			oox_serialize_common(CP_XML_STREAM());
		}
    }
}
_CP_PTR(oox_chart) oox_pie_chart::create()
{
    return boost::make_shared<oox_pie_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_radar_chart::add_series(int id)
{
	series_.push_back(oox_radar_series::create());
	series_.back()->id_= id;
}
void oox_radar_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_radar_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:radarChart")
        {
			CP_XML_NODE(L"c:radarStyle")
			{	
				CP_XML_ATTR(L"val", radarStyle_);//"standard","marker","filled"
			}
			oox_serialize_common(CP_XML_STREAM());
			
		}
    }
}
_CP_PTR(oox_chart) oox_radar_chart::create(std::wstring radarStyle)
{
    return boost::make_shared<oox_radar_chart>(radarStyle);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_scatter_chart::add_series(int id)
{
	series_.push_back(xlsx_scatter_series::create());
	series_.back()->id_= id;
}
void oox_scatter_chart::set_properties(std::vector<odf::_property> g)
{
	oox_chart::set_properties(g);

	odf::GetProperty(g, L"regression-type",iRegressionType);// none, linear, logarithmic, exponential, power
	//в xl похоже этого нет


}
void oox_scatter_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:scatterChart")
        {
			CP_XML_NODE(L"c:scatterStyle")
			{	
				CP_XML_ATTR(L"val", L"lineMarker");
			}
			
			oox_serialize_common(CP_XML_STREAM());
			
		}
    }
}

_CP_PTR(oox_chart) oox_scatter_chart::create()
{
    return boost::make_shared<oox_scatter_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oox_stock_chart::add_series(int id)
{
	series_.push_back(oox_line_series::create());
	series_.back()->id_= id;
}
void oox_stock_chart::set_properties(std::vector<odf::_property> g)
{
	properties_ = g;
	
	oox_chart::set_properties(g);

	odf::GetProperty(g, L"japanese-candle-stick",bCandleStick);
}
void oox_stock_chart::oox_serialize(std::wostream & _Wostream)
{
	oox_chart_shape shape;
	shape.set(graphic_properties_, fill_);

    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:stockChart")
        {
			oox_serialize_common(CP_XML_STREAM());
			
			//CP_XML_NODE(L"c:hiLowLines")//hiLowLines (High Low Lines) §21.2.2.80
			//{
			//	//shape.oox_serialize(CP_XML_STREAM());
			//}
			CP_XML_NODE(L"upDownBars")
			{
				CP_XML_NODE(L"c:gapWidth")
				{
					CP_XML_ATTR(L"val", 150);
				}
				CP_XML_NODE(L"c:upBars")//upDownBars (Up/Down Bars) §21.2.2.218
				{
					//shape.oox_serialize(CP_XML_STREAM());
				}
				CP_XML_NODE(L"c:downBars")
				{
					//shape.oox_serialize(CP_XML_STREAM());
				}
			}
		}//dropLines (Drop Lines) §21.2.2.53
    }
}
_CP_PTR(oox_chart) oox_stock_chart::create()
{
    return boost::make_shared<oox_stock_chart>();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
