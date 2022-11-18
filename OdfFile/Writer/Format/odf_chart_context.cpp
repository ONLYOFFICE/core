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

#include "logging.h"

#include <boost/algorithm/string.hpp>

#include "../../Formulas/formulasconvert.h"

#include <iostream>

#include "odf_chart_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_drawing_context.h"
 
#include "odf_conversion_context.h"

#include "ods_table_state.h"//???

#include "office_chart.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"
#include "style_chart_properties.h"

namespace cpdoccore 
{
	using namespace odf_types;

namespace odf_writer
{
	static const std::wstring default_MS_series_colors[] = 
	{
        L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
        //todooo - продолжить .... пока копия первых
        L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
        L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
        L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
        L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
        L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934"
	};

	struct 	odf_axis_state
	{
		unsigned int		oox_id;
		int					dimension;//1 -x, 2, -y, 3 -z
		std::wstring		name;
		office_element_ptr	elm;
	};
	struct 	odf_cash_state
	{
		std::wstring ref;
		std::wstring format_code;

		bool categories;
		bool label;

		std::vector<std::wstring>	data_str;
	};
	struct 	odf_chart_state
	{
		odf_chart_state() {clear();}
		void clear()
		{
			elements_.clear();
			chart_width_pt = chart_height_pt = 0;
		}
		double chart_width_pt;
		double chart_height_pt;

		std::vector<odf_element_state>	elements_;
	};

	struct odf_chart_level_state
	{
		style_text_properties		*text_properties_;
		graphic_format_properties	*graphic_properties_;
		style_paragraph_properties	*paragraph_properties_;
		style_chart_properties		*chart_properties_;
		
		office_element_ptr	elm;
	};	
	struct _cell_cash
	{
		int				col;
		int				row;
		bool			label;
		bool			cash_only;
		std::wstring	val;
	};
	struct odf_category_state
	{
		std::wstring ref;
		std::wstring format_code;
		int type;

	};
class odf_chart_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) : odf_context_(odf_context)
    {	
		styles_context_			= NULL;
		current_series_count_	= 0;
		local_table_reset_ref_	= false;

		if (odf_context_->type == SpreadsheetDocument)
		{
			local_table_enabled_ = false;
		}
		else
		{
			local_table_enabled_ = true;
		}
		
	} 

	odf_chart_state							current_chart_state_;
	int										current_series_count_;
	int										current_data_points_series_count_;
	
	std::vector<odf_category_state>			categories_;
	std::vector<odf_axis_state>				axis_;
	std::vector<office_element_ptr>			group_series_;
	std::vector<unsigned int>				axis_group_series_;

	_CP_OPT(int)							bar_overlap;
	_CP_OPT(int)							bar_gap_width;

	struct _range
	{
		_range(const std::wstring &r, const std::wstring &fmt_code, bool l, chart_series *s)
			: label(l),	index_cash(-1), series(s), ref(r), format_code(fmt_code)
		{}
		_range()											
			: label(false), index_cash(-1), series(NULL)
		{}
		
		chart_series	*series;
		std::wstring	ref;
		std::wstring	format_code;
		bool			label;
		int				index_cash;
	};
	std::vector<_range>					data_cell_ranges_;
	
	std::vector<odf_cash_state>			cash_;
	bool								local_table_enabled_;
	bool								local_table_reset_ref_;

	std::vector<odf_chart_level_state>	current_level_;	//постоянно меняющийся список уровней наследования
	std::vector<odf_chart_state>		chart_list_;		//все элементы .. для удобства разделение по "топам"
////////
	void set_default_series_color();
	void clear_current();
	odf_style_context			*styles_context_;
	odf_conversion_context		*odf_context_;

	chart_chart					*get_current_chart();
	chart_axis					*get_current_axis();
	chart_series				*get_current_series();

	odf_types::chart_class::type get_current_chart_class();

	std::wstring convert_formula(std::wstring oox_ref);

	void create_local_table();
	int  create_local_table_rows(int current_row, ods_table_state * table_state,std::vector<_cell_cash> & cells, bool header );
};

static formulasconvert::oox2odf_converter formulas_converter_chart;

std::wstring odf_chart_context::Impl::convert_formula(std::wstring oox_ref)
{
	std::vector<std::wstring> refs;
	boost::algorithm::split(refs, oox_ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
	
	if (refs.empty()) return L"";

	if (local_table_enabled_)
	{
		if (local_table_reset_ref_) return L"";

		//remove table name
		if (!refs.empty())
		{		
			size_t pos = refs[0].rfind(L"!");//в имени таблички может быть...
			if (std::wstring::npos != pos)
			{
				refs[0] = L"local-table!" + refs[0].substr(pos + 1, refs[0].size() - pos);
			}
			int col = -1, row = -1;
			utils::parsing_ref( refs[0], col, row);
			
			if (col < 0 && row < 0)
				return L"";

			oox_ref = refs[0];
			if (refs.size() > 1)
			{
				size_t r = refs[1].rfind(L"!");
				if (std::wstring::npos != r)
				{
					refs[1] = L"local-table!" + refs[1].substr(r + 1, refs[1].size() - r);
				}
				oox_ref += L":" + refs[1];
			}	
		}
	}
	else
	{
		//open office dont support defined names in chart formula 
		// 7501214.xlsx  - частичное заполнение local-table
		int col = -1, row = -1;
		utils::parsing_ref( refs[0], col, row);
		
		if (col < 0 && row < 0 && (odf_context_->type != SpreadsheetDocument))
		{
			local_table_enabled_	= true;
			//find defined name ????
			local_table_reset_ref_	= true;

			return L"";
		}
	}
	std::wstring odf_ref = formulas_converter_chart.convert_chart_distance(oox_ref);
	
	//XmlUtils::replace_all( odf_ref, L"$", L"");
	return odf_ref;
}

chart_chart* odf_chart_context::Impl::get_current_chart()
{
	for (long i = (long)current_level_.size() - 1; i >= 0; i--)
	{
		chart_chart * chart = dynamic_cast<chart_chart*>(current_level_[i].elm.get());
		if (chart) return chart;
	}
	return NULL;
}
odf_types::chart_class::type odf_chart_context::Impl::get_current_chart_class()
{
	for (long i = (long)current_level_.size() - 1; i >= 0; i--)
	{
		chart_chart * chart = dynamic_cast<chart_chart*>(current_level_[i].elm.get());
		if (chart)
		{
			return chart->chart_chart_attlist_.chart_class_.get_type();
		}
	}
	return odf_types::chart_class::none;
}
chart_series* odf_chart_context::Impl::get_current_series()
{
	for (long i = (long)current_level_.size() - 1; i >= 0; i--)
	{
		chart_series * chart = dynamic_cast<chart_series*>(current_level_[i].elm.get());
		if (chart) return chart;
	}
	return NULL;
}
chart_axis * odf_chart_context::Impl::get_current_axis()
{
	if (axis_.size() > 0) return dynamic_cast<chart_axis*>(axis_.back().elm.get());

	return NULL;
}
void odf_chart_context::Impl::clear_current()
{
	current_chart_state_.clear();
	categories_.clear();
	axis_.clear();
	group_series_.clear();
	data_cell_ranges_.clear();
	cash_.clear();
	
	bar_gap_width	= boost::none;
	bar_overlap		= boost::none;

	current_series_count_	= 0;
	local_table_reset_ref_	= false;
	
	if (odf_context_->type == SpreadsheetDocument)
	{
		local_table_enabled_ = false;
	}
	else
	{
		local_table_enabled_ = true;
	}
}
void odf_chart_context::Impl::set_default_series_color()
{
	if (!current_level_.back().graphic_properties_)return;

	color col = color(default_MS_series_colors[current_series_count_]);
	
	current_level_.back().graphic_properties_->common_draw_fill_attlist_.draw_fill_color_ = col;
	current_level_.back().graphic_properties_->svg_stroke_color_ = col;
}
////////////////////////////////////////////////////////////////////////////

odf_chart_context::odf_chart_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_chart_context::Impl(odf_context))
{
}

