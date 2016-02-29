#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtRefHeaderU.h>
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/TableFeatureType.h>

namespace XLS
{


// Logical representation of Feature11 record in BIFF8
class Feature11: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Feature11)
	BASE_OBJECT_DEFINE_CLASS_NAME(Feature11)
public:
	Feature11();
	~Feature11();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFeature11;



public:
	FrtRefHeaderU			frtRefHeaderU;
	_UINT16					isf;
	_UINT16					cref2;
	_UINT32					cbFeatData;
	BiffStructurePtrVector	refs2;
	BIFF_BSTR				sqref;

	TableFeatureType rgbFeat;
};

} // namespace XLS

