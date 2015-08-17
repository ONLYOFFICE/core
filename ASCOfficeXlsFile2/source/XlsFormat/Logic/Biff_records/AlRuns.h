#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FormatRun.h>

namespace XLS
{


// Logical representation of AlRuns record in BIFF8
class AlRuns: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AlRuns)
	BASE_OBJECT_DEFINE_CLASS_NAME(AlRuns)
public:
	AlRuns();
	~AlRuns();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAlRuns;

	BiffStructurePtrVector rgRuns;

};

} // namespace XLS