odf_chart_context::~odf_chart_context()
{
} 
void odf_chart_context::set_styles_context(odf_style_context * style_context)
{
	impl_->styles_context_ = style_context;
	
	impl_->odf_context_->drawing_context()->set_styles_context(style_context);
}

odf_drawing_context * odf_chart_context::drawing_context()
{
	return  impl_->odf_context_->drawing_context();
}
odf_text_context	* odf_chart_context::text_context()
{
	return impl_->odf_context_->text_context();
}
////////////////////////////////////////////////////////////////////////////////////////////////
void odf_chart_context::start_chart(office_element_ptr & root)
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"chart", chart_elm, impl_->odf_context_);

	chart_chart *chart = dynamic_cast<chart_chart*>(chart_elm.get());
	if (chart == NULL)return;

	root->add_child_element(chart_elm);
//////////	
	impl_->styles_context_->create_style(L"", style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	size_t level = impl_->current_level_.size();
	std::wstring style_name;
	
	odf_element_state		state(chart_elm, style_name, style_elm, level);
	odf_chart_level_state	level_state = {NULL, NULL, NULL, NULL, chart_elm};

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		level_state.chart_properties_ = style_->content_.get_style_chart_properties();
		
		chart->chart_chart_attlist_.common_attlist_.chart_style_name_ = style_name;
	}

	drawing_context()->start_element(chart_elm, style_elm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//if (impl_->current_level_.size()>0)	impl_->current_level_.back()->add_child_element(chart_elm); не надо ... наследование через drawing

	impl_->current_level_.push_back(level_state);


	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::set_chart_size(_CP_OPT(double) width_pt, _CP_OPT(double) height_pt)
{
	if (!width_pt && !height_pt) return;

	impl_->current_chart_state_.chart_height_pt = *height_pt;
	impl_->current_chart_state_.chart_width_pt = *width_pt;
	
	chart_chart *chart = impl_->get_current_chart();
	if (!chart)return;

	chart->chart_chart_attlist_.common_draw_size_attlist_.svg_width_ = length(length(*width_pt,length::pt).get_value_unit(length::cm), length::cm);
	chart->chart_chart_attlist_.common_draw_size_attlist_.svg_height_ = length(length(*height_pt,length::pt).get_value_unit(length::cm), length::cm);
}
void odf_chart_context::set_chart_type(odf_types::chart_class::type type)
{
	chart_chart *chart = impl_->get_current_chart();
	if (!chart)return;

	chart->chart_chart_attlist_.chart_class_ = type;
}

void odf_chart_context::set_chart_bar_type(int type)
{	
	if (!impl_->current_level_.back().chart_properties_) return;
	switch(type)
	{
		case 0:	//	st_shapeCONE = 0,
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = chart_solid_type(chart_solid_type::cone); break;
		case 1:	//	st_shapeCONETOMAX = 1,
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = chart_solid_type(chart_solid_type::cone); break;
		case 2:	//	st_shapeBOX = 2,
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = chart_solid_type(chart_solid_type::cuboid); break;
		case 3:	//	st_shapeCYLINDER = 3,
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = chart_solid_type(chart_solid_type::cylinder); break;
		case 4:	//	st_shapePYRAMID = 4,
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = chart_solid_type(chart_solid_type::pyramid); break;
		case 5:	//	st_shapePYRAMIDTOMAX = 5
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = chart_solid_type(chart_solid_type::pyramid); break;
	}
	if (type == -1)
	{
		//нужно вытащить свойство с уровня выше.
		size_t sz = impl_->current_level_.size();
		if (sz > 1)
			impl_->current_level_.back().chart_properties_->content_.solid_type_ = 
					impl_->current_level_[sz-2].chart_properties_->content_.solid_type_;
	}
}

void odf_chart_context::set_chart_bar_direction(int type)
{	
	if (!impl_->current_level_.back().chart_properties_) return;
	switch(type)
	{
		case 0:	//	st_bardirBAR = 0,
			impl_->current_level_.back().chart_properties_->content_.vertical_ = true; break;
		case 1:	//	st_bardirCOL = 1
			break;
	}
}
void odf_chart_context::set_chart_bar_gap_width(std::wstring val)
{
	size_t res = val.find(L"%");

	bool percent=false;
	if (std::wstring::npos != res)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
	
	impl_->bar_gap_width = (int)dVal;
}
void odf_chart_context::set_chart_bar_overlap(std::wstring val)
{
	size_t res = val.find(L"%");

	bool percent = false;
	if (std::wstring::npos != res)
	{
		val = val.substr(0,res);
		percent = true;
	}
	double dVal = boost::lexical_cast<double>(val);

	impl_->bar_overlap = (int)dVal;
}

void odf_chart_context::set_chart_stock_candle_stick(bool val)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	impl_->current_level_.back().chart_properties_->content_.japanese_candle_stick_ = val;

}

