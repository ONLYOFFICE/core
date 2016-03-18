#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtHeader.h>
#include <Logic/Biff_structures/List12BlockLevel.h>
#include <Logic/Biff_structures/List12TableStyleClientInfo.h>
#include <Logic/Biff_structures/List12DisplayName.h>

namespace XLS
{


// Logical representation of List12 record in BIFF8
class List12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(List12)
	BASE_OBJECT_DEFINE_CLASS_NAME(List12)
public:
	List12();
	~List12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

public:
	FrtHeader frtHeader;
	_UINT16 lsd;
	BIFF_DWORD idList;

	List12BlockLevel rgbList12BlockLevel;
	List12TableStyleClientInfo rgbList12TableStyleClientInfo;
	List12DisplayName rgbList12DisplayName;
};

} // namespace XLS

