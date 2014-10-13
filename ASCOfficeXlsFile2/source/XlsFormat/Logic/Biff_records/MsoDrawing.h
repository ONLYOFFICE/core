#pragma once

#include "BiffRecordSplit.h"
#include <Logic/Biff_structures/ODRAW/OfficeArtDgContainer.h>

namespace XLS
{;


// Logical representation of MsoDrawing record in BIFF8
class MsoDrawing: public BiffRecordSplit
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MsoDrawing)
	BASE_OBJECT_DEFINE_CLASS_NAME(MsoDrawing)
public:
	MsoDrawing(const bool is_inside_chart_sheet);
	~MsoDrawing();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

private:
	virtual const bool isStartingRecord(CFRecord& record);
	virtual const bool isEndingRecord(CFRecord& record);
	virtual void useContinueRecords(CFRecord& record);

private:
	ODRAW::OfficeArtDgContainer rgChildRec;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(rgChildRec)
	BO_ATTRIB_MARKUP_END

};

typedef boost::shared_ptr<MsoDrawing> MsoDrawingPtr;

} // namespace XLS

