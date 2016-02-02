#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Area record in BIFF8
class Area: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Area)
	BASE_OBJECT_DEFINE_CLASS_NAME(Area)
public:
	Area();
	~Area();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int	serialize(std::wostream & _stream);

	static const ElementType	type = typeArea;

	bool fStacked;
	bool f100;
	bool fHasShadow;
};

} // namespace XLS