void odf_chart_context::set_chart_radar_type(int type)
{
	switch(type)
	{
		case 0:	//	st_radarstyleSTANDARD = 0,
			break;
		case 1:	//	st_radarstyleMARKER = 1,
			set_marker_type(12);//automatic
			break;
		case 2:	//	st_radarstyleFILLED = 2
			chart_chart *chart = impl_->get_current_chart();
			if (chart)
				chart->chart_chart_attlist_.chart_class_ = chart_class(chart_class::filled_radar);
			break;
	}

}
void odf_chart_context::set_chart_bar_grouping(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	switch(type)
	{
		case 0:	//	st_groupingPERCENTSTACKED = 0,
			impl_->current_level_.back().chart_properties_->content_.percentage_ = true; break;
		case 1:	//	st_bargroupingCLUSTERED = 1,
		case 2:	//	st_bargroupingSTANDARD = 2,
			break;
		case 3:	//	st_bargroupingSTACKED = 3
			impl_->current_level_.back().chart_properties_->content_.stacked_ = true; break;
	}
}
void odf_chart_context::set_chart_grouping(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	switch(type)
	{
		case 0:	//	st_groupingPERCENTSTACKED = 0,
			impl_->current_level_.back().chart_properties_->content_.percentage_ = true; break;
		case 1:	//	st_groupingSTANDARD = 1,
			break;
		case 2:	//	st_groupingSTACKED = 2
			impl_->current_level_.back().chart_properties_->content_.stacked_ = true; break;
	}
}
void odf_chart_context::set_chart_3D(bool val)
{
	if (!impl_->current_level_.back().chart_properties_) return;
	impl_->current_level_.back().chart_properties_->content_.three_dimensional_ = val;

	impl_->current_level_.back().chart_properties_->content_.treat_empty_cells_ = boost::none;
	//impl_->current_level_.back().chart_properties_->content_.series_source_ = chart_series_source(chart_series_source::rows);

	//impl_->current_level_.back().chart_properties_->content_.
	//chart:treat-empty-cells="leave-gap" 
	//chart:series-source="rows"
		  //impl_->current_level_.back().chart_properties_->content_.deep_ = true;
	//if (!plot_area)return;

	//plot_area->chart_plot_area_attlist_.dr3d_shade_mode=L"gouraud";
	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_level_.back().elm.get());

	if (val && plot_area)
	{
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.vpn_ = odf_types::vector3D(0, 0, 0);
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.vup_ = odf_types::vector3D(0, 0, 0); 
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.projection_ = L"parallel";
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.distance_ = odf_types::length(4.2, odf_types::length::cm);
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.focal_length_ = odf_types::length(8, odf_types::length::cm);
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.lighting_mode_ = L"false";

		//plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.projection_ = L"perspective";
		//plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.distance_ = odf_types::length(4.2, odf_types::length::cm);
		//plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.focal_length_ = odf_types::length(8, odf_types::length::cm);
		//plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.vpn_ = odf_types::vector3D(0.416199821709347,0.173649045905254,0.892537795986984);
		//plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.vup_ = odf_types::vector3D(-0.0733876362771618,0.984807599917971,-0.157379306090273); 

		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.vrp_ = odf_types::vector3D(17634.6218373783, 10271.4823817647, 24594.8639082739); 
	}
}
void odf_chart_context::set_view3D(int rotX, int rotY, int depthPercent, int perspective, int hPercent, bool angAx)
{
	odf_types::chart_class::type chart_class = impl_->get_current_chart_class();

	if (chart_class == chart_class::circle ||
		chart_class == chart_class::radar ||
		chart_class == chart_class::filled_radar ||
		chart_class == chart_class::ring)
	{
		rotX += 90;
	}
	if (rotX < 0) rotX += 360;
	if (rotY < 0) rotY += 360;

	int rotZ = 0;
  
	const double DEG2RAD = 3.1415926 / 180;
    double sx, sy, sz, cx, cy, cz, theta;

    // rotation angle about X-axis (pitch)
    theta = -rotX * DEG2RAD;
    sx = sin(theta);
    cx = cos(theta);

    // rotation angle about Y-axis (yaw)
    theta = rotY * DEG2RAD;
    sy = sin(theta);
    cy = cos(theta);

    // rotation angle about Z-axis (roll)
    theta = rotZ * DEG2RAD;
    sz = sin(theta);
    cz = cos(theta);

    // determine left axis
	double left_x = cy*cz;
    double left_y = sx*sy*cz + cx*sz;
    double left_z = -cx*sy*cz + sx*sz;

    // determine up axis
    double up_x = -cy*sz;
    double up_y = -sx*sy*sz + cx*cz;
    double up_z = cx*sy*sz + sx*cz;

    // determine forward axis
    double forward_x = sy;
    double forward_y = -sx*cy;
    double forward_z = cx*cy;

	std::wstringstream sTransform;

	sTransform << L"matrix(" << left_x << L" " << up_x << L" " << forward_x << L" " << left_y << L" " << up_y << L" " << forward_y << L" " << left_z << L" " << up_z << L" " << forward_z << L" 0cm 0cm 0cm)";

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_level_.back().elm.get());
	if (plot_area)
	{
		plot_area->chart_plot_area_attlist_.common_dr3d_attlist_.transform_ = sTransform.str();

		if (impl_->current_level_.back().chart_properties_)
		{
			impl_->current_level_.back().chart_properties_->content_.right_angled_axes_ = angAx;
		}
	}
}
void odf_chart_context::set_chart_colored(bool val)
{
	if (!impl_->current_level_.back().chart_properties_) return;
	impl_->current_level_.back().chart_properties_->content_.three_dimensional_ = val;
}
void odf_chart_context::set_marker_size(int size)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	impl_->current_level_.back().chart_properties_->content_.symbol_width_ = length(size,length::pt);
	impl_->current_level_.back().chart_properties_->content_.symbol_height_ = length(size,length::pt);
}
void odf_chart_context::set_marker_type(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::namedSymbol);

	switch(type)
	{
	case 0://st_markerstyleCIRCLE = 0,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::circleSymbol);	break;
	case 1://st_markerstyleDASH = 1,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::horizontal_barSymbol);	break;
	case 2://st_markerstyleDIAMOND = 2,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::diamondSymbol);	break;
	case 3://st_markerstyleDOT = 3,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::circleSymbol);	break;
	case 4://st_markerstyleNONE = 4,
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::noneSymbol);	break;
	case 5://st_markerstylePICTURE = 5,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::asteriskSymbol);	break;
	case 6://st_markerstylePLUS = 6,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::plusSymbol);	break;
	case 7://st_markerstyleSQUARE = 7,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::squareSymbol);	break;
	case 8://st_markerstyleSTAR = 8,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::starSymbol);	break;
	case 9://st_markerstyleTRIANGLE = 9,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::arrow_upSymbol);	break;
	case 10://st_markerstyleX = 10,
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::xSymbol);	break;
	case 11://st_markerstyleAUTO = 11
		impl_->current_level_.back().chart_properties_->content_.symbol_name_ = chart_symbol_name(chart_symbol_name::autoSymbol);	break;
	default:
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
	}

}
void odf_chart_context::set_chart_scatter_type(int type)
{
	if (!impl_->current_level_.back().chart_properties_) return;

	switch(type)
	{
	case 0://st_scatterstyleNONE
		break;
	case 1://st_scatterstyleLINE
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::noneSymbol);
		break;
	case 2://st_scatterstyleLINEMARKER
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
		break;
	case 3://st_scatterstyleMARKER
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
		break;	
	case 4://st_scatterstyleSMOOTH
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::noneSymbol);
		impl_->current_level_.back().chart_properties_->content_.interpolation_ = chart_interpolation(chart_interpolation::cubicSpline);
		break;	
	case 5://st_scatterstyleSMOOTHMARKER
		impl_->current_level_.back().chart_properties_->content_.interpolation_ = chart_interpolation(chart_interpolation::cubicSpline);
		impl_->current_level_.back().chart_properties_->content_.symbol_type_ = chart_symbol_type(chart_symbol_type::autoSymbol);
		break;	
	}
}
void odf_chart_context::start_group_series()
{
}
void odf_chart_context::start_series(odf_types::chart_class::type type)
{
	office_element_ptr elm;
	create_element(L"chart", L"series", elm, impl_->odf_context_);
	
	chart_series *series = dynamic_cast<chart_series*>(elm.get());
	if (series == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"", style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		series->chart_series_attlist_.common_attlist_.chart_style_name_ = style_name;
		series->chart_series_attlist_.chart_class_ = chart_class(type);

	}
	start_element(elm, style_elm, style_name);

	impl_->group_series_.push_back(elm);
//////////////////////////////////////////////////////////////
	//может хранить отдельно общий класс чарта??
	if (type == chart_class::radar || (impl_->get_current_chart_class() == chart_class::stock && type == chart_class::line))
	{
		series->chart_series_attlist_.chart_class_ = impl_->get_current_chart_class();
	}

	if (style_)
	{
		impl_->current_level_.back().graphic_properties_ = style_->content_.get_graphic_properties();
		impl_->set_default_series_color();
	}

	impl_->current_series_count_ ++;
}
void odf_chart_context::end_series()
{
	if ((false == impl_->categories_.empty()) && impl_->categories_.back().type == 2)
	{
		add_domain(impl_->categories_.back().ref);
	}
	end_element();
}
void odf_chart_context::set_label_delete(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content_.display_label_ = !val;
}
void odf_chart_context::set_label_show_bubble_size(bool val)
{
}
void odf_chart_context::set_label_show_cat_name(bool val)
{
}
void odf_chart_context::set_label_show_leader_line(bool val)
{
}
void odf_chart_context::set_label_show_legend_key(bool val)
{
}
void odf_chart_context::set_label_formula(const std::wstring & oox_ref) //в odf не поддерживается
{
	std::wstring odf_ref = impl_->convert_formula(oox_ref);
	
	if (!odf_ref.empty())
	{
	}
}

