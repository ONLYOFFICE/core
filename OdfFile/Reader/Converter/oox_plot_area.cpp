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

#include "oox_plot_area.h"

#include <random>

#include <xml/simple_xml_writer.h>
#include <boost/algorithm/string.hpp>

#include "../Format/style_text_properties.h"

#include "oox_chart_shape.h"

#include "../../DataTypes/chartclass.h"

namespace cpdoccore {
	namespace oox {
		oox_plot_area::oox_plot_area() : no_used_local_tables_(false)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distrib(1001, 9999);

			axis_id_ += distrib(gen);
		}

		void oox_plot_area::add_chart(int type)
		{
			oox_chart_ptr chart;
			switch (type)
			{
			case odf_types::chart_class::line:
				chart = oox_line_chart::create();
				break;
			case odf_types::chart_class::area:
				chart = oox_area_chart::create();
				break;
			case odf_types::chart_class::circle:
				chart = oox_pie_chart::create();
				break;
			case odf_types::chart_class::ring:
				chart = oox_doughnut_chart::create();
				break;
			case odf_types::chart_class::scatter:
				chart = oox_scatter_chart::create();
				break;
			case odf_types::chart_class::radar:
				chart = oox_radar_chart::create();
				break;
			case odf_types::chart_class::bar:
				chart = oox_bar_chart::create();
				break;
			case odf_types::chart_class::stock:
				chart = oox_stock_chart::create();
				break;
			case odf_types::chart_class::bubble:
				chart = oox_bubble_chart::create();
				break;
			case odf_types::chart_class::surface:
				chart = oox_area_chart::create();
				break;
			case odf_types::chart_class::filled_radar:
				chart = oox_radar_chart::create(L"filled");
				break;
			case odf_types::chart_class::gantt:
			default:
				chart = oox_bar_chart::create();
				break;
			}
			charts_.push_back(chart);
		}

		void oox_plot_area::add_axis(int type, odf_reader::chart::axis & content)
		{
			unsigned int id = axis_id_++;
			oox_axis_content_ptr ax = oox_axis_content::create(type, id);
			ax->content_ = content;

			axis_.push_back(ax);
		}
		void oox_plot_area::set_no_local_table(bool val)
		{
			no_used_local_tables_ = val;
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
		void oox_plot_area::oox_serialize_view3D(std::wostream & _Wostream)
		{
			_CP_OPT(std::wstring)	strVal;
			_CP_OPT(double)			doubleVal;

			odf_reader::GetProperty(properties_3d_, L"transform", strVal);

			if (!strVal) return;

			size_t pos_matrix = strVal->find(L"matrix(");

			if (pos_matrix == std::wstring::npos) return;

			size_t pos_matrix_end = strVal->find(L")", pos_matrix);
			if (pos_matrix_end == std::wstring::npos || pos_matrix_end == pos_matrix + 7) return;

			std::wstring tmp = strVal->substr(pos_matrix + 7, pos_matrix_end - 7 - pos_matrix);

			std::vector<std::wstring> values;

			boost::algorithm::split(values, tmp, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

			if (values.size() < 12) return;

			double left_x = XmlUtils::GetDouble(values[0]);
			double up_x = XmlUtils::GetDouble(values[1]);
			double forward_x = XmlUtils::GetDouble(values[2]);
			double left_y = XmlUtils::GetDouble(values[3]);
			double up_y = XmlUtils::GetDouble(values[4]);
			double forward_y = XmlUtils::GetDouble(values[5]);
			double left_z = XmlUtils::GetDouble(values[6]);
			double up_z = XmlUtils::GetDouble(values[7]);
			double forward_z = XmlUtils::GetDouble(values[8]);

			const double DEG2RAD = 3.1415926 / 180;
			double sx, sy, sz, cx, cy, cz, theta_x, theta_y, theta_z = 0;

			sy = forward_x;

			theta_y = (sy == 0 ? 0 : asin(sy));
			cy = cos(theta_y);

			theta_y /= DEG2RAD;

			sx = -forward_y / cy;
			cx = forward_z / cy;

			theta_x = (sx == 0 ? 0 : asin(sx));
			double theta_x_test = (sx == 0 ? 0 : acos(cx));

			theta_x /= DEG2RAD;

			sz = -up_x / cy;
			cz = left_x / cy;

			theta_z = (sz == 0 ? 0 : asin(sz));
			double theta_z_test = (sz == 0 ? 0 : acos(cz));

			theta_z /= DEG2RAD;

			CP_XML_WRITER(_Wostream)
			{
				CP_XML_NODE(L"c:view3D")
				{
					CP_XML_NODE(L"c:rotX")
					{
						CP_XML_ATTR(L"val", (int)(theta_x + 90.5));
					}
					CP_XML_NODE(L"c:rotY")
					{
						CP_XML_ATTR(L"val", (int)(theta_y + 0.5));
					}
					CP_XML_NODE(L"c:depthPercent")
					{
						CP_XML_ATTR(L"val", 100);
					}
					if (theta_z == 0)
					{
						CP_XML_NODE(L"c:rAngAx")
						{
							CP_XML_ATTR(L"val", 1);
						}
					}
					else
					{
						CP_XML_NODE(L"c:rAngAx")
						{
							CP_XML_ATTR(L"val", 0);
						}
						CP_XML_NODE(L"c:perspective")
						{
							CP_XML_ATTR(L"val", (int)(theta_z * 2 + 0.5));
						}
					}

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
