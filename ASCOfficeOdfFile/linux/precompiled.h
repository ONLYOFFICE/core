// Add C includes here

#if defined __cplusplus

// Add C++ includes here


#include <iosfwd>
#include <string>
#include <ostream>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/noncopyable.hpp>
#include <map>
#include <boost/regex.hpp>
#include <boost/functional.hpp>
#include <list>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/regex.h>
#include <boost/unordered_map.hpp>
#include <boost/ref.hpp>
#include <boost/unordered_set.hpp>
#include <exception>
#include <algorithm>
#include <boost/optional.hpp>
#include <stdexcept>
#include <iterator>
#include <stack>
#include <cassert>
#include <utility>
#include <limits>
#include <typeinfo>
#include <cmath>
#include <random>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

#include "../src/odf/list.cpp"
#include "../formulasconvert/formulasconvert.h"
#include "../src/odf/calcs_styles.h"
#include "../src/odf/chart_objects.h"
#include "../src/odf/documentcontext.h"
#include "../src/odf/draw_common.h"
#include "../src/odf/list.h"
#include "../src/odf/number_style.h"
#include "../src/odf/odfcontext.h"
#include "../src/odf/odf_document_impl.h"
#include "../src/odf/office_body.h"
#include "../src/odf/office_document.h"
#include "../src/odf/office_elements.h"
#include "../src/odf/office_elements_create.h"
#include "../src/odf/office_forms.h"
#include "../src/odf/office_spreadsheet.h"
#include "../src/odf/paragraph_elements.h"
#include "../src/odf/serialize_elements.h"
#include "../src/odf/style_graphic_properties.h"
#include "../src/odf/style_paragraph_properties.h"
#include "../src/odf/style_presentation.h"
#include "../src/odf/styles.h"
#include "../src/odf/style_table_properties.h"
#include "../src/odf/style_text_properties.h"
#include "../src/odf/table.h"
#include "../src/odf/text_elements.h"
#include "../src/odf/math_elements.h"
#include "../src/odf/math_limit_elements.h"
#include "../src/odf/datatypes/borderstyle.h"
#include "../src/odf/datatypes/color.h"
#include "../src/odf/datatypes/common_attlists.h"
#include "../src/odf/datatypes/custom_shape_types_convert.h"
#include "../src/odf/datatypes/errors.h"
#include "../src/odf/datatypes/length.h"
#include "../src/odf/datatypes/lengthorpercent.h"
#include "../src/odf/datatypes/linestyle.h"
#include "../src/odf/datatypes/noteclass.h"
#include "../src/odf/datatypes/odfattributes.h"
#include "../src/odf/datatypes/percent.h"
#include "../src/odf/datatypes/stylefamily.h"
#include "../src/odf/datatypes/verticalalign.h"
#include "../src/docx/docx_conversion_context.h"
#include "../src/docx/docx_package.h"
#include "../src/docx/mediaitems.h"
#include "../src/docx/oox_chart_shape.h"
#include "../src/docx/oox_conversion_context.h"
#include "../src/docx/oox_drawing.h"
#include "../src/docx/oox_drawing_fills.h"
#include "../src/docx/oox_package.h"
#include "../src/docx/oox_rels.h"
#include "../src/docx/pptx_conversion_context.h"
#include "../src/docx/pptx_package.h"
#include "../src/docx/xlsx_cell_format.h"
#include "../src/docx/xlsxconversioncontext.h"
#include "../src/docx/xlsx_package.h"
#include "../src/docx/xlsx_table_metrics.h"
#include "../src/docx/xlsx_utils.h"
#include "../include/logging.h"
#include "../include/CPNoncopyable.h"
#include "../include/CPOptional.h"
#include "../include/CPScopedPtr.h"
#include "../include/CPSharedPtr.h"
#include "../include/odf/odf_document.h"
#include "../include/xml/attributes.h"
#include "../include/xml/nodetype.h"
#include "../include/xml/sax.h"
#include "../include/xml/simple_xml_writer.h"
#include "../include/xml/utils.h"
#include "../include/xml/xmlchar.h"


#endif
