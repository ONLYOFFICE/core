#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtClientTextbox : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 12;
	public:

		OfficeArtClientTextbox (): rh(0x0, 0x0, 0xF00D, 0x00000004), clienttextbox(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtClientTextbox (unsigned short textbox) : rh(0x0, 0x0, 0xF00D, 0x00000004), clienttextbox(textbox)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (unsigned char*)(rh), sizeof(rh) );
			offset += sizeof(rh);

			unsigned short nNum	= 0;
			
			memcpy( ( bytes + offset ), &(nNum), sizeof(unsigned short));
			offset += sizeof(unsigned short );
			
			memcpy( ( bytes + offset ), &(clienttextbox), sizeof(unsigned short));
		}

		OfficeArtClientTextbox (const OfficeArtClientTextbox& data) : rh (data.rh), clienttextbox (data.clienttextbox)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, data.bytes, SIZE_IN_BYTES);
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
			return new OfficeArtClientTextbox;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtClientTextbox(*this);
		}

	protected:

		OfficeArtRecordHeader	rh;

		unsigned short 			clienttextbox;		//	text identifier of the shape
		unsigned char					bytes[SIZE_IN_BYTES];
	};
}