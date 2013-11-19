#include "stdafx.h"
#include "OfficeArtRecord.h"
#include <XLS_bin/CFRecord.h>

namespace ODRAW
{;


OfficeArtRecord::OfficeArtRecord(const BYTE recVer, const WORD recType)
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
const WORD OfficeArtRecord::GetInstanceToStore()
{
	return 0;
}


} // namespace ODRAW
