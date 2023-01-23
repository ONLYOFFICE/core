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
