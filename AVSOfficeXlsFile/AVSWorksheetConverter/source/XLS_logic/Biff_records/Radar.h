#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Radar record in BIFF8
class Radar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Radar)
	BASE_OBJECT_DEFINE_CLASS_NAME(Radar)
public:
	Radar();
	~Radar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fRdrAxLab;
	BIFF_BOOL fHasShadow;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fRdrAxLab)
		BO_ATTRIB_MARKUP_ATTRIB(fHasShadow)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

