#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FixedPoint.h>

namespace XLS
{;


// Logical representation of PlotGrowth record in BIFF8
class PlotGrowth: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PlotGrowth)
	BASE_OBJECT_DEFINE_CLASS_NAME(PlotGrowth)
public:
	PlotGrowth();
	~PlotGrowth();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePlotGrowth;

//-----------------------------
	OSHARED::FixedPoint dxPlotGrowth;
	OSHARED::FixedPoint dyPlotGrowth;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(dxPlotGrowth)
		//BO_ATTRIB_MARKUP_ATTRIB(dyPlotGrowth)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

