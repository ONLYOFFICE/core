#include "precompiled_cpodf.h"
#include "anim_elements.h"

#include <boost/make_shared.hpp>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include <boost/lexical_cast.hpp>

#include "serialize_elements.h"
#include "odfcontext.h"
#include <cpdoccore/odf/odf_document.h>


#include "draw_common.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {



///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * anim_par::ns = L"anim";
const wchar_t * anim_par::name = L"par";

void anim_par::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void anim_par::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (anim_par_)
	{
		Context.get_slide_context().start_slide_animation();
			anim_par_->pptx_convert(Context); // это для самого слайда (то что и нужно)
		Context.get_slide_context().end_slide_animation();
	}
///////////////////////// последовательности .. (если один элемент - основная последовательность, иное - взаимодействующая анимация)
	//slide_context().animation_context().start_sequence();
	BOOST_FOREACH(const office_element_ptr& elm, anim_seq_array_)
    {
		elm->pptx_convert(Context);
	}
	//slide_context().animation_context().end_sequence();
/////////////////////////////////////////////////////////////////
//внутренние эффекты - те что внутри одной последовательности
	BOOST_FOREACH(const office_element_ptr& elm, content_)
    {
		elm->pptx_convert(Context);
	}
}
void anim_par::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(anim_par_);
	else if	CP_CHECK_NAME(L"anim", L"seq") 
		CP_CREATE_ELEMENT(anim_seq_array_);//более 1 элемента- взаимосвязанная анимация (между фигурами)
	else
		CP_CREATE_ELEMENT(content_);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * anim_seq::ns = L"anim";
const wchar_t * anim_seq::name = L"seq";

void anim_seq::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void anim_seq::pptx_convert(oox::pptx_conversion_context & Context)
{
	BOOST_FOREACH(const office_element_ptr& elm, anim_par_array_)
    {
		elm->pptx_convert(Context);
	}
}
void anim_seq::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	if	CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(anim_par_array_);
}
////////////////////////////////////////////////////////////////
void anim_transition_filter_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"smil:direction",	smil_direction_);
    CP_APPLY_ATTR(L"smil:subtype",		smil_subtype_);
    CP_APPLY_ATTR(L"smil:type",			smil_type_);
	CP_APPLY_ATTR(L"smil:fadeColor",	smil_fadeColor_);
	CP_APPLY_ATTR(L"smil:mode",			smil_mode_);

}

const wchar_t * anim_transitionFilter::ns = L"anim";
const wchar_t * anim_transitionFilter::name = L"transitionFilter";

void anim_transitionFilter::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_anim_smil_attlist_.add_attributes(Attributes);
	anim_transition_filter_attlist_.add_attributes(Attributes);
}

void anim_transitionFilter::pptx_convert(oox::pptx_conversion_context & Context)
{
	if (common_anim_smil_attlist_.smil_dur_)
	{
		Context.get_slide_context().set_animation_duration(5000/*common_anim_smil_attlist_.smil_dur_*/);//*1000); 
		//need parsing ~ 3s
	}
	_CP_OPT(std::wstring) color;
	if (smil_fadeColor_)
	{
		color = smil_fadeColor_->get_hex_value();
	}
	Context.get_slide_context().set_transitionFilter(smil_direction_,smil_type_,smil_subtype_,smil_mode_,color);

}

////////////////////////////////////////////////////////////////
}
}
