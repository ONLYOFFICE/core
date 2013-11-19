#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Guts record in BIFF8
class Guts: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Guts)
	BASE_OBJECT_DEFINE_CLASS_NAME(Guts)
public:
	Guts();
	~Guts();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD iLevelRwMac;
	BIFF_WORD iLevelColMac;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(iLevelRwMac)
		BO_ATTRIB_MARKUP_ATTRIB(iLevelColMac)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

