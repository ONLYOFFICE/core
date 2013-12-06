#pragma once

namespace ASCDocFileFormat
{
  class CSymbolOperand: public IOperand
  {
    private:
	  static const byte SIZE_IN_BYTES = 4;
	  byte bytes[SIZE_IN_BYTES];
		
	  unsigned short ftc;
	  WCHAR xchar;

    public:
	  CSymbolOperand():
      ftc(0), xchar(0)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit CSymbolOperand( unsigned short _ftc, WCHAR _xchar ):
      ftc(_ftc), xchar(_xchar)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( this->bytes, this->ftc );
		FormatUtils::SetBytes( ( this->bytes + sizeof(this->ftc) ), (unsigned short)this->xchar );
	  }

	  CSymbolOperand( const CSymbolOperand& _cSymbolOperand ):
	  ftc(_cSymbolOperand.ftc), xchar(_cSymbolOperand.xchar)
	  {
	    memset( this->bytes, 0, sizeof(this->bytes) );

		memcpy( this->bytes, _cSymbolOperand.bytes, sizeof(_cSymbolOperand.bytes) );
	  }

	  virtual ~CSymbolOperand()
	  {
	  }

	  virtual operator byte*() const
	  {
	    return (byte*)this->bytes;
	  }

	  virtual operator const byte*() const
	  {
	    return (const byte*)this->bytes;
	  }

	  virtual unsigned int Size() const
	  {
	    return sizeof(this->bytes);
	  }
  };
}