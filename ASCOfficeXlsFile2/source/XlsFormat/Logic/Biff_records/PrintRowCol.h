#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of PrintRowCol record in BIFF8
class PrintRowCol: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PrintRowCol)
	BASE_OBJECT_DEFINE_CLASS_NAME(PrintRowCol)
public:
	PrintRowCol();
	~PrintRowCol();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePrintRowCol;


//-----------------------------
	Boolean<unsigned short> printRwCol;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(printRwCol)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

