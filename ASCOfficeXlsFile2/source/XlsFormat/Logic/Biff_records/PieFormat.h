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

//-----------------------------
	BIFF_SHORT pcExplode;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(pcExplode)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

