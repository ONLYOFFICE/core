#pragma once

#include <Logic/Biff_records/BiffRecordContinued.h> 
//#include <Binary/BinSmartPointers.h>

namespace XLS
{;

// Logical representation of a record in BIFF8 that is followed by a number of Continue records 
// and the whole content is split into several records not following each other
class BiffRecordSplit : public BiffRecordContinued
{
public:
	BiffRecordSplit();
	~BiffRecordSplit();

//-----------------------------
	virtual const bool storeRecordAndDecideProceeding(CFRecordPtr record);

protected:
	virtual const bool isStartingRecord(CFRecord& record) = 0;
	virtual const bool isEndingRecord(CFRecord& record) = 0;
	virtual void useContinueRecords(CFRecord& record) = 0;

//-----------------------------
	CFRecordPtr stored_record;

};

} // namespace XLS

