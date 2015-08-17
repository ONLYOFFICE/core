#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DBQueryExt record in BIFF8
class DBQueryExt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DBQueryExt)
	BASE_OBJECT_DEFINE_CLASS_NAME(DBQueryExt)
public:
	DBQueryExt();
	~DBQueryExt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDBQueryExt;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

