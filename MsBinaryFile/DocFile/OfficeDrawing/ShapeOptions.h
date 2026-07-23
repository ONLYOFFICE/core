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

#include "Record.h"
#include "../../XlsFile/Format/Logic/Biff_structures/ODRAW/OfficeArtRGFOPTE.h"

namespace DocFileFormat
{
	//!!!TODO: Actual values do not match the specification values!!!
	typedef enum _PositionHorizontal
	{
		//msophAbs = 0x1,
		msophLeft = 0x1,
		msophCenter = 0x2,
		msophRight = 0x3,
		msophInside = 0x4,
		msophOutside = 0x5
	} PositionHorizontal;

	typedef enum _PositionHorizontalRelative
	{
		msoprhMargin,
		msoprhPage,
		msoprhText,
		msoprhChar
	} PositionHorizontalRelative;

	//!!!TODO: Actual values do not match the specification values!!!
	typedef enum _PositionVertical
	{
		//msopvAbs = 0x1,
		msopvTop = 0x1,
		msopvCenter = 0x2,
		msopvBottom = 0x3,
		msopvInside = 0x4,
		msopvOutside = 0x5
	} PositionVertical;

	typedef enum _PositionVerticalRelative
	{
		msoprvMargin,
		msoprvPage,
		msoprvText,
		msoprvLine
	} PositionVerticalRelative;

	typedef enum _LineEnd
	{
		NoEnd = 0,
		ArrowEnd,
		ArrowStealthEnd,
		ArrowDiamondEnd,
		ArrowOvalEnd,
		ArrowOpenEnd,
		ArrowChevronEnd,
		ArrowDoubleChevronEnd
	} LineEnd;

	typedef enum _LineDashing
	{
		LineDashing_Solid = 0,
		LineDashing_DashSys,
		LineDashing_DotSys,
		LineDashing_DashDotSys,
		LineDashing_DashDotDotSys,
		LineDashing_DotGEL,
		LineDashing_DashGEL,
		LineDashing_LongDashGEL,
		LineDashing_DashDotGEL,
		LineDashing_LongDashDotGEL,
		LineDashing_LongDashDotDotGEL
	} LineDashing;

	class ShapeOptions: public Record
	{
	public: 
		static const unsigned short TYPE_CODE_0xF00B = 0xF00B;
		static const unsigned short TYPE_CODE_0xF121 = 0xF121;
		static const unsigned short TYPE_CODE_0xF122 = 0xF122;

		std::vector<ODRAW::OfficeArtFOPTEPtr>					Options;
		std::map<ODRAW::ePropertyId, ODRAW::OfficeArtFOPTEPtr>	OptionsByID;
	
		ShapeOptions();
		virtual ~ShapeOptions();
		ShapeOptions (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance);

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance );
	};
}
