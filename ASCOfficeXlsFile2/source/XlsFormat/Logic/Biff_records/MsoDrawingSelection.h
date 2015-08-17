#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of MsoDrawingSelection record in BIFF8
class MsoDrawingSelection: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MsoDrawingSelection)
	BASE_OBJECT_DEFINE_CLASS_NAME(MsoDrawingSelection)
public:
	MsoDrawingSelection();
	~MsoDrawingSelection();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

public:
	//BO_ATTRIB_MARKUP_BEGIN
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

