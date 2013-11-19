#pragma once

#include "BiffRecordContinued.h"
#include <XLS_logic/Biff_structures/FrtRefHeader.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/FrtRefHeader.h>
#include <XLS_logic/Biff_structures/AFDOper.h>
#include <XLS_logic/Biff_structures/AF12Criteria.h>

namespace XLS
{;


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

public:
	FrtRefHeader frtRefHeader;
	BIFF_WORD iEntry;
	BIFF_DWORD fHideArrow;
	BIFF_DWORD ft;
	BIFF_DWORD cft;
	BIFF_DWORD cCriteria;
	BIFF_DWORD cDateGroupings;
	BIFF_DWORD idList;
	BiffStructurePtrVector rgbAF12Criteries;
	
private:
//	BIFF_WORD userName;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(frtRefHeader)
		BO_ATTRIB_MARKUP_ATTRIB(iEntry)
		BO_ATTRIB_MARKUP_ATTRIB(fHideArrow)
		BO_ATTRIB_MARKUP_ATTRIB(ft)
		BO_ATTRIB_MARKUP_ATTRIB(cft)
		BO_ATTRIB_MARKUP_ATTRIB(cCriteria)
		BO_ATTRIB_MARKUP_ATTRIB(cDateGroupings)
		BO_ATTRIB_MARKUP_ATTRIB(idList)

		if ( (ft == 0) && (cCriteria > 0) )
		{
			BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgbAF12Criteries, AF12Criteria)
		}

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

