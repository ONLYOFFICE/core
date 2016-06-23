/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

      if ( size == 12 )
      {
        // The ParagraphHeight is placed in a ParagraphProperties whose fTtp field is set, 
        //so used another bit setting
        if ( fTtpMode )
        {
		  this->fSpare = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
		  this->fUnk = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
		  this->dcpTtpNext = FormatUtils::BytesToInt16( bytes, 0, size );
		  this->dxaCol = FormatUtils::BytesToInt32( bytes, 4, size );
		  this->dymTableHeight = FormatUtils::BytesToInt32( bytes, 8, size );
        }
        else
        {
		  this->fVolatile = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0001 );
          this->fUnk = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0002 );
          this->fDiffLines = FormatUtils::BitmaskToBool( FormatUtils::BytesToInt16( bytes, 0, size ), 0x0004 );
          this->clMac = FormatUtils::BytesToUInt16( bytes, 0, size ) & 0x00FF;

          this->dxaCol = FormatUtils::BytesToInt32( bytes, 4, size );
          this->dymLine = FormatUtils::BytesToInt32( bytes, 8, size );
          this->dymHeight = FormatUtils::BytesToInt32( bytes, 8, size );
        }
      }
      else
      {
        //throw new ByteParseException("Cannot parse the struct ParagraphHeight, the length of the struct doesn't match");
      }
    }

	/*========================================================================================================*/

	void ParagraphHeight::setDefaultValues()
    {
      this->clMac = 0;
      this->dcpTtpNext = 0;
      this->dxaCol = 0;
      this->dymHeight = 0;
      this->dymLine = 0;
      this->dymTableHeight = 0;
      this->fDiffLines = false;
      this->fSpare = false;
      this->fUnk = false;
      this->fVolatile = false;
    }
}