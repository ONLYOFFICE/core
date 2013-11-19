#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Prot4Rev record in BIFF8
class Prot4Rev: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Prot4Rev)
	BASE_OBJECT_DEFINE_CLASS_NAME(Prot4Rev)
public:
	Prot4Rev();
	~Prot4Rev();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> fRevLock;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fRevLock)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

