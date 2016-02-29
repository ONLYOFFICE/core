#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/FrtRefHeader.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/FrtRefHeader.h>
#include <Logic/Biff_structures/AFDOper.h>
#include <Logic/Biff_structures/AF12Criteria.h>

namespace XLS
{


// Logical representation of AutoFilter12 record in BIFF8
class AutoFilter12: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AutoFilter12)
	BASE_OBJECT_DEFINE_CLASS_NAME(AutoFilter12)
public:
	AutoFilter12();
	~AutoFilter12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAutoFilter12;

	FrtRefHeader frtRefHeader;
	_UINT16		iEntry;
	BIFF_DWORD	fHideArrow;
	BIFF_DWORD	ft;
	BIFF_DWORD	cft;
	BIFF_DWORD	cCriteria;
	BIFF_DWORD	cDateGroupings;
	BIFF_DWORD	idList;
	
	BiffStructurePtrVector rgbAF12Criteries;
};

} // namespace XLS

