#pragma once

#include "Shd80.h"

namespace ASCDocFileFormat
{
	class DefTableShd80Operand: public IOperand
	{
	private:
		byte* bytes;
		unsigned int sizeInBytes;

	public:
		DefTableShd80Operand():
		  bytes(0), sizeInBytes(1)
		  {
			  if ( this->sizeInBytes != 0 )
			  {
				  this->bytes = new byte[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );
				  }
			  }
		  }

		  explicit DefTableShd80Operand( const vector<Shd80>& _rgShd80 ):
		  bytes(NULL), sizeInBytes(0)
		  {
			  this->sizeInBytes = ( ( _rgShd80.size() * 2 ) + sizeof(byte) );

			  if ( this->sizeInBytes != 0 )
			  {
				  this->bytes = new byte[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  this->bytes[0] = ( this->sizeInBytes - sizeof(byte) );

					  unsigned int offset = sizeof(byte);

					  for ( vector<Shd80>::const_iterator iter = _rgShd80.begin(); iter != _rgShd80.end(); iter++ )
					  {
						  unsigned short value = (unsigned short)(*iter);

						  memcpy( ( this->bytes + offset ), &value, sizeof(unsigned short) );
						  offset += sizeof(unsigned short);
					  }
				  }
			  }
		  }

		  DefTableShd80Operand( const DefTableShd80Operand& _defTableShd80Operand ):
		  bytes(NULL), sizeInBytes(_defTableShd80Operand.sizeInBytes)
		  {
			  if ( this->sizeInBytes != 0 )
			  {
				  this->bytes = new byte[this->sizeInBytes];

				  if ( this->bytes != NULL )
				  {
					  memset( this->bytes, 0, this->sizeInBytes );

					  memcpy( this->bytes, _defTableShd80Operand.bytes, this->sizeInBytes );
				  }
			  }
		  }

		  virtual ~DefTableShd80Operand()
		  {
			  RELEASEARRAYOBJECTS (bytes);
		  }

		  virtual operator byte*() const
		  {
			  return this->bytes;
		  }

		  virtual operator const byte*() const
		  {
			  return (const byte*)this->bytes;
		  }

		  virtual unsigned int Size() const
		  {
			  return this->sizeInBytes;
		  }
	};


}