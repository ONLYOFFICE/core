#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"


namespace cpdoccore {
namespace odf
{

odf_drawing_context::odf_drawing_context(odf_style_context * styles_context,odf_conversion_context *odf_context)
{
	styles_context_ = styles_context;
	odf_context_ = odf_context;

}
odf_drawing_context::~odf_drawing_context()
{
}
void odf_drawing_context::set_styles_context(odf_style_context*  styles_context)
{
	styles_context_ = styles_context;
}
}
}