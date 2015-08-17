#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of RecipName record in BIFF8
class RecipName: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RecipName)
	BASE_OBJECT_DEFINE_CLASS_NAME(RecipName)
public:
	RecipName();
	~RecipName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRecipName;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

