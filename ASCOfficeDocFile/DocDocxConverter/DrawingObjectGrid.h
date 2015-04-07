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