#pragma once

#include "BinSmartPointers.h"
#include "..\Logic\GlobalWorkbookInfo.h"

namespace XLS
{;

// The purpose of the writer is to avoid seeking back through IStream when, for example, 
// we have write an offset of a record that would appear in processing much later.
// For example this is necessary in 'Index' record that shall reference DBCell records
// appearing later in the stream. So, this class will increase performance much

class CFStreamCacheWriter
{
public:
	CFStreamCacheWriter(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info);
	~CFStreamCacheWriter();

	// Saves the next CFRecord to the CFStream or caches for later saving. Returns whether the record was saved to file or stored for later saving
	bool storeNextRecord(CFRecordPtr record);

	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() { return global_info_; }

private:
	// Actual transporting record to the CFStream and then to IStream
	const size_t writeToStream(const size_t num_of_records_min_necessary);

private:
	CFStreamPtr stream_;
	CFRecordPtrList records_cache;
	GlobalWorkbookInfoPtr global_info_;
};

} // namespace XLS
