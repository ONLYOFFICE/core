#pragma once 

#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "xlsx_color.h"
#include "oox_chart_series.h"
#include "oox_data_labels.h"

                    
namespace cpdoccore {
namespace oox {

class oox_chart;
typedef _CP_PTR(oox_chart) oox_chart_ptr;

class oox_chart
{
public:

	oox_chart()
	{
		grouping_ = L"standard";
	}
	~oox_chart(){}
 
	virtual void oox_serialize(std::wostream & _Wostream){}
	
	void oox_serialize_common(std::wostream & _Wostream);

	virtual void add_series(int id){}
	
	void set_formula_series(int ind,std::wstring val);
	void set_values_series(int ind, std::vector<std::wstring> & val);
	void set_name(std::wstring val);
	
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

	oox_data_labels data_labels_;//dLbls (Data Labels) §21.2.2.49



	std::vector<int> axisId_;//axId (Axis ID) §21.2.2.9
	std::wstring grouping_;	//	clustered | percentStacked | stacked | standard 

	virtual void set_properties(std::vector<odf::_property> g);
	virtual void set_additional_properties(std::vector<odf::_property> g){}

	std::vector<oox_series_ptr> series_;	//ser (Bar Chart Series) §21.2.2.170

	//extLst (Chart Extensibility) §21.2.2.64
};

class oox_bar_chart;
typedef _CP_PTR(oox_bar_chart) oox_bar_chart_ptr;

class oox_bar_chart: public oox_chart
{
	public:
	oox_bar_chart()
	{
		grouping_ = L"clustered";
	}
	~oox_bar_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

	virtual void set_additional_properties(std::vector<odf::_property> g);

	static _CP_PTR(oox_chart) create();

	_CP_OPT(bool)	bVertical;	//barDir (Bar Direction) §21.2.2.17
	_CP_OPT(bool)	bConnectBars;
	_CP_OPT(int)	iGapWidth;
	_CP_OPT(int)	iOverlap;

 
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
	}
	~oox_line_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

	static _CP_PTR(oox_chart) create();

	/*odf::chart_interpolation::type*/_CP_OPT(int) iInterpolation;
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
		//grouping_ = L"clustered";
	}
	~oox_area_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

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
		grouping_ = L"";
	}
	~oox_bubble_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);//тип серии чотко соответствует типу самого чарта - если нужны смешанные типы - нужно добавлять свой чарт.
	virtual void set_properties(std::vector<odf::_property> g);
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
	}
	~oox_doughnut_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

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
	}
	~oox_pie_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

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
		grouping_ = L"";
		radarStyle_ = style;
	}
	~oox_radar_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

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
		grouping_ = L"";
	}
	~oox_scatter_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);

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
		grouping_ = L"";
	}
	~oox_stock_chart(){}
 	
	virtual void oox_serialize(std::wostream & _Wostream);
	virtual void add_series(int id);
	virtual void set_properties(std::vector<odf::_property> g);
	virtual void set_additional_properties(std::vector<odf::_property> g)
	{
		graphic_properties_ = g;
	}

	static _CP_PTR(oox_chart) create();


	_CP_OPT(bool) bCandleStick;
	
	std::vector<odf::_property> properties_;
	std::vector<odf::_property> graphic_properties_;

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