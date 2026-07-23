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