void odf_chart_context::set_label_show_percent(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	//impl_->current_level_.back().chart_properties_->content_.percentage_ = val;
}
void odf_chart_context::set_label_show_ser_name(bool val)
{
}
void odf_chart_context::set_label_show_values(bool val)
{
	if (!impl_->current_level_.back().chart_properties_ || !val)return;
	impl_->current_level_.back().chart_properties_->content_.data_label_number_=chart_data_label_number(chart_data_label_number::value);
}
void odf_chart_context::add_axis_group_series(unsigned int id)
{
	impl_->axis_group_series_.push_back(id);
}
void odf_chart_context::end_group_series()
{
	if (impl_->axis_.empty() && false == impl_->categories_.empty())
	{//без осей нихера не понимает MS Office !!! - причем оси для MS должны идти обязательно перед сериями
		start_axis();
			set_axis_dimension(1);
		end_element();
		start_axis();
			set_axis_dimension(2);
		end_element();
	}

	std::wstring axis_name;

	long countX = 0;
	long countY = 0;
	long countZ = 0;
	
	for (size_t j = 0; j < impl_->axis_.size(); j++)
	{
			 if (impl_->axis_[j].dimension == 1)	countX++;
		else if (impl_->axis_[j].dimension == 2)	countY++;
		else if (impl_->axis_[j].dimension == 3)	countZ++;
	}
	
	if (countX < 1 && countY > 1)
	{
		impl_->axis_[0].dimension = 1;
		chart_axis *axis = dynamic_cast<chart_axis*>(impl_->axis_[0].elm.get());
		axis->chart_axis_attlist_.chart_dimension_ = L"x";
		countY--;
	}
	//if (countZ > 0 && impl_->axis_group_series_.size() == 3 && (countY > 1 || countX > 1))
	//{
	//	impl_->axis_.back().dimension == 3;
	//	chart_axis *axis = dynamic_cast<chart_axis*>(impl_->axis_.back().elm.get());
	//	axis->chart_axis_attlist_.chart_dimension_ = L"z";
	//	countY--;
	//}

	for (size_t i = 0; i < impl_->axis_group_series_.size(); i++)
	{
		for (size_t j = 0; j < impl_->axis_.size(); j++)
		{
			if (impl_->axis_[j].oox_id == impl_->axis_group_series_[i] && impl_->axis_[j].dimension ==2)
			{
				axis_name = impl_->axis_[j].name;//привязка оси Y
				break;
			}
		}
		if (axis_name.length() > 0)break;
	}


	for (size_t i = 0; i < impl_->group_series_.size() && axis_name.length() > 0; i++)
	{
		chart_series *series= dynamic_cast<chart_series*>(impl_->group_series_[i].get());
		if (series)
		{
			series->chart_series_attlist_.chart_attached_axis_ = axis_name;
		}		
	}
	impl_->group_series_.clear();
	impl_->axis_group_series_.clear();
}

void odf_chart_context::add_domain(const std::wstring & odf_ref)
{
	size_t level = impl_->current_level_.size();
	if (level == 0)return;

	office_element_ptr elm;
	create_element(L"chart", L"domain", elm, impl_->odf_context_);
		
	chart_domain *domain = dynamic_cast<chart_domain*>(elm.get());
	if (domain == NULL)return;

	domain->table_cell_range_address_ = odf_ref;
	
	if (impl_->current_level_.back().elm) impl_->current_level_.back().elm->add_child_element(elm);

	odf_element_state state(elm, L"", office_element_ptr(), level);
	impl_->current_chart_state_.elements_.push_back(state);
}

void odf_chart_context::add_categories(const std::wstring & odf_ref, const std::wstring & format_code, office_element_ptr & axis_elm)
{
	size_t level = impl_->current_level_.size();

	bool bDataScale = false;

	if (std::wstring::npos != format_code.find(L"mm") &&
		std::wstring::npos != format_code.find(L"yy") &&
		std::wstring::npos != format_code.find(L"d"))
			bDataScale = true;
	
	if (bDataScale && axis_elm)
	{
		office_element_ptr date_elm;
		create_element(L"chartooo", L"date-scale", date_elm, impl_->odf_context_);
	
		axis_elm->add_child_element(date_elm);
		
		odf_element_state state(date_elm, L"", office_element_ptr(), level);
		impl_->current_chart_state_.elements_.push_back(state);

		chart_axis *axis = dynamic_cast<chart_axis*>(axis_elm.get());
		axis->chart_axis_attlist_.axis_type_ = L"auto";
	}
//--------------------------------------------------------------------------------------
	office_element_ptr elm;
	create_element(L"chart", L"categories", elm, impl_->odf_context_);
		
	chart_categories *categories = dynamic_cast<chart_categories*>(elm.get());
	if (categories== NULL)return;

	categories->table_cell_range_address_ = odf_ref;
		
	if (axis_elm) axis_elm->add_child_element(elm);

	odf_element_state state(elm, L"", office_element_ptr(), level);
	impl_->current_chart_state_.elements_.push_back(state);
}

void odf_chart_context::start_axis()
{
	office_element_ptr elm;
	create_element(L"chart", L"axis", elm, impl_->odf_context_);
	
	chart_axis *axis = dynamic_cast<chart_axis*>(elm.get());
	if (axis == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"", style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		axis->chart_axis_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);

	odf_axis_state axis_state={0, 0, L"", elm};
	impl_->axis_.push_back(axis_state);
/////////////////////defaults
	impl_->current_level_.back().chart_properties_->content_.reverse_direction_ = false;
	
	if (impl_->bar_overlap)
	{
		impl_->current_level_.back().chart_properties_->content_.overlap_ = impl_->bar_overlap.get();
	}
	if (impl_->bar_gap_width)
	{
		impl_->current_level_.back().chart_properties_->content_.gap_width_ = impl_->bar_gap_width.get();
	}
}
void odf_chart_context::start_grid(int type)
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"grid", chart_elm, impl_->odf_context_);
	
	chart_grid *grid = dynamic_cast<chart_grid*>(chart_elm.get());
	if (grid == NULL)return;
//////////	
	if (type == 1) grid->chart_grid_attlist_.chart_class_ = L"major";
	if (type == 2) grid->chart_grid_attlist_.chart_class_ = L"minor";

	impl_->styles_context_->create_style(L"", style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;

		grid->chart_grid_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
}

void odf_chart_context::start_title()
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"title", chart_elm, impl_->odf_context_);
	
	chart_title *title = dynamic_cast<chart_title*>(chart_elm.get());
	if (title == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"", style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		title->chart_title_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
}
void odf_chart_context::start_plot_area()
{
	office_element_ptr chart_elm;
	create_element(L"chart", L"plot-area", chart_elm, impl_->odf_context_);
	
	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(chart_elm.get());
	if (plot_area == NULL)return;

	plot_area->chart_plot_area_attlist_.chart_data_source_has_labels_ = L"both";
//////////	
	impl_->styles_context_->create_style(L"", style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		plot_area->chart_plot_area_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(chart_elm, style_elm, style_name);
	
	if (!impl_->current_level_.back().chart_properties_) return;
	
	impl_->current_level_.back().chart_properties_->content_.treat_empty_cells_ = true;
}
void odf_chart_context::end_plot_area()
{
	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_level_.back().elm.get());
	if (plot_area)
	{
		std::wstring cell_range;
		for (size_t i = 0; i < impl_->data_cell_ranges_.size(); i++)
		{
			cell_range = cell_range + impl_->data_cell_ranges_[i].ref + L" ";
		}
		//plot_area->chart_plot_area_attlist_.table_cell_range_address_ = cell_range; - точно для локальной диагр это лишнее!!!
	}
	end_element();
}
void odf_chart_context::start_text()
{
	impl_->odf_context_->start_text_context();
	impl_->odf_context_->text_context()->set_styles_context(impl_->styles_context_);

	style *style_ = dynamic_cast<style*>(impl_->current_chart_state_.elements_.back().style_elm.get());
	if (style_)
	{
		impl_->current_level_.back().paragraph_properties_	= style_->content_.get_style_paragraph_properties();
		impl_->current_level_.back().text_properties_		= style_->content_.get_style_text_properties();
	}
	
	impl_->odf_context_->text_context()->set_single_object(true, impl_->current_level_.back().paragraph_properties_, impl_->current_level_.back().text_properties_);
}
void odf_chart_context::end_text(bool only_properties)
{
	odf_text_context *text_context_ = text_context();
	
	if (text_context_ == NULL || impl_->current_level_.empty())return;

	for (size_t i = 0; false == only_properties && i < text_context_->text_elements_list_.size(); i++)
	{
		if (text_context_->text_elements_list_[i].level == 0)
		{
			impl_->current_level_.back().elm->add_child_element(text_context_->text_elements_list_[i].elm);
		}
		size_t level_root = impl_->current_level_.size() + 1;
		
		odf_element_state state(text_context_->text_elements_list_[i].elm, 
							 	text_context_->text_elements_list_[i].style_name, 
								text_context_->text_elements_list_[i].style_elm, 
								text_context_->text_elements_list_[i].level + level_root);

		impl_->current_chart_state_.elements_.push_back(state);
	}

	impl_->odf_context_->end_text_context();
}

