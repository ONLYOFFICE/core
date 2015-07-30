#pragma once

#include "BiffRecordContinued.h"
#include <Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h>

namespace XLS
{;


// Logical representation of MsoDrawingGroup record in BIFF8
class MsoDrawingGroup: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MsoDrawingGroup)
	BASE_OBJECT_DEFINE_CLASS_NAME(MsoDrawingGroup)
public:
	MsoDrawingGroup(const bool is_inside_chart_sheet);
	~MsoDrawingGroup();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	ODRAW::OfficeArtDggContainer rgChildRec;


};

} // namespace XLS

