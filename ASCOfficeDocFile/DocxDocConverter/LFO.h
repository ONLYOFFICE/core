#pragma once

#include "Constants.h"
#include "grfhic.h"

namespace ASCDocFileFormat
{
  class LFO: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 16;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  LFO()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  explicit LFO( int _lsid, unsigned char _clfolvl, Constants::LFOField _ibstFltAutoNum, grfhic _grfhic )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( this->bytes, _lsid );

		this->bytes[sizeof(_lsid) + sizeof(int) + sizeof(int)] = _clfolvl;
        this->bytes[sizeof(_lsid) + sizeof(int) + sizeof(int) + sizeof(_clfolvl)] = (unsigned char)_ibstFltAutoNum;
        this->bytes[sizeof(_lsid) + sizeof(int) + sizeof(int) + sizeof(_clfolvl) + sizeof(unsigned char)] = (unsigned char)_grfhic;
	  }

	  LFO( const LFO& _lFO )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _lFO.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~LFO()
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