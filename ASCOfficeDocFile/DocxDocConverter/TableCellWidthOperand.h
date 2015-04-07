#pragma once

#include "ItcFirstLim.h"
#include "FtsWWidth.h"

namespace ASCDocFileFormat
{
  class TableCellWidthOperand: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 6;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  TableCellWidthOperand()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		this->bytes[0] = ( SIZE_IN_BYTES - 1 );
	  }

	  explicit TableCellWidthOperand( const ItcFirstLim& _itc, const FtsWWidth_TablePart& _FtsWWidth )
	  {
	  	memset( this->bytes, 0, SIZE_IN_BYTES );

		this->bytes[0] = ( SIZE_IN_BYTES - 1 );

		FormatUtils::SetBytes( ( this->bytes + sizeof(unsigned char) ), (short)_itc );
		memcpy( ( this->bytes + sizeof(unsigned char) + sizeof(short) ), (unsigned char*)_FtsWWidth, _FtsWWidth.Size() );
	  }

      TableCellWidthOperand( const TableCellWidthOperand& _tableCellWidthOperand )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _tableCellWidthOperand.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~TableCellWidthOperand()
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