#pragma once

#include "Constants.h"
#include "../Common/IOperand.h"
#include "../Common/FormatUtils.h"


namespace ASCDocFileFormat
{
  class MFPF: public IOperand
  {
    private:
	  Constants::PictureDataFormat mm;
      static const unsigned char SIZE_IN_BYTES = 8;
	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  MFPF():
	  mm(Constants::MM_SHAPE)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( this->bytes, (short)(this->mm) );
	  }

	  explicit MFPF( Constants::PictureDataFormat _mm ):
	  mm(_mm)
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		FormatUtils::SetBytes( this->bytes, (short)(this->mm) );
	  }

	  MFPF( const MFPF& _mFPF )
	  {
	    memset( this->bytes, 0, SIZE_IN_BYTES );

		memcpy( this->bytes, _mFPF.bytes, SIZE_IN_BYTES );
	  }

	  virtual ~MFPF()
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

	  operator Constants::PictureDataFormat() const
	  {
	    return this->mm;
	  }
  };
}