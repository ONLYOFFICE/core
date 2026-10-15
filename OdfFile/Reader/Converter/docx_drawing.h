/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <iosfwd>
#include <iostream>

#include "oox_drawing.h"
#include "mediaitems.h"
#include "../Format/style_graphic_properties.h"

namespace cpdoccore {
namespace oox {

class _docx_drawing : public _oox_drawing
{ 
public:
	_docx_drawing() : _oox_drawing(), parallel(0), isInline(false), m_isChar(false), inFrame(false), number_wrapped_paragraphs(0), posOffsetV(0), posOffsetH(0)
	{
	}
	bool isInline;
	bool m_isChar;
	bool inFrame;
    
	unsigned int parallel;
	
	//_CP_OPT(run_through)				styleRunThrough	;
	_CP_OPT(odf_types::horizontal_rel)	styleHorizontalRel;
    _CP_OPT(odf_types::horizontal_pos)	styleHorizontalPos;
    _CP_OPT(odf_types::vertical_pos)	styleVerticalPos;
    _CP_OPT(odf_types::vertical_rel)	styleVerticalRel;

	_CP_OPT(odf_types::style_wrap)		styleWrap;
	_CP_OPT(bool)						styleWrapContour;

	int number_wrapped_paragraphs;

    std::wstring relativeHeight;
    std::wstring behindDoc;

	std::wstring hyperlinkRId;

	int posOffsetV;
	int posOffsetH;

	_CP_OPT(int) pctWidth;
	_CP_OPT(int) pctHeight;
  
	int margin_rect[4];		//0-left, 1 -top, 2- right, 3 - bottom

	std::wstring content_group_;

	void serialize_text	(std::wostream & strm);
	void serialize		(std::wostream & strm/*, bool insideOtherDrawing*/, oox::docx_conversion_context & Context);
	void serialize		(std::wostream & strm/*, bool insideOtherDrawing*/);
};
}
}
