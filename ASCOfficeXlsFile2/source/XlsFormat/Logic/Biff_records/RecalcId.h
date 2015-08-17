#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of RecalcId record in BIFF8
class RecalcId: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RecalcId)
	BASE_OBJECT_DEFINE_CLASS_NAME(RecalcId)
public:
	RecalcId();
	~RecalcId();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRecalcId;


//-----------------------------
	BIFF_DWORD dwBuild;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(dwBuild)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

