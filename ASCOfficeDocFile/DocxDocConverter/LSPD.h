#pragma once

namespace ASCDocFileFormat
{
  class LSPD: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 4;

	  short dyaLine;
	  short fMultLinespace;

	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  LSPD():
      dyaLine(0), fMultLinespace(0)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit LSPD( short _dyaLine, bool _fMultLinespace = true ):
      dyaLine(0), fMultLinespace(0)
	  {
        memset( this->bytes, 0, SIZE_IN_BYTES );

		this->dyaLine = _dyaLine;
		( _fMultLinespace ) ? ( this->fMultLinespace = 0x0001 ) : ( this->fMultLinespace = 0x0000 );

		FormatUtils::SetBytes( this->bytes, this->dyaLine );
		FormatUtils::SetBytes( ( this->bytes + sizeof(this->dyaLine) ), this->fMultLinespace );
	  }

	  LSPD( const LSPD& _lspd ):
	  dyaLine(_lspd.dyaLine), fMultLinespace(_lspd.fMultLinespace)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _lspd.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~LSPD()
	  {
	  }

	  virtual operator unsigned char*() const
	  {
	    return (unsigned char*)this->bytes;
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