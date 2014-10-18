#pragma once

#include <Logic/BaseObject.h> 
#include <Logic/Biff_structures/BitMarkedStructs.h> 
//#include <Logic/Biff_structures/BiffAttribute.h>
#include <Logic/BinProcessor.h>

namespace XLS
{;

class CFStream;

// Logical representation of a record in BIFF8
class BiffRecord: public BaseObject
{
public:
	BiffRecord();
	~BiffRecord();

//	virtual const bool doSomething(BinProcessor& proc);
	virtual const bool read(CFStreamCacheReader& reader, BaseObjectPtr & parent, const bool is_mandatory); // Read self and children
	//virtual const bool write(BinWriterProcessor& proc, const bool is_mandatory); // Write self and children
	
	virtual void readFields(CFRecord& record) = 0;
	virtual void writeFields(CFRecord& record) = 0;

	virtual const CFRecordType::TypeId getTypeId() const = 0;  // made virtual to let derived assign their own names (for example, EOF_T)

private:
	virtual void readFollowingContinue(CFStreamCacheReader& reader);
	//virtual void writeCreatedContinueRecords(CFStreamCacheWriter& writer);

private:
	virtual const bool storeRecordAndDecideProceeding(CFRecordPtr record); // This function is overridden in BiffRecordSplit

protected:
	BaseObjectPtr parent_;
};

#define BIFF_RECORD_DEFINE_TYPE_INFO(class_name) public: const CFRecordType::TypeId getTypeId() const { return rt_ ## class_name; }

} // namespace XLS

