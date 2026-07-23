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

#include "ParagraphHeight.h"
#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{
	ParagraphHeight::ParagraphHeight()
    {
      //set default values
      setDefaultValues();
    }

    /*========================================================================================================*/

	ParagraphHeight::ParagraphHeight( unsigned char* bytes, int size, bool fTtpMode )
    {
	//set default values
		setDefaultValues();

	// The ParagraphHeight is placed in a ParagraphProperties whose fTtp field is set, 
	//so used another bit setting
		if ( size == 12 )
		{
			unsigned short nFlag = FormatUtils::BytesToInt16( bytes, 0, size );
			if ( fTtpMode )
			{
				fSpare			= FormatUtils::BitmaskToBool( nFlag, 0x0001 );
				fUnk			= FormatUtils::BitmaskToBool( nFlag, 0x0002 );
				dcpTtpNext		= nFlag;
				dxaCol			= FormatUtils::BytesToInt32( bytes, 4, size );
				dymTableHeight	= FormatUtils::BytesToInt32( bytes, 8, size );
			}
			else
			{
				fVolatile	= FormatUtils::BitmaskToBool( nFlag, 0x0001 );
				fUnk		= FormatUtils::BitmaskToBool( nFlag, 0x0002 );
				fDiffLines	= FormatUtils::BitmaskToBool( nFlag, 0x0004 );
				clMac		= nFlag& 0x00FF;

				dxaCol		= FormatUtils::BytesToInt32( bytes, 4, size );
				dymLine		= FormatUtils::BytesToInt32( bytes, 8, size );
				dymHeight	= dymLine;
			}
		}
		else if (size == 6)
		{
			unsigned short nFlag = FormatUtils::BytesToInt16( bytes, 0, size );
			
			fVolatile	= FormatUtils::BitmaskToBool( nFlag, 0x0001 );
			fUnk		= FormatUtils::BitmaskToBool( nFlag, 0x0002 );
			fDiffLines	= FormatUtils::BitmaskToBool( nFlag, 0x0004 );
			clMac		= nFlag & 0x000F;

			dxaCol		= FormatUtils::BytesToInt16( bytes, 2, size );
			dymLine		= FormatUtils::BytesToInt16( bytes, 4, size );
			dymHeight	= dymLine;
		}
	}

	/*========================================================================================================*/

	void ParagraphHeight::setDefaultValues()
    {
      clMac				= 0;
      dcpTtpNext		= 0;
      dxaCol			= 0;
      dymHeight			= 0;
      dymLine			= 0;
      dymTableHeight	= 0;
      fDiffLines		= false;
      fSpare			= false;
      fUnk				= false;
      fVolatile			= false;
    }
}