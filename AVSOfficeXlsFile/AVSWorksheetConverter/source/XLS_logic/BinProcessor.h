#pragma once

#include <XLS_logic/BaseObject.h>
#include <XLS_logic/Biff_structures/BiffAttribute.h>
#include <XLS_logic/Biff_structures/BiffStructure.h>
#include <XLS_logic/Biff_structures/BiffStructTagged.h>
#include <XLS_logic/GlobalWorkbookInfo.h>

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
	BinProcessor(MSXML2::IXMLDOMElementPtr parent, GlobalWorkbookInfoPtr global_info);
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
	const bool repeated(const int fromN, const int toN)
	{
		return repeated(T(), fromN, toN);
	};

	const bool repeated(BaseObject& object, const int fromN, const int toN);
	virtual const bool optional(BaseObject& object) = 0;
	virtual const bool mandatory(BaseObject& object) = 0;

	virtual void markAttribute(BiffAttribute& attrib, const _bstr_t& attrib_name) = 0;
	virtual void markTaggedAttribute(BiffStructure& attrib) = 0;
	virtual void markVector(BiffStructurePtrVector& vec, BiffStructure& exClone) = 0;

	// Universal check if the we have the specified record in our processing plans (in binary it is not more than 'num_records_to_check' records forward)
	virtual const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check) = 0;
	// Determine the next substream type
	virtual const bool getNextSubstreamType(WORD& type) = 0;
	virtual void SeekToEOF() = 0;

	MSXML2::IXMLDOMElementPtr getParent() const { return parent_;};
	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() const { return global_info_;};
protected:
	MSXML2::IXMLDOMElementPtr parent_;
	GlobalWorkbookInfoPtr global_info_;
};


class BinReaderProcessor : public BinProcessor
{
public:
	BinReaderProcessor(CFStreamCacheReader& reader, MSXML2::IXMLDOMElementPtr parent, const bool is_mandatory);

	virtual const bool optional(BaseObject& object);
	virtual const bool mandatory(BaseObject& object);

	virtual void markAttribute(BiffAttribute& attrib, const _bstr_t& attrib_name);
	virtual void markTaggedAttribute(BiffStructure& attrib);
	virtual void markVector(BiffStructurePtrVector& vec, BiffStructure& exClone);

	// Check if the next read record would be of desired type
	const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check);
	// Assume that the next record is BOF (if not - return false) and get the type without extracting is from cache
	// In the case of stream end returns false
	const bool getNextSubstreamType(WORD& type);
	void SeekToEOF();

private:
	const bool readChild(BaseObject& object, const bool is_mandatory);

private:
	CFStreamCacheReader& reader_;
	// objects that are "WANTED", i.e. the objects that weren't read at the right moment and may be read later
	BaseObjectPtrList wanted_objects;
	bool is_mandatory_;
};


class BinWriterProcessor : public BinProcessor
{
public:
	BinWriterProcessor(CFStreamCacheWriter& writer, MSXML2::IXMLDOMElementPtr parent, const bool is_mandatory);
	//BinWriterProcessor(BinWriterProcessor& copy_from_proc);
	BinWriterProcessor(BinWriterProcessor& copy_from_proc, MSXML2::IXMLDOMElementPtr own_tag, const bool is_mandatory);
	BinWriterProcessor(BinWriterProcessor& copy_from_proc, const bool is_mandatory);

	virtual const bool optional(BaseObject& object);
	virtual const bool mandatory(BaseObject& object);

	virtual void markAttribute(BiffAttribute& attrib, const _bstr_t& attrib_name);
	virtual void markTaggedAttribute(BiffStructure& attrib);
	virtual void markVector(BiffStructurePtrVector& vec, BiffStructure& exClone);

	// Check if we have the specified record tag among siblings
	const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check);
	// Determine the next substream type
	const bool getNextSubstreamType(WORD& type);
	void SeekToEOF();

	CFStreamCacheWriter& getWriter() const;
	MSXML2::IXMLDOMElementPtr getChildNode(const _bstr_t& tag_name, const bool is_mandatory);

private:
	const bool writeChild(BaseObject& object, const bool is_mandatory);

private:
	CFStreamCacheWriter& writer_;
	std::map<_bstr_t, size_t>& processed_tags;   // TODO: This stupid solution is the quickest for the moment, and shall be solved other way
	size_t& processed_tags_count;   // TODO: This stupid solution is the quickest for the moment, and shall be solved other way
	size_t processed_tags_count_own;
	std::map<_bstr_t, size_t> processed_tags_own;
	size_t substreams_processed;
	bool is_mandatory_;
};



} // namespace XLS
