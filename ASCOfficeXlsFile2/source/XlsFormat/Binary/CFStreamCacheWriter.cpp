
#include "CFStreamCacheWriter.h"
#include "CFRecord.h"
#include "CFStream.h"
#include <Logic/Biff_records/BOF.h>



namespace XLS
{;


CFStreamCacheWriter::CFStreamCacheWriter(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info)
:	stream_(stream),
	global_info_(global_info)
{
}


CFStreamCacheWriter::~CFStreamCacheWriter()
{
}


// Saves the next CFRecord to the CFStream or caches for later saving.
// Returns whether the record was saved to file or stored for later saving
bool CFStreamCacheWriter::storeNextRecord(CFRecordPtr record)
{
	record->save(stream_);
	return true;
}


// Actual transporting record to the CFStream and then to IStream
const size_t CFStreamCacheWriter::writeToStream(const size_t num_of_records_min_necessary)
{
	return 0;
}


} // namespace XLS