void odf_chart_context::add_text(const std::wstring & val)
{
	office_element_ptr paragr_elm;
	create_element(L"text", L"p", paragr_elm, impl_->odf_context_);

	text_p* p = dynamic_cast<text_p*>(paragr_elm.get());
	if (p)
		p->add_text(val);

	impl_->current_level_.back().elm->add_child_element(paragr_elm);
	
	odf_element_state state(paragr_elm, L"", office_element_ptr(), impl_->current_level_.size() + 1);
	impl_->current_chart_state_.elements_.push_back(state);
}
void odf_chart_context::set_textarea_vertical_align(int align)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	//switch(align)
	//{
	//case 0://SimpleTypes::textanchoringtypeB: 
	//		impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom);	break;
	//case 1://SimpleTypes::textanchoringtypeCtr: 
	//		impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle);	break;
	//case 2://SimpleTypes::textanchoringtypeDist: 
	//		impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Baseline);break;
	//case 3://SimpleTypes::textanchoringtypeJust: 
	//		impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Justify);	break;
	//case 4://SimpleTypes::textanchoringtypeT: 
	//		impl_->current_graphic_properties->draw_textarea_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top);		break;
	//}
}
void odf_chart_context::set_textarea_rotation(double val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	if (val < 0.001 && val > -0.001) return;
	if (val < -360 || val > 360) return;

	if (val < 0) val += 360;
	val = 360 - val;

	impl_->current_level_.back().chart_properties_->content_.common_rotation_angle_attlist_.style_rotation_angle_ = (unsigned int)val;
}

void odf_chart_context::set_textarea_padding(_CP_OPT(double) & left, _CP_OPT(double) & top, _CP_OPT(double) & right, _CP_OPT(double) & bottom)//in pt
{
	if (!impl_->current_level_.back().chart_properties_)return;

	//if (left)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_left_		= length(*left,	length::pt);
	//if (top)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_top_		= length(*top,	length::pt);
	//if (right)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_right_	= length(*right,length::pt);
	//if (bottom)	impl_->current_graphic_properties->common_padding_attlist_.fo_padding_bottom_	= length(*bottom,length::pt);
}

void odf_chart_context::start_floor()
{
	office_element_ptr elm;
	create_element(L"chart", L"floor", elm, impl_->odf_context_);
	
	chart_floor *floor = dynamic_cast<chart_floor*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && floor)
	{
		style_name = style_->style_name_;
		floor->common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);

	if (style_)
	{
		impl_->current_level_.back().graphic_properties_ = style_->content_.get_graphic_properties();
	}
}
void odf_chart_context::start_wall()
{
	office_element_ptr elm;
	create_element(L"chart", L"wall", elm, impl_->odf_context_);
	
	chart_wall *wall = dynamic_cast<chart_wall*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && wall)
	{
		style_name = style_->style_name_;
		wall->chart_wall_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);

	if (style_)
	{
		impl_->current_level_.back().graphic_properties_ = style_->content_.get_graphic_properties();
		if (impl_->current_level_.back().graphic_properties_)
			impl_->current_level_.back().graphic_properties_->common_draw_fill_attlist_.draw_fill_color_ = color(L"#ffffff");
	}
}
void odf_chart_context::start_legend()
{
	office_element_ptr elm;
	create_element(L"chart", L"legend", elm, impl_->odf_context_);
	
	chart_legend *legend = dynamic_cast<chart_legend*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && legend)
	{
		style_name = style_->style_name_;
		legend->chart_legend_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::start_stock_range_line()
{
	office_element_ptr elm;
	create_element(L"chart", L"stock-range-line", elm, impl_->odf_context_);
	
	chart_stock_range_line *line = dynamic_cast<chart_stock_range_line*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && line)
	{
		style_name = style_->style_name_;
		line->common_attlist_.chart_style_name_ = style_name;
		
		impl_->current_level_.back().graphic_properties_ = style_->content_.get_graphic_properties();
		impl_->set_default_series_color();
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::start_stock_gain_marker()
{
	office_element_ptr elm;
	create_element(L"chart", L"stock-gain-marker", elm, impl_->odf_context_);
	
	chart_stock_gain_marker *marker = dynamic_cast<chart_stock_gain_marker*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && marker)
	{
		style_name = style_->style_name_;
		marker->common_attlist_.chart_style_name_ = style_name;
		
		impl_->current_level_.back().graphic_properties_ = style_->content_.get_graphic_properties();
		impl_->set_default_series_color();
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::start_stock_loss_marker()
{
	office_element_ptr elm;
	create_element(L"chart", L"stock-loss-marker", elm, impl_->odf_context_);
	
	chart_stock_loss_marker *marker = dynamic_cast<chart_stock_loss_marker*>(elm.get());
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_ && marker)
	{
		style_name = style_->style_name_;
		marker->common_attlist_.chart_style_name_ = style_name;
	
		impl_->current_level_.back().graphic_properties_ = style_->content_.get_graphic_properties();
		impl_->set_default_series_color();	
	}
	start_element(elm, style_elm, style_name);
}
void odf_chart_context::set_stock_gain_marker_width(std::wstring val)
{
	size_t res = val.find(L"%");

	bool percent=false;
	if (std::wstring::npos != res)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
}
void odf_chart_context::set_stock_loss_marker_width(std::wstring val)
{
	size_t res = val.find(L"%");

	bool percent=false;
	if (std::wstring::npos != res)
	{
		val = val.substr(0,res);
		percent=true;
	}
	double dVal = boost::lexical_cast<double>(val);
}
long odf_chart_context::get_count_data_points_series()
{
	return impl_->current_data_points_series_count_;
}
void odf_chart_context::start_data_point_series(int count)
{
	office_element_ptr elm;
	create_element(L"chart", L"data-point", elm, impl_->odf_context_);
	
	chart_data_point *data_point = dynamic_cast<chart_data_point*>(elm.get());
	if (data_point == NULL)return;

	if (count < 1) count = get_count_data_points_series();
	data_point->chart_data_point_attlist_.chart_repeated_ = count;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Chart, true, false, -1);		
	
	office_element_ptr & style_elm = impl_->styles_context_->last_state()->get_office_element();
	
	std::wstring style_name;
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
		data_point->chart_data_point_attlist_.common_attlist_.chart_style_name_ = style_name;
	}
	start_element(elm, style_elm, style_name);
//defaults
	chart_series * series = impl_->get_current_series();
}
void odf_chart_context::set_legend_position(int val)
{
	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend == NULL)return;
	switch(val)
	{
		case 0: legend->chart_legend_attlist_.chart_legend_position_ = L"bottom";	break;//st_legendposB
		case 1: legend->chart_legend_attlist_.chart_legend_position_ = L"top-end";	break;//st_legendposTR 
		case 2: legend->chart_legend_attlist_.chart_legend_position_ = L"start";	break;//st_legendposL 
		case 3: legend->chart_legend_attlist_.chart_legend_position_ = L"end";		break;//st_legendposR
		case 4:	legend->chart_legend_attlist_.chart_legend_position_ = L"top";		break;//st_legendposT
	}
}
void odf_chart_context::set_layout_x(double *val,int mode)//edge, factor
{
	if (!val)return;
	if (mode == 0) *val = impl_->current_chart_state_.chart_width_pt * (*val);

	length x_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.common_draw_position_attlist_.svg_x_ = x_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_position_attlist_.svg_x_ = x_cm;
	
	chart_title *title = dynamic_cast<chart_title*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (title)title->chart_title_attlist_.common_draw_position_attlist_.svg_x_ = x_cm;
}
void odf_chart_context::set_display_label(bool Val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content_.display_label_ =  Val;
}
void odf_chart_context::set_display_label_position(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	//impl_->current_level_.back().chart_properties_->content_.chart:label-arrangement_ =  Val;
}
void odf_chart_context::set_axis_orientation(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	if (type == 0) impl_->current_level_.back().chart_properties_->content_.reverse_direction_ =  true;
}
void odf_chart_context::set_axis_max(double val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content_.maximum_ =  val;
}
void odf_chart_context::set_axis_min(double val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content_.minimum_ =  val;
}
void odf_chart_context::set_axis_tick_minor(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;

	switch (type)
	{
		case 0: break;//		st_tickmarkCROSS = 0,		
		case 1: //		st_tickmarkIN = 1,
		impl_->current_level_.back().chart_properties_->content_.tick_marks_minor_inner_ = true;	
		impl_->current_level_.back().chart_properties_->content_.tick_marks_minor_outer_ = false; break;
		case 2: //		st_tickmarkNONE = 2,
		impl_->current_level_.back().chart_properties_->content_.tick_marks_minor_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content_.tick_marks_minor_outer_ = false; break;
		case 3: //		st_tickmarkOUT = 3
		impl_->current_level_.back().chart_properties_->content_.tick_marks_minor_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content_.tick_marks_minor_outer_ = true; break;
	}

}
void odf_chart_context::set_no_fill(bool Val)
{
	if (Val == false)return;
	if (!impl_->current_level_.back().graphic_properties_)return;

	impl_->current_level_.back().graphic_properties_->common_draw_fill_attlist_.draw_fill_ = draw_fill(draw_fill::none);

}
void odf_chart_context::set_axis_tick_major(int type)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	switch (type)
	{
		case 0: break;//		st_tickmarkCROSS = 0,		
		case 1: //		st_tickmarkIN = 1,
		impl_->current_level_.back().chart_properties_->content_.tick_marks_major_inner_ = true;	
		impl_->current_level_.back().chart_properties_->content_.tick_marks_major_outer_ = false; break;
		case 2: //		st_tickmarkNONE = 2,
		impl_->current_level_.back().chart_properties_->content_.tick_marks_major_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content_.tick_marks_major_outer_ = false; break;
		case 3: //		st_tickmarkOUT = 3
		impl_->current_level_.back().chart_properties_->content_.tick_marks_major_inner_ = false;	
		impl_->current_level_.back().chart_properties_->content_.tick_marks_major_outer_ = true; break;
	}
}
void odf_chart_context::set_axis_logarithmic(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content_.logarithmic_ =  val;
}
void odf_chart_context::set_axis_id(unsigned int id)
{
	if (impl_->axis_.size()>0)impl_->axis_.back().oox_id = id;
}
void odf_chart_context::set_axis_visible(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	impl_->current_level_.back().chart_properties_->content_.visible_ =  val;
}
void odf_chart_context::set_axis_dimension(int type)
{
	chart_axis *axis = impl_->get_current_axis();
	if (!axis)return;

	std::wstring val;
	if (type == 1) val = L"x";
	if (type == 2) val = L"y";
	if (type == 3) val = L"z";
	
	axis->chart_axis_attlist_.chart_dimension_ = val;
	//axis->chart_axis_attlist_.chart_name_ = std::wstring(L"axis-") + boost::lexical_cast<std::wstring>(impl_->axis_.size()+1); 

	if (impl_->axis_.size()>0)
	{
		impl_->axis_.back().dimension = type;
		//impl_->axis_.back().name = *axis->chart_axis_attlist_.chart_name_;
	}
}
void odf_chart_context::set_axis_position(int type)
{	
	if (!impl_->current_level_.back().chart_properties_)return;
			//st_axposB = 0,
			//st_axposL = 1,
			//st_axposR = 2,
			//st_axposT = 3
	//if (type ==0 || type ==2)
	//	impl_->current_level_.back().chart_properties_->content_.axis_position_ = L"end";
	//if (type ==1 || type ==3)
	//	impl_->current_level_.back().chart_properties_->content_.axis_position_ = L"start";
//a value of type double – the axis line is placed at the given value on the crossing axis. 
//If the crossing axis is an axis displaying categories rather than values, a value of 1 indicates that the axis should be placed at the first category, a value of 2 indicates that the axis should be placed at the second category and so forth.
}
void odf_chart_context::set_axis_label_position(int type)
{	
	if (!impl_->current_level_.back().chart_properties_)return;
			//st_ticklblposHIGH = 0,
			//st_ticklblposLOW = 1,
			//st_ticklblposNEXTTO = 2,
			//st_ticklblposNONE = 3
	if (type ==1)impl_->current_level_.back().chart_properties_->content_.axis_label_position_ = L"outside-start";
	if (type ==0)impl_->current_level_.back().chart_properties_->content_.axis_label_position_ = L"outside-end";
	if (type ==2)impl_->current_level_.back().chart_properties_->content_.axis_label_position_ = L"near-axis";

	//near-axis-other-side
	//near-axis //default

}
void odf_chart_context::set_layout_y(double *val,int mode)
{
	if (!val)return;
	if (mode == 0)*val = impl_->current_chart_state_.chart_height_pt * (*val);

	length y_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.common_draw_position_attlist_.svg_y_ = y_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_position_attlist_.svg_y_ = y_cm;
	
	chart_title *title = dynamic_cast<chart_title*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (title)title->chart_title_attlist_.common_draw_position_attlist_.svg_y_ = y_cm;
}
void odf_chart_context::set_layout_w(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) *val = impl_->current_chart_state_.chart_width_pt * (*val);
	
	length width_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.chartooo_width_ = width_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_size_attlist_.svg_width_ = width_cm;
}
void odf_chart_context::set_layout_h(double *val,int mode)
{
	if (!val)return;
	if (mode == 0) *val = impl_->current_chart_state_.chart_height_pt * (*val);

	length height_cm = length(length(*val,length::pt).get_value_unit(length::cm),length::cm);

	chart_legend *legend = dynamic_cast<chart_legend*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (legend)legend->chart_legend_attlist_.chartooo_height_ = height_cm;

	chart_plot_area *plot_area = dynamic_cast<chart_plot_area *>(impl_->current_chart_state_.elements_.back().elm.get());
	if (plot_area)plot_area->chart_plot_area_attlist_.common_draw_size_attlist_.svg_height_ = height_cm;
}

