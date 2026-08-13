/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "oox_plot_area.h"

#include <cmath>
#include <random>

#include <xml/simple_xml_writer.h>
#include <boost/algorithm/string.hpp>

#include "../Format/odfcontext.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_chart_properties.h"

#include "oox_chart_shape.h"

#include "../../DataTypes/chartclass.h"

namespace cpdoccore {
	namespace oox {
		namespace
		{
			const double DEG2RAD = 3.14159265358979323846 / 180.;

			int round_angle(double val, int min, int max)
			{
				int result = (int)floor(val + 0.5);

				if (result < min) result = min;
				if (result > max) result = max;

				return result;
			}
			double normalize_angle(double val)
			{
				val = fmod(val, 360.);

				if (val < 0) val += 360.;

				return val;
			}
		}
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

		void oox_plot_area::add_axis(odf_reader::chart::axis& content, const _CP_OPT(double)& rel_x,
			const _CP_OPT(double)& rel_y, const _CP_OPT(double)& rel_w, const _CP_OPT(double)& rel_h)
		{
			unsigned int id = content.type_ != 0 ? axis_id_++ : 0;
			oox_axis_content_ptr ax = oox_axis_content::create(id);
			ax->content_ = content;
			ax->set_title_layout(rel_x, rel_y, rel_w, rel_h);

			if (content.dimension_ == L"x") axis_.insert(axis_.begin(), ax);
			else axis_.push_back(ax);
		}
		void oox_plot_area::set_no_local_table(bool val)
		{
			no_used_local_tables_ = val;
		}
		void oox_plot_area::set_data_table(odf_reader::chart::simple& content)
		{
			data_table_content_ = content;
		}
		void oox_plot_area::reset_cross_axis()//must be called after all additions
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
		void oox_plot_area::oox_serialize_view3D(std::wostream& _Wostream)
		{
			_CP_OPT(std::wstring)	strVal;
			_CP_OPT(bool)			perspective;
			_CP_OPT(bool)			right_angled_axes;

			odf_reader::GetProperty(properties_, L"transform", strVal);
			odf_reader::GetProperty(properties_, L"perspective", perspective);
			odf_reader::GetProperty(properties_, L"right-angled-axes", right_angled_axes);

			if (!strVal) return;

			size_t pos_matrix = strVal->find(L"matrix(");

			if (pos_matrix == std::wstring::npos) return;

			size_t pos_matrix_end = strVal->find(L")", pos_matrix);
			if (pos_matrix_end == std::wstring::npos || pos_matrix_end == pos_matrix + 7) return;

			std::wstring tmp = strVal->substr(pos_matrix + 7, pos_matrix_end - 7 - pos_matrix);

			std::vector<std::wstring> values;

			boost::algorithm::split(values, tmp, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

			if (values.size() < 12) return;

			//dr3d:transform="matrix(a b c d e f g h i j k l)" is column major - the first
			//triple is the first column of the matrix (the image of the X axis), the last
			//triple is the translation
			double m[3][3];

			for (int col = 0; col < 3; col++)
			{
				for (int row = 0; row < 3; row++)
				{
					m[row][col] = XmlUtils::GetDouble(values[col * 3 + row]);
				}
			}

			//R = Rx(theta_x) * Ry(theta_y) * Rz(theta_z). atan2 keeps the quadrant and
			//needs no division by cos(theta_y), which is zero at the gimbal lock
			double sin_y = m[0][2];

			if (sin_y >  1.) sin_y =  1.;	//rounding in the source matrix
			if (sin_y < -1.) sin_y = -1.;

			//theta_z, the roll of the scene in the screen plane, would be
			//atan2(-m[0][1], m[0][0]) - it has no counterpart in c:view3D and is dropped
			double theta_x = atan2(-m[1][2], m[2][2]) / DEG2RAD;
			double theta_y = asin(sin_y) / DEG2RAD;

			if (this->current_chart_->type_ == CHART_TYPE_RADAR ||
				this->current_chart_->type_ == CHART_TYPE_PIE ||
				this->current_chart_->type_ == CHART_TYPE_DOUGHNUT)
			{
				theta_x = 90 - theta_x;
			}
			else theta_x = (std::abs)(theta_x);

			bool bRightAngledAxes = right_angled_axes ? *right_angled_axes
													  : !(perspective && *perspective);
			CP_XML_WRITER(_Wostream)
			{
				CP_XML_NODE(L"c:view3D")
				{
					CP_XML_NODE(L"c:rotX")
					{
						CP_XML_ATTR(L"val", round_angle(theta_x, -90, 90));
					}
					CP_XML_NODE(L"c:rotY")
					{
						CP_XML_ATTR(L"val", round_angle(normalize_angle(theta_y), 0, 360));
					}
					CP_XML_NODE(L"c:depthPercent")
					{
						CP_XML_ATTR(L"val", 100);
					}
					CP_XML_NODE(L"c:rAngAx")
					{
						CP_XML_ATTR(L"val", bRightAngledAxes ? 1 : 0);
					}
					if (false == bRightAngledAxes)
					{
						CP_XML_NODE(L"c:perspective")
						{
							CP_XML_ATTR(L"val", 30);	//default
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
					layout_.oox_serialize(CP_XML_STREAM());

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
					if (data_table_content_.bEnabled)
					{
						_CP_OPT(bool) boolVal;

						CP_XML_NODE(L"c:dTable")
						{
							odf_reader::GetProperty(data_table_content_.properties_, L"show-horizontal-border", boolVal);
							if (boolVal)
							{
								CP_XML_NODE(L"c:showHorzBorder")
								{
									CP_XML_ATTR(L"val", *boolVal);
								}
							}
							odf_reader::GetProperty(data_table_content_.properties_, L"show-vertical-border", boolVal);
							if (boolVal)
							{
								CP_XML_NODE(L"c:showVertBorder")
								{
									CP_XML_ATTR(L"val", *boolVal);
								}
							}
							odf_reader::GetProperty(data_table_content_.properties_, L"show-outline", boolVal);
							if (boolVal)
							{
								CP_XML_NODE(L"c:showOutline")
								{
									CP_XML_ATTR(L"val", *boolVal);
								}
							}
							odf_reader::GetProperty(data_table_content_.properties_, L"show-keys", boolVal);
							if (boolVal)
							{
								CP_XML_NODE(L"c:showKeys")
								{
									CP_XML_ATTR(L"val", *boolVal);
								}
							}
							
							oox_chart_shape shape_table;
							shape_table.set(data_table_content_.graphic_properties_, data_table_content_.fill_);
							shape_table.oox_serialize(CP_XML_STREAM());
							
							if (data_table_content_.text_properties_)
							{
								CP_XML_NODE(L"c:txPr")
								{
									CP_XML_NODE(L"a:bodyPr") {}
									CP_XML_NODE(L"a:lstStyle") {}

									CP_XML_NODE(L"a:p")
									{
										CP_XML_NODE(L"a:pPr")
										{
											CP_XML_NODE(L"a:defRPr")
											{
												//odf_reader::fonts_container & fonts = context.fontContainer();
												odf_reader::fonts_container fonts;
												data_table_content_.text_properties_->oox_serialize(CP_XML_STREAM(), true, fonts);
											}
										}
									}
								}
							}
						}
					}
					shape.oox_serialize(CP_XML_STREAM());
				}
			}
		}
	}
}
