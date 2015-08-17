#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DConRef record in BIFF8
class DConRef: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DConRef)
	BASE_OBJECT_DEFINE_CLASS_NAME(DConRef)
public:
	DConRef();
	~DConRef();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDConRef;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

