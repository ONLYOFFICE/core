#pragma once

#include "STSHI.h"

namespace AVSDocFileFormat
{
	class LPStshi : public IOperand
	{
	public:

		LPStshi () : bytes(NULL), sizeInBytes(0)
		{

		}

		LPStshi( const STSHI& oSTSHI ): bytes(NULL), sizeInBytes(0)
		{
			sizeInBytes	=	( sizeof(unsigned short) + oSTSHI.Size() );

			bytes		=	new byte[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );

				FormatUtils::SetBytes( bytes, (unsigned short)oSTSHI.Size() );
				memcpy ( ( bytes + sizeof(unsigned short) ), (byte*)oSTSHI, oSTSHI.Size() );
			}
		}

		LPStshi (const LPStshi& oLPStshi) : bytes (NULL), sizeInBytes (oLPStshi.sizeInBytes)
		{
			bytes = new byte[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				memcpy ( bytes, oLPStshi.bytes, sizeInBytes );
			}
		}

		virtual ~LPStshi()
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