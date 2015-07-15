#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/SortCond12.h>

namespace XLS
{;


// Logical representation of SortData record in BIFF8
class SortData: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SortData)
	BASE_OBJECT_DEFINE_CLASS_NAME(SortData)
public:
	SortData();
	~SortData();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSortData;

//-----------------------------
	BIFF_BOOL fCol;
	BIFF_BOOL fCaseSensitive;
	BIFF_BOOL fAltMethod;
	BIFF_BYTE sfp;

	BIFF_BSTR rfx;
	BIFF_DWORD cconditions;
	BIFF_DWORD idParent;

	BiffStructurePtrVector sortCond12Array;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fCol)
		//BO_ATTRIB_MARKUP_ATTRIB(fCaseSensitive)
		//BO_ATTRIB_MARKUP_ATTRIB(fAltMethod)
		//BO_ATTRIB_MARKUP_ATTRIB(sfp)
		//BO_ATTRIB_MARKUP_ATTRIB(rfx)
		//BO_ATTRIB_MARKUP_ATTRIB(cconditions)
		//if(0x01 == sfp || 0x03 == sfp)
		//{
		//	//BO_ATTRIB_MARKUP_ATTRIB(idParent)
		//}
		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(sortCond12Array, SortCond12)

	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

