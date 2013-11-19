#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Backup record in BIFF8
class Backup: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Backup)
	BASE_OBJECT_DEFINE_CLASS_NAME(Backup)
public:
	Backup();
	~Backup();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> fBackup;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fBackup)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

