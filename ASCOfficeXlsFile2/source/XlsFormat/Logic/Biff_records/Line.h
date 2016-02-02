#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Line record in BIFF8
class Line: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Line)
	BASE_OBJECT_DEFINE_CLASS_NAME(Line)
public:
	Line();
	~Line();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeLine;

	int serialize(std::wostream & _stream);

//-----------------------------
	bool fStacked;
	bool f100;
	bool fHasShadow;

};

} // namespace XLS

