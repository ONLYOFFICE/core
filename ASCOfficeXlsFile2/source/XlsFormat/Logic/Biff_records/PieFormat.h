#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of PieFormat record in BIFF8
class PieFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PieFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(PieFormat)
public:
	PieFormat();
	~PieFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePieFormat;

	int serialize(std::wostream & _stream);

	BIFF_SHORT pcExplode;

};

} // namespace XLS

