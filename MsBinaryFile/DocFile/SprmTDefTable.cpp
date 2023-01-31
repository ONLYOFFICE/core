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

#include "SprmTDefTable.h"

namespace DocFileFormat
{
	SprmTDefTable::SprmTDefTable (unsigned char* bytes, int size)
	{
		numberOfColumns = bytes[0];
		int pointer = 1;

		// rgdxaCenter
		for (int i = 0; i < numberOfColumns + 1; ++i)
		{
			int pos = FormatUtils::BytesToInt16(bytes, pointer, size);
			rgdxaCenter.push_back(pos);
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
					oTC80.wWidth		=	0;

					oTC80.brcTop = new BorderCode();
					oTC80.brcLeft = new BorderCode();
					oTC80.brcBottom = new BorderCode();
					oTC80.brcRight = new BorderCode();

					rgTc80.push_back(oTC80);
				}

				break;
			}

			TC80 tc;
			// the flags

			unsigned short flags = FormatUtils::BytesToUInt16(bytes, pointer, size);

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
			tc.wWidth =	FormatUtils::BytesToInt16(bytes, pointer, size);
			pointer +=	2;

			const int borderCodeBytes = 4;

			tc.brcTop = new BorderCode((bytes + pointer), borderCodeBytes);
			pointer += borderCodeBytes;

			tc.brcLeft = new BorderCode((bytes + pointer), borderCodeBytes);
			pointer += borderCodeBytes;

			tc.brcBottom = new BorderCode((bytes + pointer), borderCodeBytes);
			pointer += borderCodeBytes;

			tc.brcRight = new BorderCode((bytes + pointer), borderCodeBytes);
			pointer += borderCodeBytes;

			rgTc80.push_back(tc);
		}
	}
}
