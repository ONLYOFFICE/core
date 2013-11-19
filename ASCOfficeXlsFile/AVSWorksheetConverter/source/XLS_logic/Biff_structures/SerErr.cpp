#include "stdafx.h"
#include "SerErr.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


SerErr::SerErr()
{
}


SerErr::SerErr(const std::wstring& word)
:	err(word.c_str())
{
}


BiffStructurePtr SerErr::clone()
{
	return BiffStructurePtr(new SerErr(*this));
}


void SerErr::store(CFRecord& record)
{
	record.storeAnyData(fixed_type);
	record << err;
	record.reserveNBytes(7); // reserved/unused
}


void SerErr::load(CFRecord& record)
{
	record >> err;
	record.skipNBytes(7); // reserved/unused
}


const std::wstring SerErr::toString() const
{
	return err.toString();
}


} // namespace XLS
