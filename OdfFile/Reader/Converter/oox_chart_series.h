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

#include <iosfwd>

#include "../Format/chart_objects.h"

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
	void oox_serialize_marker(std::wostream & _Wostream);
	
	virtual void oox_serialize(std::wostream & _Wostream) = 0;
	
	bool						is3D_;

	bool						labelPosEnabled_;
	_oox_chart_values			values_[5]; //cat, common, x, y, bubble, 

	_oox_strRef					label_; //tx (Series Text) §21.2.2.215
	int							id_;
	std::wstring				name_;	
	int							iSymbolMarkerType_;

	bool						bCacheOnly_; // used embedded or only cache values

	_CP_OPT(oox_data_labels)	data_labels_;
	
	void setName(const std::wstring &value);
	
	void setFormula	(int ind, const std::wstring & value, const std::wstring & formatCode, bool link_to_source);
	void setValues	(int ind, std::vector<std::wstring> & values);
	void setLabels	(const std::wstring &formula, std::vector<std::wstring> & values);
	void parse_properties();
	
	void set_cache_only (bool val);

	odf_reader::chart::series content_;

	virtual void set_properties(std::vector<odf_reader::_property> g) {}

	//extLst (Chart Extensibility) §21.2.2.64
	//idx (Index) §21.2.2.84
	//order (Order) §21.2.2.128

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
	
	virtual void set_properties(std::vector<odf_reader::_property> g);

	virtual void oox_serialize(std::wostream & _Wostream);
	
	_CP_OPT(int)	iSolidType;
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
