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

#include <xml/simple_xml_writer.h>
#include "oox_types_chart.h"

namespace cpdoccore {
namespace oox {

void oox_chart::set_cache_only (bool val)
{
	for (size_t i = 0 ; i < series_.size(); i++)
	{
		series_[i]->set_cache_only(val);
	}

}

void oox_chart::set_formula_series(int ind, const std::wstring& val, const std::wstring& formatCode, bool link_to_source)
{
	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setFormula(ind, val, formatCode, link_to_source);

}
void oox_chart::set_name(const std::wstring& val)
{
	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setName(val);
}
void oox_chart::set_content_series(odf_reader::chart::series & content)
{
	series_.back()->content_= content;
	series_.back()->set_properties(content.properties_);
}

void oox_chart::set_values_series(int ind, std::vector<std::wstring> & val)
{
	if (val.empty())return;

	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setValues (ind, val);
}
void oox_chart::set_label_series(const std::wstring& formula, std::vector<std::wstring> & cash)
{
	if (formula.empty() && cash.empty()) return;

	oox_series_ptr & current_ptr = series_.back();
	current_ptr->setLabels (formula, cash);
}
void oox_chart::set_properties(std::vector<odf_reader::_property> g)
{
	_CP_OPT(int) iVal;

	_CP_OPT(bool) bStacked;
	_CP_OPT(bool) bPercent;
	_CP_OPT(int) iGapWidth;

	odf_reader::GetProperty(g, L"stacked", bStacked);
	odf_reader::GetProperty(g, L"percentage", bPercent);
	odf_reader::GetProperty(g, L"gap-width", iGapWidth);

	if ( (bStacked) && (bStacked.get()))
	{
		grouping_ = L"stacked";
	}

	if ( (bPercent) && (bPercent.get()))
	{
		grouping_ = L"percentStacked";	
	}
	if (iGapWidth)
	{
		dispBlanksAs_ = L"gap";
	}
	//solid-type - трехмерные
}
void oox_chart::oox_serialize_common(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		if (!grouping_.empty())
		{
			CP_XML_NODE(L"c:grouping")
			{	
				CP_XML_ATTR(L"val", grouping_);
			}
		}
		for (size_t i = 0; i < series_.size(); i++)
		{
			if (is3D_ || type_ == CHART_TYPE_AREA || type_ == CHART_TYPE_DOUGHNUT || type_ == CHART_TYPE_RADAR)
			{
				series_[i]->labelPosEnabled_ = false;
			}
			series_[i]->oox_serialize(_Wostream);
		}
		for (size_t i = 0; i < axisId_.size(); i++)
		{
			CP_XML_NODE(L"c:axId")
			{	
				CP_XML_ATTR(L"val", axisId_[i]);
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
void oox_bar_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);

	odf_reader::GetProperty(g, L"vertical", bVertical);
	odf_reader::GetProperty(g, L"connect-bars", bConnectBars);
	
	odf_reader::GetProperty(g, L"gap-width", iGapWidth);
	odf_reader::GetProperty(g, L"overlap", iOverlap);
	odf_reader::GetProperty(g, L"solid-type", iSolidType);
}

void oox_bar_chart::set_additional_properties(std::vector<odf_reader::_property> g)
{
}

void oox_bar_chart::oox_serialize(std::wostream & _Wostream)
{
	std::wstring	strNode = L"c:barChart";
	if (is3D_)		strNode = L"c:bar3DChart";
    
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(strNode )
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
				CP_XML_ATTR(L"val", Overlap); 
			}
			if (iGapWidth)
			{
				CP_XML_NODE(L"c:gapWidth")
				{
					CP_XML_ATTR(L"val", iGapWidth.get()); 
				}
			}
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
			}
			oox_serialize_common(CP_XML_STREAM());

			if (is3D_)
			{
				CP_XML_NODE(L"c:shape")
				{
					switch (iSolidType.get_value_or(0))
					{
						case 1: CP_XML_ATTR(L"val", L"cylinder"); break;
						case 2: CP_XML_ATTR(L"val", L"cone"); break;
						case 3: CP_XML_ATTR(L"val", L"pyramid"); break;
						case 0:
						default: CP_XML_ATTR(L"val", L"box"); break;
					}
				}
			}
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
void oox_line_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);
	
	odf_reader::GetProperty(g, L"interplation",iInterpolation);
	odf_reader::GetProperty(g, L"lines",bLines);
}
void oox_line_chart::oox_serialize(std::wostream & _Wostream)
{
  	std::wstring	strNode = L"c:lineChart";
	if (is3D_)		strNode = L"c:line3DChart";

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(strNode)
        {
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
			}	
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
void oox_area_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_area_chart::oox_serialize(std::wostream & _Wostream)
{
 	std::wstring	strNode = L"c:areaChart";
	if (is3D_)		strNode = L"c:area3DChart";

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(strNode)
        {
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
			}		
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
	series_.push_back(oox_bubble_series::create());
	series_.back()->id_= id;

}
void oox_bubble_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_bubble_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:bubbleChart")
        {
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
			}	
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
void oox_doughnut_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_doughnut_chart::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:doughnutChart")
        {
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
			}	
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
void oox_pie_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);
}
void oox_pie_chart::oox_serialize(std::wostream & _Wostream)
{
  	std::wstring	strNode = L"c:pieChart";
	if (is3D_)		strNode = L"c:pie3DChart";

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(strNode)
        {
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
			}	
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
void oox_radar_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);
	grouping_ = L"";
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
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
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
	series_.push_back(oox_scatter_series::create());
	series_.back()->id_= id;
}
void oox_scatter_chart::set_properties(std::vector<odf_reader::_property> g)
{
	oox_chart::set_properties(g);

	odf_reader::GetProperty(g, L"regression-type",iRegressionType);// none, linear, logarithmic, exponential, power
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
			CP_XML_NODE(L"c:varyColors")
			{
				CP_XML_ATTR(L"val", varyColors_ ? L"1" : L"0");
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
void oox_stock_chart::set_properties(std::vector<odf_reader::_property> g)
{
	properties_ = g;
	
	oox_chart::set_properties(g);

	odf_reader::GetProperty(g, L"japanese-candle-stick", bCandleStick);
}
void oox_stock_chart::set_stock_gain_marke(odf_reader::chart::simple & obj)
{
	if (!obj.bEnabled) return;
}
void oox_stock_chart::set_stock_loss_marker(odf_reader::chart::simple & obj)
{
	if (!obj.bEnabled) return;
}
void oox_stock_chart::set_stock_range_line(odf_reader::chart::simple & obj)
{
	if (!obj.bEnabled) return;

	range_line.set(obj.graphic_properties_, obj.fill_);
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
			
			if (range_line.bEnabled)
			{
				CP_XML_NODE(L"c:hiLowLines")
				{
					range_line.oox_serialize(CP_XML_STREAM());
				}
			}
			CP_XML_NODE(L"c:upDownBars")
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
