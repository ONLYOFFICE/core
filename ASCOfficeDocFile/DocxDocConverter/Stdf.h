#pragma once

#include "StdfBase.h"
#include "StdfPost2000.h"

namespace ASCDocFileFormat
{
	class Stdf: public IOperand
	{
	public:

		Stdf() : bytes(NULL), sizeInBytes(0)
		{
		}

		explicit Stdf( const StdfBase& _stdfBase, const StdfPost2000* _stdfPost2000OrNone = NULL ) : bytes(NULL), sizeInBytes(0)
		{
			this->sizeInBytes = sizeof(_stdfBase);

			if ( _stdfPost2000OrNone != NULL )
			{
				this->sizeInBytes += sizeof(*_stdfPost2000OrNone);
			}

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, (unsigned char*)_stdfBase, sizeof(_stdfBase) );

				if ( _stdfPost2000OrNone != NULL )
				{
					memcpy( ( this->bytes + sizeof(_stdfBase) ), (unsigned char*)(*_stdfPost2000OrNone), sizeof(*_stdfPost2000OrNone) );
				}
			}
		}

		Stdf( const Stdf& _stdf ) : bytes(NULL), sizeInBytes(_stdf.sizeInBytes)
		{
			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, _stdf.bytes, this->sizeInBytes );
			}
		}

		virtual ~Stdf()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

		virtual operator unsigned char*() const
		{
			return this->bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->bytes;
		}

		virtual unsigned int Size() const
		{
			return this->sizeInBytes;
		}
	private:
	
		unsigned char* bytes;
		unsigned int sizeInBytes;
	};
}