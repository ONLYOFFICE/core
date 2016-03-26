#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of MTRSettings record in BIFF8
class MTRSettings: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MTRSettings)
	BASE_OBJECT_DEFINE_CLASS_NAME(MTRSettings)
public:
	MTRSettings();
	~MTRSettings();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	bool fMTREnabled;
	bool fUserSetThreadCount;
	
	_UINT32	cUserThreadCount;

};

} // namespace XLS

