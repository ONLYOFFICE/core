#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "common_attlists.h"
#include "../docx/xlsxconversioncontext.h"
#include "../docx/xlsx_utils.h"
#include "style_graphic_properties.h"
#include "draw_frame.h"
#include "draw_shapes.h"
#include "xlink.h"
#include "targetframename.h"

namespace _image_file_
{
	bool GetResolution(const wchar_t* fileName, int & Width, int &Height);
}

namespace cpdoccore { 
namespace odf {

class styles_lite_container;


enum BorderSide { sideTop, sideBottom, sideLeft, sideRight, sideMiddle };

int get_value_emu(const _CP_OPT(length) & len);
int get_value_emu(double pt);

int Compute_BorderWidth(const graphic_format_properties & graphicProperties, BorderSide borderSide);//emu

int GetMargin(const graphic_format_properties & graphicProperties, BorderSide borderSide);//emu

void Compute_GraphicFill(graphic_format_properties & props, styles_lite_container &styles, oox::_oox_fill & fill);

typedef double double_4[4];
bool parse_clipping(std::wstring strClipping,std::wstring fileName,double_4 & clip_rect);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_a : public office_element_impl<draw_a>
{  
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawA;
	static const xml::NodeType xml_type = xml::typeElement;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

  
	office_element_ptr_array content_;

	friend class odf_document;

private:
	common_xlink_attlist common_xlink_attlist_;

    ::std::wstring office_name_;
    _CP_OPT(target_frame_name) office_target_frame_name_;

    style_ref text_style_name_;
    style_ref text_visited_style_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_a);

void xlsx_convert_transforms(std::wstring transformStr, oox::xlsx_conversion_context & Context);
void pptx_convert_transforms(std::wstring transformStr, oox::pptx_conversion_context & Context);
void oox_convert_transforms(std::wstring transformStr,std::vector<odf::_property> & additional);
//void docx_convert_transforms(std::wstring transformStr, oox::xlsx_conversion_context & Context);

}
}