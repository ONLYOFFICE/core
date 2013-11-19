#pragma once

#include "BiffRecordContinued.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <XLS_logic/Biff_structures/FeatProtection.h>
#include <XLS_logic/Biff_structures/FeatFormulaErr2.h>
#include <XLS_logic/Biff_structures/FeatSmartTag.h>

namespace XLS
{;


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
private:
	SharedFeatureType isf;
	BIFF_WORD cref;
	BIFF_DWORD cbFeatData;
	BiffStructurePtrVector refs;
	BIFF_BSTR sqref;

	FeatProtection protection;
	FeatFormulaErr2 formula_err;
	FeatSmartTag smart_tag;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(isf)
		BO_ATTRIB_MARKUP_ATTRIB(cref)
		BO_ATTRIB_MARKUP_ATTRIB(sqref)
		BO_ATTRIB_MARKUP_ATTRIB(cbFeatData)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(refs, Ref8U)
		switch(isf)
		{
			case SharedFeatureType::ISFPROTECTION:
				BO_ATTRIB_MARKUP_COMPLEX(protection)
				break;
			case SharedFeatureType::ISFFEC2:
				BO_ATTRIB_MARKUP_COMPLEX(formula_err)
				break;
			case SharedFeatureType::ISFFACTOID:
				BO_ATTRIB_MARKUP_COMPLEX(smart_tag)
				break;
		}


	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

