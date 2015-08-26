
#include "BiffRecordSplit.h"
#include "Binary/CFStreamCacheReader.h"
#include "Binary/CFStreamCacheWriter.h"


namespace XLS
{

BiffRecordSplit::BiffRecordSplit()
{
}


BiffRecordSplit::~BiffRecordSplit()
{
}



// Read all the Continue records that follow the record
const bool BiffRecordSplit::storeRecordAndDecideProceeding(CFRecordPtr record)
{
	useContinueRecords(*record);
	record->resetPointerToBegin();
	
	if(isStartingRecord(*record))
	{
		if (stored_record)
		{
			stored_record->appendRawData(record->getData(), record->getDataSize());
		}
		else
		{
			stored_record = CFRecordPtr(new CFRecord(record->getTypeId(), record->getGlobalWorkbookInfo()));

			stored_record->appendRawData(record->getData(), record->getDataSize());
		}
	}
	else if(stored_record == NULL)
	{
		stored_record = record;
		//throw;// EXCEPT::RT::WrongBiffRecord("Split records do not match", record->getTypeString());
	}
	else
	{
		//record->insertDataFromRecordToBeginning(stored_record);
		//stored_record = record;
		stored_record->appendRawData(record->getData(), record->getDataSize());
	}
	stored_record->resetPointerToBegin();
	bool is_end = isEndingRecord(*stored_record);
	if(is_end)
	{
		stored_record->resetPointerToBegin();
	}
	return is_end;
}



} // namespace XLS

