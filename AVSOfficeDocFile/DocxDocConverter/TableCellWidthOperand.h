#pragma once

#include "ItcFirstLim.h"
#include "FtsWWidth.h"

namespace AVSDocFileFormat
{
  class TableCellWidthOperand: public IOperand
  {
    private:
	  static const byte SIZE_IN_BYTES = 6;
	  byte bytes[SIZE_IN_BYTES];

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

		FormatUtils::SetBytes( ( this->bytes + sizeof(byte) ), (short)_itc );
		memcpy( ( this->bytes + sizeof(byte) + sizeof(short) ), (byte*)_FtsWWidth, _FtsWWidth.Size() );
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