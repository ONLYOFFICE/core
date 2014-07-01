#pragma once
#include "oox_package.h"
#include <cpdoccore/CPNoncopyable.h>

#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "oox_title.h"
#include "oox_plot_area.h"
#include "oox_chart_legend.h"

namespace cpdoccore { 
namespace oox {


class oox_chart_context;
typedef _CP_PTR(oox_chart_context) oox_chart_context_ptr;


class oox_chart_context/*: noncopyable*/
{
public:
    oox_chart_context();
    oox_chart_context(std::wstring const & name);
    ~oox_chart_context();

	std::wostream & chartData();

    bool empty() const;

	void write_to(std::wostream & strm);
	static oox_chart_context_ptr create(std::wstring const & name);
	static oox_chart_context_ptr create();

	void set_title(odf::chart::title & t)
	{
		title_.set_content(t);
	}
	void add_axis(int type)
	{
		plot_area_.add_axis(type);
	}
	void set_content_axis(odf::chart::axis & content)
	{
		plot_area_.set_content_axis(content);
	}
	void set_content_series(odf::chart::series & content)
	{
		plot_area_.set_content_series(content);
	}
	void add_chart(int type)
	{
		plot_area_.add_chart(type);
		plot_area_.current_chart_ = plot_area_.charts_.back();
	}
	oox_chart_ptr get_current_chart()
	{
		return plot_area_.current_chart_;
	}
	void set_wall(odf::chart::simple & l)
	{
		//plot_area_.wall_graphic_properties_	= l.graphic_properties_;
		plot_area_.graphic_properties_	= l.graphic_properties_;
		plot_area_.fill_	= l.fill_;
	}
	void set_floor(odf::chart::simple & l)
	{
		//floor_.content_= l;
	}
	void set_legend(odf::chart::simple & l)
	{
		legend_.content_= l;
	}

	void set_chart_graphic_properties(std::vector<odf::_property> & prop, _oox_fill &fill)
	{
		graphic_properties_= prop;
		fill_ = fill;
	}
	void set_plot_area_properties(std::vector<odf::_property> & prop, _oox_fill &fill)
	{
		plot_area_.properties_		= prop; 
		plot_area_.fill_			= fill; 
	}
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
	
	cpdoccore::oox::oox_title			title_;
	cpdoccore::oox::oox_plot_area		plot_area_;
	cpdoccore::oox::oox_chart_legend	legend_;

	std::vector<odf::_property>			graphic_properties_;
	_oox_fill							fill_;
              
};
//autoTitleDeleted (Auto Title Is Deleted) §21.2.2.7
//backWall (Back Wall) §21.2.2.11
//dispBlanksAs (Display Blanks As) §21.2.2.42
//extLst (Chart Extensibility) §21.2.2.64
//floor (Floor) §21.2.2.69
//legend (Legend) §21.2.2.93
//pivotFmts (Pivot Formats) §21.2.2.143
//plotArea (Plot Area) §21.2.2.145
//plotVisOnly (Plot Visible Only) §21.2.2.146
//showDLblsOverMax (Show Data Labels over Maximum) §21.2.2.180
//sideWall (Side Wall) §21.2.2.191
//title (Title) §21.2.2.210
//view3D (View In 3D)
}
}