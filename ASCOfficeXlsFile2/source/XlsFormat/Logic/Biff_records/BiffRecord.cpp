
#include "BiffRecord.h"
#include <Binary/CFStream.h>
#include "Binary/CFStreamCacheReader.h"
#include "Binary/CFStreamCacheWriter.h"


namespace XLS
{

BiffRecord::BiffRecord()
{
}


BiffRecord::~BiffRecord()
{
}


const bool BiffRecord::read(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory)
{
	parent_ = parent;		

	// Find and read the required record
	CFRecordPtr record = reader.getNextRecord(getTypeId(), is_mandatory);
	if(!record)
	{
		return false; // Required record hasn't been found
	}
		
	readFollowingContinue(reader);

	if(storeRecordAndDecideProceeding(record))
	{
		// Read fields data
		readFields(*record); // defined in derived classes

		size_t dataSize = record->getDataSize();
		size_t rdPtr = record->getRdPtr();
		size_t typeId = getTypeId();

		if(record->getDataSize() != record->getRdPtr() && getTypeId() != rt_ANY_TYPE)
		{
			Log::warning(STR::int2str(record->getDataSize() - record->getRdPtr(), 10) + " unsigned chars were not processed while reading from " + record->getTypeString());
		}


		parent->add_child(this->clone());
	}

	return true; // Record reading OK
}

void BiffRecord::readFollowingContinue(CFStreamCacheReader& reader)
{
}


const bool BiffRecord::storeRecordAndDecideProceeding(CFRecordPtr record)
{
	return true;
}


} // namespace XLS

