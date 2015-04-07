#pragma once

#include "Stshif.h"
#include "StshiLsd.h"
//!!!A structure that has no effect and MUST be ignored.!!!
#include "STSHIB.h"

namespace ASCDocFileFormat
{
	class STSHI : public IOperand
	{
	public:
		STSHI () : bytes(NULL), sizeInBytes(0)
		{

		}

		STSHI (const Stshif& _stshif, short _ftcBi, const StshiLsd& _stshiLsd, const STSHIB& _stshiB /*!!!MUST be ignored!!!*/ ) : bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes = ( _stshif.Size() + sizeof(_ftcBi) + _stshiLsd.Size() + _stshiB.Size() );

			this->bytes = new unsigned char[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, (unsigned char*)_stshif, _stshif.Size() );
				FormatUtils::SetBytes( ( this->bytes + _stshif.Size() ), _ftcBi );
				memcpy( ( this->bytes + _stshif.Size() + sizeof(_ftcBi) ), (unsigned char*)_stshiLsd, _stshiLsd.Size() );
				memcpy( ( this->bytes + _stshif.Size() + sizeof(_ftcBi) + _stshiLsd.Size() ), (unsigned char*)_stshiB, _stshiB.Size() );
			}
		}

		STSHI (const STSHI& oSTSHI) : bytes(NULL), sizeInBytes(oSTSHI.sizeInBytes)
		{
			bytes = new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				memcpy ( bytes, oSTSHI.bytes, sizeInBytes );
			}
		}

		virtual ~STSHI()
		{
			RELEASEARRAYOBJECTS(bytes);
		}

		virtual operator unsigned char*() const
		{
			return bytes;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeInBytes;
		}

	private:

		unsigned char*			bytes;
		unsigned int	sizeInBytes;
	};
}