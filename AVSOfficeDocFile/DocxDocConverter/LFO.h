#pragma once

#include "Constants.h"
#include "grfhic.h"

namespace AVSDocFileFormat
{
  class LFO: public IOperand
  {
    private:
	  static const byte SIZE_IN_BYTES = 16;
	  byte bytes[SIZE_IN_BYTES];

    public:
	  LFO()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit LFO( int _lsid, byte _clfolvl, Constants::LFOField _ibstFltAutoNum, grfhic _grfhic )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( this->bytes, _lsid );

		this->bytes[sizeof(_lsid) + sizeof(int) + sizeof(int)] = _clfolvl;
        this->bytes[sizeof(_lsid) + sizeof(int) + sizeof(int) + sizeof(_clfolvl)] = (byte)_ibstFltAutoNum;
        this->bytes[sizeof(_lsid) + sizeof(int) + sizeof(int) + sizeof(_clfolvl) + sizeof(byte)] = (byte)_grfhic;
	  }

	  LFO( const LFO& _lFO )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _lFO.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~LFO()
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