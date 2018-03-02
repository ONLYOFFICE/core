/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../Common/FormatUtils.h"

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
			if ( fTtpMode )
			{
				fSpare			= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
				fUnk			= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
				dcpTtpNext		= FormatUtils::BytesToInt16( bytes, 0, size );
				dxaCol			= FormatUtils::BytesToInt32( bytes, 4, size );
				dymTableHeight	= FormatUtils::BytesToInt32( bytes, 8, size );
			}
			else
			{
				fVolatile	= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
				fUnk		= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
				fDiffLines	= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0004 );
				clMac		= FormatUtils::BytesToUInt16( bytes, 0, size ) & 0x00FF;

				dxaCol		= FormatUtils::BytesToInt32( bytes, 4, size );
				dymLine		= FormatUtils::BytesToInt32( bytes, 8, size );
				dymHeight	= FormatUtils::BytesToInt32( bytes, 8, size );
			}
		}
		else if (size == 6)
		{
			fVolatile	= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
			fUnk		= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
			fDiffLines	= FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0004 );
			clMac		= FormatUtils::BytesToUChar( bytes, 0, size ) & 0x000F;

			dxaCol		= FormatUtils::BytesToInt16( bytes, 2, size );
			dymLine		= FormatUtils::BytesToInt16( bytes, 4, size );
			dymHeight	= FormatUtils::BytesToInt16( bytes, 4, size );
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