#pragma once

#include "Stshif.h"
#include "StshiLsd.h"
//!!!A structure that has no effect and MUST be ignored.!!!
#include "STSHIB.h"

namespace AVSDocFileFormat
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

			this->bytes = new byte[this->sizeInBytes];

			if ( this->bytes != NULL )
			{
				memset( this->bytes, 0, this->sizeInBytes );
				memcpy( this->bytes, (byte*)_stshif, _stshif.Size() );
				FormatUtils::SetBytes( ( this->bytes + _stshif.Size() ), _ftcBi );
				memcpy( ( this->bytes + _stshif.Size() + sizeof(_ftcBi) ), (byte*)_stshiLsd, _stshiLsd.Size() );
				memcpy( ( this->bytes + _stshif.Size() + sizeof(_ftcBi) + _stshiLsd.Size() ), (byte*)_stshiB, _stshiB.Size() );
			}
		}

		STSHI (const STSHI& oSTSHI) : bytes(NULL), sizeInBytes(oSTSHI.sizeInBytes)
		{
			bytes = new byte[sizeInBytes];

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

		virtual operator byte*() const
		{
			return bytes;
		}

		virtual operator const byte*() const
		{
			return (const byte*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeInBytes;
		}

	private:

		byte*			bytes;
		unsigned int	sizeInBytes;
	};
}