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

namespace DocFileFormat
{
	class PathSegment
	{
	public:

		enum MSOPATHTYPE
		{
			msopathLineTo,
			msopathCurveTo,
			msopathMoveTo,
			msopathClose,
			msopathEnd,
			msopathEscape,
			msopathClientEscape,
			msopathInvalid
		};

		enum MSOPATHESCAPE
		{
			msopathEscapeExtension 				=	0x00000000,	
			msopathEscapeAngleEllipseTo  		=	0x00000001,
			msopathEscapeAngleEllipse 	 		=	0x00000002,
			msopathEscapeArcTo  				=	0x00000003,
			msopathEscapeArc  					=	0x00000004,
			msopathEscapeClockwiseArcTo  		=	0x00000005,
			msopathEscapeClockwiseArc 	 		=	0x00000006,
			msopathEscapeEllipticalQuadrantX  	=	0x00000007,
			msopathEscapeEllipticalQuadrantY 	=	0x00000008,
			msopathEscapeQuadraticBezier 	 	=	0x00000009,
			msopathEscapeNoFill  				=	0x0000000A,
			msopathEscapeNoLine  				=	0x0000000B,
			msopathEscapeAutoLine  				=	0x0000000C,
			msopathEscapeAutoCurve  			=	0x0000000D,
			msopathEscapeCornerLine  			=	0x0000000E,
			msopathEscapeCornerCurve 			=	0x0000000F,
			msopathEscapeSmoothLine  			=	0x00000010,
			msopathEscapeSmoothCurve  			=	0x00000011,
			msopathEscapeSymmetricLine 			=	0x00000012,
			msopathEscapeSymmetricCurve 		=	0x00000013,
			msopathEscapeFreeform 				=	0x00000014,
			msopathEscapeFillColor 				=	0x00000015,
			msopathEscapeLineColor 				=	0x00000016
		};
		
		PathSegment (unsigned short segment): Type(msopathInvalid), Count(0), EscapeCode(msopathEscapeExtension), VertexCount(0)
		{
			Type			=	(MSOPATHTYPE)FormatUtils::BitmaskToInt (segment, 0xE000);

			if (msopathEscape == Type)
			{
				EscapeCode	=	(MSOPATHESCAPE)FormatUtils::BitmaskToInt (segment, 0x1F00);
				VertexCount	=	FormatUtils::BitmaskToInt (segment, 0x00FF);
			}
			else
			{
				Count		=	FormatUtils::BitmaskToInt (segment, 0x1FFF);
			}
		}

	public:

		MSOPATHTYPE		Type;
		int				Count;
		int				VertexCount;
		MSOPATHESCAPE	EscapeCode;
	};
}