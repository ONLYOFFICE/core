#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CodePage record in BIFF8
class CodePage: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CodePage)
	BASE_OBJECT_DEFINE_CLASS_NAME(CodePage)
public:
	CodePage();
	~CodePage();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD cv;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cv)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

