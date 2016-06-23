#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

#include "oox_types_chart.h"
#include "oox_chart_axis.h"

            
namespace cpdoccore {
namespace oox {

class oox_plot_area: boost::noncopyable
{
public:
	oox_plot_area(){}
	~oox_plot_area(){}
 
    std::vector<oox_chart_ptr>			charts_;
	oox_chart_ptr						current_chart_;   
	std::vector<oox_axis_content_ptr>	axis_;
	
	std::vector<odf_reader::_property>	graphic_properties_;
	std::vector<odf_reader::_property>	properties_;
	_oox_fill							fill_; 
	
	//std::vector<odf_reader::_property> wall_graphic_properties_;
	
	void oox_serialize(std::wostream & _Wostream);

	void add_chart	(int type);
	void add_axis	(int type,	odf_reader::chart::axis & content);

 	//void set_content_series	(odf_reader::chart::series & content);

private:
	void reset_cross_axis();//обязательно после всех добавлений


//variable charts 
//catAx (Category Axis Data) §21.2.2.25
//dateAx (Date Axis) §21.2.2.39
//serAx (Series Axis) §21.2.2.175
//valAx (Value Axis) §21.2.2.226

//extLst (Chart Extensibility) §21.2.2.64
//layout (Layout) §21.2.2.88
//spPr (Shape Properties) §21.2.2.197
//dTable (Data Table) §21.2.2.54

};

}
}
