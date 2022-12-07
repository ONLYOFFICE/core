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

#include "IVisitable.h"
#include "Global.h"

namespace DocFileFormat
{
	typedef enum _BorderType
	{
		none = 0,
		single,
		thick,
		Double,
		unused,
		hairline,
		dotted,
		dashed,
		dotDash,
		dotDotDash,
		triple,
		thinThickSmallGap,
		thickThinSmallGap,
		thinThickThinSmallGap,
		thinThickMediumGap,
		thickThinMediumGap,
		thinThickThinMediumGap,
		thinThickLargeGap,
		thickThinLargeGap,
		thinThickThinLargeGap,
		wave,
		doubleWave,
		dashSmallGap,
		dashDotStroked,
		threeDEmboss,
		threeDEngrave
	} BorderType;

	class BorderCode: public IVisitable
	{
		friend class DocumentMapping;
		friend class PropertiesMapping;
		friend class VMLPictureMapping;

	private:
		/// 24-bit border color
		int cv;
		/// Width of a single line in 1/8pt, max of 32pt
		unsigned char dptLineWidth;
		/// Border type code:
		/// 0 none
		/// 1 single
		/// 2 thick
		/// 3 double
		/// 5 hairline
		/// 6 dot
		/// 7 dash large gap
		/// 8 dot dash
		/// 9 dot dot dash
		/// 10 triple
		/// 11 thin-thick small gap
		/// 12 tick-thin small gap
		/// 13 thin-thick-thin small gap
		/// 14 thin-thick medium gap
		/// 15 thick-thin medium gap
		/// 16 thin-thick-thin medium gap
		/// 17 thin-thick large gap
		/// 18 thick-thin large gap
		/// 19 thin-thick-thin large gap
		/// 20 wave
		/// 21 double wave
		/// 22 dash small gap
		/// 23 dash dot stroked
		/// 24 emboss 3D
		/// 25 engrave 3D
		unsigned char brcType;
		/// The color of the Border.
		/// Unused if cv is set.
		std::wstring ico;
		/// Width of space to maintain between border and text within border
		int dptSpace;
		/// When true, border is drawn with shadow. Must be false when BRC is substructure of the TC
		bool fShadow;
		/// When true, don't reverse the border
		bool fFrame;
		/// It's a nil BRC, bytes are FFFF.
		bool fNil;

	public:
		/// Creates a new BorderCode with default values
		BorderCode();

		/// Parses the unsigned char for a BRC
		BorderCode( unsigned char* bytes, int size );
		BorderCode( const BorderCode& bc );

		bool operator == ( const BorderCode& bc );
		bool operator != ( const BorderCode& bc );
	};
}
