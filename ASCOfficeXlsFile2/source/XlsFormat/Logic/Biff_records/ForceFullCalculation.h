#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ForceFullCalculation record in BIFF8
class ForceFullCalculation: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ForceFullCalculation)
	BASE_OBJECT_DEFINE_CLASS_NAME(ForceFullCalculation)
public:
	ForceFullCalculation();
	~ForceFullCalculation();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<unsigned int> fNoDeps;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fNoDeps)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

