#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of OleDbConn record in BIFF8
class OleDbConn: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(OleDbConn)
	BASE_OBJECT_DEFINE_CLASS_NAME(OleDbConn)
public:
	OleDbConn();
	~OleDbConn();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeOleDbConn;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

