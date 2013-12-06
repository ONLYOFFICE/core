#pragma once

#include "ItcFirstLim.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
  class CellRangeVertAlign: public IOperand
  {
    private:
	  static const byte SIZE_IN_BYTES = 4;
	  byte bytes[SIZE_IN_BYTES];

    public:
	  CellRangeVertAlign()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		this->bytes[0] = ( SIZE_IN_BYTES - 1 );
	  }

	  explicit CellRangeVertAlign( const ItcFirstLim& _itc, Constants::VerticalAlign _valign )
	  {
	  	memset( this->bytes, 0, SIZE_IN_BYTES );

		this->bytes[0] = ( SIZE_IN_BYTES - 1 );

		FormatUtils::SetBytes( ( this->bytes + sizeof(byte) ), (short)_itc );
		this->bytes[sizeof(byte) + sizeof(short)] = (byte)_valign;
	  }

      CellRangeVertAlign( const CellRangeVertAlign& _cellRangeVertAlign )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _cellRangeVertAlign.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~CellRangeVertAlign()
	  {
	  }

	  virtual unsigned int Size() const
	  {
	    return (unsigned int)sizeof(this->bytes);  
	  }

      virtual operator byte*() const
	  {
	    return (byte*)(this->bytes);
	  }

	  virtual operator const byte*() const
	  {
	    return (const byte*)(this->bytes);
	  }
  };
}