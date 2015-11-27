#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Frame record in BIFF8
class Frame: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Frame)
	BASE_OBJECT_DEFINE_CLASS_NAME(Frame)
public:
	Frame();
	~Frame();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFrame;

//-----------------------------
	BIFF_WORD frt;
	BIFF_BOOL fAutoSize;
	BIFF_BOOL fAutoPosition;

};

} // namespace XLS

