#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements.h"
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
		void set_type_chart(std::wstring type);
		void set_3D(bool Val);
		void set_size_chart(double width_pt, double height_pt);
		
		void start_series(std::wstring type);
			void set_series_value_formula(std::wstring oox_formula);
			void set_series_label_formula(std::wstring oox_formula);
			void set_series_name(std::wstring name);
			void set_category_axis_formula(std::wstring oox_formula);
	
		void start_axis();
			void set_axis_dimension(std::wstring val);
			void set_axis_orientation(int type);
			void set_axis_logarithmic(bool val);
			void set_axis_label(int type);
			void set_axis_position(int type);
			void set_axis_label_position(int type);
		void start_title();
		void start_grid(int type);
		void start_plot_area();
		void start_legend();
			void set_legend_position(int val);

		void start_element(office_element_ptr & elm, office_element_ptr & style_elm, std::wstring style_name);
		void end_element();

		void start_text();
		void end_text();

		void add_categories(std::wstring formula);

		void set_layout_x(double *val,int mode);
		void set_layout_y(double *val,int mode);
		void set_layout_w(double *val,int mode);
		void set_layout_h(double *val,int mode);

	void end_chart();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

}
}