void odf_chart_context::start_element(office_element_ptr & elm, office_element_ptr & style_elm, const std::wstring & style_name)
{
	size_t level = impl_->current_level_.size();
	
	drawing_context()->start_element(elm, style_elm);
	//if (impl_->current_level_.size()>0) impl_->current_level_.back()->add_child_element(elm); не надо...наследование через start_element в drawing
	
	odf_element_state		state={elm, style_name, style_elm, level, 0};
	odf_chart_level_state	level_state = {NULL, NULL, NULL, NULL, elm};
	
	impl_->current_chart_state_.elements_.push_back(state);
	
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		level_state.chart_properties_ = style_->content_.get_style_chart_properties();
	}
	impl_->current_level_.push_back(level_state);//стоит ли сюда перенести и current_chart_properties ????

}

void odf_chart_context::end_element()
{
	//допричесываение элемента
	//if (impl_->current_level_.size()>0)
	{
		if (impl_->current_level_.back().paragraph_properties_)
		{
			if (impl_->current_level_.back().paragraph_properties_->content_.style_writing_mode_)
			{
				switch(impl_->current_level_.back().paragraph_properties_->content_.style_writing_mode_->get_type())
				{
					case writing_mode::LrTb: 
					case writing_mode::RlTb:
					case writing_mode::Lr:
						impl_->current_level_.back().chart_properties_->content_.style_direction_ = direction(direction::Ltr); break;
					case writing_mode::TbRl:
					case writing_mode::TbLr:
					case writing_mode::Tb:
						impl_->current_level_.back().chart_properties_->content_.style_direction_ = direction(direction::Ttb); break;
				}
				impl_->current_level_.back().chart_properties_->content_.common_rotation_angle_attlist_.style_rotation_angle_ =0;
			}
		}
	}
	impl_->current_level_.pop_back();
	drawing_context()->end_element();
}

