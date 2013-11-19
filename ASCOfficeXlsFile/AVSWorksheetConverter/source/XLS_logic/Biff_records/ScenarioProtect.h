#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ScenarioProtect record in BIFF8
class ScenarioProtect: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ScenarioProtect)
	BASE_OBJECT_DEFINE_CLASS_NAME(ScenarioProtect)
public:
	ScenarioProtect();
	~ScenarioProtect();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> fScenProtect;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fScenProtect)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

