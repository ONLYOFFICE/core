#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of EOF record in BIFF8
// _T suffix is to avoid confusing EOF define equaling this to (-1)
class EOF_T: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(EOF)
	BASE_OBJECT_DEFINE_CLASS_NAME(EOF)
public:
	EOF_T();
	~EOF_T();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeEOF_T;


	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END
};

} // namespace XLS

