#pragma once

#include "LSD.h"

namespace ASCDocFileFormat
{
	class StshiLsd: public IOperand
	{
	public:
		StshiLsd () : bytes(NULL), sizeInBytes(0)
		{

		}

		StshiLsd (const vector<LSD>& _mpstiilsd) : bytes(NULL), sizeInBytes(0), mpstiilsd(_mpstiilsd)
		{
			sizeInBytes	=	( sizeof(unsigned short) + ( sizeof(LSD) * mpstiilsd.size() ) );
			bytes		=	new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				FormatUtils::SetBytes ( bytes, (unsigned short)sizeof(LSD) );

				unsigned int lsdOffset = sizeof(unsigned short);

				for (vector<LSD>::iterator iter = mpstiilsd.begin(); iter != mpstiilsd.end(); ++iter)
				{
					FormatUtils::SetBytes( ( bytes + lsdOffset ), (unsigned int)(*iter) );
					lsdOffset += sizeof(LSD);
				}
			}
		}

		StshiLsd (const StshiLsd& _stshiLsd) : bytes(NULL), sizeInBytes(_stshiLsd.sizeInBytes), mpstiilsd(_stshiLsd.mpstiilsd)
		{
			bytes = new unsigned char[sizeInBytes];

			if ( NULL != bytes )
			{
				memset ( bytes, 0, sizeInBytes );
				memcpy ( bytes, _stshiLsd.bytes, sizeInBytes );
			}
		}

		virtual ~StshiLsd()
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

		std::vector<LSD>	mpstiilsd;

		unsigned char*				bytes;
		unsigned int		sizeInBytes;
	};
}