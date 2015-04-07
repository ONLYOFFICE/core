#pragma once

#include "COLORREF.h"

namespace ASCDocFileFormat
{
  class Brc: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 8;

      unsigned char bytes[SIZE_IN_BYTES];

    public:
	  Brc()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit Brc( const COLORREF& _cv, unsigned char _dptLineWidth, unsigned char _brcType, unsigned char _dptSpace, bool fShadow, bool fFrame )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		unsigned char cvSize = _cv.Size();

		memcpy( this->bytes, (unsigned char*)_cv, cvSize );

		unsigned char dptLineWidth = 2;

		if ( _dptLineWidth > 2 )
		{
		  dptLineWidth = _dptLineWidth; 
		}

		this->bytes[cvSize] = dptLineWidth;
		this->bytes[cvSize + sizeof(dptLineWidth)] = _brcType;
		
		unsigned short dptSpaceAndFlags = 0;
        dptSpaceAndFlags |= _dptSpace; //Set dptSpace
        
		if ( fShadow )
		{
		  dptSpaceAndFlags |= 0x20; //Set fShadow
		}

		if ( fFrame )
		{
		  dptSpaceAndFlags |= 0x40; //Set fFrame 
		}

		FormatUtils::SetBytes( ( this->bytes + cvSize + sizeof(dptLineWidth) + sizeof(_brcType) ), dptSpaceAndFlags );
	  }

	  explicit Brc( const unsigned char* _bytes )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
		  
		if ( _bytes != NULL )
		{
		  memcpy( this->bytes, _bytes, SIZE_IN_BYTES );
		}
	  }

	  Brc( const Brc& _brc )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _brc.bytes, SIZE_IN_BYTES );
	  }

	  Brc& operator = ( const Brc& _brc )
	  {
	    if ( this != &_brc )
		{
		  memset( this->bytes, 0, SIZE_IN_BYTES );

		  memcpy( this->bytes, _brc.bytes, SIZE_IN_BYTES );  
		}

		return *this;
	  }

	  virtual ~Brc()
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
	    return sizeof(this->bytes);
	  }
  };

  class BrcOperand: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 9;

      unsigned char bytes[SIZE_IN_BYTES];

    public:
	  BrcOperand()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		this->bytes[0] = ( SIZE_IN_BYTES - 1 );
	  }

	  explicit BrcOperand( const Brc& _brc )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		this->bytes[0] = ( SIZE_IN_BYTES - 1 );

		memcpy( ( this->bytes + sizeof(unsigned char) ), (unsigned char*)_brc, _brc.Size() );
	  }

	  BrcOperand( const BrcOperand& _brcOperand )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _brcOperand.bytes, SIZE_IN_BYTES );
	  }

	  BrcOperand& operator = ( const BrcOperand& _brcOperand )
	  {
	    if ( this != &_brcOperand )
		{
		  memset( this->bytes, 0, SIZE_IN_BYTES );

		  memcpy( this->bytes, _brcOperand.bytes, SIZE_IN_BYTES );  
		}

		return *this;
	  }

	  virtual ~BrcOperand()
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
	    return sizeof(this->bytes);
	  }
  };
}