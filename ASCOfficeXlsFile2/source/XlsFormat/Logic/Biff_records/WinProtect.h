#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of WinProtect record in BIFF8
class WinProtect: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WinProtect)
	BASE_OBJECT_DEFINE_CLASS_NAME(WinProtect)
public:
	WinProtect();
	~WinProtect();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeWinProtect;

//-----------------------------
	Boolean<unsigned __int16> fLockWn;

};

} // namespace XLS