void odf_chart_context::end_chart()
{
	if (impl_->current_chart_state_.elements_.size() < 1) return;

	impl_->create_local_table();
	
	end_element();
///////////////////


	size_t cat = 0;
	for (size_t i = 0; i < impl_->axis_.size() && false == impl_->categories_.empty(); i++)
	{
		if (impl_->axis_[i].elm == NULL) continue;
		
		if (impl_->axis_[i].dimension == 1 )
		{
			if (cat < impl_->categories_.size())
			{
				if (impl_->categories_[cat].type == 1) 
				{
					add_categories(impl_->categories_[cat].ref, impl_->categories_[cat].format_code, impl_->axis_[i].elm);
				}
				else
				{
					if (i == 0) 
					{
						chart_axis *axis = dynamic_cast<chart_axis*>(impl_->axis_[i].elm.get());
						axis->chart_axis_attlist_.chart_dimension_ = L"x";
					}
				}
				cat++;
			}
		}
	}
///////////////
	impl_->chart_list_.push_back(impl_->current_chart_state_);

	impl_->clear_current();
}

void odf_chart_context::set_series_value_formula(const std::wstring & oox_ref, const std::wstring & format_code)
{
	std::wstring odf_ref = impl_->convert_formula(oox_ref);

	chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (series == NULL)return;

	Impl::_range r (odf_ref, format_code, false, series);
	impl_->data_cell_ranges_.push_back(r);

	if (false == odf_ref.empty())
	{
		series->chart_series_attlist_.chart_values_cell_range_address_ = odf_ref;
		impl_->current_data_points_series_count_ = formulas_converter_chart.get_count_value_points(oox_ref);
	}
}

void odf_chart_context::set_series_label_formula(const std::wstring & oox_ref)
{
	std::wstring odf_ref = impl_->convert_formula(oox_ref);

	chart_series *series = dynamic_cast<chart_series*>(impl_->current_chart_state_.elements_.back().elm.get());
	if (series == NULL)return;	

	Impl::_range r (odf_ref, L"", true, series);
	impl_->data_cell_ranges_.push_back(r);
	
	if (!odf_ref.empty())
	{
		series->chart_series_attlist_.chart_label_cell_address_ = odf_ref;
	}
}

void odf_chart_context::set_category_axis_formula(const std::wstring & oox_ref, const std::wstring & format_code, int type)
{
	std::wstring odf_ref = impl_->convert_formula(oox_ref);

	Impl::_range r (odf_ref, format_code, true, NULL);
	impl_->data_cell_ranges_.push_back(r);
	
	odf_category_state category_state = {odf_ref, format_code, type};
	impl_->categories_.push_back(category_state);
}

void odf_chart_context::set_series_pie_explosion(int val)//или точка серии
{
	if (!impl_->current_level_.back().chart_properties_)return;

	impl_->current_level_.back().chart_properties_->content_.pie_offset_ = val;	
}
void odf_chart_context::set_series_pie_bubble(bool val)
{
	if (!impl_->current_level_.back().chart_properties_)return;
	
	impl_->current_level_.back().chart_properties_->content_.pie_bubble_ = val;	
	
}
//void odf_chart_context::set_cash(std::wstring format_code, std::vector<double> &data_double)
//{
//	if (data_double.size() <1 || impl_->data_cell_ranges_.size() < 1) return;
//
//	std::wstring ref = impl_->data_cell_ranges_.back();
//	std::vector<std::wstring> data_str;
//	
//	odf_cash_state state = {ref, format_code,/*data_double,*/data_str};
//	impl_->cash_.push_back(state);
//}

