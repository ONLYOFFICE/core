#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DCon record in BIFF8
class DCon: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DCon)
	BASE_OBJECT_DEFINE_CLASS_NAME(DCon)
public:
	DCon();
	~DCon();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
//	BIFF_WORD userName;
	WORD iiftab;
	WORD fLeftCat;
	WORD fTopCat;
	WORD fLinkConsole;


public:
	BO_ATTRIB_MARKUP_BEGIN
//		BO_ATTRIB_MARKUP_ATTRIB(userName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

