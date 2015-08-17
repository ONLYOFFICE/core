#pragma once

#include "BaseObject.h"
#include "Biff_structures/BiffAttribute.h"
#include "Biff_structures/BiffStructure.h"
#include "Biff_structures/BiffStructure.h"
#include "GlobalWorkbookInfo.h"

namespace XLS
{;

class BiffString;
class CFStreamCacheWriter;

// Logical representation of composite objects
//
// Another new function is to buffer objects, that had to be read but was absent in the bin stream.
// The buffered records shall be checked for existence later.
// This feature is specially for files saved in OpenOffice Calc those don't conform to the Microsoft specification
// Call them "wanted objects"
//
class BinProcessor
{
public:
	BinProcessor(BaseObject* parent, GlobalWorkbookInfoPtr global_info);
	BinProcessor(GlobalWorkbookInfoPtr global_info);
	~BinProcessor();

	// This function doesn't 
	template<class T>
	const bool optional()
	{
		return optional(T());
	};
	template<class T>
	const bool mandatory()
	{
		return mandatory(T());
	};

	// Load the specified child object specified number of times
	// fromN = 0 means the object is optional
	// toN = 0 means no upper limit
	template<class T>
	const int repeated(const int fromN, const int toN)
	{
		return repeated(T(), fromN, toN);
	};

	const int repeated(BaseObject& object, const int fromN, const int toN);
	virtual const bool optional(BaseObject& object) = 0;
	virtual const bool mandatory(BaseObject& object) = 0;

	// Universal check if the we have the specified record in our processing plans (in binary it is not more than 'num_records_to_check' records forward)
	virtual const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check) = 0;
	// Determine the next substream type
        virtual const bool getNextSubstreamType(_UINT16& type) = 0;
	virtual void SeekToEOF() = 0;

	BaseObject* getParent() const { return parent_;};
	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() const { return global_info_;};
protected:
	BaseObject*				parent_;
	GlobalWorkbookInfoPtr	global_info_;
};


class BinReaderProcessor : public BinProcessor
{
public:
	BinReaderProcessor(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory);
	//BinReaderProcessor(CFStreamCacheReader& reader, const bool is_mandatory); //root ???

	virtual const bool optional(BaseObject& object);
	virtual const bool mandatory(BaseObject& object);

	// Check if the next read record would be of desired type
	const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check);
	// Assume that the next record is BOF (if not - return false) and get the type without extracting is from cache
	// In the case of stream end returns false
        const bool getNextSubstreamType(_UINT16& type);
	void SeekToEOF();

private:
	const bool readChild(BaseObject& object, const bool is_mandatory);

private:
	CFStreamCacheReader& reader_;
	// objects that are "WANTED", i.e. the objects that weren't read at the right moment and may be read later
	BaseObjectPtrList wanted_objects;
	bool is_mandatory_;
};


} // namespace XLS
