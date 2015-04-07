#pragma once

#include "COLORREF.h"

namespace ASCDocFileFormat
{
  class Shd: public IOperand
  {
    private:
	  COLORREF cvFore;
	  COLORREF cvBack;
	  //!!!TODO!!!
	  unsigned short ipat;

	  static const unsigned char SIZE_IN_BYTES = 10;

	  unsigned char bytes[SIZE_IN_BYTES];

    public:
	  Shd():
      cvFore(), cvBack(), ipat(0)
	  {
	    memcpy( this->bytes, this->cvFore, this->cvFore.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() ), (unsigned char*)this->cvBack, this->cvBack.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() + this->cvBack.Size() ), (unsigned char*)(&this->ipat), sizeof(this->ipat) );
	  }

	  explicit Shd( const COLORREF& _cvFore, const COLORREF& _cvBack, unsigned short _ipat ):
      cvFore(_cvFore), cvBack(_cvBack), ipat(_ipat)
	  {
 	    memcpy( this->bytes, this->cvFore, this->cvFore.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() ), (unsigned char*)this->cvBack, this->cvBack.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() + this->cvBack.Size() ), (unsigned char*)(&this->ipat), sizeof(this->ipat) );
	  }

	  Shd( const Shd& _shd ):
	  cvFore(_shd.cvFore), cvBack(_shd.cvBack), ipat(_shd.ipat)
	  {
	    memcpy( this->bytes, this->cvFore, this->cvFore.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() ), (unsigned char*)this->cvBack, this->cvBack.Size() );
		memcpy( ( this->bytes + this->cvFore.Size() + this->cvBack.Size() ), (unsigned char*)(&this->ipat), sizeof(this->ipat) );
	  }

	  virtual ~Shd()
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

  class SHDOperand: public IOperand
  {
    private:
	  static const unsigned char cb = 10;
	  Shd shd;

	  unsigned char bytes[SHDOperand::cb + 1];

    public:
	  SHDOperand():
	  shd()
	  {
	    this->bytes[0] = SHDOperand::cb;
		memcpy( ( this->bytes + sizeof(SHDOperand::cb) ), (unsigned char*)this->shd, this->shd.Size() );
	  }
 
	  explicit SHDOperand( const Shd& _shd ):
      shd(_shd)
	  {
	    this->bytes[0] = SHDOperand::cb;
		memcpy( ( this->bytes + sizeof(SHDOperand::cb) ), (unsigned char*)this->shd, this->shd.Size() );
	  }

	  SHDOperand( const SHDOperand& _shdOperand ):
	  shd(_shdOperand.shd)
	  {
	    this->bytes[0] = SHDOperand::cb;
		memcpy( ( this->bytes + sizeof(SHDOperand::cb) ), (unsigned char*)this->shd, this->shd.Size() );
	  }

	  virtual ~SHDOperand()
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