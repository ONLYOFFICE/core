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
		void set_type_chart(int type);
		void start_series();
		void end_series();
		
		void start_axis();
		void end_axis();
	void end_chart();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

}
}