#pragma once

#include "../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace OfficeArt
{
	struct IOfficeArtRecord
	{
		virtual operator const unsigned char* () const = 0;
		virtual operator unsigned char* () const = 0;
		virtual unsigned int Size() const = 0;
		virtual IOfficeArtRecord* New() const = 0;
		virtual IOfficeArtRecord* Clone() const = 0;
		virtual ~IOfficeArtRecord() {}
	};

	typedef NSCommon::smart_ptr<IOfficeArtRecord> OfficeArtRecordPtr;
}
