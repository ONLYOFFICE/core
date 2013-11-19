#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;


// Logical representation of FeatHdr11 record in BIFF8
class FeatHdr11: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FeatHdr11)
	BASE_OBJECT_DEFINE_CLASS_NAME(FeatHdr11)
public:
	FeatHdr11();
	~FeatHdr11();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

public:
	FrtHeader frt;
	SharedFeatureType isf;
	BIFF_DWORD idListNext;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(frt)
		BO_ATTRIB_MARKUP_ATTRIB(isf)
		BO_ATTRIB_MARKUP_ATTRIB(idListNext)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

