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