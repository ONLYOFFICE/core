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
#pragma once 

#include <boost/noncopyable.hpp>

#include "oox_chart_shape.h"

#include "xlsx_color.h"
#include "oox_chart_series.h"
#include "oox_data_labels.h"

#define CHART_TYPE_LINE			1
#define CHART_TYPE_AREA			2
#define CHART_TYPE_PIE			3
#define CHART_TYPE_DOUGHNUT		4
#define CHART_TYPE_SCATTER		5
#define CHART_TYPE_RADAR		6
#define CHART_TYPE_BAR			7
#define CHART_TYPE_STOCK		8
#define CHART_TYPE_BUBBLE		9

namespace cpdoccore {
namespace oox {

class oox_chart;
typedef _CP_PTR(oox_chart) oox_chart_ptr;

class oox_chart
{
public:

	oox_chart()
	{
		grouping_		= L"standard";
		is3D_			= false;
		type_			= 0;
		dispBlanksAs_	= L"zero";
		varyColors_		= true;
	}
	~oox_chart(){}
 
	virtual void oox_serialize(std::wostream & _Wostream) = 0;
	
	void oox_serialize_common(std::wostream & _Wostream);

	virtual void add_series(int id){}
	
	void set_cache_only		(bool val);
	void set_formula_series	(int ind, const std::wstring& val, const std::wstring& formatCode, bool link_to_source);
	void set_values_series	(int ind, std::vector<std::wstring> & val);
	void set_label_series	(const std::wstring& formula, std::vector<std::wstring> & val);
	void set_name			(const std::wstring& val);
	void set_content_series	(odf_reader::chart::series & content);
	
	virtual void set_stock_gain_marke(odf_reader::chart::simple & obj){}
	virtual void set_stock_loss_marker(odf_reader::chart::simple & obj){}
	virtual void set_stock_range_line(odf_reader::chart::simple & obj){}
	//void set_showBubbleSize(bool Val){data_labels_.set_showBubbleSize(Val);}
	//void set_showCatName(bool Val){data_labels_.set_showCatName(Val);}
	//void set_showLeaderLines(bool Val){data_labels_.set_showLeaderLines(Val);}
	//void set_showLegendKey(bool Val){data_labels_.set_showLegendKey(Val);}
	//void set_showPercent(bool Val){data_labels_.set_showPercent(Val);}
	//void set_showSerName(bool Val){data_labels_.set_showSerName(Val);}
	//void set_showVal(bool Val){data_labels_.set_showVal(Val);}

	void add_axis(int id)
	{
		axisId_.push_back(id);
	}

	oox_data_labels					data_labels_;//dLbls (Data Labels) §21.2.2.49

	int								type_;
	bool							is3D_;
	bool							varyColors_;
	std::wstring					dispBlanksAs_;
	std::vector<int>				axisId_;	//	axId (Axis ID) §21.2.2.9
	std::wstring					grouping_;	//	clustered | percentStacked | stacked | standard 
	std::vector<oox_series_ptr>		series_;	//	ser (Bar Chart Series) §21.2.2.170

	virtual void set_properties(std::vector<odf_reader::_property> g);
	virtual void set_additional_properties(std::vector<odf_reader::_property> g){}


	//extLst (Chart Extensibility) §21.2.2.64
};

class oox_bar_chart;
typedef _CP_PTR(oox_bar_chart) oox_bar_chart_ptr;

class oox_bar_chart: public oox_chart
{
	public:
	oox_bar_chart()
	{
		type_		= CHART_TYPE_BAR;
		grouping_	= L"clustered";
	}
	~oox_bar_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	virtual void set_additional_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create();

	_CP_OPT(bool)	bVertical;	//barDir (Bar Direction) §21.2.2.17
	_CP_OPT(bool)	bConnectBars;
	_CP_OPT(int)	iGapWidth;
	_CP_OPT(int)	iOverlap;
	_CP_OPT(int)	iSolidType;
 
	//gapWidth (Gap Width) §21.2.2.75
	//overlap (Overlap) §21.2.2.131
	//serLines (Series Lines) §21.2.2.176
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class oox_line_chart;
typedef _CP_PTR(oox_line_chart) xlsx_line_chart_ptr;

class oox_line_chart: public oox_chart
{
	public:
	oox_line_chart()
	{
		type_	= CHART_TYPE_LINE;
	}
	~oox_line_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create();

	/*odf_reader::chart_interpolation::type*/_CP_OPT(int) iInterpolation;
	_CP_OPT(bool)bLines;

	//dropLines (Drop Lines) §21.2.2.53
	//hiLowLines (High Low Lines) §21.2.2.80
	//marker (Show Marker) §21.2.2.105
	//smooth (Smoothing) §21.2.2.194
	//upDownBars (Up/Down Bars) §21.2.2.218

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_area_chart;
typedef _CP_PTR(oox_area_chart) xlsx_area_chart_ptr;

class oox_area_chart: public oox_chart
{
	public:
	oox_area_chart()
	{
		type_		= CHART_TYPE_AREA;
		//grouping_ = L"clustered";
	}
	~oox_area_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create();

