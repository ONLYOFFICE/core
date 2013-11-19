#pragma once

#include "BorderCode.h"
#include "Global.h"

namespace DocFileFormat
{
	typedef struct _TC80
	{
		/// A value from the following table that specifies how this cell merges horizontally with the neighboring cells in the same row.
		/// MUST be one of the following values:
		/// 0 - The cell is not merged with the cells on either side of it.
		/// 1 - The cell is one of a set of horizontally merged cells. It contributes its layout region to the set and its own contents are not rendered.
		/// 2, 3 - The cell is the first cell in a set of horizontally merged cells. The contents and formatting of this cell extend into any consecutive cells following it that are designated as part of the merged set.
		byte horzMerge;
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
		BorderCode brcTop;
		BorderCode brcLeft;
		BorderCode brcBottom;
		BorderCode brcRight;
	} TC80;

	class SprmTDefTable
	{
	private: 
		friend class TableCellPropertiesMapping;
	public:
		SprmTDefTable (byte* bytes, int size)
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
				TC80 tc;

				if (size <= pointer)
				{
					TC80 oTC80;
					
					memset (&oTC80, 0, sizeof(TC80));
					
					oTC80.ftsWidth		=	Global::dxa;
					oTC80.wWidth		=	3190;

					oTC80.brcTop		=	BorderCode();
					oTC80.brcLeft		=	BorderCode();
					oTC80.brcBottom		=	BorderCode();
					oTC80.brcRight		=	BorderCode();

					for (int j = i; j < numberOfColumns; ++j)
						rgTc80.push_back(oTC80);

					// static int m = 0; ATLTRACE(L"PUSH: %d\n", m); ++m;

					break;
				}	

				// the flags
				
				unsigned short flags	=	FormatUtils::BytesToUInt16(bytes, pointer, size);
				
				tc.horzMerge			=	(byte)FormatUtils::BitmaskToInt((int)flags, 0x3);
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
				byte brcTopBytes[borderCodeBytes];
				memcpy(brcTopBytes, (bytes + pointer), borderCodeBytes);
				tc.brcTop = BorderCode(brcTopBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				//border left
				byte brcLeftBytes[borderCodeBytes];
				memcpy(brcLeftBytes, (bytes + pointer), borderCodeBytes);
				tc.brcLeft = BorderCode(brcLeftBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				//border bottom
				byte brcBottomBytes[borderCodeBytes];
				memcpy(brcBottomBytes, (bytes + pointer), borderCodeBytes);
				tc.brcBottom = BorderCode(brcBottomBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				//border top
				byte brcRightBytes[borderCodeBytes];
				memcpy(brcRightBytes, (bytes + pointer), borderCodeBytes);
				tc.brcRight = BorderCode(brcRightBytes, borderCodeBytes);
				pointer += borderCodeBytes;

				rgTc80.push_back(tc);
			}
		}

	private: 

		byte numberOfColumns;
		
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