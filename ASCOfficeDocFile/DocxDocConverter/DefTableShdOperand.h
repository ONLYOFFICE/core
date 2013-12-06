#pragma once

#include "SHDOperand.h"

namespace ASCDocFileFormat
{
	class DefTableShdOperand: public IOperand
	{
	private:
		byte* bytes;
		unsigned int sizeInBytes;

	public:
		DefTableShdOperand() : bytes(0), sizeInBytes(1)
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

		explicit DefTableShdOperand( const vector<Shd>& _rgShd ) : bytes(NULL), sizeInBytes(0)
		{
			if ( _rgShd.size() <= 22 )
			{
				this->sizeInBytes = ( ( _rgShd.size() * 10 ) + sizeof(byte) );

				if ( this->sizeInBytes != 0 )
				{
					this->bytes = new byte[this->sizeInBytes];

					if ( this->bytes != NULL )
					{
						memset( this->bytes, 0, this->sizeInBytes );

						this->bytes[0] = ( this->sizeInBytes - sizeof(byte) );

						unsigned int offset = sizeof(byte);

						for ( vector<Shd>::const_iterator iter = _rgShd.begin(); iter != _rgShd.end(); iter++ )
						{
							memcpy( ( this->bytes + offset ), (byte*)(*iter), iter->Size() );
							offset += iter->Size();
						}
					}
				}
			}
		}

		DefTableShdOperand( const DefTableShdOperand& _defTableShdOperand ) : bytes(NULL), sizeInBytes(_defTableShdOperand.sizeInBytes)
		{
			if ( this->sizeInBytes != 0 )
			{
				this->bytes = new byte[this->sizeInBytes];

				if ( this->bytes != NULL )
				{
					memset( this->bytes, 0, this->sizeInBytes );

					memcpy( this->bytes, _defTableShdOperand.bytes, this->sizeInBytes );
				}
			}
		}

		virtual ~DefTableShdOperand()
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