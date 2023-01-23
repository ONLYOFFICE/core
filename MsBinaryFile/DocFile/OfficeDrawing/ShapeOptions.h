/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "Record.h"
#include "../../XlsFile/Format/Logic/Biff_structures/ODRAW/OfficeArtRGFOPTE.h"

namespace DocFileFormat
{
	//!!!TODO: Реальные значения не соответствуют значениям из спецификации!!!
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

	//!!!TODO: Реальные значения не соответствуют значениям из спецификации!!!
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
