#include "stdafx.h"
#include "SerNil.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr SerNil::clone()
{
	return BiffStructurePtr(new SerNil(*this));
}


void SerNil::store(CFRecord& record)
{
	record.storeAnyData(fixed_type);
	record.skipNBytes(8); // reserved/unused
}


void SerNil::load(CFRecord& record)
{
	record.skipNBytes(8); // reserved/unused
}

const std::wstring SerNil::toString() const
{
	return L"";
}


} // namespace XLS
