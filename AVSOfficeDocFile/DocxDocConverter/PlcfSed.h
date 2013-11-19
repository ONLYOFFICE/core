#pragma once

#include "PLC.h"

namespace AVSDocFileFormat
{
  class Sed
  {
    private:
	  static const byte SIZE_IN_BYTES = 12;
	  byte bytes[SIZE_IN_BYTES];

    public:
	  Sed()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit Sed( int _fcSepx )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( ( this->bytes + sizeof(short) ), _fcSepx );
	  }

	  Sed( const Sed& _sed )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _sed.bytes, SIZE_IN_BYTES );
	  }

	  int GetFcSepx() const
	  {
	    return FormatUtils::BytesToInt32( (byte*)(this->bytes), sizeof(short), SIZE_IN_BYTES );
	  }
  };

  typedef PLC<Sed> PlcfSed;
}