#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/FeatProtection.h>
#include <Logic/Biff_structures/FeatFormulaErr2.h>
#include <Logic/Biff_structures/FeatSmartTag.h>

namespace XLS
{


// Logical representation of Feat record in BIFF8
class Feat: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Feat)
	BASE_OBJECT_DEFINE_CLASS_NAME(Feat)
public:
	Feat();
	~Feat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFeat;

//-----------------------------
	SharedFeatureType isf;
	_UINT16 cref;
	_UINT32	cbFeatData;
	BiffStructurePtrVector refs;
	BIFF_BSTR sqref;

	FeatProtection protection;
	FeatFormulaErr2 formula_err;
	FeatSmartTag smart_tag;

};

} // namespace XLS

