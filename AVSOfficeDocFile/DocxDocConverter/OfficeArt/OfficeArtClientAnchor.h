#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtClientAnchor : public IOfficeArtRecord
	{
	private:
		static const byte SIZE_IN_BYTES = 12;
	public:
		OfficeArtClientAnchor (): rh(0x0, 0x0, 0xF010, 0x00000004), clientanchor(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtClientAnchor (int anchor) : rh(0x0, 0x000, 0xF010, 0x00000004), clientanchor(anchor)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (byte*)(rh), sizeof(rh) );
			offset += sizeof(rh);

			memcpy( ( bytes + offset ), &(clientanchor), sizeof(clientanchor) );
		}

		OfficeArtClientAnchor (const OfficeArtClientAnchor& anchor): rh (anchor.rh), clientanchor(anchor.clientanchor)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, anchor.bytes, SIZE_IN_BYTES);
		}

		virtual operator const byte* () const
		{
			return (const byte*)(&bytes);
		}

		virtual operator byte* () const
		{
			return (byte*)(&bytes);
		}

		virtual unsigned int Size() const
		{
			return sizeof(bytes);
		}

		virtual IOfficeArtRecord* New() const
		{
			return new OfficeArtClientAnchor;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtClientAnchor(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;

		int						clientanchor;
		byte					bytes[SIZE_IN_BYTES];
	};
}