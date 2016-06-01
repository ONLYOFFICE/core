#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include "../odf/chart_objects.h"

#include "oox_chart_values.h"
#include "oox_data_labels.h"
              
namespace cpdoccore {
namespace oox {


class oox_chart_series;
typedef _CP_PTR(oox_chart_series) oox_series_ptr;

class oox_chart_series
{
public:
	oox_chart_series();
	~oox_chart_series(){}
 
    void oox_serialize_common(std::wostream & _Wostream);
	virtual void oox_serialize(std::wostream & _Wostream) = 0;
	
	_oox_chart_values			values_[5]; //cat(labels), common, x, y, bubble, 
	int							id_;
	std::wstring				name_;	//tx (Series Text) §21.2.2.215
	int							iSymbolMarkerType_;
	bool						bLocalTable_;
	_CP_OPT(oox_data_labels)	data_labels_;
	
	void setName(std::wstring &value);
	
	void setFormula	(int ind, std::wstring & value, std::wstring & formatCode, bool link_to_source);
	void setValues	(int ind, std::vector<std::wstring> & values);
	void parse_properties();
	
	void set_cache_only (bool val);

	odf_reader::chart::series content_;

	//dPt (Data Point) §21.2.2.52
	//extLst (Chart Extensibility) §21.2.2.64
	//idx (Index) §21.2.2.84
	//order (Order) §21.2.2.128
	//spPr (Shape Properties) §21.2.2.197

};
////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_bar_series;
typedef _CP_PTR(oox_bar_series) oox_bar_series_ptr;

class oox_bar_series : public oox_chart_series
{
public:
	oox_bar_series(){}
	~oox_bar_series(){}
	
	static oox_series_ptr create();
	
	virtual void oox_serialize(std::wostream & _Wostream);
};
	//cat (Category Axis Data) §21.2.2.24
	//errBars (Error Bars) §21.2.2.55
	//invertIfNegative (Invert if Negative) §21.2.2.86
	//pictureOptions (Picture Options) §21.2.2.138
	//shape (Shape) §21.2.2.177
	//trendline (Trendlines) §21.2.2.211
	//val (Values)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_line_series;
typedef _CP_PTR(oox_line_series) oox_line_series_ptr;

class oox_line_series : public oox_chart_series
{
public:
	oox_line_series(){}
	~oox_line_series(){}
	
	static oox_series_ptr create();

	virtual void oox_serialize(std::wostream & _Wostream);

};
	//cat (Category Axis Data) §21.2.2.24
	//errBars (Error Bars) §21.2.2.55
	//marker (Marker) §21.2.2.106
	//smooth (Smoothing) §21.2.2.194
	//trendline (Trendlines) §21.2.2.211
	//val (Values) §21.2.2.224
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_pie_series;
typedef _CP_PTR(oox_pie_series) oox_pie_series_ptr;

class oox_pie_series : public oox_chart_series
{
public:
	oox_pie_series(){}
	~oox_pie_series(){}
	
	static oox_series_ptr create();

	virtual void oox_serialize(std::wostream & _Wostream);

};
	//cat (Category Axis Data) §21.2.2.24
	//explosion (Explosion) §21.2.2.61
	//val (Values) §21.2.2.224
/////////////////////////////////////////////////////////////////////////////////////////////////////////////class oox_pie_series;
class oox_bubble_series;
typedef _CP_PTR(oox_bubble_series) oox_bubble_series_ptr;

class oox_bubble_series : public oox_chart_series
{
public:
	oox_bubble_series(){}
	~oox_bubble_series(){}
	
	static oox_series_ptr create();

	virtual void oox_serialize(std::wostream & _Wostream);

};
	//bubble3D (3D Bubble) §21.2.2.19
	//bubbleSize (Bubble Size) §21.2.2.22
	//errBars (Error Bars) §21.2.2.55
	//invertIfNegative (Invert if Negative) §21.2.2.86
	//trendline (Trendlines) §21.2.2.211
	//xVal (X Values) §21.2.2.234
	//yVal (Y Values)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class oox_scatter_series;
typedef _CP_PTR(oox_scatter_series) oox_scatter_series_ptr;

class oox_scatter_series : public oox_chart_series
{
public:
	oox_scatter_series(){}
	~oox_scatter_series(){}
	
	static oox_series_ptr create();

	virtual void oox_serialize(std::wostream & _Wostream);

};
	//errBars (Error Bars) §21.2.2.55
	//marker (Marker) §21.2.2.106
	//smooth (Smoothing) §21.2.2.194
	//trendline (Trendlines) §21.2.2.211
	//xVal (X Values) §21.2.2.234
	//yVal (Y Values) §21.2.2.237

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class oox_area_series;
typedef _CP_PTR(oox_area_series) oox_area_series_ptr;

class oox_area_series : public oox_chart_series
{
public:
	oox_area_series(){}
	~oox_area_series(){}
	
	static oox_series_ptr create();

	virtual void oox_serialize(std::wostream & _Wostream);

};
	//cat (Category Axis Data) §21.2.2.24
	//errBars (Error Bars) §21.2.2.55
	//pictureOptions (Picture Options) §21.2.2.138
	//trendline (Trendlines) §21.2.2.211

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class oox_radar_series;
typedef _CP_PTR(oox_radar_series) oox_radar_series_ptr;

class oox_radar_series : public oox_chart_series
{
public:
	oox_radar_series(){}
	~oox_radar_series(){}
	
	static oox_series_ptr create();

	virtual void oox_serialize(std::wostream & _Wostream);
};
	//cat (Category Axis Data) §21.2.2.24
	//marker (Marker) §21.2.2.106
	//val (Values) §21.2.2.224

}
}
