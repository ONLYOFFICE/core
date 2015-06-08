#pragma once

#include "style_graphic_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_presentation.h"

#include "odfcontext.h"
#include <vector>

namespace cpdoccore { 
namespace odf_reader {

graphic_format_properties calc_graphic_properties_content(const style_instance * styleInstance);
graphic_format_properties calc_graphic_properties_content(const std::vector<const style_instance *> & styleInstances);

text_format_properties_content calc_text_properties_content(const style_instance * styleInstance);
text_format_properties_content calc_text_properties_content(const std::vector<const style_instance *> & styleInstances);

paragraph_format_properties calc_paragraph_properties_content(const style_instance * styleInstance);
paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_instance *> & styleInstances);

style_table_cell_properties_attlist calc_table_cell_properties(const style_instance * styleInstance);
style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_instance *> & styleInstances);

}
}
