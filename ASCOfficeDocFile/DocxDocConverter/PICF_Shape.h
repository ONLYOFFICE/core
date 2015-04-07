#pragma once

#include "../Common/IOperand.h"

namespace ASCDocFileFormat
{
  class PICF_Shape: public IOperand
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 14;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  PICF_Shape()
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );
	  }

	  PICF_Shape( const PICF_Shape& _pICF_Shape )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _pICF_Shape.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~PICF_Shape()
	  {
	  }

	  virtual operator unsigned char*() const
	  {
	    return (unsigned char*)(this->bytes);
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