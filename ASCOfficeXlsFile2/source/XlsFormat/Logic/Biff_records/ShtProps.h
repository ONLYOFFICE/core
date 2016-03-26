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
	bool fManSerAlloc;
	bool fManPlotArea;
	bool fPlotVisOnly;
	bool fNotSizeWith;
	bool fAlwaysAutoPlotArea;
	unsigned char	mdBlank;
};

} // namespace XLS

