#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of PlotArea record in BIFF8
class PlotArea: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PlotArea)
	BASE_OBJECT_DEFINE_CLASS_NAME(PlotArea)
public:
	PlotArea();
	~PlotArea();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

