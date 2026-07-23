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

#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{
	class DocumentTypographyInfo
	{
		friend class SettingsMapping;  

	private:
		/// True if we're kerning punctation
		bool fKerningPunct;
		/// Kinsoku method of justification:
		/// 0 = always expand
		/// 1 = compress punctation
		/// 2 = compress punctation and kana
		short iJustification;
		/// Level of kinsoku:
		/// 0 = level 1
		/// 1 = Level 2
		/// 2 = Custom
		short iLevelOfKinsoku;
		/// "2 page on 1" feature is turned on
		bool f2on1;
		/// Old East Asian feature
		bool fOldDefineLineBaseOnGrid;
		/// Custom Kinsoku
		short iCustomKsu;
		/// When set to true, use strict (level 2) Kinsoku rules
		bool fJapaneseUseLevel2;
		/// Length of rgxchFPunct
		short cchFollowingPunct;
		/// Length of rgxchLPunct
		short cchLeadingPunct;
		/// Array of characters that should never appear at the start of a line
		std::wstring rgxchFPunct;
		/// Array of characters that should never appear at the end of a line
		std::wstring rgxchLPunct;

	public:
		virtual ~DocumentTypographyInfo();
		DocumentTypographyInfo();

		/// Parses the bytes to retrieve a DocumentTypographyInfo
		DocumentTypographyInfo( unsigned char* bytes, int size );
	};
}
