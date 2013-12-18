#include "precompiled_cpodf.h"
#include "draw_page.h"

#include <boost/make_shared.hpp>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>

namespace cpdoccore { 
namespace odf {


void draw_page_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"draw:name",			draw_name_);
    CP_APPLY_ATTR(L"draw:style-name",	draw_style_name_);
    
	CP_APPLY_ATTR(L"presentation:presentation-page-layout-name", page_layout_name_);
    CP_APPLY_ATTR(L"draw:master-page-name",				master_page_name_);
    
	CP_APPLY_ATTR(L"presentation:use-date-time-name",	use_date_time_name_);
    CP_APPLY_ATTR(L"presentation:use-footer-name",		use_footer_name_);

}

// office:annotation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * draw_page::ns = L"draw";
const wchar_t * draw_page::name = L"page";

void draw_page::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void draw_page::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    draw_page_attr_.add_attributes(Attributes);
}


}
}
