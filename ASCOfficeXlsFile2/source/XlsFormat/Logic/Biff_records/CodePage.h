#pragma once

#include "BiffRecord.h"

namespace XLS
{


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
//-----------------------------
	BIFF_WORD cv;

};

} // namespace XLS

