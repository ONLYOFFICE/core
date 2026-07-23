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

#include "DrawingObjectGrid.h"
#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{  
	  DrawingObjectGrid::~DrawingObjectGrid()
	  {
	  }

	  DrawingObjectGrid::DrawingObjectGrid():
      xaGrid(0), yaGrid(0), dxaGrid(0), dyaGrid(0), dyGridDisplay(0), fTurnItOff(false), dxGridDisplay(0),
      fFollowMargins(false)
	  {
	  }
      
	  /// Parses the bytes to retrieve a DrawingObjectGrid
	  DrawingObjectGrid::DrawingObjectGrid( unsigned char* bytes, int size ):
      xaGrid(0), yaGrid(0), dxaGrid(0), dyaGrid(0), dyGridDisplay(0), fTurnItOff(false), dxGridDisplay(0),
      fFollowMargins(false)
      {
        if ( size == 10 )
        {
          this->xaGrid = FormatUtils::BytesToInt16( bytes, 0, size );
          this->yaGrid = FormatUtils::BytesToInt16( bytes, 2, size );
          this->dxaGrid = FormatUtils::BytesToInt16( bytes, 4, size );
          this->dyaGrid = FormatUtils::BytesToInt16( bytes, 6, size );

          //split unsigned char 8 and 9 into bits
          this->dyGridDisplay = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 8 ), 2, 0, 7 );
          this->fTurnItOff = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 7 );
          this->dxGridDisplay = (short)FormatUtils::GetUIntFromBytesBits( ( bytes + 8 ), 2, 8, 7 );
          this->fFollowMargins = FormatUtils::GetBitFromBytes( ( bytes + 8 ), 2, 15 );
        }
        else
        {
          //throw new ByteParseException("Cannot parse the struct DOGRID, the length of the struct doesn't match");
        }
      }  
}
