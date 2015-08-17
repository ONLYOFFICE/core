#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of String record in BIFF8
class String: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(String)
	BASE_OBJECT_DEFINE_CLASS_NAME(String)
public:
	String();
	~String();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeString;

//-----------------------------
	XLUnicodeString string;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(string)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

