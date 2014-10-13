#pragma once

#include <Logic/Biff_records/BiffRecord.h> 

namespace XLS
{;

// Logical representation of a record in BIFF8 that is followed by a number of Continue records
class BiffRecordContinued: public BiffRecord
{
public:
	BiffRecordContinued();
	~BiffRecordContinued();

protected:
	typedef std::map<CFRecordType::TypeId, std::list<CFRecordPtr> > ContinuesMap;
	ContinuesMap continue_records; // All records must be removed from the list that means they are processed

private:
	virtual void readFollowingContinue(CFStreamCacheReader& reader);
	virtual void writeCreatedContinueRecords(CFStreamCacheWriter& writer);

};

} // namespace XLS

