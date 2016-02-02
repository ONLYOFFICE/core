#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SerFmt record in BIFF8
class SerFmt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SerFmt)
	BASE_OBJECT_DEFINE_CLASS_NAME(SerFmt)
public:
	SerFmt();
	~SerFmt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSerFmt;

//-----------------------------
	bool fSmoothedLine;
	bool f3DBubbles;
	bool fArShadow;
};

} // namespace XLS

