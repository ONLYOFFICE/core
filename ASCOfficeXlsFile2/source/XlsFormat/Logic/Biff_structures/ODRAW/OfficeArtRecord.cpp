
#include "OfficeArtRecord.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtRecord::OfficeArtRecord(const unsigned char recVer, const unsigned short recType)
{
	rh_own.recVer = recVer;
	rh_own.recType = recType;
}

void OfficeArtRecord::store(XLS::CFRecord& record)
{
	rh_own.recInstance = GetInstanceToStore();

	record << rh_own;
	size_t data_start_ptr = record.getDataSize();
	storeFields(record);
	record.registerDelayedDataSource(record.getDataSize() - data_start_ptr, XLS::rt_MsoDrawing);
}


void OfficeArtRecord::load(XLS::CFRecord& record)
{
	record >> rh_own;
	loadFields(record);
}


bool OfficeArtRecord::IsThisHeaderMine(const OfficeArtRecordHeader& rh_check)
{
	return rh_check.recVer == rh_own.recVer && rh_check.recType == rh_own.recType;
}

// Default implementation suitable for most classes
const unsigned short OfficeArtRecord::GetInstanceToStore()
{
	return 0;
}


} // namespace ODRAW
