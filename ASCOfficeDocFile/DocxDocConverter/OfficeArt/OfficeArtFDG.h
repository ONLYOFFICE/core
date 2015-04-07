#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtFDG : public IOfficeArtRecord
	{
	private:
		static const unsigned char SIZE_IN_BYTES = 16;
	public:
		OfficeArtFDG(): rh(0x0, 0xFFE, 0xF008, 0x00000008), spidCur(0), csp(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtFDG(MSOSPID _spid, unsigned int _csp) : rh(0x0, 0xFFE, 0xF008, 0x00000008), spidCur(_spid), csp(_csp)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (unsigned char*)(rh), sizeof(rh) );
			offset += sizeof(rh);
			
			memcpy( ( bytes + offset ), &(csp), sizeof(csp) );
			offset += sizeof(csp);

			memcpy( ( bytes + offset ), &(spidCur), sizeof(spidCur) );
			offset += sizeof(spidCur);
		}

		OfficeArtFDG(const OfficeArtFDG& _fdg ): rh( _fdg.rh ), spidCur(_fdg.spidCur), csp(_fdg.csp)
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
			return new OfficeArtFDG;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFDG(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;
		MSOSPID					spidCur;
		unsigned int			csp;

		unsigned char					bytes[SIZE_IN_BYTES];
	};
}