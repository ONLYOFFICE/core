/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
	_docx_drawing():_oox_drawing(), parallel(0), isInline(false), number_wrapped_paragraphs(0), posOffsetV(0), posOffsetH(0)
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

	std::wstring content_group_;

    friend void docx_serialize(std::wostream & _Wostream, _docx_drawing & val);    
};
}
}
