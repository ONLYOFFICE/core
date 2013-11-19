#include "stdafx.h"
#include "SerBool.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


SerBool::SerBool()
{
}


SerBool::SerBool(const std::wstring& word)
{
	f = word == L"TRUE" ? 1 : 0;
}


BiffStructurePtr SerBool::clone()
{
	return BiffStructurePtr(new SerBool(*this));
}


void SerBool::store(CFRecord& record)
{
	record.storeAnyData(fixed_type);
	record << f;
	record.reserveNBytes(7); // reserved/unused
}


void SerBool::load(CFRecord& record)
{
	record >> f;
	record.skipNBytes(7); // reserved/unused
}


const std::wstring SerBool::toString() const
{
	return f ? L"TRUE" : L"FALSE";
}


} // namespace XLS
