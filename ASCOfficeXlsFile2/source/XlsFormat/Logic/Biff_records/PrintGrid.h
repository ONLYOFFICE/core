#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of PrintGrid record in BIFF8
class PrintGrid: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PrintGrid)
	BASE_OBJECT_DEFINE_CLASS_NAME(PrintGrid)
public:
	PrintGrid();
	~PrintGrid();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePrintGrid;

//-----------------------------
	BIFF_BOOL fPrintGrid;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fPrintGrid)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

