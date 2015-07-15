#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SCENARIO record in BIFF8
class SCENARIO: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SCENARIO)
	BASE_OBJECT_DEFINE_CLASS_NAME(SCENARIO)
public:
	SCENARIO();
	~SCENARIO();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSCENARIO;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

