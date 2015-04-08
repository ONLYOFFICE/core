#pragma once

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/chartsymbol.h"
#include "datatypes/length.h"
#include "datatypes/chartinterpolation.h"
#include "datatypes/chartsolidtype.h"
#include "datatypes/chartlabelarrangement.h"
#include "datatypes/common_attlists.h"
#include "datatypes/chartdatalabelnumber.h"
#include "datatypes/charterrorcategory.h"
#include "datatypes/chartseriessource.h"
#include "datatypes/chartregressiontype.h"
#include "datatypes/direction.h"

namespace cpdoccore { 
namespace odf {

/// \class  style_chart_properties
///         style:chart-properties
class style_chart_properties : public office_element_impl<style_chart_properties>//стили чарта не наследуются
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleChartProperties;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
public:
	std::vector<_property> content_;

	common_rotation_angle_attlist       common_rotation_angle_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_chart_properties);

}
}
