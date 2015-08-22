#pragma once

#include "BiffRecordSplit.h"
#include <Logic/Biff_structures/ODRAW/OfficeArtDgContainer.h>

namespace XLS
{


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
	void readFields();

	static const ElementType	type = typeMsoDrawing;
//-----------------------------
	virtual const bool isStartingRecord(CFRecord& record);
	virtual const bool isEndingRecord(CFRecord& record);
	virtual void useContinueRecords(CFRecord& record);

//-----------------------------
	ODRAW::OfficeArtDgContainer rgChildRec;

	bool isReading;

};

typedef boost::shared_ptr<MsoDrawing> MsoDrawingPtr;

} // namespace XLS

