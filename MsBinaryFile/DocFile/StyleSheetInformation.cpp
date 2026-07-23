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

#include "StyleSheetInformation.h"

namespace DocFileFormat
{
	/// Parses the bytes to retrieve a StyleSheetInformation
	StyleSheetInformation::StyleSheetInformation (unsigned char* bytes, int size) : 
		cstd(0), cbSTDBaseInFile(0), fStdStylenamesWritten(false), stiMaxWhenSaved(0), istdMaxFixedWhenSaved(0), nVerBuiltInNamesWhenSaved(0), cbLSD(0)
	{
		if ((0 != size) || (NULL != bytes))
		{
			cstd								=	FormatUtils::BytesToUInt16 ( bytes, 0, size );
			cbSTDBaseInFile						=	FormatUtils::BytesToUInt16 ( bytes, 2, size );

			if ( bytes[4] == 1 )
			{
				fStdStylenamesWritten			=	true;
			}

			//unsigned char 5 is spare
			this->stiMaxWhenSaved				=	FormatUtils::BytesToUInt16 ( bytes, 6, size );
			this->istdMaxFixedWhenSaved			=	FormatUtils::BytesToUInt16 ( bytes, 8, size );
			this->nVerBuiltInNamesWhenSaved		=	FormatUtils::BytesToUInt16 ( bytes, 10, size );

			this->rgftcStandardChpStsh			=	std::vector<unsigned short>( 4 );

			this->rgftcStandardChpStsh[0]		=	FormatUtils::BytesToUInt16 ( bytes, 12, size );
			this->rgftcStandardChpStsh[1]		=	FormatUtils::BytesToUInt16 ( bytes, 14, size );
			this->rgftcStandardChpStsh[2]		=	FormatUtils::BytesToUInt16 ( bytes, 16, size );

			if ( size > 18 )
				rgftcStandardChpStsh[3]			=	FormatUtils::BytesToUInt16 ( bytes, 18, size );


			// not all stylesheet contain latent styles
			if ( size > 20 )
			{
				cbLSD =	FormatUtils::BytesToUInt16 ( bytes, 20, size );

				for ( int i = 0; i < stiMaxWhenSaved; ++i)
				{
					_UINT32 iLSD = FormatUtils::BytesToUInt32 ( bytes, ( 22 + (i * cbLSD) ), size );
					
					LSD lsd;
					lsd.fLocked = GETBIT(iLSD, 0);
					lsd.fSemiHidden = GETBIT(iLSD, 1);
					lsd.fUnhideWhenUsed = GETBIT(iLSD, 2);
					lsd.fQFormat = GETBIT(iLSD, 3);
					lsd.iPriority = GETBITS(iLSD, 4, 15);
					lsd.fReserved = GETBITS(iLSD, 16, 31);
					mpstilsd.push_back( lsd );
				}
			}
		}
	}
}