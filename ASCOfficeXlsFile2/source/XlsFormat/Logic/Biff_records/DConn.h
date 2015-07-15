#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DConn record in BIFF8
class DConn: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DConn)
	BASE_OBJECT_DEFINE_CLASS_NAME(DConn)
public:
	DConn();
	~DConn();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDConn;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

