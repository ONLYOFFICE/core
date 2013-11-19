#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtClientData : public IOfficeArtRecord
	{
	private:
		static const byte SIZE_IN_BYTES = 12;
	public:
		OfficeArtClientData (): rh(0x0, 0x0, 0xF011, 0x00000004), clientdata(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtClientData (int anchor) : rh(0x0, 0x000, 0xF011, 0x00000004), clientdata(anchor)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (byte*)(rh), sizeof(rh) );
			offset += sizeof(rh);

			memcpy( ( bytes + offset ), &(clientdata), sizeof(clientdata) );
		}

		OfficeArtClientData (const OfficeArtClientData& data): rh (data.rh), clientdata (data.clientdata)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, data.bytes, SIZE_IN_BYTES);
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
			return new OfficeArtClientData;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtClientData(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;
		
		int						clientdata;		//	Word 2000 and Word 97 use this value to store a reference count of the 
												//	shape. Word 2002, Office Word 2003, Office Word 2007, and Word 2010 ignore this value.
		byte					bytes[SIZE_IN_BYTES];
	};
}