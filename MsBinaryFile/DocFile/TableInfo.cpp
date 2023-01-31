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

#include "TableInfo.h"

namespace DocFileFormat
{
	TableInfo::TableInfo( ParagraphPropertyExceptions* papx, int  nWordVersion):
			fInTable(false), fTtp(false), fInnerTtp(false), fInnerTableCell(false), iTap(0)
	{
		if ( papx != NULL )
		{
			for ( std::list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != papx->grpprl->end(); iter++ )
			{
				switch(iter->OpCode)
				{
					case sprmOldPFInTable:
					case sprmPFInTable:
					{
						fInTable = ( iter->Arguments[0] == 1 ) ? (true) : (false);

						if (iTap < 1 && fInTable) iTap = 1;
					}break;

					//case sprmOldPFTtp:
					case sprmOldPTtp:
					case sprmPFTtp:
					{
						fTtp = ( iter->Arguments[0] == 1 ) ? (true) : (false);
					}break;

					//case sprmOldPFInnerTableCell:
					case sprmPFInnerTableCell:
					{
						fInnerTableCell = ( iter->Arguments[0] == 1 ) ? (true) : (false);
					}break;

					//case sprmOldPFInnerTtp:
					case sprmPFInnerTtp:
					{
						fInnerTtp = ( iter->Arguments[0] == 1 ) ? (true) : (false);
					}break;

					//case sprmOldPItap:
					case sprmPItap:
					{
						iTap = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );

						if ( iTap > 0 )
						{
							fInTable = true;
						}
					}break;
				}

				if ( (int)( iter->OpCode ) == sprmTCnf )//66a
				{
					iTap = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );

					if ( iTap > 0 )
					{
						fInTable = true;
					}
				}
			}
		}
	}
}
