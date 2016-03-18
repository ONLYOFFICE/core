#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{


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

	static const ElementType	type = typeFeatHdr11;



public:
	FrtHeader frt;
	SharedFeatureType isf;
	BIFF_DWORD idListNext;

};

} // namespace XLS

