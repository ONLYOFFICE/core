#pragma once

#include "Constants.h"

namespace ASCDocFileFormat
{
  union Fatl
  {
    private:
	  struct
      {
        short fatlBorders:1;
	    short fatlShading:1;
	    short fatlFont:1;
	    short fatlColor:1;
	    short fatlBestFit:1;
	    short fatlHdrRows:1;
	    short fatlLastRow:1;
	    short fatlHdrCols:1;
	    short fatlLastCol:1;
	    short fatlNoRowBands:1;
	    short fatlNoColBands:1;
	    short padding:5;
      } FatlStruct;
	  short FatlShort;

    public:
	  Fatl():
	  FatlShort(0)
	  {
	  }

	  explicit Fatl( bool _fatlBorders, bool _fatlShading, bool _fatlFont, bool _fatlColor,
		             bool _fatlBestFit, bool _fatlHdrRows, bool _fatlLastRow, bool _fatlHdrCols, 
					 bool _fatlLastCol, bool _fatlNoRowBands, bool _fatlNoColBands ): 
	  FatlShort(0)
	  {
	    ( _fatlBorders ) ? ( this->FatlStruct.fatlBorders = 1 ) : ( this->FatlStruct.fatlBorders = 0 );
	    ( _fatlShading ) ? ( this->FatlStruct.fatlShading = 1 ) : ( this->FatlStruct.fatlShading = 0 );
	    ( _fatlFont ) ? ( this->FatlStruct.fatlFont = 1 ) : ( this->FatlStruct.fatlFont = 0 );
	    ( _fatlColor ) ? ( this->FatlStruct.fatlColor = 1 ) : ( this->FatlStruct.fatlColor = 0 );
	    ( _fatlBestFit ) ? ( this->FatlStruct.fatlBestFit = 1 ) : ( this->FatlStruct.fatlBestFit = 0 );
	    ( _fatlHdrRows ) ? ( this->FatlStruct.fatlHdrRows = 1 ) : ( this->FatlStruct.fatlHdrRows = 0 );
	    ( _fatlLastRow ) ? ( this->FatlStruct.fatlLastRow = 1 ) : ( this->FatlStruct.fatlLastRow = 0 );
	    ( _fatlHdrCols ) ? ( this->FatlStruct.fatlHdrCols = 1 ) : ( this->FatlStruct.fatlHdrCols = 0 );
	    ( _fatlLastCol ) ? ( this->FatlStruct.fatlLastCol = 1 ) : ( this->FatlStruct.fatlLastCol = 0 );
	    ( _fatlNoRowBands ) ? ( this->FatlStruct.fatlNoRowBands = 1 ) : ( this->FatlStruct.fatlNoRowBands = 0 );
	    ( _fatlNoColBands ) ? ( this->FatlStruct.fatlNoColBands = 1 ) : ( this->FatlStruct.fatlNoColBands = 0 );
	  }
 
	  explicit Fatl( short _fatl ):
	  FatlShort(_fatl)
	  {
	  }

	  operator short() const
	  {
	    return this->FatlShort;
	  }
  };
	
  class TLP: public IOperand
  {
    private:
	  Constants::Itl itl;
	  Fatl grfatl;

	  static const unsigned char SIZE_IN_BYTES = 4;

	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  TLP(): 
	  itl(Constants::itlNil), grfatl()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, &(this->itl), 2 );
		short fatlValue = this->grfatl;
		memcpy( ( this->bytes + 2 ), &fatlValue, sizeof(fatlValue) );
	  }

	  explicit TLP( Constants::Itl _itl, Fatl _grfatl ):
	  itl(_itl), grfatl(_grfatl)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, &(this->itl), 2 );
		short fatlValue = this->grfatl;
		memcpy( ( this->bytes + 2 ), &fatlValue, sizeof(fatlValue) );
	  }

	  virtual ~TLP()
	  {
	  }

	  virtual operator unsigned char*() const
	  {
	    return (unsigned char*)(this->bytes);
	  }

	  virtual operator const unsigned char*() const
	  {
	    return (const unsigned char*)this->bytes;
	  }

	  virtual unsigned int Size() const
	  {
	    return SIZE_IN_BYTES;
	  }
  };
}