void odf_chart_context::set_cash(std::wstring format_code, std::vector<std::wstring> & data_str, bool categories, bool label)
{
	if (impl_->data_cell_ranges_.empty())	return;
	if (data_str.empty())					return;

	std::wstring ref = impl_->data_cell_ranges_.back().ref;
	
	int count_cash_values	= 0;
	bool by_row				= true;	
	int start_col			= 0;
	int start_row			= 0;

	if(ref.empty() && label && categories)
	{
		for (size_t i = 0 ; i < impl_->cash_.size(); i++)
		{
			if (impl_->cash_[i].label || impl_->cash_[i].categories)
			{
				ref			= impl_->cash_[i].ref;
				data_str	= impl_->cash_[i].data_str;
				format_code	= impl_->cash_[i].format_code;
				break;
			}
		}
	}
	if (ref.empty() && impl_->data_cell_ranges_.size() > 1)
	{//direction, count cash points
		int index_cash_y = impl_->data_cell_ranges_[impl_->data_cell_ranges_.size() - 2].index_cash;

		std::wstring ref_y = impl_->data_cell_ranges_[impl_->data_cell_ranges_.size() - 2].ref;
		
		std::vector<std::wstring> refs;
		boost::algorithm::split(refs, ref_y, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

		int col1 = -1, col2 = -1, row1 = -1, row2 = -1;

		if (refs.size() < 1) return;	
		if (refs[0].empty()) return;

		utils::parsing_ref( refs[0], col1, row1);
		
		if (refs.size() > 1) 
		{
			utils::parsing_ref( refs[1], col2, row2);
		}
		else
		{
			col2 = col1; row2 = row1;
		}
		int count_cols = col2 - col1;
		int count_rows = row2 - row1;
	
		if (count_cols == 0)
		{
			start_col			= col2;
			start_row			= row1;
			count_cash_values	= count_rows + 1;
		}
		else
		{
			start_col			= col1;
			start_row			= row2;

			by_row = false;
			count_cash_values = count_cols + 1;
		}
		if (data_str.empty())
		{
			for (int i = 0 ; i < count_cash_values; i++)
				data_str.push_back(L"non");
		}
		std::wstring ref1, ref2;

		ref1 = std::wstring(L"local-table.") + utils::getColAddress(start_col) + boost::lexical_cast<std::wstring>(start_row);
		//
		if (by_row)
			ref2 = std::wstring(L"local-table.") + utils::getColAddress(start_col) + std::to_wstring(start_row + count_cash_values);
		else
			ref2 = std::wstring(L"local-table.") + utils::getColAddress(start_col + count_cash_values) + std::to_wstring(start_row);
	
		ref = ref1 + L":" + ref2;
	}
	if(ref.empty() && !data_str.empty())
	{
		// банальнейшая генерация А1 ... Аххх

		ref = std::wstring(L"local-table.A1:") + std::wstring(L".A") + boost::lexical_cast<std::wstring>(data_str.size());
	}

	if (!ref.empty() && !data_str.empty())
	{
		odf_cash_state state = {ref, format_code, categories, label, data_str};	
		impl_->cash_.push_back(state);
		int cash_ind = (int)impl_->cash_.size() - 1;

		impl_->data_cell_ranges_.back().index_cash = cash_ind;	

		if (impl_->data_cell_ranges_.back().ref.empty())
			impl_->data_cell_ranges_.back().ref = ref;

		if (!categories && !label && (impl_->data_cell_ranges_.back().series) && 
			(!impl_->data_cell_ranges_.back().series->chart_series_attlist_.chart_values_cell_range_address_))
		{
			impl_->data_cell_ranges_.back().series->chart_series_attlist_.chart_values_cell_range_address_ = ref;
		}
		else if (categories && !impl_->categories_.empty())
		{
			if (impl_->categories_.back().ref.empty())
			{
				impl_->categories_.back().ref = ref;
			}
			if (impl_->categories_.back().format_code.empty())
			{
				impl_->categories_.back().format_code = format_code;
			}
		}
	}
}

void odf_chart_context::set_local_table (bool Val, bool use_cash_only)
{
	impl_->local_table_enabled_		= Val;
	impl_->local_table_reset_ref_	= use_cash_only;
}

struct _sort_cells
{
	bool operator() (_cell_cash i, _cell_cash j)
	{ 
		if (i.row == j.row)
			return (i.col < j.col);
		else
			return (i.row < j.row);
	}
} sort_cells;

int odf_chart_context::Impl::create_local_table_rows(int curr_row, ods_table_state * table_state, std::vector<_cell_cash> & cells, bool header )
{
	int curr_cell = 0;

    office_element_ptr style_null;

	bool add = false;

    for (size_t i = 0; i < cells.size(); i++)
    {
		if (cells[i].cash_only)
			continue;

		add = false;

		if (cells[i].row  > curr_row + 1/* && !header*/)
		{	
			office_element_ptr row_elm;

            create_element(L"table", L"table-row", row_elm, odf_context_);
            table_state->add_row(row_elm, cells[i].row - curr_row - 1, style_null);
			table_state->set_row_hidden(true);
			
			create_element(L"table", L"table-row", row_elm, odf_context_);
            table_state->add_row(row_elm, 1, style_null);			
			
			curr_row =  cells[i].row - 1;
			add = true;
		}
		while (cells[i].row >= curr_row + 1)
		{
			office_element_ptr row_elm;

			create_element(L"table", L"table-row", row_elm, odf_context_);
			table_state->add_row(row_elm, 1, style_null);
			curr_row++;
			curr_cell = 0;
		}

		if (curr_cell + 1 < cells[i].col)
			table_state->add_default_cell(cells[i].col - curr_cell-1);
		
		office_element_ptr cell_elm;
		create_element(L"table", L"table-cell",cell_elm, odf_context_);
		
        table_state->start_cell(cell_elm, style_null);
			table_state->set_cell_value(cells[i].val, true);
			//add type ???
		table_state->end_cell();

		curr_cell = cells[i].col;
	}
	return curr_row;
}

void odf_chart_context::Impl::create_local_table()
{
	if (local_table_enabled_ == false) return;

	std::vector<_cell_cash> cells_cash;
	std::vector<_cell_cash> cells_cash_label;

	std::wstring	table_name	= L"local-table";
	int				max_columns	= 0;

	bool			col_header	= false;
	bool			row_header	= false;

	int min_col = 0xffff;
	int min_row = 0xffff;
	
	//выкинем дублирующие ref
	for (size_t i = 0; i < cash_.size(); i++)
	{
		for (size_t j = i + 1; j < cash_.size(); j++)
		{
			if (cash_[j].ref == cash_[i].ref && cash_[j].ref.length() > 1)
			{
				cash_.erase(cash_.begin() + j);
			}
		}
	}

	for (size_t i = 0; i < cash_.size(); i++)
	{
		std::vector<std::wstring> refs;
		boost::algorithm::split(refs,cash_[i].ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

        int col1 = -1, col2 = -1, row1 = -1, row2 = -1;

		if (refs.size() < 1) continue;
		
		size_t r = refs[0].rfind(L".");//в имени таблички может быть точка
		if (std::wstring::npos != r)
		{
			table_name = refs[0].substr (0, r);
			refs[0] = refs[0].substr(r + 1, refs[0].size() - r);
		}
		
		utils::parsing_ref( refs[0], col1, row1);

		if (col1 < min_col)	min_col = col1;
		if (row1 < min_row)	min_row = row1;

		if (refs.size() > 1) 
		{
			r = refs[1].rfind(L".");
			if (std::wstring::npos != r)
				refs[1] = refs[1].substr(r + 1, refs[1].size() - r);
			utils::parsing_ref( refs[1], col2, row2);

			
			if (col2 < min_col)	min_col = col2;
			if (row2 < min_row)	min_row = row2;
		}
		else
		{
			col2 = col1; row2 = row1;
		}
			
		if (row2 < 1 || row1 < 1 || col1 < 1 || col2 < 1)
			continue;

		if (cash_[i].categories || cash_[i].label) 
		{
			if (col2 - col1 == 0 && cash_[i].label)
				col_header	= true;

			if (row2 - row1 == 0)
				row_header	= true;
		}

		for (size_t j = 0; j < cash_[i].data_str.size(); j++)
		{
			_cell_cash c = {0, 0, false, false, L""};

			if (col1 >= 0 && row1 >= 0)
			{
				c.col	= (col2 == col1) ? col1 : col1 + j;
				c.row	= (row2 == row1) ? row1 : row1 + j;
				
				c.val	= cash_[i].data_str[j];
				c.label = false;
			}
			else
			{
				c.val		= cash_[i].data_str[j];
				c.cash_only = true;
			}

			if ((cash_[i].categories || cash_[i].label)  && row_header)
			{
				cells_cash_label.push_back(c);
			}
			else
			{
				cells_cash.push_back(c);
			}

			if (c.col > max_columns && c.col < 10000) max_columns = c.col;
		}
	}
	std::sort(cells_cash.begin()		, cells_cash.end()			,sort_cells);
	std::sort(cells_cash_label.begin()	, cells_cash_label.end()	,sort_cells);


/////////////////////////
	//create tables

	office_element_ptr table_elm;
	create_element(L"table", L"table",table_elm, odf_context_);
	ods_table_state * table_state = new ods_table_state(odf_context_, table_elm);

	if (table_state)
	{
		current_level_[0].elm->add_child_element(table_elm);
		size_t level = current_level_.size();
		
		odf_element_state state(table_elm, L"", office_element_ptr(), level + 1);		
		current_chart_state_.elements_.push_back(state);

		table_state->set_table_name(table_name);

		/////////////////////////////////////////////////
		office_element_ptr col_elm;
		create_element(L"table", L"table-column",col_elm, odf_context_);

		if (col_header)
		{
			office_element_ptr cols_header_elm;
			create_element(L"table", L"table-header-columns",cols_header_elm, odf_context_);
			table_elm->add_child_element(cols_header_elm);	
				cols_header_elm->add_child_element(col_elm);
		}

		office_element_ptr cols_elm;
		create_element(L"table", L"table-columns", cols_elm, odf_context_);
		table_elm->add_child_element(cols_elm);


		for (int i=0; i < max_columns - (col_header ? 1 : 0); i++)
			cols_elm->add_child_element(col_elm);

		office_element_ptr row_headers_elm;
		office_element_ptr row_elm;
        office_element_ptr style_null;

		int current_row = 0;
		
		if (cells_cash_label.size() > 0 || cells_cash.size() > 0)
		{
			int min_row = 1, r1 = 1, r2 = 1;
			int min_col = 1, c1 = 1, c2 = 1;

			if (cells_cash_label.size() > 0) 
			{
				r1 = cells_cash_label[0].row;
				c1 = cells_cash_label[0].col;
			}
			if (cells_cash.size() > 0) 
			{
				r2 = cells_cash[0].row;
				c2 = cells_cash[0].col;
			}
			if ((std::min)(r1, r2) > min_row)	min_row = (std::min)(r1, r2);
			if ((std::min)(c1, c2) > min_col)	min_col = (std::min)(c1, c2);

			for (size_t i = 0 ; i < cells_cash_label.size(); i++)
			{
				cells_cash_label[i].row -= min_row - 1;
				cells_cash_label[i].col -= min_col - 1;
			}
			for (size_t i = 0 ; i < cells_cash.size(); i++)
			{
				cells_cash[i].row -= min_row - 1;
				cells_cash[i].col -= min_col - 1;
			}
		}
		
		if (cells_cash_label.size() > 0 || cells_cash.size() > 0)
		{
			if (cells_cash_label.size() > 0 && row_header)
			{
				create_element(L"table", L"table-header-rows",row_headers_elm, odf_context_);
				
				table_state->start_headers(row_headers_elm);
					current_row = create_local_table_rows(current_row, table_state, cells_cash_label, true);
				table_state->end_headers();
			}
			if (cells_cash.size() > 0) 
			{
				current_row = create_local_table_rows(current_row, table_state, cells_cash, false);
			}
			else 
			{
				create_element(L"table", L"table-rows", row_elm, odf_context_);
                table_state->add_row(row_elm, 1, style_null);
			}
		}
		else
		{
			create_element(L"table", L"table-header-rows", row_headers_elm, odf_context_);
			
			table_state->start_headers(row_headers_elm);
			{
				create_element(L"table", L"table-row", row_elm, odf_context_);
                table_state->add_row(row_elm, 1, style_null);
				{
					office_element_ptr cell_elm;
					create_element(L"table", L"table-cell",cell_elm, odf_context_);
					
                    table_state->start_cell(cell_elm, style_null);
					table_state->end_cell();
				}
			}
			table_state->end_headers();

			create_element(L"table", L"table-rows", row_elm, odf_context_);
            table_state->add_row(row_elm, 1, style_null);
		}
	}

	delete table_state;

}

}
}
