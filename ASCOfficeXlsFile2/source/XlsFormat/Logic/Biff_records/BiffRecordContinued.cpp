
#include "BiffRecordContinued.h"
#include "Binary/CFStreamCacheReader.h"
#include "Binary/CFStreamCacheWriter.h"


namespace XLS
{

BiffRecordContinued::BiffRecordContinued()
{
}


BiffRecordContinued::~BiffRecordContinued()
{
	for (ContinuesMap::const_iterator it = continue_records.begin(),
			itEnd = continue_records.end(); it != itEnd ; ++it)
	{
		if(!(*it).second.empty())
		{
			Log::warning("Not all of the 'Continue's processed. Type: " + CFRecordType::getStringById((*it).first));
			break;
		}
	}
}



// Read all the Continue records that follow the record
void BiffRecordContinued::readFollowingContinue(CFStreamCacheReader& reader)
{
	CFRecordType::TypeId type;

	while(CFRecordType::isContinue(type = reader.getNextRecordType()))
	{
		CFRecordPtr record = reader.getNextRecord(rt_ANY_TYPE, false);
		if(!record)
		{
			return;
		}
		continue_records[type].push_back(record);
	}
}


// Read all the Continue records that follow the record
void BiffRecordContinued::writeCreatedContinueRecords(CFStreamCacheWriter& writer)
{
	for(ContinuesMap::const_iterator it = continue_records.begin(), itEnd = continue_records.end(); it != itEnd; ++it)
	{
		CFRecordType::TypeId continue_type = (*it).first;
		CFRecordPtrList continues_list = (*it).second;
		for(CFRecordPtrList::iterator it2 = continues_list.begin(), it2End = continues_list.end(); it2 != it2End; ++it2)
		{
			writer.storeNextRecord(*it2);
		}
	}
	continue_records.clear();
}


} // namespace XLS

