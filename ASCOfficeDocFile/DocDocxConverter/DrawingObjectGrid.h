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
#pragma once

namespace DocFileFormat
{
  class DrawingObjectGrid
  {
    friend class SettingsMapping;  
  
    private:  
	  /// x-coordinate of the upper left-hand corner of the grid
      short xaGrid;
      /// y-coordinate of the upper left-hand corner of the grid
      short yaGrid;
      /// Width of each grid square
      short dxaGrid;
      /// Height of each grid square
      short dyaGrid;
      /// The number of grid squares (in the y direction) between each 
      /// gridline drawn on the screen. 0 means don‘t display any 
      /// gridlines in the y direction.
      short dyGridDisplay;
      /// Suppress display of gridlines
      bool fTurnItOff;
      /// The number of grid squares (in the x direction) between each 
      /// gridline drawn on the screen. 0 means don‘t display any 
      /// gridlines in the y direction.
      short dxGridDisplay;
      /// If true, the grid will start at the left and top margins and 
      /// ignore xaGrid and yaGrid
      bool fFollowMargins;

    public:
	  virtual ~DrawingObjectGrid()
	  {
	  }

	  DrawingObjectGrid():
      xaGrid(0), yaGrid(0), dxaGrid(0), dyaGrid(0), dyGridDisplay(0), fTurnItOff(false), dxGridDisplay(0),
      fFollowMargins(false)
	  {
	  }
      
	  /// Parses the bytes to retrieve a DrawingObjectGrid
      DrawingObjectGrid( unsigned char* bytes, int size ):
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
  };
}