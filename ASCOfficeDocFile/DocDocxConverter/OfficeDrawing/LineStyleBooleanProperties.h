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

namespace DocFileFormat
{
	// 2.3.8.38 Line Style Boolean Properties 	-	[MS-ODRAW] — v20130726

	class LineStyleBooleanProperties
	{
	public:
		LineStyleBooleanProperties(unsigned int op)
		{
			fNoLineDrawDash				=	FormatUtils::BitmaskToBool(op, 0x1);
			fLineFillShape				=	FormatUtils::BitmaskToBool(op, 0x2);
			fHitTestLine				=	FormatUtils::BitmaskToBool(op, 0x4);
			fLine						=	FormatUtils::BitmaskToBool(op, 0x8);

			fArrowheadsOK				=	FormatUtils::BitmaskToBool(op, 0x10);
			fInsetPenOK					=	FormatUtils::BitmaskToBool(op, 0x20);
			fInsetPen					=	FormatUtils::BitmaskToBool(op, 0x40);

			//Reserved 0x80 0x100

			fLineOpaqueBackColor		=	FormatUtils::BitmaskToBool(op, 0x200);

			//Unused 0x400 0x800 0x1000 0x2000 0x4000 0x8000

			fUsefNoLineDrawDash			=	FormatUtils::BitmaskToBool(op, 0x10000);
			fUsefLineFillShape			=	FormatUtils::BitmaskToBool(op, 0x20000);
			fUsefHitTestLine			=	FormatUtils::BitmaskToBool(op, 0x40000);
			fUsefLine					=	FormatUtils::BitmaskToBool(op, 0x80000);
			fUsefArrowheadsOK			=	FormatUtils::BitmaskToBool(op, 0x100000);
			fUsefInsetPenOK				=	FormatUtils::BitmaskToBool(op, 0x200000);
			fUsefInsetPen				=	FormatUtils::BitmaskToBool(op, 0x400000);

			//Reserved 0x800000 0x1000000

			fUsefLineOpaqueBackColor	=	FormatUtils::BitmaskToBool(op, 0x2000000); 
		}

	public:

		bool fNoLineDrawDash;
		bool fLineFillShape;
		bool fHitTestLine;
		bool fLine;
		bool fArrowheadsOK;
		bool fInsetPenOK;
		bool fInsetPen;
		bool fLineOpaqueBackColor;

		bool fUsefNoLineDrawDash;
		bool fUsefLineFillShape;
		bool fUsefHitTestLine;
		bool fUsefLine;
		bool fUsefArrowheadsOK;
		bool fUsefInsetPenOK;
		bool fUsefInsetPen;
		bool fUsefLineOpaqueBackColor;
	};
}