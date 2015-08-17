#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DocRoute record in BIFF8
class DocRoute: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DocRoute)
	BASE_OBJECT_DEFINE_CLASS_NAME(DocRoute)
public:
	DocRoute();
	~DocRoute();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDocRoute;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

