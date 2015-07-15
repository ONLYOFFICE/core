#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DefColWidth record in BIFF8
class DefColWidth: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DefColWidth)
	BASE_OBJECT_DEFINE_CLASS_NAME(DefColWidth)
public:
	DefColWidth();
	~DefColWidth();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDefColWidth;

//-----------------------------
	BIFF_WORD cchdefColWidth;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(cchdefColWidth)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

