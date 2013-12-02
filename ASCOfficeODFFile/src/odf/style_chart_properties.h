#pragma once

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "chartsymbol.h"
#include "length.h"
#include "chartinterpolation.h"
#include "chartsolidtype.h"
#include "chartlabelarrangement.h"
#include "common_attlists.h"
#include "chartdatalabelnumber.h"
#include "charterrorcategory.h"
#include "chartseriessource.h"
#include "chartregressiontype.h"
#include "direction.h"

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
