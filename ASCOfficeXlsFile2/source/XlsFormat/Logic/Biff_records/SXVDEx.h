#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{


// Logical representation of SXVDEx record in BIFF8
class SXVDEx: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXVDEx)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXVDEx)
public:
	SXVDEx();
	~SXVDEx();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXVDEx;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

