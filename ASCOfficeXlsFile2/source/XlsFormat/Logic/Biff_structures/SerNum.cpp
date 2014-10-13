#include "precompiled_xls.h"
#include "SerNum.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


SerNum::SerNum()
{
}


SerNum::SerNum(const std::wstring& word)
:	xnum(_wtof(word.c_str()))
{
}


BiffStructurePtr SerNum::clone()
{
	return BiffStructurePtr(new SerNum(*this));
}


void SerNum::store(CFRecord& record)
{
	record.storeAnyData(fixed_type);
	record << xnum;
}


void SerNum::load(CFRecord& record)
{
	record >> xnum;
}


const std::wstring SerNum::toString() const
{
	return STR::double2str(xnum);
}


} // namespace XLS
