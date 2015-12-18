#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ShtProps record in BIFF8
class ShtProps: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ShtProps)
	BASE_OBJECT_DEFINE_CLASS_NAME(ShtProps)
public:
	ShtProps();
	~ShtProps();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
	static const ElementType	type = typeShtProps;

//-----------------------------
	BIFF_BOOL fManSerAlloc;
	BIFF_BOOL fPlotVisOnly;
	BIFF_BOOL fNotSizeWith;
	BIFF_BOOL fAlwaysAutoPlotArea;
	BIFF_BYTE mdBlank;
};

} // namespace XLS

