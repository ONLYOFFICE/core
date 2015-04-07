#pragma once

#include "Common.h"
#include "Enumerations.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

#define OFFICE_ART_FSP_GROUP      (0x00000001)
#define OFFICE_ART_FSP_CHILD      (0x00000002)
#define OFFICE_ART_FSP_PATRIARCH  (0x00000004)
#define OFFICE_ART_FSP_DELETED    (0x00000008)
#define OFFICE_ART_FSP_OLESHAPE   (0x00000010)
#define OFFICE_ART_FSP_HAVEMASTER (0x00000020)
#define OFFICE_ART_FSP_FLIPH      (0x00000040)
#define OFFICE_ART_FSP_FLIPV      (0x00000080)
#define OFFICE_ART_FSP_CONNECTOR  (0x00000100)
#define OFFICE_ART_FSP_HAVEANCHOR (0x00000200)
#define OFFICE_ART_FSP_BACKGROUND (0x00000400)
#define OFFICE_ART_FSP_HAVESPT    (0x00000800)

namespace OfficeArt
{
	class OfficeArtFSP : public IOfficeArtRecord
	{
	public:
		OfficeArtFSP() : rh( 0x2, (unsigned short)Enumerations::msosptNotPrimitive, 0xF00A, 0x00000008 ), spid(0), flags(0)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );
		}

		explicit OfficeArtFSP (Enumerations::MSOSPT _shapeType, MSOSPID _spid = 0, unsigned int _flags = 0) : rh( 0x2, (unsigned short)_shapeType, 0xF00A, 0x00000008 ), spid(_spid), flags(_flags)
		{
			memset( this->bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( this->bytes + offset ), (unsigned char*)(this->rh), sizeof(this->rh) );
			offset += sizeof(this->rh);

			memcpy( ( this->bytes + offset ), &(this->spid), sizeof(this->spid) );
			offset += sizeof(this->spid);

			memcpy( ( this->bytes + offset ), &(this->flags), sizeof(this->flags) );
			offset += sizeof(this->flags);
		}

		OfficeArtFSP (const OfficeArtFSP& _officeArtFSP) : rh(_officeArtFSP.rh), spid(_officeArtFSP.spid), flags(_officeArtFSP.flags)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, _officeArtFSP.bytes, SIZE_IN_BYTES);
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
			return new OfficeArtFSP;
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtFSP( *this );
		}

		virtual ~OfficeArtFSP() 
		{
		}

	protected:

		OfficeArtRecordHeader rh;
		MSOSPID spid;
		unsigned int flags;
		static const unsigned char SIZE_IN_BYTES = 16;
		unsigned char bytes[SIZE_IN_BYTES];
	};

	typedef OfficeArtFSP FSP;
}