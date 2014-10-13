#include "precompiled_xls.h"
#include "SerBool.h"
#include <Binary/CFRecord.h>

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
	record.reserveNunBytes(7); // reserved/unused
}


void SerBool::load(CFRecord& record)
{
	record >> f;
	record.skipNunBytes(7); // reserved/unused
}


const std::wstring SerBool::toString() const
{
	return f ? L"TRUE" : L"FALSE";
}


} // namespace XLS
