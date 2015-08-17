#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of CodeName record in BIFF8
class CodeName: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CodeName)
	BASE_OBJECT_DEFINE_CLASS_NAME(CodeName)
public:
	CodeName();
	~CodeName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCodeName;

//-----------------------------
	XLUnicodeString codeName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_ATTRIB(codeName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

