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

#include "AutoSummaryInfo.h"
#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{ 
	AutoSummaryInfo::~AutoSummaryInfo()
	{
	}
	AutoSummaryInfo::AutoSummaryInfo():
		fValid(false), fView(false), iViewBy(0), fUpdateProps(false), wDlgLevel(0),
		lHighestLevel(0), lCurrentLevel(0)
	{
	}

	/// Parses the bytes to retrieve a AutoSummaryInfo
	AutoSummaryInfo::AutoSummaryInfo( unsigned char* bytes, int size ):
		fValid(false), fView(false), iViewBy(0), fUpdateProps(false), wDlgLevel(0),
		lHighestLevel(0), lCurrentLevel(0)
	{
		if ( size == 12 )
		{
			//split unsigned char 0 and 1 into bits
			this->fValid = FormatUtils::GetBitFromBytes( bytes, size, 0 );
			this->fView = FormatUtils::GetBitFromBytes( bytes, size, 1 );
			this->iViewBy = (short)FormatUtils::GetUIntFromBytesBits( bytes, size, 2, 2 );
			this->fUpdateProps = FormatUtils::GetBitFromBytes( bytes, size, 4 );

			this->wDlgLevel = FormatUtils::BytesToInt16( bytes, 2, size );
			this->lHighestLevel = FormatUtils::BytesToInt32( bytes, 4, size );
			this->lCurrentLevel = FormatUtils::BytesToInt32( bytes, 8, size );
		}
		else
		{
			//throw new ByteParseException("Cannot parse the struct ASUMYI, the length of the struct doesn't match");
		}
	}
}
