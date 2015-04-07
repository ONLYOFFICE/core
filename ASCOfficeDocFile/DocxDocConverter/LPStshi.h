#pragma once

#include "STSHI.h"

namespace ASCDocFileFormat
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

			bytes		=	new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );

				FormatUtils::SetBytes( bytes, (unsigned short)oSTSHI.Size() );
				memcpy ( ( bytes + sizeof(unsigned short) ), (unsigned char*)oSTSHI, oSTSHI.Size() );
			}
		}

		LPStshi (const LPStshi& oLPStshi) : bytes (NULL), sizeInBytes (oLPStshi.sizeInBytes)
		{
			bytes = new unsigned char[sizeInBytes];

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