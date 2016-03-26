#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/PropertyBagStore.h>
#include <Logic/Biff_structures/EnhancedProtection.h>

namespace XLS
{


// Logical representation of FeatHdr record in BIFF8
class FeatHdr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FeatHdr)
	BASE_OBJECT_DEFINE_CLASS_NAME(FeatHdr)
public:
	FeatHdr(const bool is_called_from_Globals);
	~FeatHdr();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFeatHdr;

//-----------------------------
	bool is_contained_in_Globals;
	SharedFeatureType isf;
	_UINT32	cbHdrData;
	EnhancedProtection protection;
	OSHARED::PropertyBagStore prop;
};

} // namespace XLS

