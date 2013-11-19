#pragma once

#include "Common.h"

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtSplitMenuColorContainer : public IOfficeArtRecord
	{
	private:
		static const byte SIZE_IN_BYTES = 24;
	public:
		
		OfficeArtSplitMenuColorContainer () : rh(0x0, 0x004, 0xF11E, 0x00000010), fillColor(0), lineColor(0), shadowColor(0), color3D(0)
		{
			memset( bytes, 0, SIZE_IN_BYTES );
		}

		OfficeArtSplitMenuColorContainer (unsigned int fill, unsigned int line, unsigned int shadow, unsigned int n3D) : rh(0x0, 0x004, 0xF11E, 0x00000010), fillColor(fill), lineColor(line), shadowColor(shadow), color3D(n3D)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			unsigned int offset = 0;

			memcpy( ( bytes + offset ), (byte*)(rh), sizeof(rh) );
			offset += sizeof(rh);

			memcpy( ( bytes + offset ), &(fillColor), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
			memcpy( ( bytes + offset ), &(lineColor), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
			memcpy( ( bytes + offset ), &(shadowColor), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
			memcpy( ( bytes + offset ), &(color3D), sizeof(unsigned int) );
			offset += sizeof(unsigned int);
		}

		OfficeArtSplitMenuColorContainer(const OfficeArtSplitMenuColorContainer& color): rh(color.rh), fillColor(color.fillColor), lineColor(color.lineColor), shadowColor(color.shadowColor), color3D(color.color3D)
		{
			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, color.bytes, SIZE_IN_BYTES);
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
			return new OfficeArtSplitMenuColorContainer();
		}

		virtual IOfficeArtRecord* Clone() const
		{
			return new OfficeArtSplitMenuColorContainer(*this);
		}


	protected:

		OfficeArtRecordHeader	rh;
		
		unsigned int			fillColor;		// fill color
		unsigned int			lineColor;		// line color
		unsigned int			shadowColor;	// shadow color
		unsigned int			color3D;		// 3D color

		byte					bytes[SIZE_IN_BYTES];
	};
}