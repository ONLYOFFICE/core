#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtFSPGR : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 24;
	public:

		OfficeArtFSPGR() : rh (0x1, 0x000, 0xF009, 0x00000010), xLeft(0), yTop(0), xRight(0), yBottom(0)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
		}

		OfficeArtFSPGR (int Left, int Top, int Right, int Bottom) : rh (0x1, 0x000, 0xF009, 0x00000010), xLeft(Left), yTop(Top), xRight(Right), yBottom(Bottom) 
		{
			memset (bytes, 0, SIZE_IN_BYTES);

			unsigned int offset = 0;

			memcpy ((bytes + offset), (unsigned char*)(rh), sizeof(rh));
			offset += sizeof(rh);

			memcpy ((bytes + offset), &xLeft, sizeof(xLeft));
			offset += sizeof(xLeft);

			memcpy ((bytes + offset), &yTop, sizeof(yTop));
			offset += sizeof(yTop);

			memcpy ((bytes + offset), &xRight, sizeof(xRight));
			offset += sizeof(xRight);

			memcpy ((bytes + offset), &yBottom, sizeof(yBottom));
			offset += sizeof(yBottom);
		}

		OfficeArtFSPGR (const OfficeArtFSPGR& _office): rh(_office.rh), xLeft(_office.xLeft), yTop(_office.yTop), xRight(_office.xRight), yBottom(_office.yBottom)
		{
			memset (bytes, 0, SIZE_IN_BYTES);
			memcpy (bytes, _office.bytes, SIZE_IN_BYTES);
		}

		virtual operator const unsigned char* () const
		{
			return (const unsigned char*)(&this->bytes);
		}

		virtual operator unsigned char* () const
		{
			return (unsigned char*)(&this->bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(this->bytes);
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtFSPGR;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFSPGR (*this);
		}

	protected:

		OfficeArtRecordHeader rh;

		int xLeft;
		int yTop;
		int xRight;
		int yBottom;

		unsigned char bytes[SIZE_IN_BYTES];
	};

	typedef OfficeArtFSPGR FSPGR;
}