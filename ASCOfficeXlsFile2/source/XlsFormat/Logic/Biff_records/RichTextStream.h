#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of RichTextStream record in BIFF8
class RichTextStream: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RichTextStream)
	BASE_OBJECT_DEFINE_CLASS_NAME(RichTextStream)
public:
	RichTextStream();
	~RichTextStream();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRichTextStream;

};

} // namespace XLS

