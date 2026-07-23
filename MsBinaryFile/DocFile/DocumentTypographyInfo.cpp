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

#include "DocumentTypographyInfo.h"

namespace DocFileFormat
{	
	DocumentTypographyInfo::~DocumentTypographyInfo()
	{
	}

	DocumentTypographyInfo::DocumentTypographyInfo():
	fKerningPunct(false), iJustification(0), iLevelOfKinsoku(0), f2on1(false), fOldDefineLineBaseOnGrid(false),
		iCustomKsu(0), fJapaneseUseLevel2(false), cchFollowingPunct(0), cchLeadingPunct(0)
	{
	}

	/// Parses the bytes to retrieve a DocumentTypographyInfo
	DocumentTypographyInfo::DocumentTypographyInfo( unsigned char* bytes, int size ):
	fKerningPunct(false), iJustification(0), iLevelOfKinsoku(0), f2on1(false), fOldDefineLineBaseOnGrid(false),
		iCustomKsu(0), fJapaneseUseLevel2(false), cchFollowingPunct(0), cchLeadingPunct(0)
	{
		if ( size == 310 )
		{
			//split unsigned char 0 and 1 into bits
			this->fKerningPunct = FormatUtils::GetBitFromBytes( bytes, size, 0 );
			this->iJustification = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 1, 2 );
			this->iLevelOfKinsoku = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 3, 2 );
			this->f2on1 = FormatUtils::GetBitFromBytes( bytes, size, 5 );
			this->fOldDefineLineBaseOnGrid = FormatUtils::GetBitFromBytes( bytes, size, 6 );
			this->iCustomKsu = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 7, 3 );
			this->fJapaneseUseLevel2 = FormatUtils::GetBitFromBytes( bytes, size, 10 );

			this->cchFollowingPunct = FormatUtils::BytesToInt16( bytes, 2, size );
			this->cchLeadingPunct = FormatUtils::BytesToInt16( bytes, 4, size );

			this->rgxchFPunct = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(bytes + 6), 202 / 2);
			this->rgxchLPunct = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(bytes + 208), 102 / 2);

		}
		else
		{
			//throw new ByteParseException("Cannot parse the struct DOPTYPOGRAPHY, the length of the struct doesn't match");
		}
	}
}
