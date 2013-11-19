#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Uncalced record in BIFF8
class Uncalced: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Uncalced)
	BASE_OBJECT_DEFINE_CLASS_NAME(Uncalced)
public:
	Uncalced();
	~Uncalced();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
//	BIFF_WORD userName;
public:
	BO_ATTRIB_MARKUP_BEGIN
//		BO_ATTRIB_MARKUP_ATTRIB(userName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

