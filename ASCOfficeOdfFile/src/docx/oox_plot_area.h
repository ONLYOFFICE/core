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
 
    std::vector<oox_chart_ptr> charts_;
	oox_chart_ptr current_chart_;   
	
	std::vector<odf_reader::_property> graphic_properties_;
	std::vector<odf_reader::_property> properties_;
	_oox_fill					fill_; 
	
	//std::vector<odf_reader::_property> wall_graphic_properties_;
	
	void oox_serialize(std::wostream & _Wostream);

	void add_chart(int type);

 	void add_series(int id)
	{
		current_chart_->add_series(id);
	}
	
	std::vector<oox_axis_content_ptr> axis_;

	void add_axis(int type)
	{
		oox_axis_content_ptr ax=oox_axis_content::create(type);

		axis_.push_back(ax);
	}
	void set_content_axis(odf_reader::chart::axis & content)
	{
		axis_.back()->content_=content;
	}
	void set_content_series(odf_reader::chart::series & content)
	{
		current_chart_->series_.back()->content_=content;
	}
	void reset_cross_axis()//обязательно после всех добавлений
	{
		BOOST_FOREACH(oox_axis_content_ptr const & ax, axis_)
		{
			BOOST_FOREACH(oox_chart_ptr const & ch, charts_)
			{
				ch->add_axis(ax->get_Id());		
			}
		}
		
		BOOST_FOREACH(oox_axis_content_ptr const & a, axis_)
		{
			int curr_id = a->get_Id();
			BOOST_FOREACH(oox_axis_content_ptr const & b, axis_)
			{
				if (b->get_Id()==curr_id)continue;
				b->add_CrossedId(curr_id);
			}
		}
	}

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
