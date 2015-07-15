#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of Feature12 record in BIFF8
class Feature12: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Feature12)
	BASE_OBJECT_DEFINE_CLASS_NAME(Feature12)
public:
	Feature12();
	~Feature12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFeature12;

public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

