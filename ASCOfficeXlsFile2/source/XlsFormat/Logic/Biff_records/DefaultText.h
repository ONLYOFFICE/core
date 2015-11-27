#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DefaultText record in BIFF8
class DefaultText: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DefaultText)
	BASE_OBJECT_DEFINE_CLASS_NAME(DefaultText)
public:
	DefaultText();
	~DefaultText();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDefaultText;

//-----------------------------
	BIFF_WORD id;

};

} // namespace XLS

