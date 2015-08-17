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
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(id)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

