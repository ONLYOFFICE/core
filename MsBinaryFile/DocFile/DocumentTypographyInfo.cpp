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

			unsigned char fpunctBytes[202];
			memcpy( fpunctBytes, ( bytes + 6 ), 202 );
			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(this->rgxchFPunct), fpunctBytes, 202, ENCODING_UTF16 );

			unsigned char lpunctBytes[102];
			memcpy( lpunctBytes, ( bytes + 208 ), 102 );
			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(this->rgxchLPunct), lpunctBytes, 102, ENCODING_UTF16 );
		}
		else
		{
			//throw new ByteParseException("Cannot parse the struct DOPTYPOGRAPHY, the length of the struct doesn't match");
		}
	}
}
