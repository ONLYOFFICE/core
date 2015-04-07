#pragma once

#include "ItcFirstLim.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
  class CellRangeVertAlign: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 4;
	  unsigned char bytes[SIZE_IN_BYTES];

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

		FormatUtils::SetBytes( ( this->bytes + sizeof(unsigned char) ), (short)_itc );
		this->bytes[sizeof(unsigned char) + sizeof(short)] = (unsigned char)_valign;
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

      virtual operator unsigned char*() const
	  {
	    return (unsigned char*)(this->bytes);
	  }

	  virtual operator const unsigned char*() const
	  {
	    return (const unsigned char*)(this->bytes);
	  }
  };
}