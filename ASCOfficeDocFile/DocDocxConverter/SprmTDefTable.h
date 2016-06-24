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
#include "../../Common/DocxFormat/Source/Base/SmartPtr.h"

#include "BorderCode.h"
#include "Global.h"

namespace DocFileFormat
{
	struct TC80
	{
		TC80()
		{
			horzMerge = wWidth = 0;
			
			textFlow	=	Global::lrTb;
			vertMerge	=	Global::fvmClear;
			vertAlign	=	Global::top;
			ftsWidth	=	Global::nil;

			fFitText = fNoWrap = fHideMark = false;
		}
		/// A value from the following table that specifies how this cell merges horizontally with the neighboring cells in the same row.
		/// MUST be one of the following values:
		/// 0 - The cell is not merged with the cells on either side of it.
		/// 1 - The cell is one of a set of horizontally merged cells. It contributes its layout region to the set and its own contents are not rendered.
		/// 2, 3 - The cell is the first cell in a set of horizontally merged cells. The contents and formatting of this cell extend into any consecutive cells following it that are designated as part of the merged set.
		unsigned char horzMerge;
		/// A value from the TextFlow enumeration that specifies rotation settings for the text in the cell.
		Global::TextFlow textFlow;
		/// A value from the VerticalMergeFlag enumeration that specifies how this cell merges vertically with the cells above or below it.
		Global::VerticalMergeFlag vertMerge;
		/// A value from the VerticalAlign enumeration that specifies how contents inside this cell are aligned.
		Global::VerticalAlign vertAlign;
		/// An Fts that specifies the unit of measurement for the wWidth field in the TC80 structure.
		Global::CellWidthType ftsWidth;
		/// Specifies whether the contents of the cell are to be stretched out such that the full cell width is used.
		bool fFitText;
		/// When set, specifies that the preferred layout of the contents of this cell are as a single line, 
		/// and cell widths can be adjusted to accommodate long lines.
		/// This preference is ignored when the preferred width of this cell is set to ftsDxa.
		bool fNoWrap;
		/// When set, specifies that this cell is rendered with no height if all cells in the row are empty.
		bool fHideMark;
		/// An integer that specifies the preferred width of the cell. 
		/// The width includes cell margins, but does not include cell spacing. MUST be non-negative.
		/// The unit of measurement depends on ftsWidth.
		/// If ftsWidth is set to ftsPercent, the value is a fraction of the width of the entire table.
		short wWidth;
		
		NSCommon::smart_ptr<BorderCode> brcTop;
		NSCommon::smart_ptr<BorderCode> brcLeft;
		NSCommon::smart_ptr<BorderCode> brcBottom;
		NSCommon::smart_ptr<BorderCode> brcRight;
	} ;

	class SprmTDefTable
	{
	private: 
		friend class TableCellPropertiesMapping;
	public:
		SprmTDefTable (unsigned char* bytes, int size)
		{
			numberOfColumns = bytes[0];
			int pointer = 1;

			// rgdxaCenter

			for (int i = 0; i < numberOfColumns + 1; ++i)
			{
				rgdxaCenter.push_back(FormatUtils::BytesToInt16(bytes, pointer, size));
				pointer += 2;
			}

			// rgTc80

			for (int i = 0; i < numberOfColumns; ++i)
			{
				if (size <= pointer)
				{
					for (int j = i; j < numberOfColumns; ++j)
					{
						TC80 oTC80;
						
						oTC80.ftsWidth		=	Global::dxa;
						oTC80.wWidth		=	3190;

						oTC80.brcTop		=	new BorderCode();
						oTC80.brcLeft		=	new BorderCode();
						oTC80.brcBottom		=	new BorderCode();
						oTC80.brcRight		=	new BorderCode();
						
						rgTc80.push_back(oTC80);
					}

					// static int m = 0; //ATLTRACE(L"PUSH: %d\n", m); ++m;

					break;
				}	

				TC80 tc;
				// the flags
				
				unsigned short flags	=	FormatUtils::BytesToUInt16(bytes, pointer, size);
				
				tc.horzMerge			=	(unsigned char)FormatUtils::BitmaskToInt((int)flags, 0x3);
				tc.textFlow				=	(Global::TextFlow)FormatUtils::BitmaskToInt((int)flags, 0x1C);
				tc.vertMerge			=	(Global::VerticalMergeFlag)FormatUtils::BitmaskToInt((int)flags, 0x60);
				tc.vertAlign			=	(Global::VerticalAlign)FormatUtils::BitmaskToInt((int)flags, 0x180);
				tc.ftsWidth				=	(Global::CellWidthType)FormatUtils::BitmaskToInt((int)flags, 0xE00);
				tc.fFitText				=	FormatUtils::BitmaskToBool(flags, 0x1000);
				tc.fNoWrap				=	FormatUtils::BitmaskToBool(flags, 0x2000);
				tc.fHideMark			=	FormatUtils::BitmaskToBool(flags, 0x4000);
				
				pointer					+=	2;

				// cell width
				tc.wWidth				=	FormatUtils::BytesToInt16(bytes, pointer, size);
				pointer					+=	2;

				const int borderCodeBytes = 4;

				//border top
				unsigned char brcTopBytes[borderCodeBytes];
				memcpy(brcTopBytes, (bytes + pointer), borderCodeBytes);
				tc.brcTop = new BorderCode(brcTopBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				//border left
				unsigned char brcLeftBytes[borderCodeBytes];
				memcpy(brcLeftBytes, (bytes + pointer), borderCodeBytes);
				tc.brcLeft = new BorderCode(brcLeftBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				//border bottom
				unsigned char brcBottomBytes[borderCodeBytes];
				memcpy(brcBottomBytes, (bytes + pointer), borderCodeBytes);
				tc.brcBottom = new BorderCode(brcBottomBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				//border top
				unsigned char brcRightBytes[borderCodeBytes];
				memcpy(brcRightBytes, (bytes + pointer), borderCodeBytes);
				tc.brcRight = new BorderCode(brcRightBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				rgTc80.push_back(tc);
			}
		}

	private: 

		unsigned char numberOfColumns;
		
		/// An array of 16-bit signed integer that specifies horizontal distance in twips.
		/// MUST be greater than or equal to -31680 and less than or equal to 31680.
		vector<short> rgdxaCenter;
		
		/// An array of TC80 that specifies the default formatting for a cell in the table.
		/// Each TC80 in the array corresponds to the equivalent column in the table.
		/// If there are fewer TC80s than columns, the remaining columns are formatted with the default TC80 formatting.
		/// If there are more TC80s than columns, the excess TC80s MUST be ignored.
		vector<TC80> rgTc80;
	};
}