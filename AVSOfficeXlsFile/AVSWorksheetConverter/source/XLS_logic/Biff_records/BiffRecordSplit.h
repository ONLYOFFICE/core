#pragma once

#include <XLS_logic/Biff_records/BiffRecordContinued.h> 
//#include <XLS_bin/BinSmartPointers.h>

namespace XLS
{;

// Logical representation of a record in BIFF8 that is followed by a number of Continue records 
// and the whole content is split into several records not following each other
class BiffRecordSplit : public BiffRecordContinued
{
public:
	BiffRecordSplit();
	~BiffRecordSplit();

private:
	virtual const bool storeRecordAndDecideProceeding(CFRecordPtr record);

protected:
	virtual const bool isStartingRecord(CFRecord& record) = 0;
	virtual const bool isEndingRecord(CFRecord& record) = 0;
	virtual void useContinueRecords(CFRecord& record) = 0;

private:
	CFRecordPtr stored_record;

};

} // namespace XLS