	//dropLines (Drop Lines) §21.2.2.53
	//ser (Area Chart Series) §21.2.2.168
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_bubble_chart;
typedef _CP_PTR(oox_area_chart) xlsx_bubble_chart_ptr;

class oox_bubble_chart: public oox_chart
{
	public:
	oox_bubble_chart()
	{
		type_		= CHART_TYPE_BUBBLE;
		grouping_	= L"";
	}
	~oox_bubble_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);//тип серии чотко соответствует типу самого чарта - если нужны смешанные типы - нужно добавлять свой чарт.
	virtual void set_properties(std::vector<odf_reader::_property> g);
	// то есть первична серия - в отличии от одф

	static _CP_PTR(oox_chart) create();

	//bubble3D (3D Bubble) §21.2.2.19
	//bubbleScale (Bubble Scale) §21.2.2.21
	//ser (Bubble Chart Series) §21.2.2.174
	//showNegBubbles (Show Negative Bubbles) §21.2.2.185
	//sizeRepresents (Size Represents) §21.2.2.193
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_doughnut_chart;
typedef _CP_PTR(oox_doughnut_chart) xlsx_doughnut_chart_ptr;

class oox_doughnut_chart: public oox_chart
{
	public:
	oox_doughnut_chart()
	{
		type_		= CHART_TYPE_DOUGHNUT;
		grouping_	= L"";
	}
	~oox_doughnut_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create();

	//firstSliceAng (First Slice Angle) §21.2.2.68
	//holeSize (Hole Size) §21.2.2.82
	//ser (Pie Chart Series) §21.2.2.172
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_pie_chart;
typedef _CP_PTR(oox_pie_chart) xlsx_pie_chart_ptr;

class oox_pie_chart: public oox_chart
{
	public:
	oox_pie_chart()
	{
		type_		= CHART_TYPE_PIE;
		grouping_	= L"";
	}
	~oox_pie_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create();

	//firstSliceAng (First Slice Angle) §21.2.2.68
	//ser (Pie Chart Series) §21.2.2.172
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_radar_chart;
typedef _CP_PTR(oox_radar_chart) xlsx_radar_chart_ptr;

class oox_radar_chart: public oox_chart
{
	public:
	oox_radar_chart(std::wstring style)
	{
		type_		= CHART_TYPE_RADAR;
		grouping_	= L"";
		radarStyle_ = style;
	}
	~oox_radar_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create(std::wstring radarStyle = L"marker");

	std::wstring radarStyle_;
	//radarStyle (Radar Style) §21.2.2.154
	//ser (Radar Chart Series) §21.2.2.169
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_scatter_chart;
typedef _CP_PTR(oox_scatter_chart) xlsx_scatter_chart_ptr;

class oox_scatter_chart: public oox_chart
{
	public:
	oox_scatter_chart()
	{
		type_		= CHART_TYPE_SCATTER;
		grouping_	= L"";
	}
	~oox_scatter_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);

	static _CP_PTR(oox_chart) create();

	_CP_OPT(int)			iRegressionType;

	//scatterStyle (Scatter Style) §21.2.2.162
	//ser (Scatter Chart Series) §21.2.2.167
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_stock_chart;
typedef _CP_PTR(oox_stock_chart) xlsx_stock_chart_ptr;

class oox_stock_chart: public oox_chart
{
	public:
	oox_stock_chart()
	{
		type_		= CHART_TYPE_STOCK;
		grouping_	= L"";
	}
	~oox_stock_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf_reader::_property> g);
	virtual void set_additional_properties(std::vector<odf_reader::_property> g)
	{
		graphic_properties_ = g;
	}

	static _CP_PTR(oox_chart) create();

	virtual void set_stock_gain_marke(odf_reader::chart::simple & obj);
	virtual void set_stock_loss_marker(odf_reader::chart::simple & obj);
	virtual void set_stock_range_line(odf_reader::chart::simple & obj);

	_CP_OPT(bool) bCandleStick;
	
	std::vector<odf_reader::_property> properties_;
	std::vector<odf_reader::_property> graphic_properties_;

	_oox_fill					fill_;

	oox_chart_shape range_line;
	//dropLines (Drop Lines) §21.2.2.53
	//hiLowLines (High Low Lines) §21.2.2.80
	//ser (Line Chart Series) §21.2.2.171
	//upDownBars (Up/Down Bars) §21.2.2.218

};

}
}
////////////////////////////////////////
//area3DChart (3D Area Charts) §21.2.2.4
//bar3DChart (3D Bar Charts) §21.2.2.15
//line3DChart (3D Line Charts) §21.2.2.96
//surface3DChart (3D Surface Charts) §21.2.2.203
//pie3DChart (3D Pie Charts) §21.2.2.140
//ofPieChart (Pie of Pie or Bar of Pie Charts) §21.2.2.126
//surfaceChart (Surface Charts) §21.2.2.204