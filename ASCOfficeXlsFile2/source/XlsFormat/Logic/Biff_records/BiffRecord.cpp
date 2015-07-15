#include "precompiled_xls.h"
#include "BiffRecord.h"
#include <Binary/CFStream.h>
#include "Binary/CFStreamCacheReader.h"
#include "Binary/CFStreamCacheWriter.h"


namespace XLS
{;

BiffRecord::BiffRecord()
{
}


BiffRecord::~BiffRecord()
{
}


// Common algorithm of loading from bin to XML
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

		// XML generation
		//BiffStructurePtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
		//toFrom(BinReaderProcessor(reader, parent/*own_tag*/, is_mandatory));	

		parent->add_child(this->clone());
	}

	return true; // Record reading OK
}


//const bool BiffRecord::write(BinWriterProcessor& proc, const bool is_mandatory)
//{
//	CFRecordPtr record(new CFRecord(getTypeId(), proc.getGlobalWorkbookInfo()));
//		
//	BiffStructurePtr own_tag = proc.getChildNode(getClassName(), is_mandatory);
//	if(!own_tag)
//	{
//		return false;
//	}
//	BinWriterProcessor writer_proc(proc, own_tag, is_mandatory); // Creates new processor with empty tags counters and own tag
//	
//	toFrom(writer_proc); 
//	writeFields(*record);
//	record->commitData();
//
//	if(proc.getWriter().storeNextRecord(record))
//	{
//		writeCreatedContinueRecords(proc.getWriter());
//		return true;
//	}
//	return false;
//}
//
//
//
//
// Stub. May be overridden in derivers
void BiffRecord::readFollowingContinue(CFStreamCacheReader& reader)
{
}


//// Stub. May be overridden in derivers
//void BiffRecord::writeCreatedContinueRecords(CFStreamCacheWriter& writer)
//{
//}


// Stub. May be overridden in derivers
const bool BiffRecord::storeRecordAndDecideProceeding(CFRecordPtr record)
{
	return true;
}


} // namespace XLS

