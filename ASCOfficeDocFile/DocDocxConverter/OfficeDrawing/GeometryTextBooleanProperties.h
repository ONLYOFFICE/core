/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
	// 2.3.6.31 Geometry Boolean Properties		-	[MS-ODRAW] — v20130726

	class GeometryTextBooleanProperties
	{
	public: 
		GeometryTextBooleanProperties(unsigned int op)
		{
			gtextFStrikethrough			=	FormatUtils::BitmaskToBool(op, 0x1);
			gtextFSmallcaps				=	FormatUtils::BitmaskToBool(op, 0x1 << 1);
			gtextFShadow				=	FormatUtils::BitmaskToBool(op, 0x1 << 2);
			gtextFUnderline				=	FormatUtils::BitmaskToBool(op, 0x1 << 3);
			gtextFItalic				=	FormatUtils::BitmaskToBool(op, 0x1 << 4);
			gtextFBold					=	FormatUtils::BitmaskToBool(op, 0x1 << 5);
			gtextFDxMeasure				=	FormatUtils::BitmaskToBool(op, 0x1 << 6);
			gtextFNormalize				=	FormatUtils::BitmaskToBool(op, 0x1 << 7);
			gtextFBestFit				=	FormatUtils::BitmaskToBool(op, 0x1 << 8);
			gtextFShrinkFit				=	FormatUtils::BitmaskToBool(op, 0x1 << 9);
			gtextFStretch				=	FormatUtils::BitmaskToBool(op, 0x1 << 10);
			gtextFTight					=	FormatUtils::BitmaskToBool(op, 0x1 << 11);
			gtextFKern					=	FormatUtils::BitmaskToBool(op, 0x1 << 12);
			gtextFVertical				=	FormatUtils::BitmaskToBool(op, 0x1 << 13);
			fGtext						=	FormatUtils::BitmaskToBool(op, 0x1 << 14);
			gtextFReverseRows			=	FormatUtils::BitmaskToBool(op, 0x1 << 15);

			fUsegtextFSStrikeThrough	=	FormatUtils::BitmaskToBool(op, 0x1 << 16);
			fUsegtextFSmallcaps			=	FormatUtils::BitmaskToBool(op, 0x1 << 17);
			fUsegtextFShadow			=	FormatUtils::BitmaskToBool(op, 0x1 << 18);
			fUsegtextFUnderline			=	FormatUtils::BitmaskToBool(op, 0x1 << 19);
			fUsegtextFItalic			=	FormatUtils::BitmaskToBool(op, 0x1 << 20);
			fUsegtextFBold				=	FormatUtils::BitmaskToBool(op, 0x1 << 21);
			fUsegtextFDxMeasure			=	FormatUtils::BitmaskToBool(op, 0x1 << 22);
			fUsegtextFNormalize			=	FormatUtils::BitmaskToBool(op, 0x1 << 23);
			fUsegtextFBestFit			=	FormatUtils::BitmaskToBool(op, 0x1 << 24);
			fUsegtextFShrinkFit			=	FormatUtils::BitmaskToBool(op, 0x1 << 25);
			fUsegtextFStretch			=	FormatUtils::BitmaskToBool(op, 0x1 << 26);
			fUsegtextFTight				=	FormatUtils::BitmaskToBool(op, 0x1 << 27);
			fUsegtextFKern				=	FormatUtils::BitmaskToBool(op, 0x1 << 28);
			fUsegtextFVertical			=	FormatUtils::BitmaskToBool(op, 0x1 << 29);
			fUsefGtext					=	FormatUtils::BitmaskToBool(op, 0x1 << 30);
			fUsegtextFReverseRows		=	FormatUtils::BitmaskToBool(op, 0x40000000);
		}

	public: 

		bool gtextFStrikethrough;
		bool gtextFSmallcaps;
		bool gtextFShadow;
		bool gtextFUnderline;
		bool gtextFItalic;
		bool gtextFBold;
		bool gtextFDxMeasure;
		bool gtextFNormalize;
		bool gtextFBestFit;
		bool gtextFShrinkFit;
		bool gtextFStretch;
		bool gtextFTight;
		bool gtextFKern;
		bool gtextFVertical;
		bool fGtext;
		bool gtextFReverseRows;

		bool fUsegtextFSStrikeThrough;
		bool fUsegtextFSmallcaps;
		bool fUsegtextFShadow;
		bool fUsegtextFUnderline;
		bool fUsegtextFItalic;
		bool fUsegtextFBold;
		bool fUsegtextFDxMeasure;
		bool fUsegtextFNormalize;
		bool fUsegtextFBestFit;
		bool fUsegtextFShrinkFit;
		bool fUsegtextFStretch;
		bool fUsegtextFTight;
		bool fUsegtextFKern;
		bool fUsegtextFVertical;
		bool fUsefGtext;
		bool fUsegtextFReverseRows;
	};
}