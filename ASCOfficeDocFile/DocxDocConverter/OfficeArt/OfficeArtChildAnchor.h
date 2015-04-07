#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtChildAnchor : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 24;
	public:
		OfficeArtChildAnchor(): rh(0x0, 0x000, 0xF00F, 0x00000010), Left(0), Top(0), Right(0), Bottom(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtChildAnchor(int left, int right, int top, int bottom) : rh(0x0, 0x000, 0xF00F, 0x00000010), Left(left), Top(top), Right(right), Bottom(bottom)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (unsigned char*)(rh), sizeof(rh) );
			offset += sizeof(rh);

			memcpy( ( bytes + offset ), &(Left), sizeof(Left) );
			offset += sizeof(Left);
			memcpy( ( bytes + offset ), &(Top), sizeof(Top) );
			offset += sizeof(Top);
			memcpy( ( bytes + offset ), &(Right), sizeof(Right) );
			offset += sizeof(Right);
			memcpy( ( bytes + offset ), &(Bottom), sizeof(Bottom) );
			offset += sizeof(Bottom);
		}

		OfficeArtChildAnchor(const OfficeArtChildAnchor& _fdg ): rh( _fdg.rh ), Left(_fdg.Left), Top(_fdg.Top), Right(_fdg.Right), Bottom(_fdg.Bottom)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, _fdg.bytes, SIZE_IN_BYTES);
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(&bytes);
		}

		virtual operator unsigned char* () const
		{
			return (unsigned char*)(&bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(bytes);
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtChildAnchor;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtChildAnchor(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;
		
		int						Left;
		int						Top;
		int						Right;
		int						Bottom;

		unsigned char					bytes[SIZE_IN_BYTES];
	};
}