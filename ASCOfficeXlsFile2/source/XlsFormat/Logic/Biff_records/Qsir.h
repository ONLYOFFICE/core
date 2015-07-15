#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Qsir record in BIFF8
class Qsir: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Qsir)
	BASE_OBJECT_DEFINE_CLASS_NAME(Qsir)
public:
	Qsir();
	~Qsir();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeQsir;

//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

