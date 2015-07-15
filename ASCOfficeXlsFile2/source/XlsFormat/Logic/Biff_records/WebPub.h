#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of WebPub record in BIFF8
class WebPub: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WebPub)
	BASE_OBJECT_DEFINE_CLASS_NAME(WebPub)
public:
	WebPub();
	~WebPub();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeWebPub;

//-----------------------------
//	BIFF_WORD userName;

};

} // namespace XLS

