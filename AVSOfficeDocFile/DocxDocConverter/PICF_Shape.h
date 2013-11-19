#pragma once

namespace AVSDocFileFormat
{
  class PICF_Shape: public IOperand
  {
    private:
	  static const byte SIZE_IN_BYTES = 14;
	  byte bytes[SIZE_IN_BYTES];

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

	  virtual operator byte*() const
	  {
	    return (byte*)(this->bytes);
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