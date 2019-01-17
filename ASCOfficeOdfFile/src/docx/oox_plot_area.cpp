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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "oox_plot_area.h"

#include <CPOptional.h>
#include <xml/simple_xml_writer.h>

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
	oox_axis_content_ptr ax = oox_axis_content::create(type);
	ax->content_ = content;

	axis_.push_back(ax);
}

void oox_plot_area::reset_cross_axis()//обязательно после всех добавлений
{
	for (size_t i = 0; i < axis_.size(); i++)
	{
		for (size_t j = 0; j < charts_.size(); j++)
		{
			charts_[j]->add_axis(axis_[i]->get_Id());		
		}
	}
	
	for (size_t i = 0; i < axis_.size(); i++)
	{		
		int curr_id = axis_[i]->get_Id();

		if (curr_id < 1) continue;

		for (size_t j = 0; j < axis_.size(); j++)
		{	
			if (axis_[j]->get_Id() == curr_id)continue;
			
			axis_[j]->add_CrossedId(curr_id);
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
			
			for (size_t i = 0; i < charts_.size(); i++)
			{		
				charts_[i]->oox_serialize(CP_XML_STREAM());

				if (charts_[i]->type_ == CHART_TYPE_PIE || 
					charts_[i]->type_ == CHART_TYPE_DOUGHNUT) axisPresent = false;
			}
			if (axisPresent)
			{
				for (size_t i = 0; i < axis_.size(); i++)
				{		
					axis_[i]->oox_serialize(CP_XML_STREAM());
				}
			}
			shape.oox_serialize(CP_XML_STREAM());
		}
    }
}
}
}
