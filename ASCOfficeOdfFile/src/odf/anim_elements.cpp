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
	CP_APPLY_ATTR(L"smil:dur",			smil_dur_);

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
	_CP_OPT(std::wstring) color;
	_CP_OPT(std::wstring) dir;
	_CP_OPT(int)		  time;
	std::wstring		  type;

	_CP_OPT(std::wstring) param;

	if (anim_transition_filter_attlist_.smil_dur_)
	{
		time = anim_transition_filter_attlist_.smil_dur_->get_value();
	}	
	if (anim_transition_filter_attlist_.smil_fadeColor_)
	{
		color =anim_transition_filter_attlist_.smil_fadeColor_->get_hex_value();
	}

	smil_transition_type::type transition_type;

	if (anim_transition_filter_attlist_.smil_type_)
	{
		transition_type = anim_transition_filter_attlist_.smil_type_->get_type();
	}

	switch(transition_type)
	{
		case smil_transition_type::barnVeeWipe: 
			type = L"split";
			break; 
		case smil_transition_type::irisWipe: 
			if ((anim_transition_filter_attlist_.smil_subtype_) && (anim_transition_filter_attlist_.smil_subtype_.get()==L"diamond"))
				type = L"diamond";
			else
				type = L"zoom";
			break; 
		case smil_transition_type::miscDiagonalWipe: 
			if ((anim_transition_filter_attlist_.smil_subtype_) && (anim_transition_filter_attlist_.smil_subtype_.get()==L"doubleDiamond"))
				type = L"diamond";
			else
				type = L"zoom";
			break; 
		case smil_transition_type::ellipseWipe: 
		case smil_transition_type::eyeWipe: 
			type = L"circle";
			break; 
		case smil_transition_type::roundRectWipe: 
			type = L"zoom";
			break; 
		case smil_transition_type::fourBoxWipe: 
		case smil_transition_type::triangleWipe: 
		case smil_transition_type::arrowHeadWipe: 
		case smil_transition_type::pentagonWipe: 
		case smil_transition_type::hexagonWipe: 		
		case smil_transition_type::starWipe: 
		case smil_transition_type::miscShapeWipe: 
			type = L"plus";
			break; 
		case smil_transition_type::pinWheelWipe: 
			param = L"2";		
		case smil_transition_type::clockWipe: 
		case smil_transition_type::singleSweepWipe: //
		case smil_transition_type::doubleFanWipe: //
			type = L"wheel";
			if ((anim_transition_filter_attlist_.smil_subtype_)  && (anim_transition_filter_attlist_.smil_subtype_.get()==L"oneBlade"))	 param = L"1";
			if ((anim_transition_filter_attlist_.smil_subtype_)  && (anim_transition_filter_attlist_.smil_subtype_.get()==L"threeBlade"))param = L"3";
			if ((anim_transition_filter_attlist_.smil_subtype_)  && (anim_transition_filter_attlist_.smil_subtype_.get()==L"fourBlade")) param = L"4";
			if ((anim_transition_filter_attlist_.smil_subtype_)  && (anim_transition_filter_attlist_.smil_subtype_.get()==L"eightBlade"))param = L"8";
			break; 
		case smil_transition_type::fanWipe: 
			type = L"wedge";
			break;
		case smil_transition_type::fade:
			type = L"fade";
			param = L"1";
			break;
		case smil_transition_type::checkerBoardWipe:
			type = L"checker";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"across")	dir = L"horz";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"down")	dir = L"vert";
			break;
		case smil_transition_type::blindsWipe:
			type = L"blinds";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"vertical")	dir = L"vert";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"horizontal")	dir = L"horz";
			break;
		case smil_transition_type::diagonalWipe:
		case smil_transition_type::waterfallWipe:
			type = L"strips";			
			if (anim_transition_filter_attlist_.smil_subtype_)
			{
				if	(anim_transition_filter_attlist_.smil_subtype_.get()==L"horizontalLeft")	dir = L"rd";	
				else if(anim_transition_filter_attlist_.smil_subtype_.get()==L"horizontalRight")dir = L"lu";	
				else if(anim_transition_filter_attlist_.smil_subtype_.get()==L"verticalRight")	dir = L"ld";
				else dir = L"ru";	
			}
			break;
		case smil_transition_type::dissolve:
			type = L"dissolve";
			break;		
		case smil_transition_type::randomBarWipe:
			type = L"randomBar";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"vertical")	dir = L"vert";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"horizontal")	dir = L"horz";
			break;	
		case smil_transition_type::pushWipe: 
			type = L"push";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"combVertical")	{type = L"comb"; dir = L"vert";};
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"combHorizontal")	{type = L"comb"; dir = L"horz";};
			break;	
		case smil_transition_type::slideWipe: 
			type = L"pull";
			break;
		case smil_transition_type::boxWipe: 
			type = L"cover";
			break;
		case smil_transition_type::barnDoorWipe: 
			type = L"split";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"vertical")	param = L"vert";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"horizontal")	param = L"horz";			
			break;
		case smil_transition_type::barWipe:
			type = L"wipe";
			if (anim_transition_filter_attlist_.smil_subtype_)
			{
				if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromTopLeft")	{type = L"strips"; dir = L"rd";}
				if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromBottomLeft")	{type = L"strips"; dir = L"ru";}
				if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromTopRight")	{type = L"strips"; dir = L"ld";}
				if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromBottomRight"){type = L"strips"; dir = L"lu";}
				
				if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fadeOverColor")	{type = L"fade"; param = L"0";}
			}
			break;
///////////////////////////////////////////////////////
		case smil_transition_type::bowTieWipe:
		case smil_transition_type::veeWipe: 
		case smil_transition_type::zigZagWipe: 
		case smil_transition_type::barnZigZagWipe: 
		case smil_transition_type::doubleSweepWipe: 
		case smil_transition_type::saloonDoorWipe: 
		case smil_transition_type::windshieldWipe: 
		case smil_transition_type::snakeWipe: 
		case smil_transition_type::spiralWipe: 
		case smil_transition_type::parallelSnakesWipe: 
		case smil_transition_type::boxSnakesWipe: 
			break;
//////////////////////////////////////////////////////
	}
	if (anim_transition_filter_attlist_.smil_subtype_)
	{
		if (!dir)
		{
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"leftToRight")
			{
				if ((anim_transition_filter_attlist_.smil_direction_) && (anim_transition_filter_attlist_.smil_direction_.get()==L"reverse"))dir = L"l";
				else dir = L"r";
			}
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"topToBottom")
			{
				if ((anim_transition_filter_attlist_.smil_direction_) && (anim_transition_filter_attlist_.smil_direction_.get()==L"reverse"))dir = L"u";
				else dir = L"d";	
			}

			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromTop")		dir = L"d";	
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromLeft")		dir = L"r";	
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromRight")		dir = L"l";			
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromBottom")		dir = L"u";	
			
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"topRight")		dir = L"ld";	
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"bottomLeft")		dir = L"lu";	
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"bottomRight")	dir = L"ru";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"topLeft")		dir = L"rd";	
		
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromTopLeft")	dir = L"rd";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromBottomLeft")	dir = L"ru";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromTopRight")	dir = L"ld";
			if (anim_transition_filter_attlist_.smil_subtype_.get()==L"fromBottomRight")dir = L"lu";

		}
		
		if (!dir && (anim_transition_filter_attlist_.smil_direction_) && (anim_transition_filter_attlist_.smil_direction_.get()==L"reverse"))
			dir = L"in";
	}

	Context.get_slide_context().set_transitionFilter(type , dir, param , time);

}

////////////////////////////////////////////////////////////////
}
}
