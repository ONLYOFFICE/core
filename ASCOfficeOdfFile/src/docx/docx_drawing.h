#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "oox_drawing.h"
#include "mediaitems.h"
#include "../odf/style_graphic_properties.h"

namespace cpdoccore {
namespace oox {

struct _docx_drawing : _oox_drawing
{
	_docx_drawing():_oox_drawing(),parallel(0),isInline(false),number_wrapped_paragraphs(0),posOffsetV(0),posOffsetH(0)
	{
	}
    unsigned int parallel;
	
	//_CP_OPT(run_through)	styleRunThrough	;
	_CP_OPT(odf_types::horizontal_rel)	styleHorizontalRel;
    _CP_OPT(odf_types::horizontal_pos)	styleHorizontalPos;
    _CP_OPT(odf_types::vertical_pos)	styleVerticalPos;
    _CP_OPT(odf_types::vertical_rel)	styleVerticalRel;

	_CP_OPT(odf_types::style_wrap)  styleWrap;

	bool isInline;
	int number_wrapped_paragraphs;

    std::wstring relativeHeight;
    std::wstring behindDoc;

	int posOffsetV;
	int posOffsetH;
  
	int margin_rect[4];//0-left, 1 -top, 2- right, 3 - bottom
	//std::wstring distB;
 //   std::wstring distT;
 //   std::wstring distL;
 //   std::wstring distR;

    friend void docx_serialize(std::wostream & _Wostream, _docx_drawing & val);    
};
}
}
