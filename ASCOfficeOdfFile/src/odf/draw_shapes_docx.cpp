
#include "draw_common.h"
#include "datatypes/custom_shape_types_convert.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/regex.h>
#include <boost/algorithm/string.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/odf/odf_document.h>

#include "serialize_elements.h"
#include "../docx/xlsx_utils.h"
#include "style_graphic_properties.h"

#include "odfcontext.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



void draw_shape::common_docx_convert(oox::docx_conversion_context & Context)
{
    Context.get_drawing_context().start_shape(this);
    
    const _CP_OPT(std::wstring) name = 
        common_draw_attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.
        common_draw_name_attlist_.draw_name_;
	
	Context.get_drawing_context().add_name_object(name.get_value_or(L""));

//////////////////////////////на другом контексте

	//тут может быть не только текст , но и таблицы, другие объекты ...
 	oox::docx_conversion_context::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_shape());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));

	bool pParaState = Context.get_paragraph_state();
	bool pRunState = Context.get_run_state();
	Context.set_paragraph_state(false);		
	Context.set_run_state(false);		

	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
		ElementType type = elm->get_type();
        elm->docx_convert(Context);
    }

	Context.set_paragraph_state(pParaState);	
	Context.set_run_state(pRunState);	

	Context.get_drawing_context().get_text_stream_shape() = temp_stream.str();
	Context.set_stream_man(prev);
}
void draw_rect::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);

}
void draw_ellipse::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_circle::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_line::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	reset_svg_attributes();	
	
	common_docx_convert(Context);

	draw_shape::docx_convert(Context);
}

void draw_path::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	reset_svg_path();
	draw_shape::docx_convert(Context);
}

void draw_connector::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	reset_svg_path();
	draw_shape::docx_convert(Context);
}
void draw_polygon::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	reset_polygon_path();
	draw_shape::docx_convert(Context);
}
void draw_custom_shape::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_caption::docx_convert(oox::docx_conversion_context & Context)
{
	if (Context.get_drawing_context().get_current_level() >0 )return;

	common_docx_convert(Context);
	//...
	draw_shape::docx_convert(Context);
}
void draw_enhanced_geometry::docx_convert(oox::docx_conversion_context & Context)
{
	find_draw_type_oox();

	draw_shape * shape = Context.get_drawing_context().get_current_shape();//owner

	if (sub_type_)
	{
		shape->sub_type_ = sub_type_.get();
	}
	if (draw_type_oox_index_)
	{
		shape->additional_.push_back(_property(L"draw-type-index",draw_type_oox_index_.get()));	
	}
	if (draw_enhanced_geometry_attlist_.draw_modifiers_)
	{
		shape->additional_.push_back(_property(L"draw-modifiers",draw_enhanced_geometry_attlist_.draw_modifiers_.get()));	
		if (draw_handle_geometry_.size()>0)
		{
			if (draw_handle_geometry_[0].min < draw_handle_geometry_[0].max)
			{
				shape->additional_.push_back(_property(L"draw-modifiers-min",draw_handle_geometry_[0].min));	
				shape->additional_.push_back(_property(L"draw-modifiers-max",draw_handle_geometry_[0].max));	
			}
		}

	}
}
}
}