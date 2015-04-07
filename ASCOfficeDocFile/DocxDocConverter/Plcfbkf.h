#pragma once

#include "PLC.h"

namespace ASCDocFileFormat
{
  union BKC
  {
    private:
	  struct
	  {
	    unsigned short itcFirst:7;
		unsigned short fPub:1;
		unsigned short itcLim:6;
		unsigned short fNative:1;
		unsigned short fCol:1;
	  } BKCStruct;
	  unsigned short BKCUnsignedShort;

    public:
	  explicit BKC( unsigned short _bkc = 0 ):
	  BKCUnsignedShort(_bkc)
	  {
	  }

	  explicit BKC( unsigned char _itcFirst, unsigned char _itcLim, bool _fNative, bool _fCol ):
      BKCUnsignedShort(0)
	  {
	    this->BKCStruct.itcFirst = _itcFirst;
		this->BKCStruct.itcLim = _itcLim;

		( _fNative ) ? ( this->BKCStruct.fNative = 1 ) : ( this->BKCStruct.fNative = 0 );
		( _fCol ) ? ( this->BKCStruct.fCol = 1 ) : ( this->BKCStruct.fCol = 0 );
	  }

	  operator unsigned short () const
	  {
	    return this->BKCUnsignedShort;
	  }
  };
	
  class FBKF
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 4;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  FBKF()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit FBKF( unsigned short _ibkl, BKC _bkc )
	  {
	    FormatUtils::SetBytes( this->bytes, _ibkl );
		FormatUtils::SetBytes( ( this->bytes + sizeof(_ibkl) ), (unsigned short)_bkc );
	  }

      FBKF( const FBKF& _fBKF )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _fBKF.bytes, SIZE_IN_BYTES );
	  }

      FBKF& operator = ( const FBKF& _fBKF )
	  {
	    if ( this != &_fBKF )
		{
		  memset( this->bytes, 0, SIZE_IN_BYTES );

		  memcpy( this->bytes, _fBKF.bytes, SIZE_IN_BYTES );
		}

		return *this;
	  }

      unsigned short GetIndex() const
	  {
	    return FormatUtils::BytesToUInt16( (unsigned char*)(this->bytes), 0, SIZE_IN_BYTES );
	  }

	  BKC GetBKC() const
	  {
	    return BKC( FormatUtils::BytesToUInt16( (unsigned char*)(this->bytes), 2, SIZE_IN_BYTES ) );
	  }
  };

  typedef PLC<FBKF> Plcfbkf;
  typedef PLC<> Plcfbkl;
}