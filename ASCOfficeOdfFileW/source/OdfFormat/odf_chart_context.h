#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements_create.h"

namespace cpdoccore {
namespace odf
{

class odf_conversion_context;
class odf_style_context;
class odf_text_context;
class odf_drawing_context;

class odf_chart_context
{
public:
	odf_chart_context(odf_conversion_context *odf_context);
    ~odf_chart_context();

	void set_styles_context(odf_style_context * style_context);

	odf_drawing_context *drawing_context();
	odf_text_context	*text_context();

	void start_chart(office_element_ptr & root);
		void set_chart_type(std::wstring type);
		void set_chart_3D(bool Val);
		void set_chart_size(double width_pt, double height_pt);		
		void set_chart_colored(bool val);
		void set_chart_grouping(int type);
		void set_chart_scatter_type(int type);
		void set_chart_bar_grouping(int type);
		void set_chart_bar_direction(int type);
		void set_chart_bar_type(int type);
		void set_chart_bar_gap_width(std::wstring val);
		void set_chart_bar_overlap(std::wstring val);
		void set_chart_radar_type(int type);
		void set_chart_stock_candle_stick(bool val);

		void set_marker_size(int size);
		void set_marker_type(int type);
		
		void start_plot_area();
		void end_plot_area();

		void start_group_series();
			void add_axis_group_series(unsigned int id);
				void start_series(std::wstring type);
					void set_series_value_formula(std::wstring oox_formula);
					void set_series_label_formula(std::wstring oox_formula);
					void set_category_axis_formula(std::wstring oox_formula,int type);
					void start_data_point_series(int count);
					long get_count_data_points_series();

					void set_series_pie_explosion(int val);
				void end_series();
		void end_group_series();
		
		void set_label_name(std::wstring name);
		void set_label_delete(bool val);	
		void set_label_show_bubble_size(bool val);	
		void set_label_show_cat_name(bool val);
		void set_label_show_leader_line(bool val);
		void set_label_show_legend_key(bool val);
		void set_label_show_percent(bool val);
		void set_label_show_ser_name(bool val);
		void set_label_show_values(bool val);		
		
		void start_axis();
			void set_axis_id(unsigned int id);
			void set_axis_dimension(int type);
			void set_axis_orientation(int type);
			void set_axis_logarithmic(bool val);
			void set_display_label(bool Val); 
			void set_display_label_position(int type);
			void set_axis_position(int type);
			void set_axis_label_position(int type);
			void set_axis_max(double val);
			void set_axis_min(double val);
			void set_axis_tick_minor(int type);
			void set_axis_tick_major(int type);
		void start_title();
		void start_grid(int type);
		void start_legend();
		void start_floor();
		void start_wall();
			void set_legend_position(int val);

		void set_no_fill(bool val);
		
		void start_stock_gain_marker();
			void set_stock_gain_marker_width(std::wstring val);
		void start_stock_loss_marker();
			void set_stock_loss_marker_width(std::wstring val);
		void start_stock_range_line();

		void start_element(office_element_ptr & elm, office_element_ptr & style_elm, std::wstring style_name);
		void end_element();

		void start_text();
		void end_text();

		void add_domain(std::wstring formula);
		void add_categories(std::wstring formula, office_element_ptr & axis);

		void set_layout_x(double *val,int mode);
		void set_layout_y(double *val,int mode);
		void set_layout_w(double *val,int mode);
		void set_layout_h(double *val,int mode);

		void set_local_table(bool Val);
		//void set_cash(std::wstring format, std::vector<double>		&data);
		void set_cash(std::wstring format, std::vector<std::wstring> &data,bool label = false);

	void end_chart();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

}
}