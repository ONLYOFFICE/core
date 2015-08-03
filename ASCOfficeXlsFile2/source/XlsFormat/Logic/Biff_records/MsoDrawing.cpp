
#include "MsoDrawing.h"

namespace XLS
{;

MsoDrawing::MsoDrawing(const bool is_inside_chart_sheet)
:	rgChildRec(is_inside_chart_sheet ? ODRAW::OfficeArtRecord::CA_Chart : ODRAW::OfficeArtRecord::CA_Sheet)
{
	isReading = false;
}


MsoDrawing::~MsoDrawing()
{
}


BaseObjectPtr MsoDrawing::clone()
{
	return BaseObjectPtr(new MsoDrawing(*this));
}


void MsoDrawing::writeFields(CFRecord& record)
{
	record << rgChildRec;
}

void MsoDrawing::readFields()
{
	rgChildRec.rh_own.recLen = stored_record->getDataSize();

	rgChildRec.loadFields(*stored_record);
}

void MsoDrawing::readFields(CFRecord& record)
{
	record >> rgChildRec;

	isReading = true;
}


void MsoDrawing::useContinueRecords(CFRecord& record)
{
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	while(!recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}
}


const bool MsoDrawing::isStartingRecord(CFRecord& record)
{
	return ODRAW::OfficeArtDgContainer::CheckIfContainerStartFound(record);
}


const bool MsoDrawing::isEndingRecord(CFRecord& record)
{
	return ODRAW::OfficeArtDgContainer::CheckIfContainerSizeOK(record);
}



} // namespace XLS

