
#include "oox_plot_area.h"
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "../odf/style_text_properties.h"

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

void oox_plot_area::add_chart(int type)
{
	oox_chart_ptr chart ;
	switch(type)
	{			
	case 1://chart_line:
		chart = oox_line_chart::create();
		break;
	case 2://chart_area:
		chart = oox_area_chart::create();
		break;
	case 3://chart_circle:
		chart = oox_pie_chart::create();
		break;	
	case 4://chart_ring:
		chart = oox_doughnut_chart::create();
		break;	
	case 5://chart_scatter:
		chart = oox_scatter_chart::create();
		break;	
	case 6://chart_radar:		
		chart = oox_radar_chart::create();
		break;	
	case 7://chart_bar:		
		chart = oox_bar_chart::create();
		break;
	case 8://chart_stock:		
		chart = oox_stock_chart::create();
		break;
	case 9://chart_bubble:
		chart = oox_bubble_chart::create();
		break;			
	case 10://chart_surface:
		chart = oox_area_chart::create();
		break;	
	case 12://filled radar
		chart = oox_radar_chart::create(L"filled");
		break;
	case 11://chart_gantt:
	default:
		chart = oox_bar_chart::create();
		break;
	}
	charts_.push_back(chart);
}

void oox_plot_area::add_axis(int type, odf_reader::chart::axis & content)
{
	oox_axis_content_ptr ax=oox_axis_content::create(type);
	ax->content_ = content;

	axis_.push_back(ax);
}

void oox_plot_area::reset_cross_axis()//обязательно после всех добавлений
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
void oox_plot_area::oox_serialize(std::wostream & _Wostream)
{
	reset_cross_axis();

	oox_chart_shape shape;
	shape.set(graphic_properties_, fill_);

    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:plotArea")
        {
			//CP_XML_NODE(L"c:layout"){}
			bool axisPresent = true;
			
			BOOST_FOREACH(oox_chart_ptr const & ch, charts_)
			{
				ch->oox_serialize(CP_XML_STREAM());

				if (ch->type_ == CHART_TYPE_PIE || 
					ch->type_ == CHART_TYPE_DOUGHNUT) axisPresent = false;
			}
			if (axisPresent)
			{
				BOOST_FOREACH(oox_axis_content_ptr const & a, axis_)
				{
					a->oox_serialize(CP_XML_STREAM());
				}
			}
			shape.oox_serialize(CP_XML_STREAM());
		}
    }
}
}
}
